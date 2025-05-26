#include "stubs.hpp"
#include <decorator.h>
#include <gtest/gtest.h>
#include <cstdlib>

crf_allocator_table crf_GetNullMallocTable() {
    crf_allocator_table nullAllocator;
    nullAllocator.pfnFree = free;
    nullAllocator.pfnMalloc = crfStub_AlwaysNullMalloc;
    nullAllocator.pfnRealloc = crfStub_AlwaysNullRealloc;
    return nullAllocator;
}

crf_allocator_table crf_GetRussianRouletteMallocTable() {
    crf_allocator_table nullAllocator;
    nullAllocator.pfnFree = free;
    nullAllocator.pfnMalloc = crfStub_OddNullMalloc;
    nullAllocator.pfnRealloc = crfStub_OddNullRealloc;
    return nullAllocator;
}

TEST(AllocationErrHandler, TestCreateDecoratorNullMalloc) {
    crf_context ctx = crf_create_context();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_free_context(ctx);
}


TEST(AllocationErrHandler, TestCreateDecoratorOddMalloc) {
    crf_context ctx = crf_create_context();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = NULL;

    // needs to malloc for BOTH the decorator and the member types
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_free_context(ctx);
}

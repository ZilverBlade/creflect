#include "stubs.h"
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

TEST(creflect, TestCreateDecoratorNullMalloc) {
    crf_context ctx = crf_context_create();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "iiii";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_context_free(ctx);
}


TEST(creflect, TestCreateDecoratorOddMalloc) {
    crf_context ctx = crf_context_create();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "iiii";
    createInfo.ppMemberNames = NULL;

    // needs to malloc for BOTH the decorator and the member types
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_context_free(ctx);
}

#include "decorator.h"
#include "stubs.h"
#include <stdio.h>
#include <assert.h>

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


static void crf_TestCreateDecoratorNullMalloc() {
    crf_context ctx = crf_context_create();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "iiii";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    assert(!decorator);

    crf_context_free(ctx);

    printf("Passed %s!\n", __FUNCTION__);
}


static void crf_TestCreateDecoratorOddMalloc() {
    crf_context ctx = crf_context_create();
    crf_allocator_table table = crf_GetNullMallocTable();
    crf_context_set_allocator(ctx, &table);

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "iiii";
    createInfo.ppMemberNames = NULL;

    // needs to malloc for BOTH the decorator and the member types
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    assert(!decorator);

    crf_context_free(ctx);

    printf("Passed %s!\n", __FUNCTION__);
}

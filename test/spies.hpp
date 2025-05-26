#pragma once
#include <cstddef>
#include <cstdlib>
#include <context.h>
#include <gtest/gtest.h>

void* crf_MallocSpy(size_t size);
void crf_FreeSpy(void* ptr);
void* crf_ReallocSpy(void* ptr, size_t size);

int crf_GetNumDanglingPointers();

static crf_context crf_CreateContextWithAllocObserver() {
    crf_context ctx = crf_create_context();
    crf_allocator_table table;
    table.pfnMalloc = crf_MallocSpy;
    table.pfnRealloc = crf_ReallocSpy;
    table.pfnFree = crf_FreeSpy;
    crf_context_set_allocator(ctx, &table);
    return ctx;
}
static void crf_FreeContextAndVerify(crf_context ctx) {
    crf_free_context(ctx);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
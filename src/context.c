#include "context.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct crf_context_t {
    crf_allocator_table allocator;
} crf_context_t;

crf_context crf_context_create() {
    crf_context ctx = (crf_context)malloc(sizeof(crf_context_t));
    if (!ctx) return NULL;
    ctx->allocator.pfnMalloc = malloc;
    ctx->allocator.pfnRealloc = realloc;
    ctx->allocator.pfnFree = free;
    return ctx;
}

void crf_context_free(crf_context ctx) {
    assert(ctx && "dont free a null struct!");
    free(ctx);
}
const crf_allocator_table* crf_context_get_allocator(crf_context ctx) {
    assert(ctx && "don't pass a null struct!");
    return &ctx->allocator;
}
void crf_context_set_allocator(crf_context ctx, const crf_allocator_table* allocator) {
    assert(allocator && ctx && "Don't pass in a null struct");
    assert(allocator->pfnFree && "free not defined!");
    assert(allocator->pfnMalloc && "malloc not defined!");
    assert(allocator->pfnRealloc && "realloc not defined!");

    memcpy(&ctx->allocator, allocator, sizeof(crf_allocator_table));
}


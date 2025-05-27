#include "context.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "decorator.h"

static crf_context gContextChain = NULL;

typedef struct crf_reflection_list_entry {
    const char* szStructName;
    crf_decorator decorator;
    struct crf_reflection_list_entry* pNext;
} crf_reflection_list_entry;

typedef struct crf_context_t {
    struct crf_reflection_list_entry* pReflectionChain;
    crf_allocator_table allocator;
    crf_error_code eErrorCode;

    crf_context nextContext;
} crf_context_t;

crf_context crf_create_context() {
    crf_context ctx = (crf_context)malloc(sizeof(crf_context_t));
    if (!ctx) return NULL;
    ctx->allocator.pfnMalloc = malloc;
    ctx->allocator.pfnRealloc = realloc;
    ctx->allocator.pfnFree = free;
    ctx->pReflectionChain = NULL;

    if (gContextChain) {
        gContextChain->nextContext = ctx;
    }
    gContextChain = ctx;
    return ctx;
}

void crf_free_context(crf_context ctx) {
    assert(ctx && "dont free a null struct!");
    free(ctx);
    if (gContextChain) {
        gContextChain = gContextChain->nextContext;
    }
}

crf_context crf_get_context(size_t index) {

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

crf_error_code crf_context_get_last_error(crf_context ctx) {

}
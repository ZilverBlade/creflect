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

static void* mallocwrapper(size_t s, void* _) {
    return malloc(s);
}
static void freewrapper(void* p, void* _) {
    return free(p);
}
static void* reallocwrapper(void* p, size_t s, void* _) {
    return realloc(p, s);
}

crf_context crf_create_context() {
    crf_context ctx = (crf_context)malloc(sizeof(crf_context_t));
    if (!ctx) return NULL;
    ctx->allocator.pUserData = NULL;
    ctx->allocator.pfnMalloc = mallocwrapper;
    ctx->allocator.pfnRealloc = reallocwrapper;
    ctx->allocator.pfnFree = freewrapper;
    ctx->pReflectionChain = NULL;
    ctx->eErrorCode = CRF_EC_SUCCESS;
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
    crf_context ctx = gContextChain;
    size_t i = 0;
    while (i++ < index) {
        if (!ctx) {
            return NULL;
        }
        ctx = ctx->nextContext;
    }
    return ctx;
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

    ctx->allocator = *allocator;
}

crf_error_code crf_context_get_last_error(crf_context ctx) {
    return ctx->eErrorCode;
}
void shcrf_context_set_error(crf_context ctx, crf_error_code ec) {
    assert(ctx);
    ctx->eErrorCode = ec;
}
#include "context.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "decorator.h"

static crf_context gContextChain = NULL;

typedef struct shcrf_ReflectionListEntry {
    const char* szStructName;
    crf_decorator decorator;
    struct shcrf_ReflectionListEntry* pNext;
} shcrf_ReflectionListEntry;

#ifndef NDEBUG
typedef struct dbg_AllocationHandle_t {
    crf_data_type eType;
    void* pAllocPtr;
    dbg_AllocationHandle pNext;
} *dbg_AllocationHandle;
#endif

typedef struct crf_context_t {
    struct shcrf_ReflectionListEntry* pReflectionChain;
    crf_allocator_table allocator;
    crf_error_code eErrorCode;

    crf_context nextContext;
#ifndef NDEBUG
    dbg_AllocationHandle managedAllocations;
#endif
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
#ifndef NDEBUG
void dbgshcrf_context_mark_allocation(crf_context ctx, void* pMem, crf_data_type eType) {
    assert(ctx);
    dbg_AllocationHandle nextHandle = ctx->allocator.pfnMalloc(sizeof(struct dbg_AllocationHandle_t), ctx->allocator.pUserData);
    if (!nextHandle) {
        fprintf(stderr, "Failed to track debug object (failed allocation) %p\n", pMem);
        return; // failed to verify 
    }
    nextHandle->eType = eType;
    nextHandle->pAllocPtr = pMem;
    nextHandle->pNext = NULL;
    if (!ctx->managedAllocations) {
        ctx->managedAllocations = nextHandle;
        return;
    }
    dbg_AllocationHandle this = ctx->managedAllocations;
    while (this->pNext != NULL) {
        this = this->pNext;
    }
    this->pNext = nextHandle;
}
void dbgshcrf_context_unmark_allocation(crf_context ctx, void* pMem) {
    assert(ctx);
    dbg_AllocationHandle this = ctx->managedAllocations;
    dbg_AllocationHandle prev = NULL;
    while (this != NULL) {
        if (this->pAllocPtr == pMem) {
            if (prev) {
                prev->pNext = this->pNext;
            }
            ctx->allocator.pfnFree(this, ctx->allocator.pUserData);
            return;
        }
        this = this->pNext;
    }
    fprintf(stderr, "Allocation %p is not managed by context %p! Aborting...\n", pMem, ctx);
    assert(0 && "Context does not manage this allocation!");
}
void dbgshcrf_context_verify_zeroallocations(crf_context ctx) {
    dbg_AllocationHandle this = ctx->managedAllocations;
    if (this) {
        fprintf(stderr, "Context %p was freed before all resources have been freed! Missing Resources:\n", ctx);
        while (this != NULL) {
            fprintf(this->pAllocPtr, "\t%s:%p", crf_data_type_to_string(this->eType), this->pAllocPtr);
            this = this->pNext;
        }
    }
}
#endif
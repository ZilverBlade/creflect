#include "decorator.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct crf_decorator_t {
    crf_member_type* pMemberTypes;
    size_t szMemberCount;
    size_t szStructTotal;
} crf_decorator_t;

int crf_member_type_get_size(const crf_member_type member) {
    switch (member) {
    case CRF_MEMBER_CHAR: return CRF_MEMBER_SZ_CHAR;
    case CRF_MEMBER_SHORT: return CRF_MEMBER_SZ_SHORT;
    case CRF_MEMBER_INTEGER: return CRF_MEMBER_SZ_INTEGER;
    case CRF_MEMBER_LONG: return CRF_MEMBER_SZ_LONG;
    case CRF_MEMBER_FLOAT: return CRF_MEMBER_SZ_FLOAT;
    case CRF_MEMBER_DOUBLE: return CRF_MEMBER_SZ_DOUBLE;
    case CRF_MEMBER_POINTER: return CRF_MEMBER_SZ_POINTER;
    default: return 0;
    }
}

crf_decorator crf_create_decorator(crf_context ctx, const crf_decorator_create_info* layout) {
    assert(layout && "layout must not be null!");

    const crf_allocator_table* allocator = crf_context_get_allocator(ctx);

    crf_decorator result = (crf_decorator)allocator->pfnMalloc(sizeof(crf_decorator_t));
    if (!result) return NULL;

    result->szStructTotal = 0;
    result->szMemberCount = strlen(layout->pczMemberLayout);
    result->pMemberTypes = (crf_member_type*)allocator->pfnMalloc(result->szMemberCount * sizeof(crf_member_type));
    if (!result->pMemberTypes) goto fail_return;

    for (int i = 0; i < result->szMemberCount; ++i) {
        result->pMemberTypes[i] = (crf_member_type)layout->pczMemberLayout[i];
        int memberSz = crf_member_type_get_size(result->pMemberTypes[i]);
        if (memberSz == 0) {
            goto fail_return;
        }
        result->szStructTotal += (size_t)memberSz;
    }
    return result;

fail_return:
    crf_free_decorator(ctx, result);
    return NULL;
}

void crf_free_decorator(crf_context ctx, crf_decorator decorator) {
    assert(decorator && "cannot free a null struct!");
    if (decorator->pMemberTypes) {
        crf_context_get_allocator(ctx)->pfnFree(decorator->pMemberTypes);
    }
    free(decorator);
}

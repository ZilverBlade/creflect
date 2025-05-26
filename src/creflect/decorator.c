#include "decorator.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct crf_decorator_t {
    const char** pszMemberNames;
    crf_member_type* pMemberTypes;
    size_t cMemberCount;
    size_t cbStructTotal;
} crf_decorator_t;

crf_bool crf_is_member_name_valid(const char* szMemberName) {
    if (!szMemberName) return CRF_FALSE;
    size_t len = 0;
    while (szMemberName[len] != '\0') {
        char c = szMemberName[len];
        int bNumber = c >= '0' && c <= '9';
        if (len == 0 && bNumber) return CRF_FALSE;

        int bAlpha = c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
        int bHyphen = c == '_';

        if (!bHyphen && !bAlpha && !bNumber) return CRF_FALSE;
        ++len;
    }
    if (len == 0) return CRF_FALSE;
    return CRF_TRUE;
}

size_t crf_member_type_get_size(const crf_member_type member) {
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

    result->cbStructTotal = 0;
    result->cMemberCount = layout->szMemberLayout ? strlen(layout->szMemberLayout) : 0;
    result->pMemberTypes = NULL;
    result->pszMemberNames = NULL;

    if (result->cMemberCount > 0) {
        result->pMemberTypes = (crf_member_type*)allocator->pfnMalloc(result->cMemberCount * sizeof(crf_member_type));
        if (!result->pMemberTypes) goto fail_return;
        if (layout->pszMemberNames) {
            for (size_t i = 0; i < result->cMemberCount; ++i) {
                if (!crf_is_member_name_valid(layout->pszMemberNames[i])) goto fail_return;
                for (size_t j = 0; j < i; ++j) {
                    if (strcmp(layout->pszMemberNames[j], layout->pszMemberNames[i]) == 0) { // don't repeat member names
                        goto fail_return;
                    }
                }
            }

            result->pszMemberNames = (const char**)allocator->pfnMalloc(result->cMemberCount * sizeof(const char*));
            if (!result->pszMemberNames) goto fail_return;
            memset(result->pszMemberNames, 0, result->cMemberCount * sizeof(const char*));

            for (size_t i = 0; i < result->cMemberCount; ++i) {
                size_t clearSize = strlen(layout->pszMemberNames[i]) + 1;
                result->pszMemberNames[i] = (char*)allocator->pfnMalloc(clearSize);
                if (!result->pszMemberNames[i]) goto fail_return;
                memset(result->pszMemberNames[i], 0, clearSize);
                strncpy(result->pszMemberNames[i], layout->pszMemberNames[i], clearSize - 1);
            }
        }
        for (size_t i = 0; i < result->cMemberCount; ++i) {
            result->pMemberTypes[i] = (crf_member_type)layout->szMemberLayout[i];
            size_t memberSz = crf_member_type_get_size(result->pMemberTypes[i]);
            if (memberSz == 0) {
                goto fail_return;
            }
            result->cbStructTotal += (size_t)memberSz;
        }
    }
    return result;

fail_return:
    crf_free_decorator(ctx, result);
    return NULL;
}

void crf_free_decorator(crf_context ctx, crf_decorator decorator) {
    if (!decorator) return;
    const crf_allocator_table* allocator = crf_context_get_allocator(ctx);
    if (decorator->pszMemberNames) {
        for (size_t i = 0; i < decorator->cMemberCount; ++i) {
            allocator->pfnFree(decorator->pszMemberNames[i]);
        }
    }
    allocator->pfnFree(decorator->pMemberTypes);
    allocator->pfnFree(decorator->pszMemberNames);
    allocator->pfnFree(decorator);
}

crf_member_type crf_decorator_get_member_type(crf_decorator decorator, size_t index) {
    assert(decorator && "dont pass empty decorator ");
    if (index >= decorator->cMemberCount) return CRF_MEMBER_INVALID;
    return decorator->pMemberTypes[index];
}
size_t crf_decorator_get_member_index(crf_decorator decorator, const char* szMemberName) {
    assert(decorator && "dont pass empty decorator ");
    if (!szMemberName || !decorator->pszMemberNames) return CRF_INVALID_INDEX;
    for (size_t i = 0; i < decorator->cMemberCount; ++i) {
        if (strcmp(decorator->pszMemberNames[i], szMemberName) == 0) {
            return i;
        }
    }
    return CRF_INVALID_INDEX;
}
size_t crf_decorator_get_num_members(crf_decorator decorator) {
    assert(decorator && "dont pass empty decorator ");
    return decorator->cMemberCount;
}
size_t crf_decorator_get_size(crf_decorator decorator) {
    assert(decorator && "dont pass empty decorator ");
    return decorator->cbStructTotal;
}

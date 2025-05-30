#include "decorator.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct crf_decorator_t {
    char** pszMemberNames;
    crf_member_type* pMemberTypes;
    crf_decorator* pStructDecorators;

    size_t* pcbMemberOffsets;
    size_t* pcbMemberSizes;
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

size_t crf_member_type_get_size(crf_member_type member) {
    switch (member) {
    case CRF_MEMBER_CHAR: return CRF_MEMBER_SZ_CHAR;
    case CRF_MEMBER_SHORT: return CRF_MEMBER_SZ_SHORT;
    case CRF_MEMBER_INTEGER: return CRF_MEMBER_SZ_INTEGER;
    case CRF_MEMBER_LONG: return CRF_MEMBER_SZ_LONG;
    case CRF_MEMBER_FLOAT: return CRF_MEMBER_SZ_FLOAT;
    case CRF_MEMBER_DOUBLE: return CRF_MEMBER_SZ_DOUBLE;
    case CRF_MEMBER_POINTER: return CRF_MEMBER_SZ_POINTER;
    case CRF_MEMBER_STRUCT: return CRF_MEMBER_SZ_UNKNOWN;
    default: return CRF_MEMBER_SZ_INVALID;
    }
}

crf_decorator crf_create_decorator(crf_context ctx, const crf_decorator_create_info* layout) {
    assert(layout && "layout must not be null!");

    const crf_allocator_table* allocator = crf_context_get_allocator(ctx);

    crf_decorator result = (crf_decorator)allocator->pfnMalloc(sizeof(crf_decorator_t), allocator->pUserData);
    if (!result) {
        shcrf_context_set_error(ctx, CRF_EC_ALLOCATION_ERROR);
        return NULL;
    }
    dbgshcrf_context_mark_allocation(ctx, result, CRF_DATA_TYPE_DECORATOR);
    result->cbStructTotal = 0;
    result->cMemberCount = layout->szMemberLayout ? strlen(layout->szMemberLayout) : 0;
    result->pMemberTypes = NULL;
    result->pszMemberNames = NULL;
    result->pcbMemberSizes = NULL;
    result->pcbMemberOffsets = NULL;
    result->pStructDecorators = NULL;

    crf_error_code ec;

    if (result->cMemberCount > 0) {
        result->pMemberTypes = (crf_member_type*)allocator->pfnMalloc(result->cMemberCount * sizeof(crf_member_type), allocator->pUserData);
        if (!result->pMemberTypes) {
            ec = CRF_EC_ALLOCATION_ERROR;
            goto fail_return;
        }
        result->pcbMemberSizes = (size_t*)allocator->pfnMalloc(result->cMemberCount * sizeof(size_t), allocator->pUserData);
        if (!result->pcbMemberSizes) {
            ec = CRF_EC_ALLOCATION_ERROR;
            goto fail_return;
        }
        result->pcbMemberOffsets = (size_t*)allocator->pfnMalloc(result->cMemberCount * sizeof(size_t), allocator->pUserData);
        if (!result->pcbMemberOffsets) {
            ec = CRF_EC_ALLOCATION_ERROR;
            goto fail_return;
        }
        if (layout->pcbMemberOffsets) {
            for (size_t i = 0; i < result->cMemberCount; ++i) {
                result->pcbMemberOffsets[i] = layout->pcbMemberOffsets[i];
            }
        }
        if (layout->pszMemberNames) {
            for (size_t i = 0; i < result->cMemberCount; ++i) {
                if (!crf_is_member_name_valid(layout->pszMemberNames[i])) {
                    ec = CRF_EC_INVALID_ARG;
                    goto fail_return;
                }
                for (size_t j = 0; j < i; ++j) {
                    if (strcmp(layout->pszMemberNames[j], layout->pszMemberNames[i]) == 0) { // don't repeat member names
                        ec = CRF_EC_INVALID_ARG;
                        goto fail_return;
                    }
                }
            }

            result->pszMemberNames = (const char**)allocator->pfnMalloc(result->cMemberCount * sizeof(const char*), allocator->pUserData);
            if (!result->pszMemberNames) {
                ec = CRF_EC_ALLOCATION_ERROR;
                goto fail_return;
            }
            memset(result->pszMemberNames, 0, result->cMemberCount * sizeof(const char*));

            for (size_t i = 0; i < result->cMemberCount; ++i) {
                size_t clearSize = strlen(layout->pszMemberNames[i]) + 1;
                result->pszMemberNames[i] = (char*)allocator->pfnMalloc(clearSize, allocator->pUserData);
                if (!result->pszMemberNames[i]) {
                    ec = CRF_EC_ALLOCATION_ERROR;
                    goto fail_return;
                }
                memset(result->pszMemberNames[i], 0, clearSize);
                strncpy(result->pszMemberNames[i], layout->pszMemberNames[i], clearSize - 1);
            }
        }
        
        size_t cStructs = 0;
        for (size_t i = 0; i < result->cMemberCount; ++i) {
            result->pMemberTypes[i] = (crf_member_type)layout->szMemberLayout[i];
            size_t memberSz = crf_member_type_get_size(result->pMemberTypes[i]);
            if (!memberSz) {
                ec = CRF_EC_INVALID_ARG;
                goto fail_return;
            } else if (memberSz == CRF_MEMBER_SZ_UNKNOWN) { //struct
                if (!layout->pStructDecorators) {
                    ec = CRF_EC_MISSING_ARG;
                    goto fail_return;
                }
                if (!layout->pStructDecorators[i]) {
                    ec = CRF_EC_INVALID_ARG;
                    goto fail_return;
                }
                memberSz = layout->pStructDecorators[i]->cbStructTotal;
            }
            if (!layout->pcbMemberOffsets) {
                result->pcbMemberOffsets[i] = result->cbStructTotal;
            } else {
                if (i > 0 && (result->pcbMemberOffsets[i - 1] + result->pcbMemberSizes[i - 1] > result->pcbMemberOffsets[i])) {
                    ec = CRF_EC_INVALID_ARG;
                    goto fail_return;
                }
            }
            result->pcbMemberSizes[i] = memberSz;
            result->cbStructTotal = result->pcbMemberOffsets[i] + memberSz;
        }

        if (layout->pStructDecorators) {
            result->pStructDecorators = (crf_decorator*)allocator->pfnMalloc(cStructs * sizeof(crf_decorator), allocator->pUserData);
            if (!result->pStructDecorators) {
                ec = CRF_EC_ALLOCATION_ERROR;
                goto fail_return;
            }
            for (size_t i = 0; i < cStructs; ++i) {
                result->pStructDecorators[i] = layout->pStructDecorators[i];
            }
        }
    }
    shcrf_context_set_error(ctx, CRF_EC_SUCCESS);
    return result;

fail_return:
    crf_free_decorator(ctx, result);
    shcrf_context_set_error(ctx, ec);
    return NULL;
}

void crf_free_decorator(crf_context ctx, crf_decorator decorator) {
    if (!decorator) return;
    dbgshcrf_context_unmark_allocation(ctx, decorator);
    const crf_allocator_table* allocator = crf_context_get_allocator(ctx);
    if (decorator->pszMemberNames) {
        for (size_t i = 0; i < decorator->cMemberCount; ++i) {
            allocator->pfnFree(decorator->pszMemberNames[i], allocator->pUserData);
        }
    }
    allocator->pfnFree(decorator->pMemberTypes, allocator->pUserData);
    allocator->pfnFree(decorator->pStructDecorators, allocator->pUserData);
    allocator->pfnFree(decorator->pcbMemberOffsets, allocator->pUserData);
    allocator->pfnFree(decorator->pszMemberNames, allocator->pUserData);
    allocator->pfnFree(decorator->pcbMemberSizes, allocator->pUserData);
    allocator->pfnFree(decorator, allocator->pUserData);
    shcrf_context_set_error(ctx, CRF_EC_SUCCESS);
}

size_t crf_decorator_get_member_index(const crf_decorator decorator, const char* szMemberName) {
    assert(decorator && "dont pass empty decorator ");
    if (!szMemberName || !decorator->pszMemberNames) return CRF_INVALID_INDEX;
    for (size_t i = 0; i < decorator->cMemberCount; ++i) {
        if (strcmp(decorator->pszMemberNames[i], szMemberName) == 0) {
            return i;
        }
    }
    return CRF_INVALID_INDEX;
}
size_t crf_decorator_get_member_offset(const crf_decorator decorator, size_t index) {
    assert(decorator && "dont pass empty decorator ");
    if (index >= decorator->cMemberCount) return CRF_INVALID_INDEX;
    return decorator->pcbMemberOffsets[index];
}
size_t crf_decorator_get_member_size(const crf_decorator decorator, size_t index) {
    assert(decorator && "dont pass empty decorator ");
    if (index >= decorator->cMemberCount) return CRF_INVALID_INDEX;
    return decorator->pcbMemberSizes[index];
}
crf_member_type crf_decorator_get_member_type(const crf_decorator decorator, size_t index) {
    assert(decorator && "dont pass empty decorator ");
    if (index >= decorator->cMemberCount) return CRF_MEMBER_INVALID;
    return decorator->pMemberTypes[index];
}
size_t crf_decorator_get_num_members(const crf_decorator decorator) {
    assert(decorator && "dont pass empty decorator ");
    return decorator->cMemberCount;
}
size_t crf_decorator_get_size(const crf_decorator decorator) {
    assert(decorator && "dont pass empty decorator ");
    return decorator->cbStructTotal;
}

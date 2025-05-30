#include "struct.h"
#include <string.h>
#include <assert.h>

typedef struct crf_struct_t {
    void* pData;
    crf_decorator decorator;
    crf_bool bCtxOwnsData;
} crf_struct_t;

crf_struct crf_create_struct(crf_context ctx, const crf_decorator decorator) {
    const crf_allocator_table* alloc = crf_context_get_allocator(ctx);
    void* pData = alloc->pfnMalloc(crf_decorator_get_size(decorator), alloc->pUserData);
    if (!pData) {
        shcrf_context_set_error(ctx, CRF_EC_ALLOCATION_ERROR);
        return NULL;
    }
    crf_struct strc = crf_wrap_struct(ctx, decorator, pData);
    strc->bCtxOwnsData = CRF_TRUE;
    shcrf_context_set_error(ctx, CRF_EC_SUCCESS);
    return strc;
}

crf_struct crf_wrap_struct(crf_context ctx, const crf_decorator decorator, void* pData) {
    assert(ctx);
    if (!decorator) {
        shcrf_context_set_error(ctx, CRF_EC_INVALID_ARG);
        return NULL;
    }
    const crf_allocator_table* alloc = crf_context_get_allocator(ctx);
    crf_struct strc = alloc->pfnMalloc(sizeof(crf_struct_t), alloc->pUserData);
    if (!strc) {
        shcrf_context_set_error(ctx, CRF_EC_ALLOCATION_ERROR);
        return NULL;
    }
    dbgshcrf_context_mark_allocation(ctx, strc, CRF_DATA_TYPE_STRUCT);
    strc->bCtxOwnsData = CRF_FALSE;
    strc->decorator = decorator;
    strc->pData = pData;
    shcrf_context_set_error(ctx, CRF_EC_SUCCESS);
    return strc;
}

void crf_free_struct(crf_context ctx, crf_struct vstruct) {
    if (!vstruct) return;
    const crf_allocator_table* alloc = crf_context_get_allocator(ctx);
    if (vstruct->bCtxOwnsData) {
        alloc->pfnFree(vstruct->pData, alloc->pUserData);
    }
    dbgshcrf_context_unmark_allocation(ctx, vstruct);
    alloc->pfnFree(vstruct, alloc->pUserData);
    shcrf_context_set_error(ctx, CRF_EC_SUCCESS);
}

const crf_decorator crf_struct_get_decorator(crf_struct vstruct) {
    assert(vstruct && "dont pass a null struct");
    return vstruct->decorator;
}

crf_error_code crf_struct_member_get_value(crf_struct vstruct, size_t index, void* pOutValue) {
    assert(vstruct && "dont pass a null struct");
    size_t off = crf_decorator_get_member_offset(vstruct->decorator, index);
    if (off == CRF_INVALID_INDEX) {
        return CRF_EC_INDEX_OUT_OF_RANGE;
    }
    size_t size= crf_decorator_get_member_size(vstruct->decorator, index);
    memcpy(pOutValue, (char*)(vstruct->pData) + off, size);
    return CRF_EC_SUCCESS;
}

crf_error_code crf_struct_member_set_value(crf_struct vstruct, size_t index, const void* pInValue) {
    assert(vstruct && "dont pass a null struct");
    size_t off = crf_decorator_get_member_offset(vstruct->decorator, index);
    if (off == CRF_INVALID_INDEX) {
        return CRF_EC_INDEX_OUT_OF_RANGE;
    }
    size_t size = crf_decorator_get_member_size(vstruct->decorator, index);
    memcpy((char*)(vstruct->pData) + off, pInValue, size);
    return CRF_EC_SUCCESS;
}


crf_error_code crf_struct_member_get_value_byname(crf_struct vstruct, const char* szName, void* pOutValue) {
    assert(vstruct && "dont pass a null struct");
    if (!crf_decorator_has_member_names(vstruct->decorator)) return CRF_EC_INVALID_STATE;
    if (!szName) return CRF_EC_INVALID_ARG;

    size_t idx = crf_decorator_get_member_index(vstruct->decorator, szName);
    if (idx == CRF_INVALID_INDEX) {
        return CRF_EC_INDEX_OUT_OF_RANGE;
    }
    size_t off = crf_decorator_get_member_offset(vstruct->decorator, idx);
    size_t size = crf_decorator_get_member_size(vstruct->decorator, idx);
    memcpy(pOutValue, (char*)(vstruct->pData) + off, size);
    return CRF_EC_SUCCESS;
}

crf_error_code crf_struct_member_set_value_byname(crf_struct vstruct, const char* szName, const void* pInValue) {
    assert(vstruct && "dont pass a null struct");
    if (!crf_decorator_has_member_names(vstruct->decorator)) return CRF_EC_INVALID_STATE;
    if (!szName) return CRF_EC_INVALID_ARG;

    size_t idx = crf_decorator_get_member_index(vstruct->decorator, szName);
    if (idx == CRF_INVALID_INDEX) {
        return CRF_EC_INDEX_OUT_OF_RANGE;
    }
    size_t off = crf_decorator_get_member_offset(vstruct->decorator, idx);
    size_t size = crf_decorator_get_member_size(vstruct->decorator, idx);
    memcpy((char*)(vstruct->pData) + off, pInValue, size);
    return CRF_EC_SUCCESS;
}

void* crf_struct_get_data_ref_ptr(crf_struct vstruct) {
    assert(vstruct && "dont pass a null struct");
    return vstruct->pData;
}

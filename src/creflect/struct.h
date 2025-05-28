#ifndef CREFLECT_PACK_H
#define CREFLECT_PACK_H

#include "decorator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct crf_struct_t* crf_struct;

CREFLECT_API crf_struct crf_create_struct(crf_context ctx, const crf_decorator decorator);
CREFLECT_API void crf_free_struct(crf_context ctx, crf_struct vstruct);

CREFLECT_API const crf_decorator crf_struct_get_decorator(crf_struct vstruct);

CREFLECT_API void crf_struct_member_get_value(crf_struct vstruct, size_t index, void* pOutValue);
CREFLECT_API void crf_struct_member_set_value(crf_struct vstruct, size_t index, const void* pInValue);

CREFLECT_API void* crf_struct_get_data_ref_ptr(crf_struct vstruct);

#define crf_struct_get_data_ref(stype, vstruct) (stype*)crf_struct_get_data_ref_ptr(vstruct)
#define crf_struct_get_data(stype, vstruct) *(stype*)crf_struct_get_data_ref_ptr(vstruct)
#ifdef __cplusplus
}
#endif
#endif
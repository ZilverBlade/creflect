#ifndef CREFLECT_REFLECTION_H
#define CREFLECT_REFLECTION_H
#include "decorator.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

enum crf_primitive_type_mappings {
    _crfmapped_crf_int8 = CRF_MEMBER_CHAR,
    _crfmapped_crf_int16 = CRF_MEMBER_SHORT,
    _crfmapped_crf_int32 = CRF_MEMBER_INTEGER,
    _crfmapped_crf_int64 = CRF_MEMBER_LONG,
    _crfmapped_crf_float32 = CRF_MEMBER_FLOAT,
    _crfmapped_crf_float64 = CRF_MEMBER_DOUBLE,
    _crfmapped_crf_ptr = CRF_MEMBER_POINTER,

    _crfmapped_crf_char = _crfmapped_crf_int8,
    _crfmapped_crf_short = _crfmapped_crf_int16,
    _crfmapped_crf_integer = _crfmapped_crf_int32,
    _crfmapped_crf_long = _crfmapped_crf_int64,
    _crfmapped_crf_float = _crfmapped_crf_float32,
    _crfmapped_crf_double = _crfmapped_crf_float64
};

typedef struct crf_reflection_struct_t* crf_reflection_struct;

//void crf_reflection_decorator_add_with_offset(crf_context ctx, const char* szStructureName,
//    crf_member_type eType, const char* szMemberName, size_t cbOffset);
//void crf_reflection_emit_structure(crf_context ctx, const char* szStructureName, size_t cbVerifySize);
//crf_reflection_struct crf_get_reflection(crf_context ctx, const char* szStructureName);
//
//const char* crf_reflection_struct_stringify(crf_context ctx, crf_reflection_struct reflection, const void* pStruct);
//const char* crf_reflection_layout_stringify(crf_context ctx, crf_reflection_struct reflection);
//
//void crf_reflection_struct_serialize(crf_context ctx, crf_reflection_struct reflection, const void* pInStruct, FILE** pOutBuffer);
//void crf_reflection_struct_deserialize(crf_context ctx, crf_reflection_struct reflection, const FILE* pInBuffer, void* pOutStruct);

#define CREFLECT_STRUCT(ctx, structure, ...) struct structure
#define CREFLECT_MEMBER(ctx,structure, type, member) crf_reflection_decorator_add_with_offset(#structure,\
(crf_member_type)##_crfmapped_##type, #member, offsetof(structure, member))
#define CREFLECT_ARRAY(ctx,structure, type, member) crf_reflection_decorator_add_array_with_offset(#structure,\
(crf_member_type)##_crfmapped_##type, #member, sizeof(member) / sizeof(*member), offsetof(structure, member))

#define CREFLECT_GET_MEMBER_TYPE(ctx,structure, member)
#define CREFLECT_GET_MEMBER_OFFSET(ctx,structure, member)
#define CREFLECT_GET_MEMBER_VALUE(ctx,structure, member, type)

#define CREFLECT_EMIT(structure) crf_reflection_emit_structure(#structure, sizeof(structure))



#ifdef __cplusplus
}
#endif
#endif
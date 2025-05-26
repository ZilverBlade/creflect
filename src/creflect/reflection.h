#ifndef CREFLECT_REFLECTION_H
#define CREFLECT_REFLECTION_H
#include "decorator.h"
#ifdef __cplusplus
extern "C" {
#endif

enum crf_primitive_type_mappings {
    _crfmapped_crf_int8 = CRF_MEMBER_INTEGER,
    _crfmapped_crf_int16 = CRF_MEMBER_INTEGER,
    _crfmapped_crf_int32 = CRF_MEMBER_INTEGER,
    _crfmapped_crf_int64 = CRF_MEMBER_INTEGER,
    _crfmapped_crf_ptr = CRF_MEMBER_POINTER
};

static void crf_reflection_decorator_add_with_offset(const char* szStructureName, crf_member_type type, const char* szMemberName, size_t offset) {

}
static void crf_reflection_emit_structure(const char* szStructureName, size_t cbVerifySize) {

}

#define CREFLECT_STRUCT(structure, ...) struct structure
#define CREFLECT_REFLECT(structure, type, member) crf_reflection_decorator_add_with_offset(#structure,\
(crf_member_type)##_crfmapped_##type, #member, offsetof(structure, member))


#define CREFLECT_REFLECT_EMIT(structure) crf_reflection_emit_structure(#structure, sizeof(structure))
CREFLECT_STRUCT(test_struct_t) {
    int iTestVar;
    void* pUserData;
};

void onreflect() {
    CREFLECT_REFLECT(test_struct_t, crf_int32, iTestVar);
    CREFLECT_REFLECT(test_struct_t, crf_ptr, pUserData);
    CREFLECT_REFLECT_EMIT(test_struct_t);

}

#ifdef __cplusplus
}
#endif
#endif
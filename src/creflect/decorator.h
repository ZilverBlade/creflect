#ifndef CREFLECT_STRUCT_LAYOUT_H
#define CREFLECT_STRUCT_LAYOUT_H
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum crf_member_type_t {
    CRF_MEMBER_INVALID = '\0',

    CRF_MEMBER_CHAR = 'c',
    CRF_MEMBER_SHORT = 's',
    CRF_MEMBER_INTEGER = 'i',
    CRF_MEMBER_LONG = 'l',
    CRF_MEMBER_FLOAT = 'f',
    CRF_MEMBER_DOUBLE = 'd',
    CRF_MEMBER_POINTER = 'p'

} crf_member_type_t;
typedef char crf_member_type;

typedef enum crf_member_size_t {
    CRF_MEMBER_SZ_CHAR = 1,
    CRF_MEMBER_SZ_SHORT = 2,
    CRF_MEMBER_SZ_INTEGER = 4,
    CRF_MEMBER_SZ_LONG = 8,
    CRF_MEMBER_SZ_FLOAT = 4,
    CRF_MEMBER_SZ_DOUBLE = 8,

    CRF_MEMBER_SZ_POINTER = sizeof(void*)
} crf_member_size_t;

typedef const char* crf_member_layout;

typedef struct crf_decorator_t* crf_decorator;

typedef struct crf_decorator_create_info {
    crf_member_layout szMemberLayout;
    const char* const* pszMemberNames;
} crf_decorator_create_info;

CREFLECT_API crf_bool crf_is_member_name_valid(const char* szMemberName);
CREFLECT_API size_t crf_member_type_get_size(const crf_member_type member);

CREFLECT_API crf_decorator crf_create_decorator(crf_context ctx, const crf_decorator_create_info* layout);
CREFLECT_API void crf_free_decorator(crf_context ctx, crf_decorator decorator);

CREFLECT_API crf_member_type crf_decorator_get_member_type(crf_decorator decorator, size_t index);
CREFLECT_API size_t crf_decorator_get_member_index(crf_decorator decorator, const char* szMemberName);
CREFLECT_API size_t crf_decorator_get_num_members(crf_decorator decorator);
CREFLECT_API size_t crf_decorator_get_size(crf_decorator decorator);


#ifdef __cplusplus
}
#endif
#endif
#ifndef CREFLECT_STRUCT_LAYOUT_H
#define CREFLECT_STRUCT_LAYOUT_H
#include "context.h"

typedef enum crf_member_type {
    CRF_MEMBER_CHAR = 'c',
    CRF_MEMBER_SHORT = 's',
    CRF_MEMBER_INTEGER = 'i',
    CRF_MEMBER_LONG = 'l',
    CRF_MEMBER_FLOAT = 'f',
    CRF_MEMBER_DOUBLE = 'd',
    CRF_MEMBER_POINTER = 'p',

    CRF_MAX_VAL = 0xFF
} crf_member_type;

typedef enum crf_member_size {
    CRF_MEMBER_SZ_CHAR = 1,
    CRF_MEMBER_SZ_SHORT = 2,
    CRF_MEMBER_SZ_INTEGER = 4,
    CRF_MEMBER_SZ_LONG = 8,
    CRF_MEMBER_SZ_FLOAT = 4,
    CRF_MEMBER_SZ_DOUBLE = 8,

    CRF_MEMBER_SZ_POINTER = sizeof(void*)
} crf_member_size;

typedef const char* crf_member_layout;

typedef struct crf_decorator_t* crf_decorator;

typedef struct crf_decorator_create_info {
    crf_member_layout pczMemberLayout;
    const char** ppMemberNames;
} crf_decorator_create_info;

int crf_member_type_get_size(const crf_member_type member);

crf_decorator crf_create_decorator(crf_context ctx, const crf_decorator_create_info* layout);
void crf_free_decorator(crf_context ctx, crf_decorator decorator);

#endif
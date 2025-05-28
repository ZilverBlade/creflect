#ifndef CREFLECT_DEFS_H
#define CREFLECT_DEFS_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CRF_INVALID_INDEX ((size_t)(SIZE_MAX))
typedef enum crf_bool {
    CRF_FALSE = 0,
    CRF_TRUE = 1
} crf_bool;

typedef int8_t crf_char;
typedef uint8_t crf_uchar;
typedef crf_char crf_int8;
typedef crf_uchar crf_uint8;
typedef int16_t crf_short;
typedef uint16_t crf_ushort;
typedef crf_short crf_int16;
typedef crf_ushort crf_uint16;
typedef int32_t crf_integer;
typedef uint32_t crf_uinteger;
typedef crf_integer crf_int32;
typedef crf_uinteger crf_uint32;
typedef int64_t crf_long;
typedef uint64_t crf_ulong;
typedef crf_long crf_int64;
typedef crf_ulong crf_uint64;
typedef void* crf_ptr;

#if defined(CREFLECT_SHARED_LIB) && defined(_WIN32)
#ifdef creflect_src
#define CREFLECT_API __declspec(dllexport)
#else
#define CREFLECT_API __declspec(dllimport)
#endif
#else
#define CREFLECT_API 
#endif

#ifdef __cplusplus
}
#endif
#endif
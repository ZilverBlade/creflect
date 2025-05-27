#ifndef CREFLECT_HASHMAP_H
#define CREFLECT_HASHMAP_H
#include "allocator.h"
#include "defs.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct crf_fixed_hashmap_t* crf_fixed_hashmap;

typedef size_t(*crf_fn_hash_code)(const void*);
typedef int(*crf_fn_equals)(const void*, const void*);

CREFLECT_API crf_fixed_hashmap crf_create_fixed_hashmap(const crf_allocator_table* pAllocator, size_t cSize, const crf_fn_hash_code pfnHashCode, const crf_fn_equals pfnEquals);
CREFLECT_API void crf_free_fixed_hashmap(const crf_allocator_table* pAllocator, crf_fixed_hashmap map);

CREFLECT_API const void* crf_fixed_hashmap_insert(crf_fixed_hashmap map, const void* key, const void* value);
CREFLECT_API crf_bool crf_fixed_hashmap_remove(crf_fixed_hashmap map, const void* key);
CREFLECT_API const void* crf_fixed_hashmap_get_ptr(crf_fixed_hashmap map, const void* key);

#define crf_fixed_hashmap_get(pType, map, key) (pType)crf_fixed_hashmap_get_ptr(map, key)

#ifdef __cplusplus
}
#endif
#endif
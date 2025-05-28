#ifndef CREFLECT_ALLOCATOR_H
#define CREFLECT_ALLOCATOR_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void* (*crf_fn_malloc)(size_t, void*);
typedef void  (*crf_fn_free)(void*, void*);
typedef void* (*crf_fn_realloc)(void*, size_t, void*);

typedef struct crf_allocator_table {
    void*           pUserData;
    crf_fn_malloc   pfnMalloc;
    crf_fn_free     pfnFree;
    crf_fn_realloc  pfnRealloc;
} crf_allocator_table;

#ifdef __cplusplus
}
#endif
#endif
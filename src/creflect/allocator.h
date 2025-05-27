#ifndef CREFLECT_ALLOCATOR_H
#define CREFLECT_ALLOCATOR_H
#include <stddef.h>

typedef void* (*crf_fn_malloc)(size_t);
typedef void  (*crf_fn_free)(void*);
typedef void* (*crf_fn_realloc)(void*, size_t);

typedef struct crf_allocator_table {
    crf_fn_malloc   pfnMalloc;
    crf_fn_free     pfnFree;
    crf_fn_realloc  pfnRealloc;
} crf_allocator_table;


#endif
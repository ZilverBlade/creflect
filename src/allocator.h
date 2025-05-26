#ifndef CREFLECT_ALLOCATOR_H
#define CREFLECT_ALLOCATOR_H
#include <stddef.h>

typedef struct crf_allocator_table {
    void* (*pfnMalloc)(size_t);
    void  (*pfnFree)(void*);
    void* (*pfnRealloc)(void*, size_t);
} crf_allocator_table;


#endif
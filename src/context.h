#ifndef CREFLECT_CONTEXT_H
#define CREFLECT_CONTEXT_H
#include "allocator.h"

typedef struct crf_context_t* crf_context;

crf_context crf_context_create();
void crf_context_free(crf_context ctx);
const crf_allocator_table* crf_context_get_allocator(crf_context ctx);
void crf_context_set_allocator(crf_context ctx, const crf_allocator_table* allocator);

#endif
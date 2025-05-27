#ifndef CREFLECT_CONTEXT_H
#define CREFLECT_CONTEXT_H
#include "defs.h"
#include "error.h"
#include "allocator.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct crf_context_t* crf_context;\

crf_context crf_create_context();
void crf_free_context(crf_context ctx);

crf_context crf_get_context(size_t index);

const crf_allocator_table* crf_context_get_allocator(crf_context ctx);
void crf_context_set_allocator(crf_context ctx, const crf_allocator_table* allocator);

crf_error_code crf_context_get_last_error(crf_context ctx);

#ifdef __cplusplus
}
#endif
#endif
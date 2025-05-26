#ifndef CREFLECT_CONTEXT_H
#define CREFLECT_CONTEXT_H
#include "allocator.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CRF_INVALID_INDEX (size_t)(-1)
typedef struct crf_context_t* crf_context;

crf_context crf_create_context();
void crf_free_context(crf_context ctx);
const crf_allocator_table* crf_context_get_allocator(crf_context ctx);
void crf_context_set_allocator(crf_context ctx, const crf_allocator_table* allocator);

#ifdef __cplusplus
}
#endif
#endif
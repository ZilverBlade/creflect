#ifndef CREFLECT_CONTEXT_H
#define CREFLECT_CONTEXT_H
#include "defs.h"
#include "error.h"
#include "allocator.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct crf_context_t* crf_context;

CREFLECT_API crf_context crf_create_context();
CREFLECT_API void crf_free_context(crf_context ctx);

CREFLECT_API crf_context crf_get_context(size_t index);

CREFLECT_API const crf_allocator_table* crf_context_get_allocator(crf_context ctx);
CREFLECT_API void crf_context_set_allocator(crf_context ctx, const crf_allocator_table* allocator);

CREFLECT_API crf_error_code crf_context_get_last_error(crf_context ctx);

#ifdef creflect_src
void shcrf_context_set_error(crf_context ctx, crf_error_code ec);
#ifndef NDEBUG
void dbgshcrf_context_mark_allocation(crf_context ctx, void* pMem, crf_data_type eType);
void dbgshcrf_context_unmark_allocation(crf_context ctx, void* pMem);
void dbgshcrf_context_verify_zeroallocations(crf_context ctx);
#else
#define dbgshcrf_context_mark_allocation(x,y,z)
#define dbgshcrf_context_unmark_allocation(x,y)
#define dbgshcrf_context_verify_zeroallocations(x)
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
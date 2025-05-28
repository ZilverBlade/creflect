#ifndef CREFLECT_ERROR_H
#define CREFLECT_ERROR_H
#ifdef __cplusplus
extern "C" {
#endif

typedef enum crf_error_code_t {
    CRF_EC_SUCCESS = 0,
    CRF_EC_UNKNOWN = 1,
    CRF_EC_INVALID_ARG = 2,
    CRF_EC_MISSING_ARG = 3,
    CRF_EC_INVALID_STATE = 4,
    CRF_EC_ALLOCATION_ERROR = 5,
    CRF_EC_INDEX_OUT_OF_RANGE = 6,

} crf_error_code_t;
typedef char crf_error_code;

#ifdef __cplusplus
}
#endif
#endif
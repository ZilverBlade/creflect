#ifndef CREFLECT_ERROR_H
#define CREFLECT_ERROR_H
#ifdef __cplusplus
extern "C" {
#endif

typedef enum crf_error_code_t {
    CRF_EC_SUCCESS = 0,
    CRF_EC_UNKNOWN = 1,
    CRF_EC_INVALID_ARG = 2,
    CRF_EC_INVALID_STATE = 3,
    CRF_EC_ALLOCATION_ERROR = 4
} crf_error_code_t;
typedef char crf_error_code;

#ifdef __cplusplus
}
#endif
#endif
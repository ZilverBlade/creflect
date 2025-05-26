#include <stddef.h>

static void* crfStub_AlwaysNullMalloc(size_t size) {
    return NULL;
}
static void* crfStub_AlwaysNullRealloc(void* ptr, size_t size) {
    return NULL;
}

static void* crfStub_OddNullMalloc(size_t size) {
    static int state = 0;
    state = !state;
    if (state) return malloc(size);
    return NULL;
}
static void* crfStub_OddNullRealloc(void* ptr, size_t size) {
    static int state = 0;
    state = !state;
    if (state) return realloc(ptr, size);
    return NULL;
}

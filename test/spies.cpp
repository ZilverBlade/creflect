#include "spies.hpp"
#include <unordered_set>

static std::unordered_set<void*> allocatedPtrs{};

void* crf_MallocSpy(size_t size) {
    void* ptr = malloc(size);
    if (ptr) allocatedPtrs.insert(ptr);
    return ptr;
}
void crf_FreeSpy(void* ptr) {
    free(ptr);
    allocatedPtrs.erase(ptr);
}
void* crf_ReallocSpy(void* ptr, size_t size) {
    void* newPtr = realloc(ptr, size);
    if (newPtr) {
        allocatedPtrs.insert(newPtr);
        allocatedPtrs.erase(ptr);
    }
    return newPtr;
}

int crf_GetNumDanglingPointers() {
    return allocatedPtrs.size();
}
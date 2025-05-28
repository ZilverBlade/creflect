#pragma once
#include <cstddef>
#include <cstdlib>
#include <unordered_map>
#include <creflect/allocator.h>

class AbstractMalloc {
public:
    virtual void* Malloc(size_t size) = 0;
    virtual void  Free(void* ptr) = 0;
    virtual void* Realloc(void* ptr, size_t size) = 0;

    crf_allocator_table BindAllocator() {
        crf_allocator_table table{};
        table.pfnMalloc = [](size_t sz, void* self) -> void* {
            return reinterpret_cast<AbstractMalloc*>(self)->Malloc(sz);
        };
        table.pfnRealloc = [](void* ptr, size_t sz, void* self) -> void* {
            return reinterpret_cast<AbstractMalloc*>(self)->Realloc(ptr, sz);
        };
        table.pfnFree = [](void* ptr, void* self) {
            reinterpret_cast<AbstractMalloc*>(self)->Free(ptr);
        };
        table.pUserData = this;
        return table;
    }
};
class ProgrammableMalloc : public AbstractMalloc {
public:
    ProgrammableMalloc() = default;
    virtual ~ProgrammableMalloc() {}
    virtual void* Malloc(size_t size) override {
        size_t idx = mallocAttempt++;
        bool success = defaultMalloc;
        if (willSucceedMalloc.find(idx) != willSucceedMalloc.end()) {
            success = willSucceedMalloc[idx];
        }
        if (success) {
            printf("malloc success\n");
            return malloc(size);
        }
        printf("malloc fail\n");
        return NULL;
    }
    virtual void Free(void* ptr) override {
        free(ptr);
    }
    virtual void* Realloc(void* ptr, size_t size) override {
        size_t idx = reallocAttempt++;
        bool success = defaultRealloc;
        if (willSucceedRealloc.find(idx) != willSucceedRealloc.end()) {
            success = willSucceedRealloc[idx];
        }
        if (success) {
            return realloc(ptr, size);
        }
        return NULL;
    }

    ProgrammableMalloc& WhenMallocAttempt(size_t attempt, bool success) {
        willSucceedMalloc[attempt] = success;
        return *this;
    }
    ProgrammableMalloc& WhenReallocAttempt(size_t attempt, bool success) {
        willSucceedRealloc[attempt] = success;
        return *this;
    }
    ProgrammableMalloc& MallocByDefault(bool success) {
        defaultMalloc = success;
        return *this;
    }
    ProgrammableMalloc& ReallocByDefault(bool success) {
        defaultRealloc = success;
        return *this;
    }

private:
    bool defaultMalloc = true;
    bool defaultRealloc = true;
    std::unordered_map<size_t, bool> willSucceedMalloc;
    std::unordered_map<size_t, bool> willSucceedRealloc;
    size_t mallocAttempt = 0;
    size_t reallocAttempt = 0;
};
class NullMalloc : public AbstractMalloc {
public:
    NullMalloc() = default;
    virtual void* Malloc(size_t size) override {
        return NULL;
    }
    virtual void Free(void* ptr) override {

    }
    virtual void* Realloc(void* ptr, size_t size) override {
        return NULL;
    }
};
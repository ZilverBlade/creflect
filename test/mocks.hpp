#pragma once
#include "stubs.hpp"
#include <unordered_map>
#include <gmock/gmock.h>

class NullMalloc : public AbstractMalloc {
public:
    virtual void* Malloc(size_t size) override {
        return NULL;
    }
    virtual void Free(void* ptr) override {

    }
    virtual void* Realloc(void* ptr, size_t size) override {
        return NULL;
    }
};
class MockNullMalloc : public NullMalloc {
public:
    MOCK_METHOD(void*, Malloc, (size_t), (override));
    MOCK_METHOD(void, Free, (void*), (override));
    MOCK_METHOD(void*, Realloc, (void*, size_t), (override));
};
class ProgrammableMalloc : public AbstractMalloc {
public:
    virtual void* Malloc(size_t size) override {
        size_t idx = mallocAttempt++;
        bool success = defaultMalloc;
        if (willSucceedMalloc.find(idx) != willSucceedMalloc.end()) {
            success = willSucceedMalloc[idx];
        }
        if (success) {
            return malloc(size);
        }
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
class MockProgrammableMalloc : public ProgrammableMalloc {
public:
    MOCK_METHOD(void*, Malloc, (size_t), (override));
    MOCK_METHOD(void, Free, (void*), (override));
    MOCK_METHOD(void*, Realloc, (void*, size_t), (override));
};

using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::NotNull;
using ::testing::IsNull;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnNew;
using ::testing::ReturnArg;
using ::testing::Gt;
using ::testing::Ge;
using ::testing::Eq;
using ::testing::Ne;
using ::testing::Lt;
using ::testing::Le;
using ::testing::_;
using ::testing::Exactly;

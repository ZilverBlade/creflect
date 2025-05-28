#pragma once
#include "spies.hpp"
#include "stubs.hpp"
#include <gmock/gmock.h>

class MockMalloc : public AbstractMalloc {
public:
    MOCK_METHOD(void*, Malloc, (size_t), (override));
    MOCK_METHOD(void, Free, (void*), (override));
    MOCK_METHOD(void*, Realloc, (void*, size_t), (override));
};
class MockProgrammableMalloc : public ProgrammableMalloc {
public:
    MOCK_METHOD(void*, Malloc, (size_t), (override));
    MOCK_METHOD(void, Free, (void*), (override));
    MOCK_METHOD(void*, Realloc, (void*, size_t), (override));
};

class MockMallocSpyVerifyMemory : public MallocSpyVerifyMemory {
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

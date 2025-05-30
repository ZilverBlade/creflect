#include <creflect/struct.h>
#include "util.hpp"
#include "spies.hpp"
#include <mocks.hpp>

struct MyStruct {
    void* first;
    int second;
    double third;
};

static std::vector<const char*> names{
    "first",
    "second",
    "third"
};

static std::vector<size_t> offsets{
    offsetof(MyStruct, first),
    offsetof(MyStruct, second),
    offsetof(MyStruct, third)
};
static const char* layout = "pid";

static crf_decorator GetDecorator(crf_context ctx) {
    crf_decorator_create_info decoratorInfo{};
    decoratorInfo.szMemberLayout = layout;
    decoratorInfo.pszMemberNames = names.data();
    decoratorInfo.pcbMemberOffsets = offsets.data();

    crf_decorator dec = crf_create_decorator(ctx, &decoratorInfo);
    EXPECT_NOT_NULL(dec);
    CRF_EXP_CTX_SUCCESS(ctx);
    return dec;
}
static crf_decorator GetDecoratorNoNames(crf_context ctx) {
    crf_decorator_create_info decoratorInfo{};
    decoratorInfo.szMemberLayout = layout;
    decoratorInfo.pszMemberNames = NULL;
    decoratorInfo.pcbMemberOffsets = offsets.data();

    crf_decorator dec = crf_create_decorator(ctx, &decoratorInfo);
    EXPECT_NOT_NULL(dec);
    CRF_EXP_CTX_SUCCESS(ctx);
    return dec;
}

TEST(Struct, CreateStructWithAlloc) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    crf_struct struc = crf_create_struct(ctx, decorator);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(Struct, CreateStructWithWrapperVerifySameRef) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};

    data.third = 4.2421531;

    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 0;
    EXPECT_EQ(crf_struct_member_get_value(struc, 2, &v), CRF_EC_SUCCESS);
    EXPECT_EQ(data.third, v);

    v = 59.2497194;
    EXPECT_EQ(crf_struct_member_set_value(struc, 2, &v), CRF_EC_SUCCESS);
    EXPECT_EQ(data.third, v);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(Struct, TestGetValueByNameSuccess) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};
    data.third = 4.2421531;
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 0;
    EXPECT_EQ(crf_struct_member_get_value_byname(struc, "third", &v), CRF_EC_SUCCESS);
    EXPECT_EQ(data.third, v);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(Struct, TestSetValueByNameSuccess) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 4.2421531;
    EXPECT_EQ(crf_struct_member_set_value_byname(struc, "third", &v), CRF_EC_SUCCESS);
    EXPECT_EQ(data.third, v);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(Struct, TestInvalidMemberNameSet) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 4.2421531;
    EXPECT_EQ(crf_struct_member_set_value_byname(struc, "blah", &v), CRF_EC_INDEX_OUT_OF_RANGE);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(Struct, TestMemberNameSetDecoratorHasNoMemberNames) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecoratorNoNames(ctx);

    MyStruct data{};
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 4.2421531;
    EXPECT_EQ(crf_struct_member_set_value_byname(struc, "third", &v), CRF_EC_INVALID_STATE);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(Struct, TestInvalidMemberNameGet) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 4.2421531;
    EXPECT_EQ(crf_struct_member_get_value_byname(struc, "blah", &v), CRF_EC_INDEX_OUT_OF_RANGE);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(Struct, TestMemberNameGetDecoratorHasNoMemberNames) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecoratorNoNames(ctx);

    MyStruct data{};
    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 4.2421531;
    EXPECT_EQ(crf_struct_member_get_value_byname(struc, "third", &v), CRF_EC_INVALID_STATE);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(Struct, TestStructGetOutOfRange) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};

    data.third = 4.2421531;

    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 0;
    EXPECT_EQ(crf_struct_member_get_value(struc, 3, &v), CRF_EC_INDEX_OUT_OF_RANGE);
    EXPECT_NE(data.third, v);

    v = 59.2497194;
    EXPECT_EQ(crf_struct_member_get_value(struc, -1, &v), CRF_EC_INDEX_OUT_OF_RANGE);
    EXPECT_NE(data.third, v);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(Struct, TestStructSetOutOfRange) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    crf_decorator decorator = GetDecorator(ctx);

    MyStruct data{};

    data.third = 4.2421531;

    crf_struct struc = crf_wrap_struct(ctx, decorator, &data);
    EXPECT_NOT_NULL(struc);
    CRF_EXP_CTX_SUCCESS(ctx);

    double v = 0;
    EXPECT_EQ(crf_struct_member_set_value(struc, 3, &v), CRF_EC_INDEX_OUT_OF_RANGE);
    EXPECT_NE(data.third, v);

    v = 59.2497194;
    EXPECT_EQ(crf_struct_member_set_value(struc, -1, &v), CRF_EC_INDEX_OUT_OF_RANGE);
    EXPECT_NE(data.third, v);

    crf_free_struct(ctx, struc);
    CRF_EXP_CTX_SUCCESS(ctx);
    crf_free_decorator(ctx, decorator);
    CRF_EXP_CTX_SUCCESS(ctx);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

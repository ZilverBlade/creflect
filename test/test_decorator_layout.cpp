#include <creflect/decorator.h>
#include "spies.hpp"
#include <util.hpp>

TEST(DecoratorLayout, DecoratorSize) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "cdfi";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 4);
    EXPECT_EQ(crf_decorator_get_size(decorator), CRF_MEMBER_SZ_CHAR + CRF_MEMBER_SZ_DOUBLE +
        CRF_MEMBER_SZ_FLOAT + CRF_MEMBER_SZ_INTEGER);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_CHAR);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 1), CRF_MEMBER_DOUBLE);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 2), CRF_MEMBER_FLOAT);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 3), CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, NullMemberString) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = NULL;
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, EmptyMemberString) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, BadMemberString) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "OI";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, BadMixMemberString) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "Iipfd";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, AllGoodMemberString) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "csilfdp";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeCharCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "c";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_CHAR);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeShortCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "s";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_SHORT);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, MemberTypeIntCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "i";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeLongCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "l";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_LONG);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeFloatCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "f";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_FLOAT);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeDoubleCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "d";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_DOUBLE);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypePointerCorrectness) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "p";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_POINTER);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, InvalidMemberIndex) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pip";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, -1), CRF_MEMBER_INVALID);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 3), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 2), CRF_MEMBER_INVALID);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, InvalidMemberOffset) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    std::vector<size_t> offsets{
        0,
        3 // int32 cannot be offset less than the previous member
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "ii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = offsets.data();
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, InvalidMemberOffsetSwappedPositions) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    std::vector<size_t> offsets{
        4,
        0 // int32 cannot be placed before the previous member
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "ii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = offsets.data();
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(DecoratorLayout, InvalidMemberOffsetIdentical) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    std::vector<size_t> offsets{
        4,
        4 // int32 cannot be placed in the same spot
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "ii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = offsets.data();
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, ValidMemberOffset) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    std::vector<size_t> offsets{ 1, 7 };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "ii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = offsets.data();
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 0), offsets[0]);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 1), offsets[1]);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, ValidMemberOffsetDifferenTypes) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();
    std::vector<size_t> offsets{ 0, 2, 10 };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "cli";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = offsets.data();
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 0), offsets[0]);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 1), offsets[1]);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 2), offsets[2]);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(DecoratorLayout, ValidMemberNoExplicitOffsetsStillAccurateOffsets) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "cilli";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 0), 0);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 1), 1);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 2), 5);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 3), 13);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 4), 21);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, InvalidMemberOffsetAccess) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "f";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;

    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, -1), CRF_INVALID_INDEX);
    EXPECT_EQ(crf_decorator_get_member_offset(decorator, 1), CRF_INVALID_INDEX);
    EXPECT_NE(crf_decorator_get_member_offset(decorator, 0), CRF_INVALID_INDEX);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, ValidNestedStructDecorator) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator decorator1, decorator2;
    {
        crf_decorator_create_info createInfo;
        createInfo.szMemberLayout = "fifi";
        createInfo.pszMemberNames = NULL;
        createInfo.pcbMemberOffsets = NULL;
        createInfo.pStructDecorators = NULL;

        decorator1 = crf_create_decorator(ctx, &createInfo);
        CRF_EXP_CTX_SUCCESS(ctx);
        EXPECT_NOT_NULL(decorator1);
    }
    {
        crf_decorator_create_info createInfo;
        createInfo.szMemberLayout = "S";
        createInfo.pszMemberNames = NULL;
        createInfo.pcbMemberOffsets = NULL;
        createInfo.pStructDecorators = &decorator1;

        decorator2 = crf_create_decorator(ctx, &createInfo);
        CRF_EXP_CTX_SUCCESS(ctx);
        EXPECT_NOT_NULL(decorator2);
    }
    EXPECT_EQ(crf_decorator_get_member_type(decorator2, 0), CRF_MEMBER_STRUCT);

    crf_free_decorator(ctx, decorator2);
    crf_free_decorator(ctx, decorator1);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, InvalidNestedStructDecoratorMissing) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "S";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;

    crf_decorator decorator2 = crf_create_decorator(ctx, &createInfo);
    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_MISSING_ARG);
    EXPECT_NULL(decorator2);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

#include <decorator.h>
#include <gtest/gtest.h>

TEST(creflect, TestDecoratorSize) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "cdfi";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    EXPECT_TRUE(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 4);
    EXPECT_EQ(crf_decorator_get_size(decorator), CRF_MEMBER_SZ_CHAR + CRF_MEMBER_SZ_DOUBLE +
        CRF_MEMBER_SZ_FLOAT + CRF_MEMBER_SZ_INTEGER);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_CHAR);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 1), CRF_MEMBER_DOUBLE);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 2), CRF_MEMBER_FLOAT);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 3), CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestNullMemberString) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = NULL;
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestEmptyMemberString) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}


TEST(creflect, TestBadMemberString) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "OI";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_context_free(ctx);
}


TEST(creflect, TestBadMixMemberString) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "Iipfd";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_context_free(ctx);
}

TEST(creflect, TestAllGoodMemberString) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "csilfdp";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypeCharCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "c";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_CHAR);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypeShortCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "s";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_SHORT);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}


TEST(creflect, TestMemberTypeIntCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "i";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypeLongCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "l";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_LONG);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypeFloatCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "f";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_FLOAT);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypeDoubleCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "d";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_DOUBLE);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

TEST(creflect, TestMemberTypePointerCorrectness) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "p";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_POINTER);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}


TEST(creflect, TestInvalidMemberIndex) {
    crf_context ctx = crf_context_create();

    crf_decorator_create_info createInfo;
    createInfo.pczMemberLayout = "pip";
    createInfo.ppMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, -1), CRF_MEMBER_INVALID);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 3), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 2), CRF_MEMBER_INVALID);

    crf_free_decorator(ctx, decorator);
    crf_context_free(ctx);
}

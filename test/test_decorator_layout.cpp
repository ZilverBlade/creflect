#include <creflect/decorator.h>
#include <gtest/gtest.h>
#include "spies.hpp"

TEST(DecoratorLayout, DecoratorSize) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "cdfi";
    createInfo.pszMemberNames = NULL;
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
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, NullMemberString) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = NULL;
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, EmptyMemberString) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_num_members(decorator), 0);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, BadMemberString) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "OI";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, BadMixMemberString) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "Iipfd";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_FALSE(decorator);

    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, AllGoodMemberString) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "csilfdp";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeCharCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "c";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_CHAR);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeShortCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "s";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_SHORT);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, MemberTypeIntCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "i";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INTEGER);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeLongCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "l";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_LONG);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeFloatCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "f";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_FLOAT);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypeDoubleCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "d";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_DOUBLE);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

TEST(DecoratorLayout, MemberTypePointerCorrectness) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "p";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_POINTER);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}


TEST(DecoratorLayout, InvalidMemberIndex) {
    crf_context ctx = crf_CreateContextWithAllocObserver();

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pip";
    createInfo.pszMemberNames = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_TRUE(decorator);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, -1), CRF_MEMBER_INVALID);
    EXPECT_EQ(crf_decorator_get_member_type(decorator, 3), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 0), CRF_MEMBER_INVALID);
    EXPECT_NE(crf_decorator_get_member_type(decorator, 2), CRF_MEMBER_INVALID);

    crf_free_decorator(ctx, decorator);
    crf_FreeContextAndVerify(ctx);
}

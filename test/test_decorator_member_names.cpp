#include <creflect/decorator.h>
#include "util.hpp"
#include "spies.hpp"
#include <vector>

TEST(DecoratorMemberNames, SingleName) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    std::vector<const char*> names{
        "pBufferData"
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "p";
    createInfo.pszMemberNames = names.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 1);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(DecoratorMemberNames, TwoNames) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    std::vector<const char*> names{
        "pBufferData",
        "pUserData",
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pp";
    createInfo.pszMemberNames = names.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 2);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(DecoratorMemberNames, InvalidDuplicate) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    std::vector<const char*> names{
        "pBufferData",
        "pBufferData"
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pp";
    createInfo.pszMemberNames = names.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_INVALID_ARG);
    EXPECT_NULL(decorator);

    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}
TEST(DecoratorMemberNames, CorrectIndices) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    std::vector<const char*> names{
        "pBufferData",
        "cBufferSize",
        "pUserData"
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pip";
    createInfo.pszMemberNames = names.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 3);
    EXPECT_EQ(crf_decorator_get_member_index(decorator, names[0]), 0);
    EXPECT_EQ(crf_decorator_get_member_index(decorator, names[1]), 1);
    EXPECT_EQ(crf_decorator_get_member_index(decorator, names[2]), 2);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

TEST(DecoratorMemberNames, InvalidIndices) {
    crf_context ctx = MallocSpyVerifyMemory::CreateContextWithAllocObserver();

    std::vector<const char*> names{
        "pBufferData",
        "cBufferSize",
        "pUserData"
    };

    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "pip";
    createInfo.pszMemberNames = names.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    CRF_EXP_CTX_SUCCESS(ctx);
    EXPECT_NOT_NULL(decorator);

    EXPECT_EQ(crf_decorator_get_num_members(decorator), 3);
    EXPECT_EQ(crf_decorator_get_member_index(decorator, "what"), CRF_INVALID_INDEX);
    /* not VB.net, these are NOT case sensitive */
    EXPECT_EQ(crf_decorator_get_member_index(decorator, "puserdata"), CRF_INVALID_INDEX);

    crf_free_decorator(ctx, decorator);
    MallocSpyVerifyMemory::FreeContextAndVerify(ctx);
}

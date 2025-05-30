#include <creflect/decorator.h>
#include <gtest/gtest.h>
#include "providers.hpp"

TEST(MemberNameValidator, InvalidEmptyName) {
    EXPECT_FALSE(crf_is_member_name_valid(""));
}
TEST(MemberNameValidator, InvalidNullName) {
    EXPECT_FALSE(crf_is_member_name_valid(NULL));
}
//
//RC_GTEST_PROP(MemberNameValidator, ValidAlphaName, ()) {
//    RC_ASSERT(crf_is_member_name_valid(ProvideAlphabeticString().c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, ValidAlphaNumName, ()) {
//    RC_ASSERT(crf_is_member_name_valid((ProvideValidAlphanumbericString()).c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, ValidAlphaUnderscName, ()) {
//    RC_ASSERT(crf_is_member_name_valid((ProvideValidMemberNameNoNumbers()).c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, ValidAnyName, ()) {
//    RC_ASSERT(crf_is_member_name_valid((ProvideValidMemberNameWithNumbers()).c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, InvalidLeadingNumberName, ()) {
//    RC_ASSERT_FALSE(crf_is_member_name_valid((ProvideNumericCharOnly() + ProvideAlphabeticString()).c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, InvalidName, ()) {
//    RC_ASSERT_FALSE(crf_is_member_name_valid((ProvideInvalidMemberName()).c_str()));
//}
//RC_GTEST_PROP(MemberNameValidator, InvalidName2, ()) {
//    RC_ASSERT_FALSE(crf_is_member_name_valid((ProvideValidInvalidNameMix()).c_str()));
//}
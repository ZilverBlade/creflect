#include <decorator.h>
#include <gtest/gtest.h>

TEST(MemberNameValidator, InvalidEmptyName) {
    EXPECT_FALSE(crf_is_member_name_valid(""));
}
TEST(MemberNameValidator, InvalidNullName) {
    EXPECT_FALSE(crf_is_member_name_valid(NULL));
}
TEST(MemberNameValidator, InvalidSpacedName) {
    EXPECT_FALSE(crf_is_member_name_valid("he llo"));
}
TEST(MemberNameValidator, InvalidLeadingNumberName) {
    EXPECT_FALSE(crf_is_member_name_valid("1thing"));
}
TEST(MemberNameValidator, InvalidHyphenName) {
    EXPECT_FALSE(crf_is_member_name_valid("-p-"));
}
TEST(MemberNameValidator, ValidTrailingNumberNameMin) {
    EXPECT_TRUE(crf_is_member_name_valid("thing0"));
}
TEST(MemberNameValidator, ValidTrailingNumberNameMax) {
    EXPECT_TRUE(crf_is_member_name_valid("thing9"));
}
TEST(MemberNameValidator, ValidUnderscoreName) {
    EXPECT_TRUE(crf_is_member_name_valid("hello_world"));
}
TEST(MemberNameValidator, ValidMixStressName) {
    EXPECT_TRUE(crf_is_member_name_valid("_az09AZ_"));
}
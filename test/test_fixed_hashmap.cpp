#include <creflect/hashmap.h>
#include "util.hpp"
#include "spies.hpp"

crf_fn_hash_code string_hashcode = [](const void* key_) -> size_t {
    if (key_ == NULL) return 0;
    const char* key = (char*)key_;
    int i = 0;
    size_t hash = (size_t)0xDEADBEEFUL;
    while (key[i] != '\0') {
        hash *= (size_t)536870911UL;
        hash ^= (size_t)(key[i]);
        ++i;
    }
    return hash;
};
crf_fn_hash_code string_bad_hashcode = [](const void* key_) -> size_t {
    return 0;
};
crf_fn_hash_code string_hashcode_bad2 = [](const void* key_) -> size_t {
    return 2;
};
crf_fn_equals string_equals = [](const void* key1, const void* key2) -> int {
    if (key1 == key2) return CRF_TRUE;
    if (key1 == NULL || key2 == NULL) return CRF_FALSE;
    return strcmp((char*)key1, (char*)key2) == 0;
};

static crf_allocator_table allocator = { crf_MallocSpy, crf_FreeSpy, crf_ReallocSpy };

TEST(FixedHashMap, ValidInitialisation) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, InvalidAccess) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NULL(crf_fixed_hashmap_get_ptr(hashmap, "hello"));
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, ValidAccessAdd) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    EXPECT_NOT_NULL(crf_fixed_hashmap_get(char*, hashmap, "hello"));
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, ValidAccessRemove) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    EXPECT_TRUE(crf_fixed_hashmap_remove(hashmap, "hello"));
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, ValidAccessCollission) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_bad_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_STREQ((char*)crf_fixed_hashmap_insert(hashmap, "hello", "test"), "test");
    EXPECT_STREQ((char*)crf_fixed_hashmap_insert(hashmap, "what?", "another test"), "another test");
    EXPECT_STREQ(crf_fixed_hashmap_get(char*, hashmap, "hello"), "test");
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}

TEST(FixedHashMap, OverFillNull) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 2, string_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "what?", "another test"));
    EXPECT_NULL(crf_fixed_hashmap_insert(hashmap, "test?", "another test again"));
    EXPECT_STREQ(crf_fixed_hashmap_get(char*, hashmap, "hello"), "test");
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, ExistingKeyInsert) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 10, string_bad_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    char* result = (char*)crf_fixed_hashmap_insert(hashmap, "hello", "another test");
    EXPECT_STREQ(result, "another test");
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}

TEST(FixedHashMap, ValidAccessCollissionEndOfArray) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 3, string_bad_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "what?", "another test"));
    EXPECT_STREQ(crf_fixed_hashmap_get(char*, hashmap, "hello"), "test");
    EXPECT_STREQ(crf_fixed_hashmap_get(char*, hashmap, "what?"), "another test");
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
TEST(FixedHashMap, ValidAccessCollissionAfterRemoval) {
    crf_fixed_hashmap hashmap = crf_create_fixed_hashmap(&allocator, 3, string_bad_hashcode, string_equals);
    EXPECT_NOT_NULL(hashmap);
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "hello", "test"));
    EXPECT_NOT_NULL(crf_fixed_hashmap_insert(hashmap, "what?", "another test"));
    EXPECT_TRUE(crf_fixed_hashmap_remove(hashmap, "hello"));
    EXPECT_STREQ(crf_fixed_hashmap_get(char*, hashmap, "what?"), "another test");
    crf_free_fixed_hashmap(&allocator, hashmap);
    EXPECT_EQ(crf_GetNumDanglingPointers(), 0);
}
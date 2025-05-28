#include "stubs.hpp"
#include "util.hpp"
#include "mocks.hpp"
#include <vector>

#include <creflect/hashmap.h>
#include <creflect/decorator.h>

TEST(AllocationErrHandler, TestCustomAllocator) {
    crf_context ctx = crf_create_context();
    MockProgrammableMalloc oddMalloc{};
    oddMalloc
        .MallocByDefault(true)
        .ReallocByDefault(true);

    EXPECT_CALL(oddMalloc, Malloc(_))
        .Times(AtLeast(1));
    EXPECT_CALL(oddMalloc, Free(_))
        .Times(AtLeast(1));

    crf_allocator_table table = oddMalloc.BindAllocator();
    crf_context_set_allocator(ctx, &table);
    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    crf_free_decorator(ctx, decorator);

    crf_free_context(ctx);
}

TEST(AllocationErrHandler, CreateDecoratorNullMalloc) {
    crf_context ctx = crf_create_context();
    MockNullMalloc nullMalloc{};

    EXPECT_CALL(nullMalloc, Malloc(_))
        .Times(Exactly(1))
        .WillOnce(Return(nullptr));

    crf_allocator_table table = nullMalloc.BindAllocator();
    crf_context_set_allocator(ctx, &table);
    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);
    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_ALLOCATION_ERROR);
    EXPECT_NULL(decorator);

    crf_free_context(ctx);
}


TEST(AllocationErrHandler, CreateDecoratorFailedMallocAfterOnce) {
    crf_context ctx = crf_create_context();
    MockProgrammableMalloc oddMalloc{};
    oddMalloc
        .MallocByDefault(true)
        .WhenMallocAttempt(1, false);

    EXPECT_CALL(oddMalloc, Malloc(_))
        .Times(Exactly(1))
        .WillOnce(ReturnNull());

    EXPECT_CALL(oddMalloc, Free(_))
        .Times(AtLeast(1));

    crf_allocator_table table = oddMalloc.BindAllocator();
    crf_context_set_allocator(ctx, &table);
    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = NULL;
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;

    // needs to malloc for BOTH the decorator and the member types
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_ALLOCATION_ERROR);
    EXPECT_NULL(decorator);

    crf_free_context(ctx);
}


TEST(AllocationErrHandler, CreateDecoratorFailedMallocAfterTwice) {
    crf_context ctx = crf_create_context();
    MockProgrammableMalloc oddMalloc{};
    oddMalloc
        .MallocByDefault(true)
        .WhenMallocAttempt(2, false);

    EXPECT_CALL(oddMalloc, Malloc(_))
        .Times(Exactly(3))
        .WillOnce(ReturnNull());

    EXPECT_CALL(oddMalloc, Free(_))
        .Times(AtLeast(2));

    std::vector<const char*> memberNames{
        "m1", "m2", "m3", "m4"
    };
    crf_allocator_table table = oddMalloc.BindAllocator();
    crf_context_set_allocator(ctx, &table);
    crf_decorator_create_info createInfo;
    createInfo.szMemberLayout = "iiii";
    createInfo.pszMemberNames = memberNames.data();
    createInfo.pcbMemberOffsets = NULL;
    createInfo.pStructDecorators = NULL;

    // needs to malloc for BOTH the decorator and the member types
    crf_decorator decorator = crf_create_decorator(ctx, &createInfo);

    EXPECT_EQ(crf_context_get_last_error(ctx), CRF_EC_ALLOCATION_ERROR);
    EXPECT_NULL(decorator);

    crf_free_context(ctx);
}


TEST(AllocationErrHandler, CreateHashMapNullAllocator) {
    MockNullMalloc nullMalloc{};

    EXPECT_CALL(nullMalloc, Malloc(_))
        .Times(Exactly(1))
        .WillOnce(ReturnNull());

    crf_allocator_table table = nullMalloc.BindAllocator();
    // needs to malloc for BOTH the decorator and the member types
    crf_fixed_hashmap map = crf_create_fixed_hashmap(&table, 0, [](const void*) {return size_t(); }, [](const void*, const void*) {return int(); });
    EXPECT_NULL(map);
}

TEST(AllocationErrHandler, CreateHashMapOddAllocator) {
    MockProgrammableMalloc oddMalloc{};
    oddMalloc
        .MallocByDefault(true)
        .WhenMallocAttempt(1, false);

    EXPECT_CALL(oddMalloc, Malloc(_))
        .Times(Exactly(2))
        .WillOnce(ReturnNull());

    EXPECT_CALL(oddMalloc, Free(_))
        .Times(AtLeast(1));
    crf_allocator_table table = oddMalloc.BindAllocator();
    // needs to malloc for BOTH the map and the entries
    crf_fixed_hashmap map = crf_create_fixed_hashmap(&table, 0, [](const void*) {return size_t(); }, [](const void*, const void*) {return int(); });
    EXPECT_NULL(map);

}

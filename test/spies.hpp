#pragma once
#include <creflect/context.h>
#include <cstdlib>
#include <unordered_set>
#include <gtest/gtest.h>
#include "stubs.hpp"

class MallocSpyVerifyMemory : public AbstractMalloc {
public:
    virtual void* Malloc(size_t size) override {
        void* ptr = malloc(size);
        if (ptr) allocatedPtrs.insert(ptr);
        return ptr;
    }
    virtual void  Free(void* ptr) override {
        free(ptr);
        if (allocatedPtrs.find(ptr) != allocatedPtrs.end()) {
            allocatedPtrs.erase(ptr);
        }
    }
    virtual void* Realloc(void* ptr, size_t size) override {
        void* newPtr = realloc(ptr, size);
        if (newPtr) {
            allocatedPtrs.insert(newPtr);
            if (allocatedPtrs.find(ptr) != allocatedPtrs.end()) {
                allocatedPtrs.erase(ptr);
            }
        }
        return newPtr;
    }

    int GetNumDanglingPointers() {
        return allocatedPtrs.size();
    }

    static crf_context CreateContextWithAllocObserver() {
        crf_context ctx = crf_create_context();
        crf_allocator_table table = CreateAllocObserver();
        crf_context_set_allocator(ctx, &table);
        return ctx;
    }
    static crf_allocator_table CreateAllocObserver() {
        MallocSpyVerifyMemory* spy = new MallocSpyVerifyMemory();
        crf_allocator_table table = spy->BindAllocator();
        return table;
    }
    static void FreeAllocObserverAndVerify(const crf_allocator_table* table) {
        MallocSpyVerifyMemory* spy = reinterpret_cast<MallocSpyVerifyMemory*>(table->pUserData);
        EXPECT_EQ(spy->GetNumDanglingPointers(), 0);
        delete spy;
    }
    static void FreeContextAndVerify(crf_context ctx) {
        MallocSpyVerifyMemory* spy = reinterpret_cast<MallocSpyVerifyMemory*>(crf_context_get_allocator(ctx)->pUserData);
        crf_free_context(ctx);
        EXPECT_EQ(spy->GetNumDanglingPointers(), 0);
        delete spy;
    }
private:
    std::unordered_set<void*> allocatedPtrs;
};
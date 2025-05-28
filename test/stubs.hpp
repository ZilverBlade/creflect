#pragma once
#include <cstddef>
#include <cstdlib>
#include <creflect/allocator.h>

class AbstractMalloc {
public:
    virtual void* Malloc(size_t size) = 0;
    virtual void  Free(void* ptr) = 0;
    virtual void* Realloc(void* ptr, size_t size) = 0;

    crf_allocator_table BindAllocator() {
        crf_allocator_table table{};
        table.pfnMalloc = [](size_t sz, void* self) -> void* {
            return reinterpret_cast<AbstractMalloc*>(self)->Malloc(sz);
        };
        table.pfnRealloc = [](void* ptr, size_t sz, void* self) -> void* {
            return reinterpret_cast<AbstractMalloc*>(self)->Realloc(ptr, sz);
        };
        table.pfnFree = [](void* ptr, void* self) {
            reinterpret_cast<AbstractMalloc*>(self)->Free(ptr);
        };
        table.pUserData = this;
        return table;
    }
};
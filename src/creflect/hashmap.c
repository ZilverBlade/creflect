#include "hashmap.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

enum hashmap_entry_state {
    HASHMAP_NULL,
    HASHMAP_TOMBSTONE,
    HASHMAP_FULL,
};

typedef struct crf_hashmap_entry {
    char eState;
    void* pValue;
    const void* pKey;
} crf_hashmap_entry;

typedef struct crf_fixed_hashmap_t {
    crf_fn_hash_code pfnHashCode;
    crf_fn_equals pfnEquals;
    size_t cSize;
    crf_hashmap_entry* pEntries;
} crf_fixed_hashmap_t;

static size_t hash_compression(size_t hash, size_t max) {
    return (hash) % max;
}
static void probe_linearly_idx(crf_fixed_hashmap map, size_t* index, const void* key, crf_bool returnEmptyEntries) {
    assert(map);
    
    for (size_t i = 0; i < map->cSize; ++i) {
        crf_hashmap_entry* pEntry = map->pEntries + *index;
        switch (pEntry->eState) {
        case HASHMAP_NULL:
            if (!returnEmptyEntries) {
                *index = CRF_INVALID_INDEX;
            }
            return;
        case HASHMAP_FULL:
            if (map->pfnEquals(key, pEntry->pKey)) {
                return;
            }
            break;
        case HASHMAP_TOMBSTONE:
            if (returnEmptyEntries) {
                return;
            }
            break;
        }
        size_t nextIdx = (*index) + 1;
        // modulo wrap around
        if (nextIdx >= map->cSize) {
            nextIdx = 0;
        }
        *index = nextIdx;
    }
    *index = CRF_INVALID_INDEX;
}
static size_t get_fixedhashmap_entry_index(crf_fixed_hashmap map, const void* key, crf_bool returnEmptyEntries) {
    assert(map);
    size_t hash = map->pfnHashCode(key);
    size_t index = hash_compression(hash, map->cSize);
    probe_linearly_idx(map, &index, key, returnEmptyEntries);
    return index;
}

crf_fixed_hashmap crf_create_fixed_hashmap(const crf_allocator_table* pAllocator, size_t cSize, const crf_fn_hash_code pfnHashCode, const crf_fn_equals pfnEquals) {
    assert(pAllocator && "don't pass a null allocator");
    assert(pfnHashCode && pfnEquals && "don't pass null functions for allocators and hashcodes");

    crf_fixed_hashmap map = (crf_fixed_hashmap)pAllocator->pfnMalloc(sizeof(crf_fixed_hashmap_t));
    if (!map) return NULL;

    map->cSize = cSize;
    map->pfnEquals = pfnEquals;
    map->pfnHashCode = pfnHashCode;
    map->pEntries = pAllocator->pfnMalloc(cSize * sizeof(crf_hashmap_entry));
    if (!map->pEntries) {
        pAllocator->pfnFree(map);
        return NULL;
    }
    memset(map->pEntries, 0, cSize * sizeof(crf_hashmap_entry));
    return map;
}

void crf_free_fixed_hashmap(const crf_allocator_table* pAllocator, crf_fixed_hashmap map) {
    assert(pAllocator && "don't pass a null allocator");
    if (map) {
        pAllocator->pfnFree(map->pEntries);
    }
    pAllocator->pfnFree(map);
}

void* crf_fixed_hashmap_insert(crf_fixed_hashmap map, const void* key, void* value) {
    assert(map && "don't pass a null map");

    size_t index = get_fixedhashmap_entry_index(map, key, CRF_TRUE);
    if (index == CRF_INVALID_INDEX) { //overfilled!
        return NULL;
    }
    crf_hashmap_entry* pEntry = map->pEntries + index;
    if (pEntry->eState != HASHMAP_FULL) {
        pEntry->eState = HASHMAP_FULL;
        pEntry->pKey = key;
    }
    pEntry->pValue = value;
    return value;
}

crf_bool crf_fixed_hashmap_remove(crf_fixed_hashmap map, const void* key) {
    assert(map && "don't pass a null map");
    
    size_t index = get_fixedhashmap_entry_index(map, key, CRF_FALSE);
    if (index == CRF_INVALID_INDEX) { // doesn't exist
        return CRF_FALSE;
    }
    crf_hashmap_entry* pEntry = map->pEntries + index;
    pEntry->eState = HASHMAP_TOMBSTONE;
    return CRF_TRUE;
}

void* crf_fixed_hashmap_get_ptr(crf_fixed_hashmap map, const void* key) {
    assert(map && "don't pass a null map");
    
    size_t index = get_fixedhashmap_entry_index(map, key, CRF_FALSE);
    if (index == CRF_INVALID_INDEX) { //overfilled!
        return NULL;
    }
    crf_hashmap_entry* pEntry = map->pEntries + index;
    return pEntry->pValue;
}

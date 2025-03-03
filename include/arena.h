#ifndef _ARENA_H
#define _ARENA_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#define UNMAP_SUCCESS 0
#define MIN_REGION_SIZE_CHUNKS 1024

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct Region Region;
typedef struct Arena Arena;

struct Region
{
    Region* next;
    size_t capacity;
    size_t buff_ptr;
    uintptr_t buffer[];
};

struct Arena
{
    Region* begin;
    Region* end;
};

Region* region_alloc(size_t capacity)
{
    size_t size_bytes = sizeof(Region) + sizeof(uintptr_t) * capacity;
    Region* region = mmap(NULL, size_bytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (region == MAP_FAILED)
        return NULL;

    region->next     = NULL;
    region->capacity = capacity;
    region->buff_ptr = 0;

    return region;
}

int region_free(Region* r)
{
    size_t size_bytes = sizeof(Region) + sizeof(uintptr_t) * r->capacity;
    return munmap(r, size_bytes);
}

void* arena_alloc(Arena* a, size_t size_bytes)
{
    assert(size_bytes != 0 && "Amount of bytes to allocate has to be greater than zero.");

    size_t size_chunks = (size_bytes - 1 + sizeof(uintptr_t)) / sizeof(uintptr_t);

    if (a->end == NULL)
    {
        size_t capacity = MAX(MIN_REGION_SIZE_CHUNKS, size_chunks);
        Region* region  = region_alloc(capacity);

        if (region == NULL)
            return NULL;

        a->end   = region;
        a->begin = region;
    }

    Region* curr_region = a->begin;

    while (curr_region != NULL)
    {
        if (curr_region->buff_ptr + size_chunks < curr_region->capacity)
            break;

        curr_region = curr_region->next;
    }

    if (curr_region == NULL)
    {
        size_t capacity = MAX(MIN_REGION_SIZE_CHUNKS, size_chunks);
        curr_region     = region_alloc(capacity);

        if (curr_region == NULL)
            return NULL;

        a->end->next = curr_region;
        a->end       = curr_region;
    }

    void* result = &curr_region->buffer[curr_region->buff_ptr];
    curr_region->buff_ptr += size_chunks;

    return result;
}

int arena_free(Arena* a)
{
    Region* curr_region = a->begin;
    int result          = UNMAP_SUCCESS;

    while (curr_region != NULL)
    {
        Region* tmp = curr_region;
        curr_region = curr_region->next;
        result      = region_free(tmp);
    }
    return result;
}

#endif

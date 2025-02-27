#ifndef _ARENA_H
#define _ARENA_H

#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#define UNMAP_SUCCESS 0
#define MAX_REGION_SIZE 1024 * 8
#define MIN(x, y) (((x) > (y)) ? (x) : (y))

typedef struct Region Region;
typedef struct Arena Arena;

struct Region
{
    Region* next;
    size_t capacity;
    uintptr_t buffer[];
};

struct Arena
{
    Region* begin;
    Region* end;
};

Region* region_alloc(size_t capacity)
{
    size_t size    = sizeof(Region) + sizeof(uintptr_t) * capacity;
    Region* region = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (region == MAP_FAILED)
        return NULL;

    region->next     = NULL;
    region->capacity = capacity;

    return region;
}

int region_free(Region* r)
{
    size_t size = sizeof(Region) + sizeof(uintptr_t) * r->capacity;
    return munmap(r, size);
}

void* arena_alloc(Arena* a, size_t size)
{
    if (size == 0)
        return NULL;

    size_t capacity = MIN(MAX_REGION_SIZE, size);
    Region* region  = region_alloc(capacity);

    if (region == NULL)
        return NULL;

    if (a->end == NULL)
    {
        a->end   = region;
        a->begin = region;
    }
    else
    {
        a->end->next = region;
        a->end       = region;
    }
    return &a->end->buffer[0];
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

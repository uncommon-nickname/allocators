#ifndef _LINEAR_ALLOC
#define _LINEAR_ALLOC

#include <stddef.h>
#include <stdint.h>

#define LINEAR_MEMORY_SIZE 1024 * 10

typedef struct Linear Linear;

struct Linear
{
    size_t curr_idx;
    uintptr_t data[LINEAR_MEMORY_SIZE];
};

void* linear_alloc(Linear* l, size_t size)
{
    if (l->curr_idx + size > LINEAR_MEMORY_SIZE || size == 0)
        return NULL;

    void* mem = &l->data[l->curr_idx];
    l->curr_idx += size;

    return mem;
}

#endif

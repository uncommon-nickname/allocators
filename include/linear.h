#ifndef _LINEAR_ALLOC
#define _LINEAR_ALLOC

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define LINEAR_MEMORY_SIZE_CHUNKS 1024

typedef struct Linear Linear;

struct Linear
{
    size_t buff_ptr;
    uintptr_t buffer[LINEAR_MEMORY_SIZE_CHUNKS];
};

void* linear_alloc(Linear* l, size_t size_bytes)
{
    assert(size_bytes != 0 && "Amount of bytes to allocate has to be greater than zero.");

    size_t size_chunks = (size_bytes - 1 + sizeof(uintptr_t)) / sizeof(uintptr_t);

    if (l->buff_ptr + size_chunks > LINEAR_MEMORY_SIZE_CHUNKS)
        return NULL;

    void* mem = &l->buffer[l->buff_ptr];
    l->buff_ptr += size_chunks;

    return mem;
}

#endif

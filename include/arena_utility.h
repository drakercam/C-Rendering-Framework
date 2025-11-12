#ifndef ARENA_UTILITY_H
#define ARENA_UTILITY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stdint.h>

typedef struct
{
    void* start;
    void* current;
    void* end;
    
} Arena;

#define ALIGN(val, al) (((val) + (al) - 1) / (al)) * (al)
static inline size_t alignment = alignof(max_align_t);

static inline Arena Arena_Create(size_t capacity)
{   
    void* initial = malloc(capacity);

    Arena a;
    a.start = initial;
    a.current = initial;
    a.end = (char*)initial + capacity;  // ✅ cast to char* for pointer arithmetic

    return a;
}

static inline void Arena_Reset(Arena* a)
{
    a->current = a->start;
}

static inline void* Arena_Alloc(Arena* a, size_t size)
{
    if (!a)
        return NULL;

    size_t aligned_size = ALIGN(size, alignment);
    if ((char*)a->current + aligned_size >= (char*)a->end)  // ✅ cast for arithmetic
        return NULL;

    void* p = a->current;
    a->current = (char*)a->current + aligned_size;  // ✅ cast for arithmetic

    return p;
}

static inline void Arena_Free(Arena* a)
{
    free(a->start);
    a->start = NULL;
    a->current = NULL;
    a->end = NULL;
}

#endif

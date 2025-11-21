#ifndef ARENA_UTILITY_H
#define ARENA_UTILITY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    void* start;
    void* current;
    void* end;
    
} Arena;

typedef union 
{
    long long ll;
    long double ld;
    void* p;

} max_align_t_c99;

#define ALIGN_UP(x, a)  (((x) + ((a) - 1)) & ~((a) - 1))
static const size_t alignment = sizeof(max_align_t_c99);

static inline Arena Arena_Create(size_t capacity)
{   
    void* initial = malloc(capacity);

    Arena a;
    a.start = initial;
    a.current = initial;
    a.end = (char*)initial + capacity;

    return a;
}

static inline void Arena_Reset(Arena* a)
{
    a->current = a->start;
}

static inline void* Arena_Alloc(Arena* a, size_t size)
{
    if (!a) return NULL;

    size_t aligned_size = ALIGN_UP(size, alignment);

    if ((char*)a->current + aligned_size >= (char*)a->end)
        return NULL;

    void* p = a->current;
    a->current = (char*)a->current + aligned_size;

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
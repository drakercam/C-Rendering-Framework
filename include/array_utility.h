#ifndef ARRAY_UTILITY_H
#define ARRAY_UTILITY_H

#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <stddef.h>
#include "arena_utility.h"

typedef struct
{
    void* data;
    size_t element_size;
    size_t length;
    size_t capacity;
    arena* allocator;   // if NULL, use malloc/free

} array;

static inline array array_create(size_t element_size, size_t capacity, arena* allocator)
{
    array a;
    a.element_size = element_size;
    a.length = 0;
    a.capacity = capacity;
    a.allocator = allocator;

    if (allocator)
    {
        a.data = arena_alloc(allocator, a.element_size * a.capacity);
    }
    else
    {
        a.data = malloc(a.element_size * a.capacity);
    }

    if (!a.data)
    {
        fprintf(stderr, "Failed to allocate memory for vector\n");
        return (array){0};
    }

    return a;
}

#define array_create_T(T, capacity, allocator) array_create(sizeof(T), capacity, allocator);

static inline void array_free(array* a)
{
    if (!a) return;
    if (!a->allocator && a->data) free(a->data);
    a->data = NULL;
    a->capacity = 0;
    a->element_size = 0;
    a->length = 0;
}

static inline size_t array_size(const array* a)
{
    return a ? a->length : 0;
}

static inline void array_pushback(array* a, void* data)
{
    if (!a || !data)
    {
        fprintf(stderr, "vector or data is NULL\n");
        return;
    }

    if (a->length == a->capacity)
    {
        if (a->allocator)
        {
            fprintf(stderr, "Arena-backed vector capacity exceeded!\n");
            return;
        }

        a->capacity *= 2;
        void* temp = realloc(a->data, a->element_size * a->capacity);

        if (!temp)
        {
            fprintf(stderr, "Failed to reallocated memory for vector\n");
            return;
        }
        a->data = temp;
    }

    char* target = (char*)a->data + (a->length * a->element_size);
    memcpy(target, data, a->element_size);
    a->length += 1;
}

#define array_push_T(T, array, val) do { T tmp = (val); array_pushback((array), &tmp); } while(0)

static inline void array_popback(array* a, void* out)
{
    if (!a || a->length == 0)
    {
        fprintf(stderr, "Cannot pop from NULL vector or a vector of length 0");
        return;
    }

    a->length -= 1;
    if (out)
    {
        memcpy(out, (char*)a->data + a->length * a->element_size, a->element_size);
    }
}

#define array_pop_T(T, array, out) do { (array_popback(array, (void*)&out)); } while(0)

static inline void* array_get(const array* a, size_t index)
{
    if (!a || a->length == 0 || index >= a->length)
    {
        fprintf(stderr, "Unable to retrieve value at index %zu\n", index);
        return NULL;
    }

    char* target = (char*)a->data + (index *a->element_size);
    return target;
}

#define array_get_T(T, array, index) (*(T*)array_get((array), (index)))

#define array_set_T(T, array, index, val) ((*(T*)array_get((array), (index))) = (val))

static inline array array_copy(const array* src, arena* allocator)
{
    if (!src)
    {
        fprintf(stderr, "Source vector is NULL\n");
        return (array){0};
    }

    array copy = {0};
    copy.element_size = src->element_size;
    copy.length = src->length;
    copy.capacity = src->capacity;
    copy.allocator = allocator;

    if (allocator)
    {
        arena_alloc(allocator, copy.capacity * copy.element_size);
    }
    else
    {
        copy.data = malloc(copy.capacity * copy.element_size);
    }

    if (!copy.data)
    {
        fprintf(stderr, "Failed to allocated memory for copy of vector\n");
        return (array){0};
    }

    memcpy(copy.data, src->data, src->length * src->element_size);

    return copy;
}

#endif
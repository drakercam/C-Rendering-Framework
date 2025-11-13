#ifndef DYN_ARRAY_UTILITY_H
#define DYN_ARRAY_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include "arena_utility.h"

typedef struct
{
    void* data;
    size_t element_size;
    size_t length;
    size_t capacity;
    Arena* allocator;   // if NULL, use malloc/free

} DynArray;

static inline DynArray DynArray_Create(size_t element_size, size_t capacity, Arena* allocator)
{
    DynArray a;
    a.element_size = element_size;
    a.length = 0;
    a.capacity = capacity;
    a.allocator = allocator;

    if (allocator)
    {
        a.data = Arena_Alloc(allocator, a.element_size * a.capacity);
    }
    else
    {
        a.data = malloc(a.element_size * a.capacity);
    }

    if (!a.data)
    {
        fprintf(stderr, "Failed to allocate memory for vector\n");
        return (DynArray){0};
    }

    return a;
}

#define DynArray_Create_T(T, capacity, allocator) DynArray_Create(sizeof(T), capacity, allocator)

static inline void DynArray_Free(DynArray* a)
{
    if (!a) return;
    if (!a->allocator && a->data) free(a->data);
    a->data = NULL;
    a->capacity = 0;
    a->element_size = 0;
    a->length = 0;
}

static inline size_t DynArray_Size(const DynArray* a)
{
    return a ? a->length : 0;
}

static inline void DynArray_Push(DynArray* a, void* data)
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

#define DynArray_Push_T(T, array, val) do { T tmp = (val); DynArray_Push((array), &tmp); } while(0)

static inline void DynArray_Pop(DynArray* a, void* out)
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

#define DynArray_Pop_T(T, array, out_ptr) do { (DynArray_Pop(array, (void*)(out_ptr))); } while(0)

static inline void* DynArray_Get(const DynArray* a, size_t index)
{
    if (!a || index >= a->length)
    {
        fprintf(stderr, "Unable to retrieve value at index %zu\n", index);
        return NULL;
    }

    char* target = (char*)a->data + (index *a->element_size);
    return target;
}

#define DynArray_Get_T(T, array, index) (*(T*)DynArray_Get((array), (index)))

#define DynArray_Set_T(T, array, index, val) ((*(T*)DynArray_Get((array), (index))) = (val))

static inline DynArray DynArray_Copy(const DynArray* src, Arena* allocator)
{
    if (!src)
    {
        fprintf(stderr, "Source vector is NULL\n");
        return (DynArray){0};
    }

    DynArray copy = {0};
    copy.element_size = src->element_size;
    copy.length = src->length;
    copy.capacity = src->capacity;
    copy.allocator = allocator;

    if (allocator)
    {
        copy.data = Arena_Alloc(allocator, copy.capacity * copy.element_size);
    }
    else
    {
        copy.data = malloc(copy.capacity * copy.element_size);
    }

    if (!copy.data)
    {
        fprintf(stderr, "Failed to allocated memory for copy of vector\n");
        return (DynArray){0};
    }

    memcpy(copy.data, src->data, src->length * src->element_size);

    return copy;
}

#ifdef __cplusplus
}
#endif

#endif
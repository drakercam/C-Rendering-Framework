#ifndef DARRAY_UTILITY_H
#define DARRAY_UTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include "arena_utility.h"

typedef struct                                                                                      
{                                                                                                   
    void* data;                                                                                     
    size_t element_size;                                                                            
    size_t size;                                                                                    
    size_t capacity;                                                                                
    Arena* allocator;     
    size_t type_id;                                                                          
                                                                                                    
}DArray;                                                                                            
                                                                                                    
static inline DArray DArray_Create(size_t element_size, size_t capacity, Arena* allocator, size_t type_id)        
{                                                                                                   
    DArray a;                                                                                       
    a.element_size = element_size;                                                                  
    a.size = 0;      
    
    if (capacity == 0)
        capacity = 1;
    
    a.capacity = capacity;                                                                          
    a.allocator = allocator;        
    a.type_id = type_id;  
                                                                                                    
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
        return (DArray){0};                                                                         
    }                     

    return a;                                                                                                                                                                                                                                                                                                           
}         

#define TYPE_ID(T) ((size_t)(uintptr_t)&(T*)0)

#define DArray_Create_T(T, capacity, allocator) \
    DArray_Create(sizeof(T), (capacity), (allocator), TYPE_ID(T))

static inline void DArray_Free(DArray* a)
{
    if (!a) return;
    if (!a->allocator && a->data) free(a->data);
    a->data = NULL;
    a->capacity = 0;
    a->element_size = 0;
    a->size = 0;
    a->type_id = 0;
}

static inline size_t DArray_Size(const DArray* a)
{
    return a ? a->size : 0;
}

static inline void DArray_Push(DArray* a, const void* data, size_t type_id)
{
    if (!a || !data)
    {
        fprintf(stderr, "dynamic array or data is NULL\n");
        return;
    }

    if (a->element_size == 0)
    {
        fprintf(stderr, "Invalid Element size\n");
        return;
    }

    if (a->type_id != type_id)
    {
        fprintf(stderr, "Type mismatch while pushing to DArray!\n");
        return;
    }

    if (a->size == a->capacity)
    {
        if (a->allocator)
        {
            fprintf(stderr, "Arena-backed dynamic array capacity exceeded!\n");
            return;
        }

        a->capacity *= 2;
        void* temp = realloc(a->data, a->element_size * a->capacity);

        if (!temp)
        {
            fprintf(stderr, "DArray_Push: realloc failed\n");
            return;
        }
        a->data = temp;
    }

    char* target = (char*)a->data + (a->size * a->element_size);
    memcpy(target, data, a->element_size);
    a->size += 1;
}

#define DArray_Push_T(T, a_ptr, value)                     \
    do {                                               \
        T temp = (value);                              \
        DArray_Push((a_ptr), &temp, TYPE_ID(T));           \
    } while (0)

static inline void DArray_Pop(DArray* a, void* out)
{
    if (!a || a->size == 0)
    {
        fprintf(stderr, "Cannot pop from NULL dynamic array or a darray of length 0");
        return;
    }

    a->size -= 1;
    if (out)
    {
        memcpy(out, (char*)a->data + a->size * a->element_size, a->element_size);
    }
}

#define DArray_Pop_T(T, a_ptr, out_ptr) do { (DArray_Pop(a_ptr, (void*)(out_ptr))); } while(0)

static inline const void* DArray_Get(const DArray* a, size_t index)
{
    if (!a || index >= a->size)
    {
        fprintf(stderr, "Unable to retrieve value at index %zu\n", index);
        return NULL;
    }

    char* target = (char*)a->data + (index *a->element_size);
    return target;
}

#define DArray_Get_T(T, a_ptr, index) (*(T*)DArray_Get((a_ptr), (index)))

#define DArray_Set_T(T, a_ptr, index, val) ((*(T*)DArray_Get((a_ptr), (index))) = (val))

static inline DArray DArray_Copy(const DArray* src, Arena* allocator)
{
    if (!src)
    {
        fprintf(stderr, "Source vector is NULL\n");
        return (DArray){0};
    }

    DArray copy = {0};
    copy.element_size = src->element_size;
    copy.size = src->size;
    copy.capacity = src->capacity;
    copy.allocator = allocator;
    copy.type_id = src->type_id;

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
        return (DArray){0};
    }

    memcpy(copy.data, src->data, src->size * src->element_size);

    return copy;
}

#endif
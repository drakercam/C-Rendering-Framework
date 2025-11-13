#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arena_utility.h"

typedef struct
{
    char* data;
    size_t length;
    size_t capacity;
    Arena* allocator;   // IF NULL, use malloc/free

} String;

static inline void String_Append(String* str, const char* suffix)
{
    if (!str || suffix == NULL)
    {
        fprintf(stderr, "Cannot append NULL to a string or string is NULL\n");
        return;
    }

    if (str->capacity < strlen(suffix))
    {
        fprintf(stderr, "Size of suffix exceeds the capacity of the str, abort!\n");
        return;
    }

    size_t suffix_len = strlen(suffix);
    size_t needed = str->length + suffix_len + 1;

    // arena-based strings cannot grow dynamically
    if (needed > str->capacity)
    {
        if (str->allocator)
        {
            fprintf(stderr, "Arena-based string exceeded capacity!\n");
            return;
        }

        str->capacity = needed * 2;
        char* temp = (char*) realloc(str->data, sizeof(char) * str->capacity);

        if (!temp)
        {
            fprintf(stderr, "Memory reallocation failed for string\n");
            return;
        }

        str->data = temp;
    }

    memcpy(str->data + str->length, suffix, suffix_len);
    str->length += suffix_len;
    str->data[str->length] = '\0';
}

static inline String String_Create(size_t capacity, const char* c_str, Arena* allocator)
{
    String str;

    if (capacity == 0)
        capacity = 1;

    str.capacity = capacity;
    str.length = 0;

    if (allocator)
    {
        str.data = (char*)Arena_Alloc(allocator, capacity);
    }
    else
    {
        str.data = (char*) malloc(sizeof(char) * str.capacity);
    }

    if (!str.data)
    {
        fprintf(stderr, "Memory allocation failed for string\n");
        return (String){0};
    }

    if (c_str)
    {
        String_Append(&str, c_str);
    }

    return str;
}

static inline void String_Free(String* str)
{
    if (!str) 
        return;
    if (!str->allocator && str->data) 
        free(str->data);

    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

static inline size_t String_Length(const String* str)
{
    return str ? str->length : 0;
}

static inline void String_Clear(String* str)
{
    if (str && str->data)
    {
        str->data[0] = '\0';
        str->length = 0;
    }
}

static inline int String_Compare(const String* a, const String* b)
{
    if (!a || !b)
        return (a == b) ? 0 : (a ? 1 : -1);
        
    return strcmp(a->data, b->data);
}

static inline void String_Print(const String* str)
{
    if (str && str->data)
        fprintf(stdout, "%s\n", str->data);
}

static inline String String_Copy(const String* src, Arena* allocator)
{
    if (!src)
    {
        fprintf(stderr, "Source string is NULL\n");
        return (String){0};
    }

    String copy = {0};
    copy.length = src->length;
    copy.capacity = src->length + 1;
    copy.allocator = allocator;

    if (allocator)
    {
        copy.data = (char*)Arena_Alloc(allocator, copy.capacity);
    }
    else
    {
        copy.data = (char*) malloc(sizeof(char) * copy.capacity);
    }

    if (!copy.data)
    {
        fprintf(stderr, "Failed to allocate memory for copy of string\n");
        return (String){0};
    }

    memcpy(copy.data, src->data, copy.length + 1);

    return copy;
}

#ifdef __cplusplus
}
#endif

#endif
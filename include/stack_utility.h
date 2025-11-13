#ifndef STACK_UTILITY_H
#define STACK_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

#define STACK_DEFINE(type, name, capacity)                             \
typedef struct                                                         \
{                                                                      \
    type data[capacity];                                               \
    int top;                                                           \
                                                                       \
} name;                                                                \
                                                                       \
static inline void name##_Create(name* s) { s->top = 0; }              \
static inline int name##_Push(name* s, type val)                       \
{                                                                      \
    if (s->top < capacity) { s->data[s->top++] = val; return 1; }      \
    else { printf("Stack Overflow!\n"); return 0; }                    \
}                                                                      \
static inline type name##_Pop(name* s)                                 \
{                                                                      \
    return (s->top > 0) ? s->data[--s->top] : (type){0};               \
}                                                                      \
static inline type name##_Peek(const name* s)                          \
{                                                                      \
    return (s->top > 0) ? s->data[s->top-1] : (type){0};               \
}                                                                      \
static inline bool name##_IsEmpty(name* s)                             \
{                                                                      \
    return (s->top == 0) ? true : false;                               \
}                                                                      \

#ifdef __cplusplus
}
#endif

#endif
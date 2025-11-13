#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "string_utility.h"

static inline String File_Load(const char* name)
{
    String contents = String_Create(4096, NULL, NULL);

    FILE* file = fopen(name, "r");
    if (!file)
    {
        printf("Failed to open file: %s\n", name);
        return (String){0};
    }

    char line[512];

    while (fgets(line, sizeof(line), file))
    {
        String_Append(&contents, line);
    }

    fclose(file);

    return contents;
}

#ifdef __cplusplus
}
#endif

#endif
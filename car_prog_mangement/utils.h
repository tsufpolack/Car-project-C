#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#define CleanScreen() system("cls")
#else
#define CleanScreen() system("clear")
#endif

#define SAFE_FREE(ptr) do { if ((ptr) != NULL) {free(ptr); (ptr) = NULL;} } while(0)
#define ARRAY_REMOVE_SHIFT(array, index, count) do { \
    for (int i = (index); i < (count) - 1; i++) { \
        (array)[i] = (array)[i + 1]; \
    } \
} while(0)

void* safe_malloc(size_t size);
void* safe_realloc(void* ptr, size_t size);
char* safe_strdup(const char* s);
void safe_int_scanf(int* des);
void add_element_to_array(void** arr, size_t elementSize, int* numElements, int* maxElements, const void* newElement);
void remove_element_from_array(void** arr, size_t elementSize, int* numElements, int* maxElements, int index);
void save_string_bin(const char* data, FILE* file);
char* load_string_bin(FILE* file);

void clean_buffer();
char* get_safe_input();
#endif // UTILS_H

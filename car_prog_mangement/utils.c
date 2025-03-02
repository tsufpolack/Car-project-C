#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Function to safely allocate memory
void* safe_malloc(size_t size) {
    if(size == 0) return NULL; // Allocate at least 1 byte (avoid undefined behavior with malloc(0))
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Function to safely allocate memory for an array of elements
void* safe_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

// Function to safely duplicate a string
char* safe_strdup(const char* s) {
    char* new_str = _strdup(s);
    if (!new_str) {
        fprintf(stderr, "String duplication failed\n");
        exit(EXIT_FAILURE);
    }
    return new_str;
}

// Function to add a new element to a dynamic array
void add_element_to_array(void** arr, size_t elementSize, int* numElements, int* maxElements, const void* newElement) {
    if (*maxElements == 0) { // First allocation
        *maxElements = 1; // Start with space for 1 element
        *arr = safe_malloc(*maxElements * elementSize);
    }
    else if (*numElements == *maxElements) { // Need more space
        *maxElements *= 2; // Double the available space
        *arr = safe_realloc(*arr, *maxElements * elementSize);
    }

    // Copy the new element into the array
    void* dest = (char*)(*arr) + (*numElements * elementSize); // Calculate destination address

    memcpy(dest, newElement, elementSize); // Copy element into the array

    *numElements = *numElements + 1; // Increment the count of elements in the array
}

// Function to remove an element from a dynamic array
void remove_element_from_array(void** arr, size_t elementSize, int* numElements, int* maxElements, int index)
{
    	if (index < 0 || index >= *numElements) {
		fprintf(stderr, "Index out of bounds\n");
		exit(EXIT_FAILURE);
	}

	// Calculate the address of the element to be removed
	void* element = (char*)*arr + (index * elementSize);

	// Calculate the address of the element after the one to be removed
	void* nextElement = (char*)element + elementSize;

	// Calculate the number of bytes to be moved
	size_t numBytes = (*numElements - index - 1) * elementSize;

	// Move the elements after the one to be removed
	if (numBytes > 0) {
		memmove(element, nextElement, numBytes);
	}

	(*numElements)--; // Decrement the count of elements in the array
}

// Function to free a dynamic array
void save_string_bin(const char* data, FILE* file)
{
	int length = strlen(data);
	fwrite(&length, sizeof(int), 1, file);
	fwrite(data, sizeof(char), length, file);
}

// Function to save a string to a binary file
char* load_string_bin(FILE* file)
{
    int length;
	fread(&length, sizeof(int), 1, file);
	char* data = (char*)safe_malloc(length + 1);
	fread(data, sizeof(char), length, file);
	data[length] = '\0';
	return data;
}

// Function to load a string from a binary file
void clean_buffer()
{
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

// Function to clean the input buffer
char* get_safe_input()
{
    char* res = (char*)safe_malloc(16*sizeof(char));
    int max_size = 16;
    int i = 0;
    char c;
    while((c = getchar()) != '\n')
	{
		if(i == max_size)
		{
			max_size *= 2;
			res = (char*)safe_realloc(res, max_size*sizeof(char));
		}
		res[i++] = c;
	}
    if (i == max_size)
    {
        max_size *= 2;
        res = (char*)safe_realloc(res, max_size * sizeof(char));
    }
    res[i] = '\0';
    res = (char*)safe_realloc(res, (i+1)*sizeof(char));
    return res;
}

// Function to safely read a string from the input
void safe_int_scanf(int* des)
{
	while(scanf("%d", des) != 1)
	{
		clean_buffer();
		printf("Invalid input. Please enter an integer: ");
	}
}


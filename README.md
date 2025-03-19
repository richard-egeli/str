# String Library

A lightweight, flexible string manipulation library for C with a focus on memory safety and ease of use.

## Overview

This library provides a set of functions for working with strings in C, offering a more convenient and safer alternative to the standard C string functions. It implements a custom string type that keeps track of length and capacity, avoiding many common pitfalls associated with C strings.

## Features

- Memory-safe string operations
- Easy string creation and manipulation
- String length tracking (no need for `strlen()`)
- Automatic memory management
- Efficient string slicing
- String comparison

## API Reference

### String Creation

```c
/* Create a new empty string */
char* string_new(void);

/* Create a string with specific initial capacity */
char* string_alloc(size_t size);

/* Create a string from a standard C string */
char* string_from_raw(const char* str);

/* Create a copy of an existing string */
char* string_from(str);
```

### String Information

```c
/* Get the length of a string */
size_t string_length(str);
```

### String Manipulation

```c
/* Extract a portion of a string (start is inclusive, end is exclusive) */
char* string_slice(str, size_t start, size_t end);

/* Append a standard C string to a string */
ssize_t string_append_raw(base, const char* str);

/* Append a string to another string */
ssize_t string_append(base, str);
```

### String Comparison

```c
/* Check if two strings are equal */
bool string_equals(str1, str2);
```

### Memory Management

```c
/* Free a string and its associated memory */
void string_free(str);
```

## Usage Examples

### Basic Usage

```c
#include "string_lib.h"

int main() {
    // Create a new string
    char* greeting = string_new();
    
    // Append to the string
    string_append_raw(greeting, "Hello, ");
    string_append_raw(greeting, "world!");
    
    // Use the string
    printf("Greeting: %s\n", greeting);
    printf("Length: %zu\n", string_length(greeting));
    
    // Create a slice
    char* sub = string_slice(greeting, 0, 5);
    printf("Slice: %s\n", sub);
    
    // Free memory
    string_free(greeting);
    string_free(sub);
    
    return 0;
}
```

### String Manipulation

```c
char* text = string_from_raw("The quick brown fox");
char* animal = string_slice(text, 16, 19);  // "fox"
printf("%s\n", animal);  // Prints: fox

string_free(text);
string_free(animal);
```

### String Comparison

```c
char* str1 = string_from_raw("Hello");
char* str2 = string_from_raw("Hello");
char* str3 = string_from_raw("World");

if (string_equals(str1, str2)) {
    printf("str1 and str2 are equal\n");
}

if (!string_equals(str1, str3)) {
    printf("str1 and str3 are different\n");
}

string_free(str1);
string_free(str2);
string_free(str3);
```

## Implementation Details

The library uses a hidden header structure before each string to store metadata such as length and capacity. This allows the API to present simple C-style strings to the user while maintaining additional information internally.

```
Memory layout:
+----------------+----------------+
| string_t       | char data[]    |
| (header)       | (visible part) |
+----------------+----------------+
```

The string functions operate on this structure, but the user only sees and works with the character data portion.

## Notes

- All strings created by this library must be freed with `string_free()` to avoid memory leaks.
- String functions that may reallocate memory (like `string_append_raw`) can change the pointer value, so always use the returned value.
- The library uses `NULL` termination for compatibility with standard C string functions.


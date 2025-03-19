#ifndef STR_STRING_H_
#define STR_STRING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/**
 * Compares two strings for equality
 *
 * @param a1 Memory address of the first string
 * @param a2 Memory address of the second string
 * @return true if strings are identical, false otherwise
 */
bool __string_equals_impl(uintptr_t a1, uintptr_t a2);
#define string_equals(str1, str2) __string_equals_impl((uintptr_t)str1, (uintptr_t)str2)

/**
 * Creates a new string that is a subset of the original string
 *
 * @param addr Memory address of the source string
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return Newly allocated string containing the slice
 */
char* __string_slice_impl(uintptr_t addr, size_t start, size_t end);
#define string_slice(str, start, end) __string_slice_impl((uintptr_t)str, start, end)

/**
 * Returns the length of a string
 *
 * @param addr Memory address of the string
 * @return Length of the string in bytes (excluding null terminator)
 */
size_t __string_length_impl(uintptr_t addr);
#define string_length(str) __string_length_impl((uintptr_t)str)

/**
 * Appends a raw C string to a string
 *
 * @param base Pointer to the target string (may be reallocated)
 * @param str Raw C string to append
 * @return 0 on success, negative value on error
 */
ssize_t __string_append_raw_impl(char** base, const char* str);
#define string_append_raw(base, str) __string_append_raw_impl((char**)&base, str)

/**
 * Appends one string to another
 *
 * @param base Pointer to the target string (may be reallocated)
 * @param str String to append
 * @return 0 on success, negative value on error
 */
ssize_t __string_append_impl(char** base, const char* str);
#define string_append(base, str) __string_append_impl((char**)&base, str)

/**
 * Creates a new string as a copy of an existing string
 *
 * @param addr Memory address of the source string
 * @return Newly allocated string containing a copy of the source
 */
char* __string_from_impl(uintptr_t addr);
#define string_from(str) __string_from_impl((uintptr_t)str);

/**
 * Creates a new string from a raw C string
 *
 * @param str Raw C string to copy
 * @return Newly allocated string containing a copy of the source
 */
char* string_from_raw(const char* str);

/**
 * Frees a string and its associated memory
 *
 * @param addr Memory address of the string to free
 */
void __string_free_impl(uintptr_t addr);
#define string_free(str) __string_free_impl((uintptr_t)str)

/**
 * Allocates a new string with the specified capacity
 *
 * @param size Initial capacity in bytes (excluding header)
 * @return Newly allocated empty string with the specified capacity
 */
char* string_alloc(size_t size);

/**
 * Creates a new empty string with default capacity
 *
 * @return Newly allocated empty string
 */
char* string_new(void);

#endif  // STR_STR_H_

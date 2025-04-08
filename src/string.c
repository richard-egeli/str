#include "str/string.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct string {
    size_t capacity;
    size_t length;
    char data[];
} string_t;

bool __string_equals_impl(uintptr_t a1, uintptr_t a2) {
    assert((void*)a1 != NULL);
    assert((void*)a2 != NULL);

    string_t* s1 = (string_t*)(a1 - offsetof(string_t, data));
    string_t* s2 = (string_t*)(a2 - offsetof(string_t, data));
    assert(s1->length == strlen(s1->data) && "string length not matching!");
    assert(s2->length == strlen(s2->data) && "string length not matching!");

    if (s1->length != s2->length) {
        return false;
    }

    return strncmp(s1->data, s2->data, s1->length) == 0;
}

char* __string_slice_impl(uintptr_t addr, size_t start, size_t end) {
    assert((void*)addr != NULL);
    assert(start <= end);

    const string_t* base = (string_t*)(addr - offsetof(string_t, data));
    start                = start >= base->length ? base->length : start;
    end                  = end >= base->length ? base->length : end;
    assert(base->length == strlen(base->data) && "string length not matching!");

    const size_t size = end - start;
    string_t* str     = malloc(sizeof(string_t) + size + 1);
    if (str == NULL) {
        return NULL;
    }

    memcpy(str->data, base->data + start, size);
    str->capacity   = size + 1;
    str->length     = size;
    str->data[size] = '\0';

    assert(str->length == strlen(str->data) && "string length not matching!");
    return str->data;
}

size_t __string_length_impl(uintptr_t addr) {
    const string_t* const base = (string_t*)(addr - offsetof(string_t, data));
    assert(base->length == strlen(base->data) && "string length not matching!");
    return base->length;
}

ssize_t __string_append_raw_impl(char** base, const char* str) {
    assert(*base != NULL);
    assert(base != NULL);
    assert(str != NULL);

    string_t* s1            = (string_t*)((uintptr_t)(*base) - offsetof(string_t, data));
    const size_t str_size   = strlen(str);
    const size_t new_length = s1->length + str_size;
    assert(s1->length == strlen(s1->data) && "string length not matching!");

    if (new_length > s1->capacity) {
        string_t* ptr = realloc(s1, sizeof(string_t) + new_length + 1);
        if (ptr == NULL) {
            return -ENOMEM;
        }

        ptr->capacity = new_length + 1;
        *base         = ptr->data;
        s1            = ptr;
    }

    memcpy(s1->data + s1->length, str, str_size);
    s1->length += str_size;
    s1->data[s1->length] = '\0';
    return 0;
}

ssize_t __string_append_impl(char** base, const char* str) {
    assert(*base != NULL);
    assert(base != NULL);
    assert(str != NULL);

    const size_t offset     = offsetof(string_t, data);
    string_t* s1            = (string_t*)((uintptr_t)(*base) - offset);
    string_t* s2            = (string_t*)((uintptr_t)str - offset);
    const size_t new_length = s1->length + s2->length;
    assert(s1->length == strlen(s1->data) && "string length not matching!");
    assert(s2->length == strlen(s2->data) && "string length not matching!");

    if (new_length >= s1->capacity) {
        string_t* ptr = realloc(s1, sizeof(string_t) + new_length + 1);
        if (ptr == NULL) {
            return -ENOMEM;
        }

        ptr->capacity = new_length + 1;
        *base         = ptr->data;
        s1            = ptr;
    }

    memcpy(s1->data + s1->length, s2->data, s2->length);
    s1->length += s2->length;
    s1->data[s1->length] = '\0';
    return 0;
}

char* __string_from_impl(uintptr_t addr) {
    assert((void*)addr != NULL);

    string_t* str = (string_t*)(addr - offsetof(string_t, data));
    assert(str->length == strlen(str->data) && "string length not matching!");

    string_t* new = malloc(sizeof(string_t) + str->capacity);
    if (new == NULL) {
        return NULL;
    }

    memcpy(new, str, sizeof(string_t) + str->capacity);
    return new->data;
}

char* string_from_raw(const char* str) {
    assert(str != NULL);

    const size_t size = strlen(str);
    string_t* ptr     = calloc(1, sizeof(string_t) + size + 1);
    if (ptr == NULL) {
        return NULL;
    }

    ptr->capacity = size + 1;
    ptr->length   = size;
    memcpy(ptr->data, str, size);

    assert(ptr->length == strlen(ptr->data) && "string length not matching!");
    return ptr->data;
}

void __string_free_impl(uintptr_t str) {
    free((string_t*)(str - offsetof(string_t, data)));
}

char* string_alloc(size_t size) {
    string_t* str = calloc(1, sizeof(string_t) + size + 1);
    if (str == NULL) {
        return NULL;
    }

    str->capacity = size + 1;
    return str->data;
}

char* string_new(void) {
    string_t* str = calloc(1, sizeof(string_t) + 1);
    if (str == NULL) {
        return NULL;
    }

    str->capacity = 1;
    return str->data;
}

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unity.h>

#include "str/string.h"
#include "unity.h"
#include "unity_internals.h"

/* Test Fixtures */

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
}

/* Basic Tests */

static void test_string_create(void) {
    char* str = string_new();
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL(0, string_length(str));
    TEST_ASSERT_EQUAL_STRING("", str);
    string_free(str);
}

static void test_string_from(void) {
    const char* input = "Hello, World!";
    char* str         = string_from_raw(input);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL(strlen(input), string_length(str));
    TEST_ASSERT_EQUAL_STRING(input, str);
    string_free(str);
}

static void test_string_copy(void) {
    char* str1 = string_from_raw("Test String");
    char* str2 = string_from(str1);

    TEST_ASSERT_NOT_NULL(str2);
    TEST_ASSERT_EQUAL(string_length(str1), string_length(str2));
    TEST_ASSERT_EQUAL_STRING(str1, str2);

    string_free(str1);
    string_free(str2);
}

/* Append Tests */

static void test_string_append_raw_empty(void) {
    char* str          = string_new();
    const char* append = "Appended Text";

    int result         = string_append_raw(str, append);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(strlen(append), string_length(str));
    TEST_ASSERT_EQUAL_STRING(append, str);

    string_free(str);
}

static void test_string_append_raw(void) {
    char* str            = string_from_raw("Initial ");
    const char* append   = "Appended";
    const char* expected = "Initial Appended";

    int result           = string_append_raw(str, append);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(strlen(expected), string_length(str));
    TEST_ASSERT_EQUAL_STRING(expected, str);

    string_free(str);
}

static void test_string_append_empty(void) {
    char* str1 = string_from_raw("Base String");
    char* str2 = string_new();

    int result = string_append(str1, str2);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_STRING("Base String", str1);

    string_free(str1);
    string_free(str2);
}

static void test_string_append(void) {
    char* str1           = string_from_raw("First ");
    char* str2           = string_from_raw("Second");
    const char* expected = "First Second";

    int result           = string_append(str1, str2);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(strlen(expected), string_length(str1));
    TEST_ASSERT_EQUAL_STRING(expected, str1);

    // Verify str2 is unchanged
    TEST_ASSERT_EQUAL_STRING("Second", str2);

    string_free(str1);
    string_free(str2);
}

/* Edge Cases */

static void test_string_append_large(void) {
    char* str = string_new();
    char buffer[27];

    // Fill buffer with repeating pattern
    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = 'A' + i;
    }
    buffer[sizeof(buffer) - 1] = '\0';

    // Append multiple times to force reallocation
    for (int i = 0; i < 10; i++) {
        int result = string_append_raw(str, buffer);
        TEST_ASSERT_EQUAL(0, result);
    }

    // Verify first and last characters
    TEST_ASSERT_EQUAL('A', str[0]);
    TEST_ASSERT_EQUAL('Z', str[10 * (sizeof(buffer) - 1) - 1]);

    string_free(str);
}

static void test_string_null_checks(void) {
    char* str = string_new();

    // These should assert in debug mode, but we can at least test that
    // the valid cases don't crash in release mode
    string_append_raw(str, "Valid");
    string_free(str);
}

/* Stress Test */

static void test_string_append_many(void) {
    char* str            = string_new();
    const int iterations = 1000;

    for (int i = 0; i < iterations; i++) {
        char small_buffer[16];
        sprintf(small_buffer, "%d", i);
        int result = string_append_raw(str, small_buffer);
        TEST_ASSERT_EQUAL(0, result);
    }

    // Verify string contains what we expect
    TEST_ASSERT_TRUE(string_length(str) > 0);
    TEST_ASSERT_EQUAL('0', str[0]);

    string_free(str);
}

static void test_string_slice(void) {
    // Create a test string
    char* original = string_new();
    TEST_ASSERT_NOT_NULL(original);

    // Fill it with known content
    const char* test_content = "Hello, world! This is a test string.";
    int result               = string_append_raw(original, test_content);
    TEST_ASSERT_EQUAL(0, result);

    // Test case 1: Slice from middle
    char* slice1 = string_slice(original, 7, 12);
    TEST_ASSERT_NOT_NULL(slice1);
    TEST_ASSERT_EQUAL_STRING("world", slice1);
    TEST_ASSERT_EQUAL(5, string_length(slice1));

    // Test case 2: Slice from beginning
    char* slice2 = string_slice(original, 0, 5);
    TEST_ASSERT_NOT_NULL(slice2);
    TEST_ASSERT_EQUAL_STRING("Hello", slice2);

    // Test case 3: Slice to end
    char* slice3 = string_slice(original, 24, string_length(original));
    TEST_ASSERT_NOT_NULL(slice3);
    TEST_ASSERT_EQUAL_STRING("test string.", slice3);

    // Test case 4: Empty slice (start equals end)
    char* slice4 = string_slice(original, 5, 5);
    TEST_ASSERT_NOT_NULL(slice4);
    TEST_ASSERT_EQUAL_STRING("", slice4);
    TEST_ASSERT_EQUAL(0, string_length(slice4));

    // Test case 5: Out of bounds (start beyond length)
    char* slice5 = string_slice(original, 100, 105);
    TEST_ASSERT_NOT_NULL(slice5);
    TEST_ASSERT_EQUAL_STRING("", slice5);

    // Test case 6: End beyond length
    char* slice6 = string_slice(original, 10, 100);
    TEST_ASSERT_NOT_NULL(slice6);
    TEST_ASSERT_EQUAL_STRING("ld! This is a test string.", slice6);

    // Clean up
    string_free(original);
    string_free(slice1);
    string_free(slice2);
    string_free(slice3);
    string_free(slice4);
    string_free(slice5);
    string_free(slice6);
}

static void test_string_equals(void) {
    // Test case 1: Equal strings
    char* str1 = string_from_raw("Hello, world!");
    char* str2 = string_from_raw("Hello, world!");
    TEST_ASSERT_TRUE(string_equals(str1, str2));

    // Test case 2: Different strings, same length
    char* str3 = string_from_raw("Hello, World!");  // Capital 'W'
    TEST_ASSERT_FALSE(string_equals(str1, str3));

    // Test case 3: Different strings, different length
    char* str4 = string_from_raw("Hello");
    TEST_ASSERT_FALSE(string_equals(str1, str4));

    // Test case 4: Empty strings
    char* empty1 = string_new();
    char* empty2 = string_new();
    TEST_ASSERT_TRUE(string_equals(empty1, empty2));

    // Test case 5: Empty string and non-empty string
    TEST_ASSERT_FALSE(string_equals(empty1, str1));

    // Test case 6: String equals itself
    TEST_ASSERT_TRUE(string_equals(str1, str1));

    // Clean up
    string_free(str1);
    string_free(str2);
    string_free(str3);
    string_free(str4);
    string_free(empty1);
    string_free(empty2);
}

/* Main Function */

int main(void) {
    UNITY_BEGIN();

    // Basic tests
    RUN_TEST(test_string_create);
    RUN_TEST(test_string_from);
    RUN_TEST(test_string_copy);
    RUN_TEST(test_string_slice);
    RUN_TEST(test_string_equals);

    // Append tests
    RUN_TEST(test_string_append_raw_empty);
    RUN_TEST(test_string_append_raw);
    RUN_TEST(test_string_append_empty);
    RUN_TEST(test_string_append);

    // Edge cases
    RUN_TEST(test_string_append_large);
    RUN_TEST(test_string_null_checks);

    // Stress test
    RUN_TEST(test_string_append_many);

    return UNITY_END();
}

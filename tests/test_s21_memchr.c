#include <check.h>
#include <string.h>

#include "../string.h"

/**
 * test 1: test the s21_memchr function with a basic test where the character is
 * found in the string. it compares the result of s21_memchr and the standard
 * memchr function.
 */
START_TEST(test_memchr_basic_found) {
  char str[] = "Hello, World!";
  char c = 'o';
  void *result_s21 = s21_memchr(str, c, 13);
  void *result_og = memchr(str, c, 13);
  ck_assert_ptr_eq(result_s21, result_og);  // 'o' should be found at index 4
}
END_TEST

/**
 * test 2: test the s21_memchr function with a basic test where the character is
 * not found in the string. it compares the result of s21_memchr and the
 * standard memchr function, expecting NULL.
 */
START_TEST(test_memchr_basic_not_found) {
  char str[] = "Hello, World!";
  char c = 'z';
  void *result_s21 = s21_memchr(str, c, 13);
  void *result_og = memchr(str, c, 13);
  ck_assert_ptr_eq(result_s21, result_og);  // 'z' not found, should return NULL
}
END_TEST

/**
 * test 3: test the s21_memchr function with an empty string.
 * this ensures the function correctly returns NULL when searching in an empty
 * string.
 */
START_TEST(test_memchr_empty_string) {
  char str[] = "";
  char c = 'a';
  void *result_s21 = s21_memchr(str, c, 0);
  void *result_og = memchr(str, c, 0);
  ck_assert_ptr_eq(result_s21, result_og);  // empty string should return NULL
}
END_TEST

/**
 * test 4: test the s21_memchr function with n = 0 (search length is zero).
 * the function should return NULL as no search can be performed.
 */
START_TEST(test_memchr_zero_length) {
  char str[] = "Test";
  char c = 'T';
  void *result_s21 = s21_memchr(str, c, 0);
  void *result_og = memchr(str, c, 0);
  ck_assert_ptr_eq(result_s21, result_og);  // should return NULL for length 0
}
END_TEST

/**
 * test 5: test the s21_memchr function with n equal to the size of the string.
 * this test ensures the function searches the entire string.
 */
START_TEST(test_memchr_full_search) {
  char str[] = "Test";
  char c = 'T';
  void *result_s21 = s21_memchr(str, c, 4);
  void *result_og = memchr(str, c, 4);
  ck_assert_ptr_eq(result_s21,
                   result_og);  // should return pointer to 'T' at the start
}
END_TEST

/**
 * test 6: test the s21_memchr function where the character appears multiple
 * times in the string. the function should return the first occurrence.
 */
START_TEST(test_memchr_multiple_occurrences) {
  char str[] = "abcabc";
  char c = 'c';
  void *result_s21 = s21_memchr(str, c, 6);
  void *result_og = memchr(str, c, 6);
  ck_assert_ptr_eq(result_s21,
                   result_og);  // should return pointer to first 'c'
}
END_TEST

/**
 * test 7: test the s21_memchr function where the character is at the end of the
 * string. the function should correctly find the character at the end.
 */
START_TEST(test_memchr_end_of_string) {
  char str[] = "Test";
  char c = 't';
  void *result_s21 = s21_memchr(str, c, 4);
  void *result_og = memchr(str, c, 4);
  ck_assert_ptr_eq(result_s21,
                   result_og);  // should return pointer to 't' at the end
}
END_TEST

/**
 * test 8: test the s21_memchr function with large string and a character found
 * in it. this ensures the function works well with large strings.
 */
START_TEST(test_memchr_large_string) {
  char str[1000];
  memset(str, 'A', 999);
  str[999] = '\0';  // null-terminate the string
  char c = 'A';
  void *result_s21 = s21_memchr(str, c, 1000);
  void *result_og = memchr(str, c, 1000);
  ck_assert_ptr_eq(result_s21,
                   result_og);  // should return pointer to 'A' at the start
}
END_TEST

/**
 * test 9: test the s21_memchr function with n greater than the string's length.
 * this ensures that the function does not go out of bounds and handles large n
 * gracefully.
 */
START_TEST(test_memchr_large_n) {
  char str[] = "Hello";
  char c = 'e';
  void *result_s21 = s21_memchr(str, c, 1000);
  void *result_og = memchr(str, c, 1000);
  ck_assert_ptr_eq(
      result_s21,
      result_og);  // should still find 'e' within the string's length
}
END_TEST

Suite *s21_memchr_suite(void) {
  Suite *suite = suite_create("s21_memchr");
  TCase *tc_core = tcase_create("Core");

  // adding all tests to the test case group
  tcase_add_test(tc_core, test_memchr_basic_found);           // Test 1
  tcase_add_test(tc_core, test_memchr_basic_not_found);       // Test 2
  tcase_add_test(tc_core, test_memchr_empty_string);          // Test 3
  tcase_add_test(tc_core, test_memchr_zero_length);           // Test 4
  tcase_add_test(tc_core, test_memchr_full_search);           // Test 5
  tcase_add_test(tc_core, test_memchr_multiple_occurrences);  // Test 6
  tcase_add_test(tc_core, test_memchr_end_of_string);         // Test 7
  tcase_add_test(tc_core, test_memchr_large_string);          // Test 8
  tcase_add_test(tc_core, test_memchr_large_n);               // Test 9

  suite_add_tcase(suite, tc_core);

  return suite;
}

int main(void) {
  int number_failed;
  Suite *suite = s21_memchr_suite();
  SRunner *sr = srunner_create(suite);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}

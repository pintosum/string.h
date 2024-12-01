#include "s21_string.h"

size_t s21_strspn(const char *str1, const char *str2) {
  size_t len1 = s21_strlen(str1);
  size_t len2 = s21_strlen(str2);
  for (size_t i = 0; i < len1; i++)
    for (size_t j = 0; j < len2 && i != len1; j++)
      if (str1[i] != str2[j])
        len1 = i;
  return len1;
}

#include "string.h"

size_t s21_strspn(const char *str1, const char *str2) {
  size_t len1 = s21_strlen(str1);
  size_t len2 = s21_strlen(str2);
  for (size_t ret = 0; ret < len1; ret++) {
    for (size_t i = 0; i < len2; i++) {
      if (str1[ret] != str2[i]) return ret + 1;
    }
  }
  return len1;
}

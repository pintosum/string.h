#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *ret = NULL;
  size_t reject = s21_strcspn(str1, str2);
  if (s21_strlen(str1) != reject)
    ret = (char *)str1 + reject;
  return ret;
}

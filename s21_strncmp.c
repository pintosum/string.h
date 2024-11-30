#include "string.h"

int s21_strncmp(const char *str1, const char *str2, size_t n) {
  int ret = 0;
  while (n-- && *str1 && *str2 && *str1 == *str2) {
    str1++;
    str2++;
  }
  if (*str1 < *str2)
    ret = -1;
  else if (*str1 > *str2)
    ret = 1;
  return ret;
}
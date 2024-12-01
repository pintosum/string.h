#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, size_t n) {
  char *ret = dest;
  while (n--)
    *dest++ = *src++;
  return ret;
}

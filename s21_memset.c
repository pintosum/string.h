#include "string.h"

void *s21_memset(void *str, int c, size_t n) {
  char *p = (char *)str;
  while (n--) *p++ = c;
  return str;
}

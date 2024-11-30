#include "string.h"

void *s21_memchr(const void *s, int c, size_t n) {
  const unsigned char *ptr = s;
  while (n--)
    if (*ptr++ == (unsigned char)c) return (void *)(ptr - 1);
  return NULL;
}
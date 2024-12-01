#include "s21_string.h"

void *s21_memchr(const void *s, int c, size_t n) {
  const unsigned char *ptr = s;
  void *ret = NULL;
  while (n-- && !ret)
    if (*ptr++ == (unsigned char)c)
      ret = (void *)(ptr - 1);
  return ret;
}

#include "string.h"

int s21_memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  while (n--)
    if (*p1++ != *p2++) return *(p1 - 1) - *(p2 - 1);
  return 0;
}

#include "string.h"
void *memchr(const void *s, int c, size_t n) {
  unsigned char *ptr = (unsigned char *)s;
  while (n--)
    if (*ptr++ == (unsigned char)c)
      return (void *)(ptr - 1);
  return NULL;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char *p1 = (unsigned char *)s1;
  unsigned char *p2 = (unsigned char *)s2;
  while (n--)
    if (*p1++ != *p2++)
      return *(p1 - 1) - *(p2 - 1);
  return 0;
}

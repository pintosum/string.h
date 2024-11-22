#include "string.h"
void *memchr(const void *s, int c, size_t n) {
  const unsigned char *ptr = s;
  while (n--)
    if (*ptr++ == (unsigned char)c)
      return (void *)(ptr - 1);
  return NULL;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  while (n--)
    if (*p1++ != *p2++)
      return *(p1 - 1) - *(p2 - 1);
  return 0;
}

void *memcpy(void *dest, const void *src, size_t n){
  const unsigned char *s = src;
  unsigned char *d = dest;
  while(n--)
    *d++ = *s++;
  return (void *)d;
}

void *memmove(void *dest, const void *src, size_t n){
  void *cp = malloc(n);
  memcpy(cp, src, n);
  memcpy(dest, cp, n);
  free(cp);
  return dest;
}

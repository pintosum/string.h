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

void *memcpy(void *dest, const void *src, size_t n) {
  const unsigned char *s = src;
  unsigned char *d = dest;
  while (n--)
    *d++ = *s++;
  return (void *)d;
}

void *memmove(void *dest, const void *src, size_t n) {
  void *cp = malloc(n);
  memcpy(cp, src, n);
  memcpy(dest, cp, n);
  free(cp);
  return dest;
}

void *memset(void *str, int c, size_t n) {
  char *p = (char *)str;
  while (n--)
    *p++ = c;
  return (void *)p;
}

char *strcat(char *dest, const char *src){
  char *ret = dest;
  while(*dest)
    dest++;
  while(*src)
    *dest++ = *src++;
  return ret;
}

char *strncat(char *dest, const char *src, size_t n){
  char *ret = dest;
  while(*dest)
    dest++;
  while(n--)
    *dest++ = *src++;
  return ret;
}

char *strchr(const char *str, int c){
  while(*str != 0 && *str != c)
    str++;
  return (char *)str;
}

int strcmp(const char *str1, const char *str2){
  int ret = 0;
  while(*str1 && *str2 && *str1 == *str2){
    str1++;
    str2++;
  }
  if (*str1 < *str2) ret = -1;
  else if(*str1 > *str2) ret = 1;
  return ret;
}

int strncmp(const char *str1, const char *str2, size_t n){
  int ret = 0;
  while(n-- && *str1 && *str2 && *str1 == *str2){
    str1++;
    str2++;
  }
  if (*str1 < *str2) ret = -1;
  else if(*str1 > *str2) ret = 1;
  return ret;

}

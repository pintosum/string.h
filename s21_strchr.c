#include "string.h"

char *s21_strchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  while (len--)
    if (*str++ == c) return (char *)str;
  return NULL;
}

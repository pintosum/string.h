#include "string.h"

char *s21_strrchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  while (len--)
    if (str[len] == c) return (char *)str + len;
  return NULL;
}
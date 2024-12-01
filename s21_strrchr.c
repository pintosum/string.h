#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  char *ret = NULL;
  while (len--)
    if (str[len] == c)
      ret = (char *)str + len;
  return ret;
}

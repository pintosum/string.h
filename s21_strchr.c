#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  char *ret = NULL;
  while (len-- && !ret)
    if (*str++ == c)
      ret = (char *)--str;
  return ret;
}

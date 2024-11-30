#include "string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *prev;
  char *ret;
  if (!str) str = prev;
  str += s21_strspn(str, delim);
  if (*str == 0) {
    prev = str;
    ret = NULL;
  } else {
    ret = str;
    str += s21_strcspn(str, delim);
    if (*str == 0)
      prev = str;
    else {
      *str = 0;
      prev = str + 1;
    }
  }
  return ret;
}
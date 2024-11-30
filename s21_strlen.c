#include "string.h"

size_t s21_strlen(const char *str) {
  size_t ret = 0;
  while (*str++) ret++;
  return ret;
}
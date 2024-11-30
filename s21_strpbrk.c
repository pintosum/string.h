#include "string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  return (char *)(str1 + s21_strcspn(str1, str2));
}
#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  if (needle) {
    int len1 = s21_strlen(haystack);
    int len2 = s21_strlen(needle);
    for (int i = 0; i < len1; i++) {
      if (haystack[i] == needle[0]) {
        int j = 0;
        for (; i + j < len1 && j < len2; j++) {
          if (haystack[i + j] != needle[j])
            break;
        }
        if (j == len2)
          return (char *)haystack + i;
      }
    }
  }
  return (char *)haystack;
}

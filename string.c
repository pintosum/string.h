#include "string.h"
void *s21_memchr(const void *s, int c, size_t n) {
  const unsigned char *ptr = s;
  while (n--)
    if (*ptr++ == (unsigned char)c)
      return (void *)(ptr - 1);
  return NULL;
}

int s21_memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  while (n--)
    if (*p1++ != *p2++)
      return *(p1 - 1) - *(p2 - 1);
  return 0;
}

void *s21_memcpy(void *dest, const void *src, size_t n) {
  const unsigned char *s = src;
  unsigned char *d = dest;
  while (n--)
    *d++ = *s++;
  return dest;
}

void *s21_memset(void *str, int c, size_t n) {
  char *p = (char *)str;
  while (n--)
    *p++ = c;
  return str;
}

char *s21_strncat(char *dest, const char *src, size_t n) {
  char *ret = dest;
  while (*dest)
    dest++;
  while (n--)
    *dest++ = *src++;
   *dest = '\0'; // Null-terminate
  return ret;
}

char *s21_strchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  while (len--)
    if (*str++ == c)
      return (char *)str;
  return NULL;
}

int s21_strncmp(const char *str1, const char *str2, size_t n) {
  int ret = 0;
  while (n-- && *str1 && *str2 && *str1 == *str2) {
    str1++;
    str2++;
  }
  if (*str1 < *str2)
    ret = -1;
  else if (*str1 > *str2)
    ret = 1;
  return ret;
}

char *s21_strncpy(char *dest, const char *src, size_t n) {
  char *ret = dest;
  while (n--)
    *dest++ = *src++;
  while (n--)
    *dest++ = '\0'; // Null-terminate if src is shorter
  return ret;
}

size_t s21_strcspn(const char *str1, const char *str2) {
  size_t len1 = s21_strlen(str1);
  size_t len2 = s21_strlen(str2);
  for (size_t ret = 0; ret < len1; ret++)
    for (size_t i = 0; i < len2; i++)
      if (str2[i] == str1[ret])
        return ret + 1;

  /* suggested 
  size_t ret = 0;
    while (str1[ret]) {
        if (s21_strchr(str2, str1[ret])) {
            return ret; // Return index of first occurrence
        }
        ret++;
    }
    return ret;
  */
  return len1;
}

char *s21_strerror(int errnum);

size_t s21_strlen(const char *str) {
  size_t ret = 0;
  while (*str++)
    ret++;
  return ret;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  return (char *)(str1 + s21_strcspn(str1, str2));
}

char *s21_strrchr(const char *str, int c) {
  size_t len = s21_strlen(str);
  while (len--)
    if (str[len] == c)
      return (char *)str + len;
  return NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  // return NULL if  meedle is an empty string
if (needle == NULL || *needle == '\0')
  return NULL;  
 
  size_t len1 = s21_strlen(haystack);
  size_t len2 = s21_strlen(needle);

for (size_t i = 0; i <= len1-len2; i++) // updated the loop that searches
  {
    size_t j = 0;
    while (j < len2 && haystack[i+j] == needle[j])
      {
        j++;
      }
    if (j == len2)
    {
          return (char *)(haystack + i);
    }
  }
  return NULL;
  /*
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
*/
}

char *s21_strtok(char *str, const char *delim) {
  static char *prev;
  char *ret;
  if (delim == NULL) // added proccessing of a case if delim is NULL
    ret = NULL;
  if (!str)
    str = prev;
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

size_t s21_strspn(const char *str1, const char *str2) {
  size_t len1 = s21_strlen(str1);
  size_t len2 = s21_strlen(str2);
  for (int ret = 0; ret < len1; ret++) {
    for (int i = 0; i < len2; i++) {
      if (str1[ret] != str2[i])
        return ret + 1;
    }
  }
  return len1;
}

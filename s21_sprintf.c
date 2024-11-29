#include "string.h"
#include <stdarg.h>

struct options {
  unsigned char spec;
  unsigned char flag;
  unsigned char len;
  unsigned char padding;
  int width;
  int precision;
};

static int s21_is_digit(const char *str) {
  int ret = 0;
  if (*str <= 57 && *str >= 48)
    ret = 1;
  return ret;
}

static int s21_atoi(const char **str) {
  int ret = 0;
  const char *ptr = *str;
  int done = 0;
  while (*ptr && !done) {
    if (s21_is_digit(ptr)) {
      ret = ret * 10 + *ptr - 48;
    } else {
      done = 1;
      *str = ptr;
    }
    ptr++;
  }
  return ret;
}

static const char *s21_get_spec(const char *format, struct options *options) {
  static struct options *opts;
  static const char *inp;
  if (format)
    inp = format;
  if (options)
    opts = options;

  s21_memset(opts, 0, sizeof(struct options));

  const char *spec = s21_strchr(inp, '%');
  if (spec) {
    int done = 0;
    while (*spec && !done) {
      if (*spec == ' ' || *spec == '-' || *spec == '+')
        opts->flag = *spec;
      else if (*spec == '.') {
        spec++;
        opts->precision = s21_atoi(&spec);
      } else if (s21_is_digit(spec)) {
        opts->width = s21_atoi(&spec);
      } else if (*spec == 'h' || *spec == 'l') {
        opts->len = *spec;
      } else if (*spec == 'c' || *spec == 'd' || *spec == 'f' || *spec == 's' ||
                 *spec == 'u') {
        opts->spec = *spec;
      } else {
        done = 1;
        inp = spec;
      }
      spec++;
    }
  }
  return inp;
}

static char *s21_sputch(char *str, int ch, struct options *opts) {
  if (opts->flag == '-')
    *str++ = ch;
  while (--opts->width)
    *str++ = opts->padding;
  if (opts->flag != '-')
    *str++ = ch;
  return str;
}

static char *s21_sputdec(int dec, char *dest, struct options *opts) {
  int i = 0;
  int sign;
  if ((sign = dec) < 0)
    dec = -dec;
  do {
    dest[i++] = dec % 10 + '0';
  } while ((dec /= 10) > 0);
  if (sign < 0) {
    dest[i++] = '-';
  } else if (opts->flag) {
    dest[i++] = opts->flag;
  }

  while (i < opts->width) {
    dest[i++] = opts->padding;
  }
  int last_index = i;

  i--;
  for (int j = 0; j < i; j++, i--) {
    char temp = dest[j];
    dest[j] = dest[i];
    dest[i] = temp;
  }

  return dest + last_index;
}

static void s21_put_spec(char *str, va_list *args, struct options *opts) {
  char c;
  int d;
  char *s;
  double f;
  unsigned int u;
  switch (opts->spec) {
  case 'c':
    c = va_arg(*args, int);
    *str = c;
    break;
  case 'd':
    d = va_arg(*args, int);
    break;
  case 'f':
    f = va_arg(*args, double);
    break;
  case 's':
    s = va_arg(*args, char *);
    break;
  case 'u':
    u = va_arg(*args, unsigned);
    break;
  default:
    break;
  }
}

int s21_sprintf(char *str, const char *format, ...) {
  int ret;
  const char *inp = format;
  va_list args;
  va_start(args, format);
  struct options opts;
  s21_get_spec(format, &opts);
  while (opts.spec) {
    size_t n = s21_strcspn(inp, "%");
    str = s21_memcpy(str, inp, n) + n - 1;
    s21_put_spec(str, &args, &opts);
    inp = s21_get_spec(NULL, NULL);
  }
  return ret;
}

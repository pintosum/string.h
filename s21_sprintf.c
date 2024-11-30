#include "string.h"
#include <stdarg.h>

struct options {
  unsigned char type;
  unsigned char flag_space;
  unsigned char flag_plus;
  unsigned char flag_minus;
  unsigned char flag_hash;
  unsigned char flag_zero;
  unsigned char len;
  unsigned char padding;
  int width;
  int precision;
};

static int s21_is_digit(const char *str) {
  int ret = 0;
  if (*str <= '9' && *str >= '0')
    ret = 1;
  return ret;
}

static int s21_atoi(const char **str) {
  int ret = 0;
  const char *ptr = *str;
  int done = 0;
  while (*ptr && !done) {
    if (s21_is_digit(ptr)) {
      ret = ret * 10 + *ptr - '0';
    } else {
      done = 1;
      *str = ptr;
    }
    ptr++;
  }
  return ret;
}

static const char *parse_flags(const char *format, struct options *opts) {
  const char *flags = " -+0#";
  int len = s21_strspn(format, flags);
  int i = len;
  while (i) {
    switch (format[--i]) {
    case ' ':
      opts->flag_space = 1;
      break;
    case '-':
      opts->flag_minus = 1;
      break;
    case '+':
      opts->flag_plus = 1;
      break;
    case '0':
      opts->flag_zero = 1;
      break;
    case '#':
      opts->flag_hash = 1;
      break;
    default:
      i = 0;
      break;
    }
  }
  if (opts->flag_minus && opts->flag_zero)
    opts->flag_zero = 0;
  if (opts->flag_plus && opts->flag_space)
    opts->flag_space = 0;
  return format + len;
}

static const char *parse_width(const char *format, struct options *opts) {
  if (s21_is_digit(format)) {
    opts->width = s21_atoi(&format);
    if (opts->flag_zero)
      opts->padding = '0';
    else
      opts->padding = ' ';
  }
  return format;
}

static const char *parse_precision(const char *format, struct options *opts) {
  if (*format == '.') {
    format++;
    opts->precision = s21_atoi(&format);
  }
  return format;
}

static const char *parse_length(const char *format, struct options *opts) {
  if (*format == 'h' || *format == 'l' || *format == 'L')
    opts->len = *format++;
  return format;
}

static const char *parse_type(const char *format, struct options *opts) {
  if (*format == 'c' || *format == 'd' || *format == 'f' || *format == 's' ||
      *format == 'u') {
    opts->type = *format++;
  }
  return format;
}

static const char *parse_spec(const char *format, struct options *opts) {
  format = parse_flags(format, opts);
  format = parse_width(format, opts);
  format = parse_precision(format, opts);
  format = parse_length(format, opts);
  format = parse_type(format, opts);
  return format;
}

static const char *s21_get_spec(const char *format, struct options *opts) {
  s21_memset(opts, 0, sizeof(struct options));

  const char *spec = s21_strchr(format, '%');
  if (spec) {
    spec = parse_spec(spec, opts);
  } else
    spec = format;
  return spec;
}

static char *s21_sputch(char *str, int ch, struct options *opts) {
  if (opts->flag_minus)
    *str++ = ch;
  while (--opts->width)
    *str++ = opts->padding;
  if (opts->flag_minus)
    *str++ = ch;
  return str;
}

static char *s21_sputdec(char *dest, long dec, struct options *opts) {
  int i = 0;
  long sign;

  if ((sign = dec) < 0)
    dec = -dec;

  int len = 1;
  for (; (dec /= 10) > 0; len++)
    ;

  for (int c = len; c < opts->width && opts->flag_minus; c++) {
    dest[i++] = opts->padding;
  }

  do {
    dest[i++] = dec % 10 + '0';
  } while ((dec /= 10) > 0);

  if (sign < 0) {
    dest[i++] = '-';
  } else if (opts->flag_plus) {
    dest[i++] = '+';
  } else if (opts->flag_space) {
    dest[i++] = ' ';
  }

  while (i < opts->width && !opts->flag_minus) {
    dest[i++] = opts->padding;
  }
  int last_index = i;
  dest[i--] = 0;

  for (int j = 0; j < i; j++, i--) {
    char temp = dest[j];
    dest[j] = dest[i];
    dest[i] = temp;
  }
  return dest + last_index;
}

static char *s21_sputfloat(char *dest, long double point,
                           struct options *opts) {

  return dest;
}

static void s21_put_spec(char *str, va_list *args, struct options *opts) {
  char c;
  long ld;
  char *s;
  double f;
  unsigned int u;
  switch (opts->type) {
  case 'c':
    c = va_arg(*args, int);
    s21_sputch(str, c, opts);
    break;
  case 'd':
    ld = va_arg(*args, long);
    s21_sputdec(str, ld, opts);
    break;
  case 'f':
    f = va_arg(*args, long double);
    s21_sputfloat(str, f, opts);
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
  while (opts.type) {
    size_t n = s21_strcspn(inp, "%");
    str = s21_memcpy(str, inp, n) + n - 1;
    s21_put_spec(str, &args, &opts);
    inp = s21_get_spec(NULL, NULL);
  }
  return ret;
}

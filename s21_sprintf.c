#include "s21_string.h"
#include <stdarg.h>
#include <stdio.h>
// #include <wchar.h>

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
  printf("flags : %d\n", len);
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
  }
  return format;
}

static const char *parse_precision(const char *format, struct options *opts) {
  if (*format == '.') {
    format++;
    opts->precision = s21_atoi(&format);
    opts->flag_zero = 0;
  } else
    opts->precision = -1;
  return format;
}

static const char *parse_length(const char *format, struct options *opts) {
  if (*format == 'h' || *format == 'l' || *format == 'L')
    opts->len = *format++;
  return format;
}

static const char *parse_type(const char *format, struct options *opts) {
  if (*format == 'c' || *format == 'd' || *format == 'f' || *format == 's' ||
      *format == 'u' || *format == 'o' || *format == 'x' || *format == 'X') {
    opts->type = *format++;
    if (opts->precision == -1) {
      if (opts->type == 'f' || opts->type == 'E' || opts->type == 'g' ||
          opts->type == 'G')
        opts->precision = 6;
      else if (opts->precision == -1)
        opts->precision = 1;
    }
    if (opts->flag_zero)
      opts->padding = '0';
    else
      opts->padding = ' ';
  } else {
    opts->type = *format;
    opts->precision = 1;
    opts->padding = opts->flag_zero ? '0' : ' ';
    format++;
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
    spec = parse_spec(spec + 1, opts);
  } else
    spec = format;
  return spec;
}

static char *s21_sputch(char *str, int ch, struct options *opts) {
  if (!opts->width && ch) {
    *str++ = ch;
  } else {
    opts->width--;
    if (opts->flag_minus)
      *str++ = ch;
    while (opts->width--)
      *str++ = opts->padding;
    if (!opts->flag_minus)
      *str++ = ch;
  }
  return str;
}

static char *s21_sputint(char *dest, long dec, struct options *opts) {
  int i = 0;
  long sign;
  int j = 0;
  if ((sign = dec) < 0)
    dec = -dec;

  if (opts->len == 'h')
    dec &= 0xFFFF; // nullify all bytes except first 2
  else if (opts->len != 'l')
    dec &= 0xFFFFFFFF; // nullify all bytes except first 4

  int len = 1;
  for (long d = dec; (d /= 10) > 0; len++)
    ;

  len += sign < 0 || opts->flag_plus || opts->flag_space ? 1 : 0;

  int total = opts->precision > len ? opts->precision : len;
  for (int c = total; c < opts->width && opts->flag_minus; c++) {
    dest[i++] = opts->padding;
    j++;
  }

  do {
    dest[i++] = dec % 10 + '0';
  } while ((dec /= 10) > 0);

  while (i - j < opts->precision) {
    dest[i++] = '0';
  }

  while (i < opts->width && !opts->flag_minus && opts->padding == '0') {
    dest[i++] = opts->padding;
  }

  if (sign < 0) {
    dest[i++] = '-';
  } else if (opts->flag_plus) {
    dest[i++] = '+';
  } else if (opts->flag_space) {
    dest[i++] = ' ';
  }

  while (i < opts->width && !opts->flag_minus && opts->padding == ' ') {
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

static char *s21_sputuns(char *dest, unsigned long dec, struct options *opts) {

  if (opts->len == 'h') {
    dec &= 0xFFFF; // nullify all bytes except first 2
  } else if (opts->len != 'l')
    dec &= 0xFFFFFFFF; // nullify all bytes except first 4

  int base = 10;  // choose base
  if (opts->type == 'o')
    base = 8;
  else if (opts->type == 'x' || opts->type == 'X')
    base = 16;

  int i = 0;
  int len = 1;
  for (long d = dec; (d /= base) > 0; len++)  // count len of number
    ;

  // count formatted len of number
  int total = opts->precision > len ? opts->precision : len;

  int prefix = 0; // prefix for oct and hex numbers
  if (base == 8 && opts->flag_hash)
    prefix = 1;
  else if (base == 16 && opts->flag_hash)
    prefix = 2;

  total += prefix;

  // fill width field if left-justification is specified
  for (int c = total; c < opts->width && opts->flag_minus; c++) {
    dest[i++] = opts->padding;
  }

  // print digits
  do {
    dest[i++] = dec % base + '0';
  } while ((dec /= base) > 0);

  // fill precision field
  while (i < opts->precision) {
    dest[i++] = '0';
  }

  // print prefix for octal and hex format
  if (base == 8 && opts->flag_hash)
    dest[i++] = '0';
  else if (base == 16 && opts->flag_hash) {
    dest[i++] = opts->type;
    dest[i++] = '0';
  }

  // fill width field if right-justified (as default)
  while (i < opts->width && !opts->flag_minus) {
    dest[i++] = opts->padding;
  }
  // put null to the end of printed string
  int last_index = i;
  dest[i--] = 0;

  for (int j = 0; j < i; j++, i--) { // reverse added string
    char temp = dest[j];
    dest[j] = dest[i];
    dest[i] = temp;
  }

  // return pointer to the end of string
  return dest + last_index;
}

/*static char *s21_sputfloat(char *dest, double num, struct options *opts) {

  return dest;
}*/

static char *s21_sputstr(char *dest, const char *src, struct options *opts) {
  int len = s21_strlen(src);
  int i = 0;

  if (opts->type == 's') {
    len = len > opts->precision ? opts->precision : len;
  }

  while (i++ < len)
    *dest++ = *src++;

  *dest = '\0';
  return dest;
}

static char *s21_put_spec(char *str, va_list *args, struct options *opts) {
  char c;
  long ld;
  const char *s;
  // double f;
  unsigned long u;
  switch (opts->type) {
  case 'c':
    c = va_arg(*args, int);
    str = s21_sputch(str, c, opts);
    break;
  case 'd':
    ld = va_arg(*args, long);
    str = s21_sputint(str, ld, opts);
    break;
  case 'f':
    // f = va_arg(*args, long double);
    // str = s21_sputfloat(str, f, opts);
    break;
  case 's':
    if (opts->len != 'l') {
      s = va_arg(*args, char *);
      str = s21_sputstr(str, s, opts);
    }
    break;
  case 'u':
  case 'o':
  case 'x':
  case 'X':
    u = va_arg(*args, long unsigned);
    str = s21_sputuns(str, u, opts);
    break;
  default:
    str = s21_sputch(str, opts->type, opts);
    break;
  }
  *str = 0;
  return str;
}

void print_opts(struct options *opts) {
  printf("\n"
         "type      : %c\n"
         "flag_minus: %d\n"
         "flag_plus : %d\n"
         "flag_zero : %d\n"
         "flag_hash : %d\n"
         "flag_space: %d\n"
         "length    : %c\n"
         "padding   : %d\n"
         "width     : %d\n"
         "precision : %d\n\n",
         opts->type, opts->flag_minus, opts->flag_plus, opts->flag_zero,
         opts->flag_hash, opts->flag_space, opts->len, opts->padding,
         opts->width, opts->precision);
}

int s21_sprintf(char *str, const char *format, ...) {
  int ret = 0;
  const char *inp = format;
  const char *prev = NULL;
  va_list args;
  va_start(args, format);
  struct options opts;
  prev = s21_get_spec(format, &opts);
  print_opts(&opts);
  while (opts.type) {
    size_t n = s21_strcspn(inp, "%");
    str = s21_memcpy(str, inp, n) + n;
    str = s21_put_spec(str, &args, &opts);
    prev = s21_get_spec(prev, &opts);
    inp = prev;
  }
  va_end(args);
  s21_memset(&opts, 0, sizeof(struct options));
  s21_sputstr(str, inp, &opts);
  return ret;
}

/*int main() {
  char str[20];
  // s21_memset(str, 0, sizeof(str));
  s21_sprintf(str, "hello %#10.4x", 22);
  char s[20];
  // s21_memset(s, 0, sizeof(str));
  sprintf(s, "hello %#10.4x", 22);
  printf("%#05o\n", 29);
  for (int i = 0; i < sizeof(str); i++)
    printf("%d ", str[i]);
  puts("");
  for (int i = 0; i < sizeof(s); i++)
    printf("%d ", s[i]);
}*/

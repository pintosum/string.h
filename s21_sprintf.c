#include "string.h"
#include <stdarg.h>

struct options {
  unsigned char spec;
  unsigned char flag;
  unsigned char len;
  int width;
  int precision;
};

static int s21_is_digit(const char *str){
  int ret = 0;
  if(*str <= 57 && *str >= 48)
    ret = 1;
  return ret;
}

static int s21_atoi(const char **str){
  int ret = 0;
  const char *ptr = *str;
  int done = 0;
  while(*ptr && !done){
    if(s21_is_digit(ptr)){
      ret = ret * 10 + *ptr - 48;
    }
    else{
      done = 1;
    }
    ptr++;
  }
  *str = ptr;
  return ret;
}

const char *s21_get_spec(const char *format, struct options *options) {
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
      if(*spec == ' ' || *spec == '-' || *spec == '+')
        opts->flag = *spec;
      else if(*spec == '.'){
          spec++;
          opts->precision = s21_atoi(&spec);
      }
      else if(s21_is_digit(spec)){
        opts->width = s21_atoi(&spec);
      }
      else if(*spec == 'h' || *spec =='l'){
        opts->len = *spec;
      }
      else if(*spec == 'c' || *spec == 'd' || *spec == 'f' || *spec == 's' || *spec == 'u'){
        opts->spec = *spec;
      }
      else{
        done = 1;
        inp = spec;
      }
      spec++;
    }
  }
  return inp;
}


int s21_sprintf(char *str, const char *format, ...) {
  int ret;
  const char *inp = format;
  va_list va;
  va_start(va, format);
  struct options opts = {0};
  s21_get_spec(format, &opts);
  while(opts.spec){
    va_arg();
    s21_get_spec(NULL, NULL);
  }
  return ret;
}

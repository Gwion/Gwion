#include <stdarg.h>
struct JitCC_ {
  char data[4096];
  uint16_t idx;
};

static inline void jit_add(CC cc, int i, ...) {
  va_list arg;
  va_start(arg, i);
  if(cc->idx == SIZEOF_MEM -1) {
    puts("jit error");
    exit(2);
  }
puts("arg");
while(i--) {
  cc->data[cc->idx++] = (char)va_arg(arg, int);
}
  va_end(arg);
}

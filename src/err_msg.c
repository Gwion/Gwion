#include <stdio.h>
#include <string.h>
#include "err_msg.h"

static const char* err_headers[] = { "PARSE", "SCAN0", "SCAN1", "SCAN2_", "CHECK", "EMIT!", "UTILS", "LEXER",
                                     "INSTR", " UDP ", "ALSA", " VM  ", " OP  "
                                   };

void err_msg(a_header header, long int pos, const char* fmt, ...)
{
  char msg[256];
  va_list arg;
  va_start(arg, fmt);
#ifdef COLOR
  fprintf(stderr, "\033[1m[\033[0m\033[31m%s\033[0m\033[1m]\033[0m", err_headers[header]);
#else
  fprintf(stderr, "%s", err_headers[header]);
#endif
  if(pos > 0)
#ifdef COLOR
    fprintf(stderr, " \033[4mline \033[1m%li\033[0m\t", pos);
#else
    fprintf(stderr, " line: %li\t", pos);
#endif
  vsnprintf(msg, 256, fmt, arg);
  fprintf(stderr, "\t%s\n", msg);
  va_end(arg);
}


#ifdef DEBUG
void debug_msg(const char* header, const char* fmt, ...)
{
  char msg[256];
  va_list arg;
  va_start(arg, fmt);
#ifdef COLOR
  fprintf(stderr, "\033[1m[\033[0m\033[31m%s\033[0m\033[1m]\033[0m", header);
#else
  fprintf(stderr, "%s", header);
#endif
  memset(msg, 0, 256);
  vsnprintf(msg, 256, fmt, arg);
  fprintf(stderr, "\t%s\n", msg);
  va_end(arg);
}
#endif

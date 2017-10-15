#include <stdarg.h>
typedef enum { PARSE_, SCAN0_, SCAN1_, SCAN2_, TYPE_, EMIT_, UTIL_, COMPILE_, INSTR_, UDP, ALSA_,
               VM_, OPERATOR_
             } a_header;
char err_msg(a_header header, long int pos, const char* fmt, ...);

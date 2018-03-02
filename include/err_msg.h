#include <stdarg.h>
typedef enum { PARSE_, SCAN0_, SCAN1_, SCAN2_, TYPE_, EMIT_, UTIL_, COMPILE_, INSTR_, UDP, ALSA_,
               VM_, OPERATOR_
             } a_header;
m_bool err_msg(a_header header, m_uint pos, const char* fmt, ...);
m_bool gw_err(const char* fmt, ...);
m_bool gw_out(const char* fmt, ...);
#ifdef CURSES
m_bool gw_exe(const m_str func, char* fmt, ...);
void gw_pos(int pos);
#endif


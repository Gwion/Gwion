#include <stdarg.h>
typedef enum { PARSE_, SCAN0_, SCAN1_, SCAN2_, TYPE_, EMIT_, UTIL_, COMPILE_, INSTR_, UDP, ALSA_,
               VM_, OPERATOR_
             } a_header;
m_bool err_msg(a_header header, m_uint pos, const char* fmt, ...);
m_bool gw_err(const char* fmt, ...);
m_bool gw_out(const char* fmt, ...);

#define ERR_O(a,b,...) { err_msg(a,b, ## __VA_ARGS__); return NULL; }
#define ERR_B(a,b,...) { err_msg(a,b, ## __VA_ARGS__); return   -1; }
#ifdef CURSES
#define GWDEBUG_EXE gw_exe((const m_str)__func__, "(%s:%i)\n", __FILE__, __LINE__);
m_bool gw_exe(const m_str func, char* fmt, ...);
void gw_pos(int pos);
#else
#define GWDEBUG_EXE
#endif

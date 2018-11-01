#include <stdarg.h>
m_bool err_msg(m_uint pos, const char* fmt, ...);
m_bool gw_err(const char* fmt, ...);
m_bool gw_out(const char* fmt, ...);

#define ERR_O(a,...) { err_msg(a, ## __VA_ARGS__); return NULL; }
#define ERR_B(a,...) { err_msg(a, ## __VA_ARGS__); return   -1; }
#define GWDEBUG_EXE

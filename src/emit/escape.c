#include "defs.h"
#include "err_msg.h"

static char g_escape[256];
__attribute__((constructor(300)))
void escape_table() {
    g_escape['0'] = '0';
    g_escape['\''] = '\'';
    g_escape['"'] = '"';
    g_escape['\\'] = '\\';
    g_escape['a'] = (char)7; // audible bell
    g_escape['b'] = (char)8; // back space
    g_escape['f'] = (char)12; // form feed
    g_escape['n'] = (char)10; // new line
    g_escape['r'] = (char)13; // carriage return
    g_escape['t'] = (char)9; // horizontal tab
    g_escape['v'] = (char)11; // vertical tab
}

static char get_escape(const char c, const int pos) {
  if(g_escape[(int)c])
    return g_escape[(int)c];
  return err_msg(pos, "unrecognized escape sequence '\\%c'", c);
}

m_bool escape_str(m_str str_lit, const int pos) {
  m_str str = str_lit;
  while(*str_lit) {
    if(*str_lit == '\\')  {
      ++str_lit;
      if(*str_lit == '\0')
        ERR_B(pos, "invalid: string ends with escape charactor '\\'")
      const unsigned char c = *(str_lit);
      const unsigned char c2 = *(str_lit+1);
      if(c >= '0' && c <= '7') {
        if(c == '0' && (c2 < '0' || c2 > '7'))
          *str++ = '\0';
        else {
          const unsigned char c3 = *(str_lit+2);
          if(c2 >= '0' && c2 <= '7' && c3 >= '0' && c3 <= '7') {
            *str++ = (c-'0') * 64 + (c2-'0') * 8 + (c3-'0');
            str_lit += 2;
          } else
            ERR_B(pos, "malformed octal escape sequence '\\%c%c%c'", c, c2, c3)
        }
      } else if(c == 'x') {
        ++str_lit;
        const unsigned char c = *(str_lit);
        const unsigned char c2 = *(str_lit+1);
        if(c >= '0' && c <= 'F' && c2 >= '0' && c2 <= 'F') {
          *str++ =(c-'0') * 16 + (c2-'0');
          ++str_lit;
        } else
          ERR_B(pos, "malformed hex escape sequence '\\%c%c'", c, c2)
      } else
        CHECK_BB((*str++ = get_escape(c, pos)))
    }
    else
        *str++ = *str_lit;
    ++str_lit;
  }
  *str = '\0';
  return 1;
}

ANN m_int str2char(const m_str c, const m_int pos) {
  if(c[0] != '\\')
    return c[0];
  return get_escape(c[1], pos);
 }


#include "gwion_util.h"

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

static int get_escape(const char c, const uint pos) {
  if(g_escape[(int)c])
    return g_escape[(int)c];
  return err_msg(pos, "unrecognized escape sequence '\\%c'", c);
}

m_bool escape_str(const m_str base, const uint pos) {
  unsigned char* str_lit = (unsigned char*)base;
  m_str str = base;
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
            *str++ = (char)((c-'0') * 64 + (c2-'0') * 8 + (c3-'0'));
            str_lit += 2;
          } else
            ERR_B(pos, "malformed octal escape sequence '\\%c%c%c'", c, c2, c3)
        }
      } else if(c == 'x') {
        ++str_lit;
        const unsigned char c1 = *(str_lit);
        const unsigned char c3 = *(str_lit+1);
        if(c1 >= '0' && c1 <= 'F' && c3 >= '0' && c3 <= 'F') {
          *str++ = (char)((c1-'0') * 16 + (c3-'0'));
          ++str_lit;
        } else
          ERR_B(pos, "malformed hex escape sequence '\\%c%c'", c1, c3)
      } else
        CHECK_BB((*str++ = (char)get_escape((char)c, pos)))
    }
    else
        *str++ = (char)*str_lit;
    ++str_lit;
  }
  *str = '\0';
  return 1;
}

ANN m_int str2char(const m_str c, const uint pos) {
  if(c[0] != '\\')
    return c[0];
  return get_escape(c[1], pos);
 }


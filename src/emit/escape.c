#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "escape.h"

char *escape_table(MemPool p) {
  char *escape = (char *)mp_calloc2(p, 256);
  escape['0']  = '0';
  escape['\''] = '\'';
  escape['"']  = '"';
  escape['\\'] = '\\';
  escape['a']  = (char)7;  // audible bell
  escape['b']  = (char)8;  // back space
  escape['f']  = (char)12; // form feed
  escape['n']  = (char)10; // new line
  escape['r']  = (char)13; // carriage return
  escape['t']  = (char)9;  // horizontal tab
  escape['v']  = (char)11; // vertical tab
  return escape;
}

static int get_escape(const Emitter emit, const char c, const loc_t pos) {
  if (emit->info->escape[(int)c]) return emit->info->escape[(int)c];
  env_err(emit->env, pos, _("unrecognized escape sequence '\\%c'"), c);
  return GW_ERROR;
}

m_bool escape_str(const Emitter emit, const m_str base, const loc_t pos) {
  unsigned char *str_lit = (unsigned char *)base;
  m_str          str     = base;
  while (*str_lit) {
    if (*str_lit == '\\') {
      ++str_lit;
      const unsigned char c  = *(str_lit);
      const unsigned char c2 = *(str_lit + 1);
      if (c >= '0' && c <= '7') {
        if (c == '0' && (c2 < '0' || c2 > '7'))
          *str++ = '\0';
        else {
          const unsigned char c3 = *(str_lit + 2);
          if (c2 >= '0' && c2 <= '7' && c3 >= '0' && c3 <= '7') {
            *str++ = (char)((c - '0') * 64 + (c2 - '0') * 8 + (c3 - '0'));
            str_lit += 2;
          } else {
            env_err(emit->env, pos,
                    _("malformed octal escape sequence '\\%c%c%c'"), c, c2, c3);
            return GW_ERROR;
          }
        }
      } else if (c == 'x' || c == 'x' || c == 'u' || c == 'U') {
        ++str_lit;
        const unsigned char c1 = *(str_lit);
        const unsigned char c3 = *(str_lit + 1);
        if (c1 >= '0' && c1 <= 'F' && c3 >= '0' && c3 <= 'F') {
          *str++ = (char)((c1 - '0') * 16 + (c3 - '0'));
          ++str_lit;
        } else {
          env_err(emit->env, pos, _("malformed hex escape sequence '\\%c%c'"),
                  c1, c3);
          return GW_ERROR;
        }
      } else
        CHECK_BB((*str++ = (char)get_escape(emit, (char)c, pos)));
    } else
      *str++ = (char)*str_lit;
    ++str_lit;
  }
  *str = '\0';
  return GW_OK;
}

ANN char str2char(const Emitter emit, const m_str c, const loc_t pos) {
  return c[0] != '\\' ? c[0] : get_escape(emit, c[1], pos);
}

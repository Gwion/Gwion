#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN static m_float basic_locale(m_str str) {
  const char base = str[0];
  str++;
  if(base < 'A' || base > 'G') return -1;
  m_int bnote = base - 'A';
  if(*str == '#') { bnote++; str++; }
  else if(*str == 'b') { bnote--; str++; }
  char *remainder;
  const long octave = strtol(str, &remainder, 10);
  if(*remainder != '\0') return -1;
  const int note = bnote + 12 * octave + 21;
  return (pow(2, (note - 69) / 12.0) * 440.0);
}

static SFUN(BasicLocale) {
  const M_Object arg = *(M_Object*)MEM(0);
  const m_float ret = basic_locale(STRING(arg));
  if(ret == -1.0)
    handle(shred, "invalid value for locale");
  *(m_float*)RETURN = ret;
}

GWION_IMPORT(locale) {
  gwidoc(gwi, "Definition of the basic locale");
  GWI_BB(gwi_func_ini(gwi, "float", "BasicLocale"));
  GWI_BB(gwi_func_arg(gwi, "string", "str"));
  GWI_BB(gwi_func_end(gwi, BasicLocale, ae_flag_none));
  return GW_OK;
}

#include <stdlib.h>
#include <string.h>
#include "defs.h"

static const m_str op_str[] = {
// arithmetic
  "=", "+", "-", "*", "/", "%",
// logical
  "&&", "||",	"==", "!=",
  ">", ">=", "<", "<=",
// bitwise
  "<<", ">>",
  "|", "&", "^",
// unary
  "++", "--", "!", "~",
  "new", "spork",
// reverse arithmetic
  "=>", "+=>", "-=>", "*=>", "/=>", "%=>",
// reverse logical
  "&&=>", "||=>", "==>", "!=>",
  ">=>", ">==>", "<=<", "<==<",
// reverse bitwise
  "<<=>", ">>=>", "&=>", "|=>", "^=>",
// unchuck
  "=<",
// at_chuck
  "@=>", "@=<",
// trig
  "]=>", "]=<", NULL
};

static const m_str op_name[] = {
// arithmetic
  "assign", "plus", "minus", "times", "divide", "modulo",
// logical
  "and", "or", "eq", "neq",
  "gt", "ge", "lt", "le",
// bitwise
  "shiftleft", "shiftright",
  "s_or", "s_and", "s_xor",
// unary
  "plusplus", "minusminus", "exclamation", "tilda",
  "new", "spork",
// reverse arithmetic
  "chuck", "pluschuck", "minuschuck", "timeschuck", "dividechuck", "modulochuck",
// reverse logical
  "chuck_and", "chuck_or", "chuck_eq", "chuck_neq",
  "chuck_gt", "chuck_ge", "chuck_lt", "chuck_le",
// reverse bitwise
  "rsl", "rsr", "rsand", "rsor", "rsxor",
// unchuck
  "unchuck",
// at
  "at_chuck", "at_unchuck",
// trig
  "trig", "untrig", NULL
  // more to come
};

m_int name2op(m_str name) {
  m_uint i = 0;
  while(op_name[i]) {
    if(!strcmp(name, op_name[i]))
      return i;
    i++;
  }
  return -1;
}

const m_str op2str(Operator op) {
  if(op >= (sizeof(op_name) / sizeof(char*)))
    return NULL;
  return op_str[op];
}

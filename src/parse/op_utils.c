#include <stdlib.h>
#include <string.h>
#include "defs.h"

static const m_str op_str[] = {
  "=", "+", "-", "*", "/", "%",
  "&&", "||",	"==", "!=", ">", ">=", "<", "<=",
  "<<", ">>", "|", "&", "^",
  "++", "--", "!", "~", "new", "spork",
  "+=>", "-=>", "*=>", "/=>", "%=>",
  "<<=>", ">>=>", "&=>", "|=>", "^=>",
  "=>", "=<", "@=>", "@=<", "]=>", "]=<",
  NULL
};

m_int name2op(m_str name) {
  m_uint i = 0;
  while(op_str[i]) {
    if(!strcmp(name, op_str[i]))
      return i;
    i++;
  }
  return -1;
}

const m_str op2str(Operator op) {
  if(op >= (sizeof(op_str) / sizeof(char*)))
    return NULL;
  return op_str[op];
}

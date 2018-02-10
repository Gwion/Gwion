#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"

m_str code_name_set(const m_str name, const m_str file) {
  size_t len = strlen(name) + strlen(file) + 2;
  m_str  str = calloc(1, len);
  sprintf(str, "%s$%s", name, file);
puts(str);
  return str;
}

const m_str code_name(const m_str name, const m_bool b) {
  m_str str = strchr(name, '$');
  return (b && str) ?  (str + 1) : name;
}

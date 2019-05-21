#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"

m_str code_name_set(const m_str name, const m_str file) {
  const size_t len = strlen(name) + strlen(file) + 2;
  m_str str = (const m_str)xcalloc(1, len);
  sprintf(str, "%s$%s", name, file);
  return str;
}

m_str code_name(const m_str name, const m_bool b) {
  const m_str str = strchr(name, '$');
  return (b && str) ?  (str + 1) : name;
}

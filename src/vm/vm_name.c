#include "gwion_util.h"
#include "gwion_ast.h"

m_str code_name_set(MemPool p, const m_str name, const m_str file) {
  const m_str search  = strrchr(name, '@');
  const m_str is_file = search ? search + 1 : name;
  if(!strcmp(is_file, file)) {
    return mstrdup(p, name);
  }
  const size_t len = strlen(name) + strlen(file) + 2;
  const m_str  str = (const m_str)_mp_malloc(p, len);
  sprintf(str, "%s$%s", name, file);
  return str;
}

m_str code_name(const m_str name, const bool b) {
  const m_str str = strchr(name, '$');
  return (b && str) ? (str + 1) : name;
}

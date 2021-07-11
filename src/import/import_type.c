#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN2(1) static Type get_parent(const Gwi gwi, const m_str parent_name) {
  Type_Decl *td = parent_name ? gwi_str2td(gwi, parent_name) : NULL;
  if (td) {
    if (td->array || td->types) {
      const m_str str = td->array ? "array" : "template";
      free_type_decl(gwi->gwion->mp, td);
      GWI_ERR_O(_("can't use gwi_mk_type to extend %s types"), str)
    }
  }
  const Type parent = td ? known_type(gwi->gwion->env, td) : NULL;
  if (td) free_type_decl(gwi->gwion->mp, td);
  return parent;
}

ANN2(1, 2)
Type gwi_mk_type(const Gwi gwi, const m_str name, const m_uint size,
                 const m_str parent_name) {
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint(gwi->lint, "{+C}primitive{0} {+}%s{0}", name);
    if (parent_name) lint(gwi->lint, " {+C}extends{0} {+}%s{0}", parent_name);
    lint_sc(gwi->lint);
    lint_nl(gwi->lint);
  }
  CHECK_OO(gwi_str2sym(gwi, name));
  const Type parent = get_parent(gwi, parent_name);
  const Type t      = new_type(gwi->gwion->mp, name, parent);
  t->size           = size;
  if(safe_tflag(parent, tflag_float))
    set_tflag(t, tflag_float);
  return t;
}

ANN void gwi_add_type(const Gwi gwi, const Type type) {
  return env_add_type(gwi->gwion->env, type, gwi->loc);
}

ANN void gwi_set_global_type(const Gwi gwi, const Type type,
                             const type_enum te) {
  gwi->gwion->type[te] = type;
  gwi_add_type(gwi, type);
}

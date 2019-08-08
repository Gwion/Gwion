#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "func.h"
#include "value.h"
#include "nspc.h"
#include "traverse.h"
#include "template.h"
#include "vm.h"
#include "parse.h"
#include "gwion.h"
#include "cpy_ast.h"


ANN static Symbol tuple_sym(const Env env, const Vector v) {
  GwText text = { .mp=env->gwion->mp };
  text_add(&text, t_tuple->name);
  text_add(&text, "<~");
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const Type t = (Type)vector_at(v, i);
    text_add(&text, t != 1 ? t->name : "_");
    if(i+1 < vector_size(v))
      text_add(&text, ",");
  }
  text_add(&text, "~>");
  const Symbol sym = insert_symbol(text.str);
  text_release(&text);
  return sym;
}

ANN Type tuple_type(const Env env, const Vector v, const loc_t pos) {
  const Symbol sym = tuple_sym(env, v);
  const Type exists = nspc_lookup_type1(env->curr, sym);
  if(exists)
    return exists;
  Stmt_List base = NULL, curr = NULL;
  Type_List tlbase = NULL, tl = NULL;
  for(m_uint i = 0; i < vector_size(v); ++i) {
    char name[num_digit(i) + 2];
    sprintf(name, "e%lu", i);
    const Symbol sym = insert_symbol(name);
    const Type t = (Type)vector_at(v, i);
    const Symbol tsym = insert_symbol(t != 1 ? t->name : "@Undefined");
//    const Symbol tsym = t != 1 ? insert_symbol(t->name) : insert_symbol("@undefined");
    Exp decl = decl_from_id(env->gwion->mp, tsym, sym, loc_cpy(env->gwion->mp, pos));
    const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_exp, decl);
    const Stmt_List slist = new_stmt_list(env->gwion->mp, stmt, NULL);
    if(curr)
      curr->next = slist;
    if(!base)
      base = slist;
{// build Type_List
  const ID_List ilist = new_id_list(env->gwion->mp, tsym,
      loc_cpy(env->gwion->mp, pos));
  Type_Decl *td = new_type_decl(env->gwion->mp, ilist);
  Type_List tmp_tl = new_type_list(env->gwion->mp, td, NULL);
    if(tl)
      tl->next = tmp_tl;
    if(!tlbase)
      tlbase = tl = tmp_tl;

}
    curr = slist;
  }
  Section * section = new_section_stmt_list(env->gwion->mp, base);
  Class_Body body = new_class_body(env->gwion->mp, section, NULL);
  const ID_List ilist = new_id_list(env->gwion->mp, insert_symbol(t_tuple->name),
      loc_cpy(env->gwion->mp, pos));
  Type_Decl *td = new_type_decl(env->gwion->mp, ilist);
  Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_template,
        sym, td, body, loc_cpy(env->gwion->mp, pos));
  Tmpl* tmpl = new_tmpl_call(env->gwion->mp, tlbase);
  cdef->base.tmpl = tmpl;
  CHECK_BO(scan0_class_def(env, cdef))
  SET_FLAG(cdef->base.type, abstract);
  cdef->base.type->e->tuple_tl = tlbase;
//  CHECK_BO(scan1_cdef(env, cdef))
  CHECK_BO(traverse_cdef(env, cdef))
  nspc_add_type(env->curr, sym, cdef->base.type);
//  map_set((Map)vector_front(&env->curr->info->type), sym, cdef->base.type);
  return cdef->base.type;
}

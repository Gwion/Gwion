#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"

#define GWI_ERR_B(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return GW_ERROR; }
#define GWI_ERR_O(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return NULL; }
#define ENV_ERR_B(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return GW_ERROR; }
#define ENV_ERR_O(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return NULL; }

#include "parser.h"
struct Path {
  m_str path, curr;
  m_uint len;
  loc_t loc;
};

ANN static ID_List templater_def(SymTable *st, const Gwi gwi) {
  const Templater* templater = &gwi->templater;
  ID_List list[templater->n];
  list[0] = new_id_list(st->p, insert_symbol(st, templater->list[0]), loc_cpy(gwi->gwion->mp, gwi->loc));
  for(m_uint i = 1; i < templater->n; i++) {
    list[i] = new_id_list(st->p, insert_symbol(st, templater->list[i]), loc_cpy(gwi->gwion->mp, gwi->loc));
    list[i - 1]->next = list[i];
  }
  return list[0];
}

ANN VM* gwi_vm(const Gwi gwi) {
  return gwi->gwion->vm;
}

ANN m_int gwi_tmpl_end(const Gwi gwi) {
  gwi->templater.n = 0;
  gwi->templater.list = NULL;
  return GW_OK;
}

ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str* list) {
  gwi->templater.n = n;
  gwi->templater.list = (m_str*)list;
  return GW_OK;
}

ANN2(1,2,3) static void dl_func_init(DL_Func* a, const restrict m_str t,
    const restrict m_str n, const f_xfun addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

ANN m_int gwi_func_ini(const Gwi gwi, const restrict m_str t, const restrict m_str n, const f_xfun addr) {
  dl_func_init(&gwi->func, t, n, addr);
  return GW_OK;
}

ANN static void dl_func_func_arg(DL_Func* a, const restrict m_str t, const restrict m_str n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

ANN m_int gwi_func_arg(const Gwi gwi, const restrict m_str t, const restrict m_str n) {
  if(gwi->func.narg == DLARG_MAX - 1)
    GWI_ERR_B(_("too many arguments for function '%s'."), gwi->func.name)
  dl_func_func_arg(&gwi->func, t, n);
  return GW_OK;
}

ANN static m_bool mk_gack(MemPool p, const Type type, const f_gack d) {
  const VM_Code code = new_vm_code(p, NULL, SZ_INT, ae_flag_member | ae_flag_builtin, "@gack");
  code->native_func = (m_uint)d;
  type->e->gack = code;
  return GW_OK;
}

ANN m_bool gwi_gack(const Gwi gwi, const Type type, const f_gack d) {
  return mk_gack(gwi->gwion->mp, type, d);
}

ANN static m_bool mk_xtor(MemPool p, const Type type, const m_uint d, const ae_flag e) {
  VM_Code* code = e == ae_flag_ctor ? &type->nspc->pre_ctor : &type->nspc->dtor;
  const m_str name = type->name;
  *code = new_vm_code(p, NULL, SZ_INT, e | ae_flag_member | ae_flag_builtin, name);
  (*code)->native_func = (m_uint)d;
  type->flag |= e;
  return GW_OK;
}

ANN2(1,2) Type gwi_mk_type(const Gwi gwi NUSED, const m_str name, const m_uint size, const m_str parent_name) {
  m_uint depth = 0;
  Type_Decl* td = parent_name ? str2decl(gwi->gwion->env, parent_name, &depth, gwi->loc) : NULL;
  if(depth) {
    free_type_decl(gwi->gwion->mp, td);
    GWI_ERR_O(_("can't create a type with explicit array parent. use typedef for that"))
  }
  const Type parent = td ? known_type(gwi->gwion->env, td) : NULL;
  const Type t = new_type(gwi->gwion->mp, 0, name, parent);
  t->size = size;
  return t;
}

ANN m_int gwi_add_type(const Gwi gwi, const Type type) {
  CHECK_BB(name_valid(gwi, type->name));
  env_add_type(gwi->gwion->env, type);
  return (m_int)type->xid;
}

ANN m_int gwi_set_global_type(const Gwi gwi, const Type type, const type_enum te) {
  GWI_BB(gwi_add_type(gwi, type))
  gwi->gwion->type[te] = type;
  return GW_OK;
}

ANN2(1,2) static void import_class_ini(const Env env, const Type type,
    const f_xtor pre_ctor, const f_xtor dtor) {
  type->nspc = new_nspc(env->gwion->mp, type->name);
  type->nspc->parent = env->curr;
  if(pre_ctor)
    mk_xtor(env->gwion->mp, type, (m_uint)pre_ctor, ae_flag_ctor);
  if(dtor)
    mk_xtor(env->gwion->mp, type, (m_uint)dtor, ae_flag_dtor);
  if(type->e->parent) {
    type->nspc->info->offset = type->e->parent->nspc->info->offset;
    if(type->e->parent->nspc->info->vtable.ptr)
      vector_copy2(&type->e->parent->nspc->info->vtable, &type->nspc->info->vtable);
  }
  type->e->owner = env->curr;
  SET_FLAG(type, checked);
  env_push_type(env, type);
}

ANN2(1,2) m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor) {
  if(type->nspc)
    GWI_ERR_B(_("during import: class '%s' already imported."), type->name)
  if(gwi->templater.n) {
    const ID_List types = templater_def(gwi->gwion->st, gwi);
    type->e->def = new_class_def(gwi->gwion->mp, 0, insert_symbol(gwi->gwion->st, type->name), NULL, NULL, loc_cpy(gwi->gwion->mp, gwi->loc));
    type->e->def->base.tmpl = new_tmpl(gwi->gwion->mp, types, -1);
    type->e->def->base.type = type;
    SET_FLAG(type, template);
  } else
    SET_FLAG(type, scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  gwi_add_type(gwi, type);
  import_class_ini(gwi->gwion->env, type, pre_ctor, dtor);
  return (m_int)type->xid;
}

ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td) {
  if(!gwi->gwion->env->class_def)
    GWI_ERR_B(_("gwi_class_ext invoked before gwi_class_ini"))
  const VM_Code ctor = gwi->gwion->env->class_def->nspc->pre_ctor;
  if(gwi->gwion->env->class_def->e->parent ||
      (gwi->gwion->env->class_def->e->def && gwi->gwion->env->class_def->e->def->base.ext))
    GWI_ERR_B(_("class extend already set"))
  if(td->array && !td->array->exp)
    GWI_ERR_B(_("class extend array can't be empty"))
  if(!gwi->gwion->env->class_def->e->def) {
    DECL_OB(const Type, t, = known_type(gwi->gwion->env, td))
    if(td->array)
      SET_FLAG(gwi->gwion->env->class_def, typedef);
    gwi->gwion->env->class_def->e->parent = t;
    gwi->gwion->env->class_def->nspc->info->offset = t->nspc->info->offset;
    if(t->nspc->info->vtable.ptr)
      vector_copy2(&t->nspc->info->vtable, &gwi->gwion->env->class_def->nspc->info->vtable);
    CHECK_OB((gwi->gwion->emit->code = emit_class_code(gwi->gwion->emit,
          gwi->gwion->env->class_def->name)))
    if(td->array)
      CHECK_BB(emit_array_extend(gwi->gwion->emit, t, td->array->exp))
    if(ctor)
      emit_ext_ctor(gwi->gwion->emit, ctor);
    emit_class_finish(gwi->gwion->emit, gwi->gwion->env->class_def->nspc);
    free_type_decl(gwi->gwion->mp, td);
  } else {
    SET_FLAG(td, typedef);
    gwi->gwion->env->class_def->e->def->base.ext = td;
  }
  return GW_OK;
}

ANN m_int gwi_class_end(const Gwi gwi) {
  if(!gwi->gwion->env->class_def)
    GWI_ERR_B(_("import: too many class_end called."))
  nspc_allocdata(gwi->gwion->mp, gwi->gwion->env->class_def->nspc);
  env_pop(gwi->gwion->env, 0);
  return GW_OK;
}

static Array_Sub make_dll_arg_list_array(MemPool p, Array_Sub array_sub,
  m_uint* array_depth, const m_uint array_depth2) {
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) { // TODO: clean me
    array_sub = new_array_sub(p, NULL);
    for(m_uint i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
  }
  return array_sub;
}

ANN static Arg_List make_dll_arg_list(const Gwi gwi, DL_Func * dl_fun) {
  const Env env = gwi->gwion->env;
  Arg_List arg_list    = NULL;
  for(m_uint i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Type_Decl* type_decl = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2;
    if(!(type_decl = str2decl(env, arg->type, &array_depth, gwi->loc))) {
      if(arg_list)
        free_arg_list(env->gwion->mp, arg_list);
      GWI_ERR_O(_("  ...  at argument '%"UINT_F"'"), i + 1)
    }
    if((type_path2 = str2list(env, arg->name, &array_depth2, gwi->loc)))
      free_id_list(env->gwion->mp, type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(env->gwion->mp, type_decl);
      if(arg_list)
        free_arg_list(env->gwion->mp, arg_list);
      GWI_ERR_O(_("array subscript specified incorrectly for built-in module"));
    }
    const Array_Sub array_sub = make_dll_arg_list_array(env->gwion->mp, NULL, &array_depth, array_depth2);
    const Var_Decl var_decl = new_var_decl(env->gwion->mp, insert_symbol(env->gwion->st, arg->name), array_sub, loc_cpy(gwi->gwion->mp, gwi->loc));
    arg_list = new_arg_list(env->gwion->mp, type_decl, var_decl, arg_list);
  }
  return arg_list;
}

ANN static Func_Def make_dll_as_fun(const Gwi gwi, const struct func_checker *ck) {
  const MemPool mp = gwi->gwion->mp;
  DECL_OO(Type_Decl*, type_decl, = import_td(gwi, gwi->func.type, GW_ERROR))
  const m_str name = gwi->func.name;
  const Arg_List arg_list = make_dll_arg_list(gwi, &gwi->func);
  const Func_Def func_def = new_func_def(mp, new_func_base(mp, type_decl, insert_symbol(gwi->gwion->st, name), arg_list),
    NULL, ck->flag | ae_flag_builtin, loc_cpy(mp, gwi->loc));
  func_def->d.dl_func_ptr = (void*)(m_uint)gwi->func.addr;
  return func_def;
}

ANN static m_bool section_fdef(const Gwi gwi, const Func_Def fdef) {
  Section* section = new_section_func_def(gwi->gwion->mp, fdef);
  const Class_Body body = new_class_body(gwi->gwion->mp, section, NULL);
  gwi_body(gwi, body);
  return GW_OK;
}

ANN static m_bool error_fdef(const Gwi gwi, const Func_Def fdef) {
  fdef->d.dl_func_ptr = NULL;
  free_func_def(gwi->gwion->mp, fdef);
  return GW_ERROR;
}

ANN2(1) static Func_Def template_fdef(const Gwi gwi, const struct func_checker *ck) {
  const Arg_List arg_list = make_dll_arg_list(gwi, &gwi->func);
  m_uint depth;
  Type_Decl *td = str2decl(gwi->gwion->env, gwi->func.type, &depth, gwi->loc);
  if(depth)
    td->array = make_dll_arg_list_array(gwi->gwion->mp, NULL, &depth, 0);
  const Func_Def fdef = new_func_def(gwi->gwion->mp, new_func_base(gwi->gwion->mp,
      td, insert_symbol(gwi->gwion->st, ck->name), arg_list), NULL, ae_flag_builtin, loc_cpy(gwi->gwion->mp, gwi->loc));
  fdef->base->tmpl = new_tmpl(gwi->gwion->mp, ck->tmpl, -1);
  fdef->d.dl_func_ptr = (void*)(m_uint)gwi->func.addr;
  SET_FLAG(fdef, template | ae_flag_builtin);
  return fdef;
}

ANN m_int gwi_func_valid(const Gwi gwi, const struct func_checker *ck) {
  gwi->func.name = ck->name;
  DECL_OB(Func_Def, fdef, = (!ck->tmpl ? make_dll_as_fun : template_fdef)(gwi, ck))
  if(gwi->gwion->env->class_def && GET_FLAG(gwi->gwion->env->class_def, template))
    return section_fdef(gwi, fdef);
  if(traverse_func_def(gwi->gwion->env, fdef) < 0)
    return error_fdef(gwi, fdef);
  return GW_OK;
}

ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag) {
  struct func_checker ck = { .name=gwi->func.name, .flag=flag };
  CHECK_BB(check_typename_def(gwi, &ck))
  if(gwi_func_valid(gwi, &ck) > 0)
    return GW_OK;
  func_checker_clean(gwi, &ck);
  return GW_ERROR;
}

ANN m_int gwi_fptr_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  dl_func_init(&gwi->func, type, name, 0);
  return GW_OK;
}

ANN static Fptr_Def import_fptr(const Gwi gwi, DL_Func* dl_fun, ae_flag flag) {
  const Env env = gwi->gwion->env;
  struct func_checker ck = { .name=gwi->func.name, .flag=flag };
  CHECK_BO(check_typename_def(gwi, &ck))
  m_uint depth;
  const ID_List type_path = str2list(env, dl_fun->type, &depth, gwi->loc);
  if(type_path) {
    Type_Decl *td = new_type_decl(env->gwion->mp, type_path);
    td->array = make_dll_arg_list_array(env->gwion->mp, NULL, &depth, 0);
    const Arg_List args = make_dll_arg_list(gwi, dl_fun);
    Func_Base *base = new_func_base(env->gwion->mp, td, insert_symbol(env->gwion->st, ck.name), args);
    if(ck.tmpl)
      base->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
    return new_fptr_def(env->gwion->mp, base, flag | ae_flag_builtin);
  }
  func_checker_clean(gwi, &ck);
  return NULL;
}

ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  DECL_OO(const Fptr_Def, fptr, = import_fptr(gwi, &gwi->func, flag))
  const m_bool ret = traverse_fptr_def(gwi->gwion->env, fptr);
  if(ret > 0)
    SET_FLAG(fptr->base->func, builtin);
  const Type t = ret > 0 ? fptr->type : NULL;
  free_fptr_def(gwi->gwion->mp, fptr);
  return t;
}

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  gwi->val.type = type;
  gwi->val.name = name;
  return GW_OK;
}

ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  struct func_checker ck = { .name=gwi->val.name, .flag=flag };
  CHECK_BO(check_typename_def(gwi, &ck))
  Type_Decl* td = import_td(gwi, gwi->val.type, 0); // TODO: make it GW_PASS
  if(td) {
    td->flag |= flag;
    const Symbol sym = insert_symbol(gwi->gwion->st, ck.name);
    const Type_Def tdef = new_type_def(gwi->gwion->mp, td, sym);
    if(ck.tmpl)
      tdef->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
    traverse_type_def(gwi->gwion->env, tdef);
    const Type t = tdef->type;
    free_type_def(gwi->gwion->mp, tdef);
    return t;
  }
  func_checker_clean(gwi, &ck);
  return NULL;
}

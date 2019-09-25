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

#include "parser.h"
struct Path {
  m_str path, curr;
  m_uint len;
  YYLTYPE loc;
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

ANN static m_bool check_illegal(char* curr, const char c, const m_uint i) {
  if(isalnum(c) || c == '_' || (i == 1 && c== '@'))
    curr[i - 1] = c;
  else
    return GW_ERROR;
  return GW_OK;
}

ANN static m_bool name_valid(const Gwi gwi, const m_str a) {
  const m_uint len = strlen(a);
  m_uint lvl = 0;
  for(m_uint i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    if(c == '<') {
      lvl++;
      continue;
    }
    if(c == ',') {
      if(!lvl)
        GWI_ERR_B(_("illegal use of ',' outside of templating in name '%s'."), a)
      continue;
    }
    if(c == '>') {
      if(!lvl)
        GWI_ERR_B(_("illegal templating in name '%s'."), a)
      lvl--;
      continue;
    }
    GWI_ERR_B(_("illegal character '%c' in name '%s'."), c, a)
  }
  return !lvl ? 1 : -1;
}

ANN static void path_valid_inner(const m_str curr) {
  const size_t size = strlen(curr);
  for(m_uint j = (size / 2) + 1; --j;) {
    const char s = curr[j];
    curr[j] = curr[size - j - 1];
    curr[size - j - 1] = s;
  }
}

ANN static m_bool path_valid(const Env env, ID_List* list, const struct Path* p) {
  char last = '\0';
  for(m_uint i = p->len + 1; --i;) {
    const char c = p->path[i - 1];
    if(c != '.' && check_illegal(p->curr, c, i) < 0) {
      env_err(env, &p->loc, _("illegal character '%c' in path '%s'."), c, p->path);
      return GW_ERROR;
    }
    if(c == '.' || i == 1) {
      if((i != 1 && last != '.' && last != '\0') ||
          (i ==  1 && c != '.')) {
        path_valid_inner(p->curr);
        *list = prepend_id_list(env->gwion->st->p, insert_symbol(env->gwion->st, p->curr), *list, loc_cpy(env->gwion->mp, &p->loc));
        memset(p->curr, 0, p->len + 1);
      } else {
        env_err(env, &p->loc, _("path '%s' must not ini or end with '.'."), p->path);
        return GW_ERROR;
      }
    }
    last = c;
  }
  return GW_OK;
}

ANN /* static */ ID_List str2list(const Env env, const m_str path, m_uint* array_depth) {
  const m_uint len = strlen(path);
  ID_List list = NULL;
  m_uint depth = 0;
  char curr[len + 1];
  struct Path p = { path, curr, len, { 1,1,1,1} };
  memset(curr, 0, len + 1);

  while(p.len > 2 && path[p.len - 1] == ']' && path[p.len - 2] == '[') {
    depth++;
    p.len -= 2;
  }
  *array_depth = depth;
  if(path_valid(env, &list, &p) < 0) {
    if(list)
      free_id_list(env->gwion->mp, list);
    return NULL;
  }
  CHECK_OO(list)
  strncpy(curr, path, p.len);
  list->xid = insert_symbol(env->gwion->st, curr);
  return list;
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

ANN2(1,2) Type gwi_mk_type(const Gwi gwi NUSED, const m_str name, const m_uint size, const Type parent) {
  const Type t = new_type(gwi->gwion->mp, 0, name, parent);
  t->size = size;
  return t;
}

ANN m_int gwi_add_type(const Gwi gwi, const Type type) {
  if(type->name[0] != '@')
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

ANN static void dl_var_new_exp_array(MemPool p, DL_Var* v) {
  v->t.array = new_array_sub(p, NULL);
  v->t.array->depth = v->array_depth;
  v->var.array = new_array_sub(p, NULL);
  v->var.array->depth = v->array_depth;
}

ANN static void dl_var_set(MemPool p, DL_Var* v, const ae_flag flag) {
  v->list.self = &v->var;
  v->t.flag = flag;
  v->exp.exp_type = ae_exp_decl;
  v->exp.d.exp_decl.td   = &v->t;
  v->exp.d.exp_decl.list = &v->list;
  if(v->array_depth)
    dl_var_new_exp_array(p, v);
}

ANN static void dl_var_release(MemPool p, const DL_Var* v) {
  if(v->array_depth) {
    free_array_sub(p, v->t.array);
    free_array_sub(p, v->var.array);
  }
  free_id_list(p, v->t.xid);
}

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DL_Var* v = &gwi->var;
  memset(v, 0, sizeof(DL_Var));
  if(!(v->t.xid = str2list(gwi->gwion->env, type, &v->array_depth)))
    GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->class_def->name, name)
    v->var.xid = insert_symbol(gwi->gwion->st, name);
  return GW_OK;
}

#undef gwi_item_end

static void gwi_body(const Gwi gwi, const Class_Body body) {
  if(!gwi->gwion->env->class_def->e->def->body)
    gwi->gwion->env->class_def->e->def->body = body;
  else
    gwi->body->next = body;
  gwi->body = body;
}

ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr) {
  DL_Var* v = &gwi->var;
  dl_var_set(gwi->gwion->mp, v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(gwi->gwion->env->class_def && GET_FLAG(gwi->gwion->env->class_def, template)) {
    Type_Decl *type_decl = new_type_decl(gwi->gwion->mp, v->t.xid);
    type_decl->flag = flag;
    const Var_Decl var_decl = new_var_decl(gwi->gwion->mp, v->var.xid, v->var.array, loc_cpy(gwi->gwion->mp, gwi->loc));
    const Var_Decl_List var_decl_list = new_var_decl_list(gwi->gwion->mp, var_decl, NULL);
    const Exp exp = new_exp_decl(gwi->gwion->mp, type_decl, var_decl_list);
    const Stmt stmt = new_stmt_exp(gwi->gwion->mp, ae_stmt_exp, exp);
    const Stmt_List list = new_stmt_list(gwi->gwion->mp, stmt, NULL);
    Section* section = new_section_stmt_list(gwi->gwion->mp, list);
    const Class_Body body = new_class_body(gwi->gwion->mp, section, NULL);
    type_decl->array = v->t.array;
    gwi_body(gwi, body);
    return GW_OK;
  }
  v->exp.pos = gwi->loc;
  CHECK_BB(traverse_decl(gwi->gwion->env, &v->exp.d.exp_decl))
  SET_FLAG(v->var.value, builtin);
if(v->var.value->type->array_depth)
ADD_REF(v->var.value->type);
  dl_var_release(gwi->gwion->mp, v);
  return (m_int)v->var.value->offset;
}

static Array_Sub make_dll_arg_list_array(MemPool p, Array_Sub array_sub,
  m_uint* array_depth, const m_uint array_depth2) {
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) {
    array_sub = new_array_sub(p, NULL);
    for(m_uint i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
  }
  return array_sub;
}

ANN /*static */ Type_List str2tl(const Env env, const m_str s, m_uint *depth) {
  DECL_OO(Type_Decl*, td, = str2decl(env, s, depth))
  td->array = make_dll_arg_list_array(env->gwion->mp, NULL, depth, 0);
  return new_type_list(env->gwion->mp, td, NULL);
}

ANN Type_Decl* str2decl(const Env env, const m_str s, m_uint *depth) {
  m_uint i = 0;
  DECL_OO(m_str, type_name, = get_type_name(env, s, i++))
  DECL_OO(ID_List, id, = str2list(env, type_name, depth))
  Type_Decl* td = new_type_decl(env->gwion->mp, id);
  Type_List tmp = NULL;
  if(!td) {
    free_id_list(env->gwion->mp, id);
    return NULL;
  }
  while((type_name = get_type_name(env, s, i++))) {
    m_uint d = 0;
    if(!tmp)
      td->types = tmp = str2tl(env, type_name, &d);
    else {
      tmp->next = str2tl(env, type_name, &d);
      tmp = tmp->next;
    }
  }
  if(td->types)
    CHECK_OO(type_decl_resolve(env, td))
  return td;
}

ANN static Arg_List make_dll_arg_list(const Gwi gwi, DL_Func * dl_fun) {
  const Env env = gwi->gwion->env;
  Arg_List arg_list    = NULL;
  for(m_uint i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Type_Decl* type_decl = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2;
    if(!(type_decl = str2decl(env, arg->type, &array_depth))) {
      if(arg_list)
        free_arg_list(env->gwion->mp, arg_list);
      GWI_ERR_O(_("  ...  at argument '%i'"), i + 1)
    }
    if((type_path2 = str2list(env, arg->name, &array_depth2)))
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

ANN Type_Decl* import_td(const Gwi gwi, const m_str name) {
  const Env env = gwi->gwion->env;
  m_uint array_depth;
  DECL_OO(const ID_List, type_path, = str2list(env, name, &array_depth))
  Type_Decl* type_decl = new_type_decl(env->gwion->mp, type_path);
  if(!type_decl) {
    free_id_list(env->gwion->mp, type_path);
    return NULL;
  }
  if(array_depth) {
    Array_Sub array_sub = new_array_sub(env->gwion->mp, NULL);
    for(m_uint i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
    type_decl = add_type_decl_array(type_decl, array_sub);
  }
  return type_decl;
}

ANN static Func_Def make_dll_as_fun(const Gwi gwi, DL_Func * dl_fun, ae_flag flag) {
  const MemPool mp = gwi->gwion->mp;
  flag |= ae_flag_builtin;
  Type_Decl* type_decl = import_td(gwi, dl_fun->type);
  if(!type_decl)
    GWI_ERR_O(_("  ...  during @ function import '%s' (type)."), dl_fun->name)
  const m_str name = dl_fun->name;
  const Arg_List arg_list = make_dll_arg_list(gwi, dl_fun);
  const Func_Def func_def = new_func_def(mp, new_func_base(mp, type_decl, insert_symbol(gwi->gwion->st, name), arg_list),
    NULL, flag, loc_cpy(mp, gwi->loc));
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->addr;
  return func_def;
}

ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag) {
  CHECK_BB(name_valid(gwi, gwi->func.name));
  DECL_OB(Func_Def, def, = make_dll_as_fun(gwi, &gwi->func, flag))
  if(gwi->templater.n) {
    def = new_func_def(gwi->gwion->mp, new_func_base(gwi->gwion->mp, NULL, NULL, NULL), NULL, 0, loc_cpy(gwi->gwion->mp, gwi->loc));
    const ID_List list = templater_def(gwi->gwion->st, gwi);
    def->base->tmpl = new_tmpl(gwi->gwion->mp, list, -1);
    SET_FLAG(def, template);
  }
  if(gwi->gwion->env->class_def && GET_FLAG(gwi->gwion->env->class_def, template)) {
    Section* section = new_section_func_def(gwi->gwion->mp, def);
    const Class_Body body = new_class_body(gwi->gwion->mp, section, NULL);
    gwi_body(gwi, body);
    return GW_OK;
  }
  if(traverse_func_def(gwi->gwion->env, def) < 0) {
    free_func_def(gwi->gwion->mp, def);
    return GW_ERROR;
  }
  return GW_OK;
}

static Type get_type(const Env env, const m_str str) {
  m_uint depth = 0;
  const ID_List list = (str && str != (m_str)OP_ANY_TYPE) ? str2list(env, str, &depth) : NULL;
  const Type  t = (str == (m_str) OP_ANY_TYPE) ? OP_ANY_TYPE : list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(env->gwion->mp, list);
  return t ? (depth ? array_type(env, t, depth) : t) : NULL;
}

ANN2(1,2) static int import_op(const Gwi gwi, const DL_Oper* op,
    const f_instr f) {
  const Env env = gwi->gwion->env;
  const Type lhs = op->lhs ? get_type(env, op->lhs) : NULL;
  const Type rhs = op->rhs ? get_type(env, op->rhs) : NULL;
  const Type ret = get_type(env, op->ret);
  const struct Op_Import opi = { lhs, rhs, ret,
    op->ck, op->em, (uintptr_t)f, gwi->loc, op->op };
  return add_op(gwi->gwion, &opi);
}


ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const restrict m_str l,
    const restrict m_str r, const restrict m_str t) {
  gwi->oper.ret = t;
  gwi->oper.rhs = r;
  gwi->oper.lhs = l;
  return GW_OK;
}

ANN m_int gwi_oper_add(const Gwi gwi, Type (*ck)(Env, void*, m_bool*)) {
  gwi->oper.ck = ck;
  return GW_OK;
}

ANN m_int gwi_oper_emi(const Gwi gwi, const opem em) {
  gwi->oper.em = em;
  return GW_OK;
}

ANN m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f) {
  gwi->oper.op = insert_symbol(gwi->gwion->st, op);
  const m_bool ret = import_op(gwi, &gwi->oper, f);
  gwi->oper.ck = NULL;
  gwi->oper.em = NULL;
  return ret;
}

ANN m_int gwi_oper_cond(const Gwi gwi, const m_str type,
  const f_instr f1, const f_instr f2) {
  GWI_BB(gwi_oper_ini(gwi, NULL, type, "int"))
  GWI_BB(gwi_oper_end(gwi, "@conditionnal", f1))
  GWI_BB(gwi_oper_end(gwi, "@unconditionnal", f2))
  return GW_OK;
}

ANN m_int gwi_fptr_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  dl_func_init(&gwi->func, type, name, 0);
  return GW_OK;
}

ANN static Fptr_Def import_fptr(const Gwi gwi, DL_Func* dl_fun, ae_flag flag) {
  const Env env = gwi->gwion->env;
  m_uint array_depth;
  ID_List type_path;
  Type_Decl* type_decl = NULL;
  const Arg_List args = make_dll_arg_list(gwi, dl_fun);
  flag |= ae_flag_builtin;
  if(!(type_path = str2list(env, dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(env->gwion->mp, type_path)))
    GWI_ERR_O(_("  ...  during fptr import '%s' (type)."), dl_fun->name);
  Func_Base *base = new_func_base(env->gwion->mp, type_decl, insert_symbol(env->gwion->st, dl_fun->name), args);
  return new_fptr_def(env->gwion->mp, base, flag);
}

ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  const Fptr_Def fptr = import_fptr(gwi, &gwi->func, flag);
  CHECK_BO(traverse_fptr_def(gwi->gwion->env, fptr))
  if(gwi->gwion->env->class_def)
    SET_FLAG(fptr->base->func->def, builtin);
  else
    SET_FLAG(fptr->base->func, builtin);
  const Type t = fptr->type;
  free_fptr_def(gwi->gwion->mp, fptr);
  return t;
}

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  gwi->val.type = type;
  gwi->val.name = name;
  return GW_OK;
}

ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  Type_Decl* td = import_td(gwi, gwi->val.type);
  if(!td)
    GWI_ERR_O(_("'%s' : invalid type"), gwi->val.type);
  td->flag |= flag;
  const Symbol sym = insert_symbol(gwi->gwion->st, gwi->val.name);
  const Type_Def tdef = new_type_def(gwi->gwion->mp, td, sym);
  traverse_type_def(gwi->gwion->env, tdef);
  const Type t = tdef->type;
  free_type_def(gwi->gwion->mp, tdef);
  return t;
}

ANN static Exp make_exp(const Gwi gwi, const m_str type, const m_str name) {
  const Env env = gwi->gwion->env;
  Type_Decl *type_decl;
  ID_List id_list;
  m_uint array_depth;
  Array_Sub array = NULL;
  CHECK_OO((id_list = str2list(env, type, &array_depth)))
  if(array_depth) {
    array = new_array_sub(env->gwion->mp, NULL);
    array->depth = array_depth;
  }
  type_decl = new_type_decl(env->gwion->mp, id_list);
  const Var_Decl var_decl = new_var_decl(env->gwion->mp,
      insert_symbol(env->gwion->st, name), array, loc_cpy(env->gwion->mp, gwi->loc));
  const Var_Decl_List var_decl_list = new_var_decl_list(env->gwion->mp, var_decl, NULL);
  return new_exp_decl(env->gwion->mp, type_decl, var_decl_list);
}

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str name) {
  if(name)
    gwi->union_data.xid = insert_symbol(gwi->gwion->st, name);
  return GW_OK;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DECL_OB(const Exp, exp, = make_exp(gwi, type, name))
  DECL_OB(const Type, t, = known_type(gwi->gwion->env, exp->d.exp_decl.td))
  if(isa(t, gwi->gwion->type[et_object]) > 0)
    SET_FLAG(exp->d.exp_decl.td, ref);
  gwi->union_data.list = new_decl_list(gwi->gwion->mp, exp, gwi->union_data.list);
  return GW_OK;
}

ANN Type gwi_union_end(const Gwi gwi, const ae_flag flag) {
  if(!gwi->union_data.list)
    GWI_ERR_O(_("union is empty"));
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->union_data.list, loc_cpy(gwi->gwion->mp, gwi->loc));
  udef->flag = flag;
  CHECK_BO(traverse_union_def(gwi->gwion->env, udef))
  emit_union_offset(udef->l, udef->o);
  if(GET_FLAG(udef, member))
    gwi->gwion->env->class_def->nspc->info->offset =
      udef->o + udef->s;
  const Type t = udef->xid ? udef->value->type :
    udef->type_xid ? udef->type : gwi->gwion->type[et_int];
  free_union_def(gwi->gwion->mp, udef);
  gwi->union_data.list = NULL;
  gwi->union_data.xid  = NULL;
  return t;
}

ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  gwi->enum_data.t = type;
  vector_init(&gwi->enum_data.addr);
  return GW_OK;
}

ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  const ID_List list = new_id_list(gwi->gwion->mp, insert_symbol(gwi->gwion->st, name), loc_cpy(gwi->gwion->mp, gwi->loc));
  DL_Enum* d = &gwi->enum_data;
  ALLOC_PTR(gwi->gwion->mp, addr, m_int, i);
  vector_add(&gwi->enum_data.addr, (vtype)addr);
  if(!d->base)
    d->base = list;
  else
    d->curr->next = list;
  d->curr = list;
  return list ? 1 : -1;
}

ANN static void import_enum_end(const Gwi gwi, const Vector v) {
  DL_Enum* d = &gwi->enum_data;
  for(m_uint i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&d->addr, i);
    SET_FLAG(value, builtin);
    ADD_REF(value->type);
    if(!gwi->gwion->env->class_def)
      value->d.ptr = (m_uint*)(addr ? addr : i);
    else
      value->d.ptr = (m_uint*)(addr ? *(m_uint*)addr : i);
  }
  d->t = NULL;
  d->base = NULL;
  vector_release(&d->addr);
}

ANN Type gwi_enum_end(const Gwi gwi) {
  DL_Enum* d = &gwi->enum_data;
  const Enum_Def edef  = new_enum_def(gwi->gwion->mp, d->base, d->t ? insert_symbol(gwi->gwion->st, d->t) : NULL, 
    loc_cpy(gwi->gwion->mp, gwi->loc));
  if(traverse_enum_def(gwi->gwion->env, edef) < 0) {
    free_id_list(gwi->gwion->mp, d->base);
    return NULL;
  }
  import_enum_end(gwi, &edef->values);
  const Type t =edef->t;
  free_enum_def(gwi->gwion->mp, edef);
  return t;
}

ANN void register_freearg(const Gwi gwi, const f_instr _exec, const f_freearg _free) {
  map_set(&gwi->gwion->data->freearg, (vtype)_exec, (vtype)_free);
}

ANN void gwi_reserve(const Gwi gwi, const m_str str) {
  vector_add(&gwi->gwion->data->reserved, (vtype)insert_symbol(gwi->gwion->st, str));
}

ANN void gwi_specialid(const Gwi gwi, const m_str id, const SpecialId spid) {
  struct SpecialId_ *a = mp_calloc(gwi->gwion->mp, SpecialId);
  a->type = spid->type;
  a->ck = spid->ck;
  a->exec = spid->exec;
  a->em = spid->em;
  map_set(&gwi->gwion->data->id, (vtype)insert_symbol(gwi->gwion->st, id), (vtype)a);
}

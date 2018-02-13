#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "import.h"
#include "gwi.h"
#include "emit.h"

struct Path {
  m_str path, curr;
  m_uint len;
};

static ID_List templater_def(Templater* templater) {
  m_uint i;
  ID_List list[templater->n];
  list[0] = new_id_list(insert_symbol(templater->list[0]), 0);
  for(i = 1; i < templater->n; i++) {
    list[i] = new_id_list(insert_symbol(templater->list[i]), 0);
    list[i - 1]->next = list[i];
  }
  return list[0];
}

m_int gwi_tmpl_end(Gwi gwi) {
  gwi->templater.n = 0;
  gwi->templater.list = NULL;
  return 1;
}

m_int gwi_tmpl_ini(Gwi gwi, m_uint n, const m_str* list) {
  gwi->templater.n = n;
  gwi->templater.list = (m_str*)list;
  return 1;
}

static void dl_func_init(DL_Func* a, const m_str t, const m_str n, const f_xfun addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

m_int gwi_func_ini(Gwi gwi, const m_str t, const m_str n, f_xfun addr) {
  dl_func_init(&gwi->func, t, n, addr);
  return 1;
}

static void dl_func_func_arg(DL_Func* a, const m_str t, const m_str n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

m_int gwi_func_arg(Gwi gwi, const m_str t, const m_str n) {
  if(gwi->func.narg == DLARG_MAX - 1)
    CHECK_BB(err_msg(UTIL_, 0,
          "too many arguments for function '%s'.", gwi->func.name))
  dl_func_func_arg(&gwi->func, t, n);
  return 1;
}

static m_bool check_illegal(char* curr, char c, m_uint i) {
  if(isalnum(c) || c == '_' || (i == 1 && c== '@'))
    curr[i - 1] = c;
  else
    return -1;
  return 1;
}

static m_bool name_valid(m_str a) {
  m_uint i, len = strlen(a);
  m_uint lvl = 0;
  for(i = 0; i < len; i++) {
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
        CHECK_BB(err_msg(UTIL_,  0, "illegal use of ',' outside of templating in name '%s'...", a))
      continue;
    }
    if(c == '>') {
      if(!lvl)
        CHECK_BB(err_msg(UTIL_,  0, "illegal templating in name '%s'...", a))
      lvl--;
      continue;
    }
    CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in name '%s'...", c, a))
  }
  return !lvl ? 1 : -1;
}

static void path_valid_inner(m_str curr) {
  m_int j, size = strlen(curr);
  for(j = (size / 2) + 1; --j;) {
    char s = curr[j];
    curr[j] = curr[size - j - 1];
    curr[size - j - 1] = s;
  }
}

static m_bool path_valid(ID_List* list, struct Path* p) {
  char last = '\0';
  m_uint i;
  for(i = p->len + 1; --i;) {
    char c = p->path[i - 1];
    if(c != '.' && check_illegal(p->curr, c, i) < 0)
      CHECK_BB(err_msg(UTIL_,  0,
            "illegal character '%c' in path '%s'...", c, p->path))
    if(c == '.' || i == 1) {
      if((i != 1 && last != '.' && last != '\0') ||
          (i ==  1 && c != '.')) {
        path_valid_inner(p->curr);
        *list = prepend_id_list(insert_symbol(p->curr), *list, 0);
        memset(p->curr, 0, p->len + 1);
      } else
        CHECK_BB(err_msg(UTIL_,  0,
              "path '%s' must not ini or end with '.'", p->path))
    }
    last = c;
  }
  return 1;
}

static ID_List str2list(m_str path, m_uint* array_depth) {
  m_uint len = strlen(path);
  ID_List list = NULL;
  m_uint depth = 0;
  char curr[len + 1];
  struct Path p = { path, curr, len };
  memset(curr, 0, len + 1);

  while(p.len > 2 && path[p.len - 1] == ']' && path[p.len - 2] == '[') {
    depth++;
    p.len -= 2;
  }
  if(path_valid(&list, &p) < 0) {
    if(list)
      free_id_list(list);
    return NULL;
  }
  CHECK_OO(list)
  strncpy(curr, path, p.len);
  list->xid = insert_symbol(curr);
  *array_depth = depth;
  return list;
}

static m_bool mk_xtor(Type type, m_uint d, e_func e) {
  VM_Code* code = e == NATIVE_CTOR ? &type->info->pre_ctor : &type->info->dtor;
  m_str name = type->name;
//  m_str filename = e == NATIVE_CTOR ? "[ctor]" : "[dtor]";

  SET_FLAG(type, e == NATIVE_CTOR ? ae_flag_ctor : ae_flag_dtor);
//  *code = new_vm_code(NULL, SZ_INT, 1, code_name_set(name, filename));
  *code = new_vm_code(NULL, SZ_INT, 1, name);
  (*code)->native_func = (m_uint)d;
  (*code)->flag = (e | _NEED_THIS_);
  return 1;
}

m_int gwi_add_type(Gwi gwi, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  CHECK_BB(env_add_type(gwi->env, type))
  return type->xid;
}

static m_bool import_class_ini(Env env, Type type, f_xtor pre_ctor, f_xtor dtor) {
  type->info = new_nspc(type->name);
  type->info->parent = env->curr;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
    mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->info->offset;
    if(type->parent->info->vtable.ptr)
      vector_copy2(&type->parent->info->vtable, &type->info->vtable);
  }
  type->owner = env->curr;
  SET_FLAG(type, ae_flag_checked);
  CHECK_BB(env_push_class(env, type))
  return 1;
}

m_int gwi_class_ini(Gwi gwi, Type type, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info)
    CHECK_BB(err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name))
  if(gwi->templater.n) {
    ID_List types = templater_def(&gwi->templater); // improve me ?
    type->def = calloc(1, sizeof(struct Class_Def_));
    type->def->tmpl = new_tmpl_class(types, 1);
    type->def->type = type;
    SET_FLAG(type, ae_flag_template);
  } else
    SET_FLAG(type, ae_flag_scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  CHECK_BB(gwi_add_type(gwi, type))
  CHECK_BB(import_class_ini(gwi->env, type, pre_ctor, dtor))
  return type->xid;
}

m_int gwi_class_ext(Gwi gwi, Type_Decl* td) {
  if(!td->types)
    CHECK_BB(err_msg(TYPE_, 0, "gwi_class_ext invoked before "
          "gwi_class_ini"))
  VM_Code ctor = gwi->env->class_def->info->pre_ctor;
  if(gwi->env->class_def->parent ||
      (gwi->env->class_def->def && gwi->env->class_def->def->ext))
    CHECK_BB(err_msg(TYPE_, 0, "class extend already set"))
  if(td->array && !td->array->exp_list)
    CHECK_BB(err_msg(TYPE_, 0, "class extend array can't be empty"))
  if(!gwi->env->class_def->def) {
    Type t;
    CHECK_OB((t = type_decl_resolve(gwi->env, td)))
    if(td->array)
      SET_FLAG(gwi->env->class_def, ae_flag_typedef);
    gwi->env->class_def->parent = t;
    gwi->env->class_def->info->offset = t->info->offset;
    if(t->info->vtable.ptr)
      vector_copy2(&t->info->vtable, &gwi->env->class_def->info->vtable);
      gwi->env->class_def->info->pre_ctor = new_vm_code(NULL,
//          SZ_INT, 1, code_name_set(gwi->env->class_def->name,
//            "ext ctor"));
          SZ_INT, 1, gwi->env->class_def->name);
    CHECK_OB((gwi->emit->code = emit_class_code(gwi->emit,
          gwi->env->class_def->name)))
    if(td->array)
      CHECK_BB(emit_array_extend(gwi->emit, t, td->array->exp_list))
    if(ctor)
      CHECK_BB(emit_ext_ctor(gwi->emit, ctor))
    CHECK_BB(emit_class_finish(gwi->emit, gwi->env->class_def->info))
    free_type_decl(td);
  } else {
// use ctor here too?
    SET_FLAG(td, ae_flag_typedef);
    gwi->env->class_def->def->ext = td;
  }
  return 1;
}

static m_int import_class_end(Env env) {
  if(!env->class_def)
    CHECK_BB(err_msg(TYPE_, 0, "import: too many class_end called..."))
  Nspc nspc = env->class_def->info;
  if(nspc->class_data_size && !nspc->class_data)
    nspc->class_data = calloc(1, nspc->class_data_size);
  CHECK_BB(env_pop_class(env))
  return 1;
}

m_int gwi_class_end(Gwi gwi) {
  return import_class_end(gwi->env);
}

static void dl_var_new_array(DL_Var* v) {
  v->t.array = new_array_sub(NULL, 0);
  v->t.array->depth = v->array_depth;
  v->var.array = new_array_sub(NULL, 0);
  v->var.array->depth = v->array_depth;
}

static void dl_var_set(DL_Var* v, ae_flag flag) {
  v->list.self = &v->var;
  v->t.flag = flag;
  v->exp.exp_type = ae_exp_decl;
  v->exp.d.exp_decl.type = &v->t;
  v->exp.d.exp_decl.list = &v->list;
  v->exp.d.exp_decl.self = &v->exp;
  if(v->array_depth)
    dl_var_new_array(v);
}

static void dl_var_release(DL_Var* v) {
  if(v->array_depth) {
    free_array_sub(v->t.array);
    free_array_sub(v->var.array);
  }
  free_id_list(v->t.xid);
}

m_int gwi_item_ini(Gwi gwi, const m_str type, const m_str name) {
  DL_Var* v = &gwi->var;
  memset(v, 0, sizeof(DL_Var));
  if(!(v->t.xid = str2list(type, &v->array_depth)))
    CHECK_BB(err_msg(TYPE_, 0, "... during var import '%s.%s'...",
          gwi->env->class_def->name, name))
  v->var.xid = insert_symbol(name);
  return 1;
}

#undef gwi_item_end
m_int gwi_item_end(Gwi gwi, const ae_flag flag, const m_uint* addr) {
  DL_Var* v = &gwi->var;
  dl_var_set(v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(gwi->env->class_def && GET_FLAG(gwi->env->class_def, ae_flag_template)) {
    Type_Decl *type_decl = new_type_decl(v->t.xid, flag, 0);
    Var_Decl var_decl = new_var_decl(v->var.xid, v->var.array, 0);
    Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL, 0);
    Exp exp = new_exp_decl(type_decl, var_decl_list, 0);
    Stmt stmt = new_stmt_exp(exp, 0);
    Stmt_List list = new_stmt_list(stmt, NULL, 0);
    Section* section = new_section_stmt_list(list, 0);
    Class_Body body = new_class_body(section, NULL, 0);
    type_decl->array = v->t.array;
    if(!gwi->env->class_def->def->body)
      gwi->env->class_def->def->body = gwi->body = body;
    else {
      gwi->body->next = body;
      gwi->body = body;
    }
    return 1;
  }
  CHECK_BB(traverse_decl(gwi->env, &v->exp.d.exp_decl))
  SET_FLAG(v->var.value, ae_flag_builtin);
  dl_var_release(v);
  return v->var.value->offset;
}

static Array_Sub make_dll_arg_list_array(Array_Sub array_sub,
  m_uint* array_depth, m_uint array_depth2) {
  m_uint i;
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) {
    array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
  }
  return array_sub;
}

static Type_Decl* str2decl(Env env, m_str s, m_uint *depth);
static Type_List str2tl(Env env, m_str s, m_uint *depth) {
  Type_Decl* td = str2decl(env, s, depth);
  td->array = make_dll_arg_list_array(NULL, depth, 0);
  Type_List tl = new_type_list(td, NULL, 0);
  return tl;
}

static Type_Decl* str2decl(Env env, m_str s, m_uint *depth) {
  m_uint i = 0;
  m_str type_name = get_type_name(s, i++);
  CHECK_OO(type_name)
  ID_List id = str2list(type_name, depth);
  CHECK_OO(id)
  Type_Decl* td = new_type_decl(id, 0, 0);
  Type_List tmp = NULL;
  if(!td) {
    free_id_list(id);
    return NULL;
  }
  while((type_name = get_type_name(s, i++))) {
    m_uint depth = 0;
    if(!tmp)
      td->types = tmp = str2tl(env, type_name, &depth);
    else {
      tmp->next = str2tl(env, type_name, &depth);
      tmp = tmp->next;
    }
  }
  if(td->types)
    CHECK_OO(type_decl_resolve(env, td))
  return td;
}

static Arg_List make_dll_arg_list(Env env, DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  m_int i = 0;

  for(i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Array_Sub array_sub = NULL;
    Type_Decl* type_decl = NULL;
    Var_Decl var_decl    = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2;
    if(!(type_decl = str2decl(env, arg->type, &array_depth))) {
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_,  0, "...at argument '%i'...", i + 1))
    }
    if((type_path2 = str2list(arg->name, &array_depth2)))
      free_id_list(type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(type_decl);
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_, 0, "array subscript specified incorrectly for built-in module"))
    }
    array_sub = make_dll_arg_list_array(array_sub, &array_depth, array_depth2);
    var_decl = new_var_decl(insert_symbol(arg->name), array_sub, 0);
    arg_list = new_arg_list(type_decl, var_decl, arg_list, 0);
  }
  return arg_list;
}

static Func_Def make_dll_as_fun(Env env, DL_Func * dl_fun, ae_flag flag) {
  Func_Def func_def = NULL;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  flag |= ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0, 0)))
    CHECK_BO(err_msg(TYPE_, 0, "...during @ function import '%s' (type)...", dl_fun->name))
  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }
  name = dl_fun->name;
  arg_list = make_dll_arg_list(env, dl_fun);
  func_def = new_func_def(flag, type_decl, insert_symbol(name), arg_list, NULL, 0);
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->addr;
  return func_def;
}

static Func_Def import_fun(Env env, DL_Func * mfun, ae_flag flag) {
  CHECK_OO(mfun) // probably deserve an err msg or attr non-null
  CHECK_BO(name_valid(mfun->name));
  return make_dll_as_fun(env, mfun, flag);
}

m_int gwi_func_end(Gwi gwi, ae_flag flag) {
  Func_Def def = import_fun(gwi->env, &gwi->func, flag);

  CHECK_OB(def)
  if(gwi->templater.n) {
    def = calloc(1, sizeof(struct Class_Def_));
    ID_List list = templater_def(&gwi->templater);
    def->tmpl = new_tmpl_list(list, 1);
    SET_FLAG(def, ae_flag_template);
  }
  if(gwi->env->class_def && GET_FLAG(gwi->env->class_def, ae_flag_template)) {
    Section* section = new_section_func_def(def, 0);
    Class_Body body = new_class_body(section, NULL, 0);
    if(!gwi->env->class_def->def->body)
      gwi->env->class_def->def->body = gwi->body = body;
    else {
      gwi->body->next = body;
      gwi->body = body;
    }
    return 1;
  }
  if(traverse_func_def(gwi->env, def) < 0) {
    free_func_def(def);
    return -1;
  }
  return 1;
}

static Type get_type(Env env, const m_str str) {
  m_uint depth = 0;
  ID_List list = (str && str != (m_str)OP_ANY_TYPE) ? str2list(str, &depth) : NULL;
  Type  t = (str == (m_str) OP_ANY_TYPE) ? OP_ANY_TYPE : list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(list);
  return t ? (depth ? array_type(t, depth) : t) : NULL;
}

static m_int import_op(Env env, DL_Oper* op,
                const f_instr f) {
  Type lhs = op->lhs ? get_type(env, op->lhs) : NULL;
  Type rhs = op->rhs ? get_type(env, op->rhs) : NULL;
  Type ret = get_type(env, op->ret);
  struct Op_Import opi = { op->op, lhs, rhs, ret,
    op->ck, op->em, (uintptr_t)f };
  return env_add_op(env, &opi);
}

m_int gwi_oper_ini(Gwi gwi, const m_str l, const m_str r, const m_str t) {
  gwi->oper.ret = t;
  gwi->oper.rhs = r;
  gwi->oper.lhs = l;
  return 1;
}

m_int gwi_oper_add(Gwi gwi, Type (*ck)(Env, void*)) {
  gwi->oper.ck = ck;
  return 1;
}

m_int gwi_oper_emi(Gwi gwi, m_bool (*em)(Emitter, void*)) {
  gwi->oper.em = em;
  return 1;
}

m_int gwi_oper_end(Gwi gwi, Operator op, const f_instr f) {
  m_bool ret;
  gwi->oper.op = op;
  ret = import_op(gwi->env, &gwi->oper, f);
  gwi->oper.ck = NULL;
  gwi->oper.em = NULL;
  return ret;
}

m_int gwi_fptr_ini(Gwi gwi, const m_str type, const m_str name) {
  dl_func_init(&gwi->func, type, name, 0);
  return 1;
}

static Stmt import_fptr(Env env, DL_Func* dl_fun, ae_flag flag) {
  m_uint array_depth;
  ID_List type_path;
  Type_Decl* type_decl = NULL;
  Arg_List args = make_dll_arg_list(env, dl_fun);
  flag |= ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0, 0)))
    CHECK_BO(err_msg(TYPE_, 0, "...during @ function import '%s' (type)...",
          dl_fun->name))
  return new_func_ptr_stmt(flag, insert_symbol(dl_fun->name), type_decl, args, 0);
}
#include "func.h"
m_int gwi_fptr_end(Gwi gwi, ae_flag flag) {
  Stmt stmt = import_fptr(gwi->env, &gwi->func, flag);
  CHECK_BB(traverse_stmt_fptr(gwi->env, &stmt->d.stmt_ptr))
  if(gwi->env->class_def)
    SET_FLAG(stmt->d.stmt_ptr.func->def, ae_flag_builtin);
  else
    SET_FLAG(stmt->d.stmt_ptr.func, ae_flag_builtin);
  free(stmt);
  return 1;
}

static Exp make_exp(const m_str type, const m_str name) {
  Type_Decl *type_decl;
  ID_List id_list;
  m_uint array_depth;
  Array_Sub array = NULL;
  CHECK_OO((id_list = str2list(type, &array_depth)))
  if(array_depth) {
    array = new_array_sub(NULL, 0);
    array->depth = array_depth;
  }
  type_decl = new_type_decl(id_list, 0, 0);
  Var_Decl var_decl = new_var_decl(insert_symbol(name), array, 0);
  Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL, 0);
  return new_exp_decl(type_decl, var_decl_list, 0);
}

m_int gwi_union_ini(Gwi gwi, const m_str name) {
  if(name)
    gwi->union_data.xid = insert_symbol(name);
  return 1;
}

m_int gwi_union_add(Gwi gwi, const m_str type, const m_str name) {
  Exp exp = make_exp(type, name);
  gwi->union_data.list = new_decl_list(exp, gwi->union_data.list);
  return 1;
}

m_int gwi_union_end(Gwi gwi, ae_flag flag) {
  Stmt stmt = new_stmt_union(gwi->union_data.list, 0);
  CHECK_BB(traverse_stmt_union(gwi->env, &stmt->d.stmt_union))
  emit_union_offset(stmt->d.stmt_union.l, stmt->d.stmt_union.o);
  if(GET_FLAG((&stmt->d.stmt_union), ae_flag_member))
    gwi->env->class_def->info->offset =
      stmt->d.stmt_union.o + stmt->d.stmt_union.s;
  free_stmt(stmt);
  gwi->union_data.list = NULL;
  gwi->union_data.xid  = NULL;
  return 1;
}

m_int gwi_enum_ini(Gwi gwi, const m_str type) {
  gwi->enum_data.t = type;
  vector_init(&gwi->enum_data.addr);
  return 1;
}

m_int gwi_enum_add(Gwi gwi, const m_str name, const m_uint addr) {
  ID_List list = new_id_list(insert_symbol(name), 0);
  DL_Enum* d = &gwi->enum_data;
  vector_add(&gwi->enum_data.addr, addr);
  if(!d->base)
    d->base = list;
  else
    d->curr->next = list;
  d->curr = list;
  return list ? 1 : -1;
}

static void import_enum_end(DL_Enum* d, Vector v) {
  m_uint i;

  for(i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&d->addr, i);
    SET_FLAG(value, ae_flag_builtin);
    value->d.ptr = (m_uint*)(addr ? addr : i);
  }
  d->t = NULL;
  d->base = NULL;
  vector_release(&d->addr);
}

m_int gwi_enum_end(Gwi gwi) {
  DL_Enum* d = &gwi->enum_data;
  Stmt stmt = new_stmt_enum(d->base, d->t ? insert_symbol(d->t) : NULL, 0);

  CHECK_OB(stmt)
  if(traverse_stmt_enum(gwi->env, &stmt->d.stmt_enum) < 0) {
    free_id_list(d->base);
    return -1;
  }
  import_enum_end(d, &stmt->d.stmt_enum.values);
  free_stmt(stmt);
  return 1;
}

m_int gwi_add_value(Gwi gwi, const m_str name, Type type, const m_bool is_const, void* value) {
  return env_add_value(gwi->env, name, type, is_const, value);
}

OP_CHECK(opck_const_lhs) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->lhs->meta != ae_meta_var) {
    if(err_msg(TYPE_, bin->pos, "cannot assign '%s' on types '%s' and'%s'..."
          "...(reason: --- left-side operand is not mutable)",
          op2str(bin->op), bin->lhs->type->name, bin->lhs->type->name) < 0)
    return &t_null;
  }
  return bin->lhs->type;
}

OP_CHECK(opck_assign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_const_lhs(env, data) == &t_null)
    return &t_null;
  bin->lhs->emit_var = 1;
  return bin->lhs->type;
}

OP_CHECK(opck_rhs_emit_var) {
  Exp_Binary* bin = (Exp_Binary*)data;
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

OP_CHECK(opck_rassign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->meta != ae_meta_var) {
    if(err_msg(TYPE_, bin->pos,
          "cannot assign '%s' on types '%s' and'%s'...\n"
          "\t...(reason: --- right-side operand is not mutable)",
          op2str(bin->op), bin->lhs->type->name, bin->rhs->type->name) < 0)
      return &t_null;
  }
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

OP_CHECK(opck_unary_meta) {
  Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->meta = ae_meta_value;
  return unary->exp->type;
}

OP_CHECK(opck_unary) {
  Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->exp->meta != ae_meta_var)
    if(err_msg(TYPE_, unary->exp->pos,
          "unary operator '%s' cannot be used on non-mutable data-types...",
          op2str(unary->op)) < 0)
      return &t_null;
  unary->exp->emit_var = 1;
  unary->self->meta = ae_meta_value;
  return unary->exp->type;
}
Type check_exp_unary_spork(Env env, Stmt code);
OP_CHECK(opck_spork) {
  Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->exp && unary->exp->exp_type == ae_exp_call)
    return &t_shred;
  else if(unary->code)
    return check_exp_unary_spork(env, unary->code);
  else
    CHECK_BO(err_msg(TYPE_,  unary->pos,
          "only function calls can be sporked..."))
  return NULL;
}

OP_CHECK(opck_post) {
  Exp_Postfix* post = (Exp_Postfix*)data;
  if(post->exp->meta != ae_meta_var)
    if(err_msg(TYPE_, post->exp->pos,
          "post operator '%s' cannot be used on non-mutable data-type...",
          op2str(post->op)) < 0)
        return &t_null;
  post->exp->emit_var = 1;
  post->self->meta = ae_meta_value;
  return post->exp->type;
}

Type   check_exp(Env env, Exp exp);
m_bool check_exp_array_subscripts(Env env, Exp exp);
OP_CHECK(opck_new) {
  Exp_Unary* unary = (Exp_Unary*)data;
  Type t = type_decl_resolve(env, unary->type);
  if(!t)
    CHECK_BO(type_unknown(unary->type->xid, "'new' expression"))
  if(unary->type->array) {
    CHECK_OO(check_exp(env, unary->type->array->exp_list))
    CHECK_BO(check_exp_array_subscripts(env, unary->type->array->exp_list))
  } else
    CHECK_BO(prim_ref(unary->type, t))
  return t;
}

#include "instr.h"
m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array, m_bool is_ref);
OP_EMIT(opem_new) {
  Exp_Unary* unary = (Exp_Unary*)data;
  CHECK_BB(emit_instantiate_object(emit, unary->self->type,
    unary->type->array, GET_FLAG(unary->type, ae_flag_ref)))
  CHECK_OB(emitter_add_instr(emit, add2gc))
  return 1;
}


m_bool emit_exp_spork(Emitter emit, Exp_Func* exp);
m_bool emit_exp_spork1(Emitter emit, Stmt stmt);
OP_EMIT(opem_spork) {
  Exp_Unary* unary = (Exp_Unary*)data;
  CHECK_BB((unary->code ? emit_exp_spork1(emit, unary->code) : 
        emit_exp_spork(emit, &unary->exp->d.exp_func)))
  return 1;
}

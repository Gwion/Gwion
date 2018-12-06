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
#include "import.h"
#include "gwi.h"
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "operator.h"

struct Path {
  m_str path, curr;
  m_uint len;
};

ANN static ID_List templater_def(const Templater* templater) {
  ID_List list[templater->n];
  list[0] = new_id_list(insert_symbol(templater->list[0]), 0);
  for(m_uint i = 1; i < templater->n; i++) {
    list[i] = new_id_list(insert_symbol(templater->list[i]), 0);
    list[i - 1]->next = list[i];
  }
  return list[0];
}

ANN VM* gwi_vm(const Gwi gwi) {
  return gwi->vm;
}

ANN m_int gwi_tmpl_end(const Gwi gwi) {
  gwi->templater.n = 0;
  gwi->templater.list = NULL;
  return 1;
}

ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str* list) {
  gwi->templater.n = n;
  gwi->templater.list = (m_str*)list;
  return 1;
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
  return 1;
}

ANN static void dl_func_func_arg(DL_Func* a, const restrict m_str t, const restrict m_str n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

ANN m_int gwi_func_arg(const Gwi gwi, const restrict m_str t, const restrict m_str n) {
  if(gwi->func.narg == DLARG_MAX - 1)
    ERR_B(0, "too many arguments for function '%s'.", gwi->func.name)
  dl_func_func_arg(&gwi->func, t, n);
  return 1;
}

ANN static m_bool check_illegal(char* curr, const char c, const m_uint i) {
  if(isalnum(c) || c == '_' || (i == 1 && c== '@'))
    curr[i - 1] = c;
  else
    return -1;
  return 1;
}

ANN static m_bool name_valid(const m_str a) {
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
        ERR_B(0, "illegal use of ',' outside of templating in name '%s'.", a)
      continue;
    }
    if(c == '>') {
      if(!lvl)
        ERR_B(0, "illegal templating in name '%s'.", a)
      lvl--;
      continue;
    }
    ERR_B(0, "illegal character '%c' in name '%s'.", c, a)
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

ANN static m_bool path_valid(ID_List* list, const struct Path* p) {
  char last = '\0';
  for(m_uint i = p->len + 1; --i;) {
    const char c = p->path[i - 1];
    if(c != '.' && check_illegal(p->curr, c, i) < 0)
      ERR_B(0, "illegal character '%c' in path '%s'.", c, p->path)
    if(c == '.' || i == 1) {
      if((i != 1 && last != '.' && last != '\0') ||
          (i ==  1 && c != '.')) {
        path_valid_inner(p->curr);
        *list = prepend_id_list(insert_symbol(p->curr), *list, 0);
        memset(p->curr, 0, p->len + 1);
      } else
        ERR_B(0, "path '%s' must not ini or end with '.'.", p->path)
    }
    last = c;
  }
  return 1;
}

ANN static ID_List str2list(const m_str path, m_uint* array_depth) {
  const m_uint len = strlen(path);
  ID_List list = NULL;
  m_uint depth = 0;
  char curr[len + 1];
  struct Path p = { path, curr, len };
  memset(curr, 0, len + 1);

  while(p.len > 2 && path[p.len - 1] == ']' && path[p.len - 2] == '[') {
    depth++;
    p.len -= 2;
  }
  *array_depth = depth;
  if(path_valid(&list, &p) < 0) {
    if(list)
      free_id_list(list);
    return NULL;
  }
  CHECK_OO(list)
  strncpy(curr, path, p.len);
  list->xid = insert_symbol(curr);
  return list;
}

ANN static m_bool mk_xtor(const Type type, const m_uint d, const ae_flag e) {
  VM_Code* code = e == ae_flag_ctor ? &type->nspc->pre_ctor : &type->nspc->dtor;
  const m_str name = type->name;
  *code = new_vm_code(NULL, SZ_INT, 1, name);
  (*code)->native_func = (m_uint)d;
  (*code)->flag = (e | ae_flag_member | ae_flag_builtin);
  type->flag |= e;
  return 1;
}

ANN2(1,2) Type gwi_mk_type(const Gwi gwi __attribute__((unused)), const m_str name, const m_uint size, const Type parent) {
  const Type t = new_type(0, name, parent);
  t->size = size;
  return t;
}

ANN m_int gwi_add_type(const Gwi gwi, const Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  env_add_type(gwi->env, type);
  return (m_int)type->xid;
}

ANN2(1,2) static m_bool import_class_ini(const Env env, const Type type,
    const f_xtor pre_ctor, const f_xtor dtor) {
  type->nspc = new_nspc(type->name);
  type->nspc->parent = env->curr;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, ae_flag_ctor);
  if(dtor)
    mk_xtor(type, (m_uint)dtor, ae_flag_dtor);
  if(type->parent) {
    type->nspc->offset = type->parent->nspc->offset;
    if(type->parent->nspc->vtable.ptr)
      vector_copy2(&type->parent->nspc->vtable, &type->nspc->vtable);
  }
  type->owner = env->curr;
  SET_FLAG(type, checked);
  m_uint scope;
  env_push(env, type, type->nspc, &scope);
  return 1;
}

ANN2(1,2) m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor) {
  if(type->nspc)
    ERR_B(0, "during import: class '%s' already imported.", type->name)
  if(gwi->templater.n) {
    const ID_List types = templater_def(&gwi->templater);
    type->def = new_class_def(0, new_id_list(insert_symbol(type->name), 0), NULL, NULL);
    type->def->tmpl = new_tmpl_class(types, -1);
    type->def->type = type;
    SET_FLAG(type, template);
  } else
    SET_FLAG(type, scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  gwi_add_type(gwi, type);
  CHECK_BB(import_class_ini(gwi->env, type, pre_ctor, dtor))
  return (m_int)type->xid;
}

ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td) {
  if(!gwi->env->class_def)
    ERR_B(0, "gwi_class_ext invoked before "
          "gwi_class_ini")
  const VM_Code ctor = gwi->env->class_def->nspc->pre_ctor;
  if(gwi->env->class_def->parent ||
      (gwi->env->class_def->def && gwi->env->class_def->def->ext))
    ERR_B(0, "class extend already set")
  if(td->array && !td->array->exp)
    ERR_B(0, "class extend array can't be empty")
  if(!gwi->env->class_def->def) {
    const Type t = known_type(gwi->env, td, "builtin class extend");
    CHECK_OB(t)
    if(td->array)
      SET_FLAG(gwi->env->class_def, typedef);
    gwi->env->class_def->parent = t;
    gwi->env->class_def->nspc->offset = t->nspc->offset;
    if(t->nspc->vtable.ptr)
      vector_copy2(&t->nspc->vtable, &gwi->env->class_def->nspc->vtable);
    CHECK_OB((gwi->emit->code = emit_class_code(gwi->emit,
          gwi->env->class_def->name)))
    if(td->array)
      CHECK_BB(emit_array_extend(gwi->emit, t, td->array->exp))
    if(ctor)
      emit_ext_ctor(gwi->emit, ctor);
    emit_class_finish(gwi->emit, gwi->env->class_def->nspc);
    free_type_decl(td);
  } else {
    SET_FLAG(td, typedef);
    gwi->env->class_def->def->ext = td;
  }
  return 1;
}

ANN static m_int import_class_end(const Env env) {
  if(!env->class_def)
    ERR_B(0, "import: too many class_end called.")
  const Nspc nspc = env->class_def->nspc;
  if(nspc->class_data_size && !nspc->class_data)
    nspc->class_data = (m_bit*)xcalloc(1, nspc->class_data_size);
  env_pop(env, 0);
  return 1;
}

#include "mpool.h"
ANN m_int gwi_class_end(const Gwi gwi) {
  if(!gwi->env->class_def)return -1;
  const Type t = gwi->env->class_def;
  if(t->nspc && t->nspc->offset)
    t->p = mp_ini((uint32_t)t->nspc->offset);
  return import_class_end(gwi->env);
}

ANN static void dl_var_new_exp_array(DL_Var* v) {
  v->t.array = new_array_sub(NULL);
  v->t.array->depth = v->array_depth;
  v->var.array = new_array_sub(NULL);
  v->var.array->depth = v->array_depth;
}

ANN static void dl_var_set(DL_Var* v, const ae_flag flag) {
  v->list.self = &v->var;
  v->t.flag = flag;
  v->exp.exp_type = ae_exp_decl;
  v->exp.d.exp_decl.td   = &v->t;
  v->exp.d.exp_decl.list = &v->list;
  v->exp.d.exp_decl.self = &v->exp;
  if(v->array_depth)
    dl_var_new_exp_array(v);
}

ANN static void dl_var_release(const DL_Var* v) {
  if(v->array_depth) {
    free_array_sub(v->t.array);
    free_array_sub(v->var.array);
  }
  free_id_list(v->t.xid);
}

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DL_Var* v = &gwi->var;
  memset(v, 0, sizeof(DL_Var));
  if(!(v->t.xid = str2list(type, &v->array_depth)))
    ERR_B(0, "\t...\tduring var import '%s.%s'.",
          gwi->env->class_def->name, name)
  v->var.xid = insert_symbol(name);
  return 1;
}

#undef gwi_item_end

static void gwi_body(const Gwi gwi, const Class_Body body) {
  if(!gwi->env->class_def->def->body)
    gwi->env->class_def->def->body = body;
  else
    gwi->body->next = body;
  gwi->body = body;
}

ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr) {
  DL_Var* v = &gwi->var;
  dl_var_set(v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(gwi->env->class_def && GET_FLAG(gwi->env->class_def, template)) {
    Type_Decl *type_decl = new_type_decl(v->t.xid, flag);
    const Var_Decl var_decl = new_var_decl(v->var.xid, v->var.array, 0);
    const Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL);
    const Exp exp = new_exp_decl(type_decl, var_decl_list);
    const Stmt stmt = new_stmt_exp(ae_stmt_exp, exp);
    const Stmt_List list = new_stmt_list(stmt, NULL);
    Section* section = new_section_stmt_list(list);
    const Class_Body body = new_class_body(section, NULL);
    type_decl->array = v->t.array;
    gwi_body(gwi, body);
    return 1;
  }
  CHECK_BB(traverse_decl(gwi->env, &v->exp.d.exp_decl))
  SET_FLAG(v->var.value, builtin);
  dl_var_release(v);
  return (m_int)v->var.value->offset;
}

static Array_Sub make_dll_arg_list_array(Array_Sub array_sub,
  m_uint* array_depth, const m_uint array_depth2) {
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) {
    array_sub = new_array_sub(NULL);
    for(m_uint i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
  }
  return array_sub;
}

ANN static Type_List str2tl(const Env env, const m_str s, m_uint *depth) {
  Type_Decl* td = str2decl(env, s, depth);
  td->array = make_dll_arg_list_array(NULL, depth, 0);
  return new_type_list(td, NULL);
}

ANN Type_Decl* str2decl(const Env env, const m_str s, m_uint *depth) {
  m_uint i = 0;
  m_str type_name = get_type_name(s, i++);
  CHECK_OO(type_name)
  ID_List id = str2list(type_name, depth);
  CHECK_OO(id)
  Type_Decl* td = new_type_decl(id, 0);
  Type_List tmp = NULL;
  if(!td) {
    free_id_list(id);
    return NULL;
  }
  while((type_name = get_type_name(s, i++))) {
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

ANN static Arg_List make_dll_arg_list(const Env env, DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  for(m_uint i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Array_Sub array_sub = NULL;
    Type_Decl* type_decl = NULL;
    Var_Decl var_decl    = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2;
    if(!(type_decl = str2decl(env, arg->type, &array_depth))) {
      if(arg_list)
        free_arg_list(arg_list);
      ERR_O(0, "\t...\tat argument '%i'", i + 1)
    }
    if((type_path2 = str2list(arg->name, &array_depth2)))
      free_id_list(type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(type_decl);
      if(arg_list)
        free_arg_list(arg_list);
      ERR_O(0, "array subscript specified incorrectly for built-in module")
    }
    array_sub = make_dll_arg_list_array(array_sub, &array_depth, array_depth2);
    var_decl = new_var_decl(insert_symbol(arg->name), array_sub, 0);
    arg_list = new_arg_list(type_decl, var_decl, arg_list);
  }
  return arg_list;
}

ANN static Func_Def make_dll_as_fun(const Env env, DL_Func * dl_fun, ae_flag flag) {
  Func_Def func_def = NULL;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  flag |= ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0)))
    ERR_O(0, "\t...\tduring @ function import '%s' (type).", dl_fun->name)
  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL);
    for(i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
    type_decl = add_type_decl_array(type_decl, array_sub);
  }
  name = dl_fun->name;
  arg_list = make_dll_arg_list(env, dl_fun);
  func_def = new_func_def(type_decl, insert_symbol(name), arg_list, NULL, flag);
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->addr;
  return func_def;
}

ANN static Func_Def import_fun(const Env env, DL_Func * mfun, const ae_flag flag) {
  CHECK_BO(name_valid(mfun->name));
  return make_dll_as_fun(env, mfun, flag);
}

ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag) {
  Func_Def def = import_fun(gwi->env, &gwi->func, flag);
  CHECK_OB(def)
  if(gwi->templater.n) {
    def = new_func_def(NULL, NULL, NULL, NULL, 0);
    const ID_List list = templater_def(&gwi->templater);
    def->tmpl = new_tmpl_list(list, -1);
    SET_FLAG(def, template);
  }
  if(gwi->env->class_def && GET_FLAG(gwi->env->class_def, template)) {
    Section* section = new_section_func_def(def);
    Class_Body body = new_class_body(section, NULL);
    gwi_body(gwi, body);
    return 1;
  }
  if(traverse_func_def(gwi->env, def) < 0) {
    free_func_def(def);
    return -1;
  }
  return 1;
}

static Type get_type(const Env env, const m_str str) {
  m_uint depth = 0;
  const ID_List list = (str && str != (m_str)OP_ANY_TYPE) ? str2list(str, &depth) : NULL;
  const Type  t = (str == (m_str) OP_ANY_TYPE) ? OP_ANY_TYPE : list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(list);
  return t ? (depth ? array_type(t, depth) : t) : NULL;
}

ANN2(1,2) static int import_op(const Env env, const DL_Oper* op,
    const f_instr f) {
  const Type lhs = op->lhs ? get_type(env, op->lhs) : NULL;
  const Type rhs = op->rhs ? get_type(env, op->rhs) : NULL;
  const Type ret = get_type(env, op->ret);
  const struct Op_Import opi = { op->op, lhs, rhs, ret,
    op->ck, op->em, (uintptr_t)f };
  return env_add_op(env, &opi);
}


ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const restrict m_str l,
    const restrict m_str r, const restrict m_str t) {
  gwi->oper.ret = t;
  gwi->oper.rhs = r;
  gwi->oper.lhs = l;
  return 1;
}

ANN m_int gwi_oper_add(const Gwi gwi, Type (*ck)(Env, void*)) {
  gwi->oper.ck = ck;
  return 1;
}

ANN m_int gwi_oper_emi(const Gwi gwi, m_bool (*em)(Emitter, void*)) {
  gwi->oper.em = em;
  return 1;
}

ANN m_int gwi_oper_end(const Gwi gwi, const Operator op, const f_instr f) {
  gwi->oper.op = op;
  const m_bool ret = import_op(gwi->env, &gwi->oper, f);
  gwi->oper.ck = NULL;
  gwi->oper.em = NULL;
  return ret;
}

ANN m_int gwi_fptr_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  dl_func_init(&gwi->func, type, name, 0);
  return 1;
}

ANN static Stmt import_fptr(const Env env, DL_Func* dl_fun, ae_flag flag) {
  m_uint array_depth;
  ID_List type_path;
  Type_Decl* type_decl = NULL;
  const Arg_List args = make_dll_arg_list(env, dl_fun);
  flag |= ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0)))
    ERR_O(0, "\t...\tduring fptr import '%s' (type).",
          dl_fun->name)
  return new_stmt_fptr(insert_symbol(dl_fun->name), type_decl, args, flag);
}

ANN m_int gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  const Stmt stmt = import_fptr(gwi->env, &gwi->func, flag);

  CHECK_BB(traverse_stmt_fptr(gwi->env, &stmt->d.stmt_fptr))
  if(gwi->env->class_def)
    SET_FLAG(stmt->d.stmt_fptr.func->def, builtin);
  else
    SET_FLAG(stmt->d.stmt_fptr.func, builtin);
  ADD_REF(stmt->d.stmt_fptr.type);
  free_stmt(stmt);
  return 1;
}

ANN static Exp make_exp(const m_str type, const m_str name) {
  Type_Decl *type_decl;
  ID_List id_list;
  m_uint array_depth;
  Array_Sub array = NULL;
  CHECK_OO((id_list = str2list(type, &array_depth)))
  if(array_depth) {
    array = new_array_sub(NULL);
    array->depth = array_depth;
  }
  type_decl = new_type_decl(id_list, 0);
  const Var_Decl var_decl = new_var_decl(insert_symbol(name), array, 0);
  const Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL);
  return new_exp_decl(type_decl, var_decl_list);
}

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str name) {
  if(name)
    gwi->union_data.xid = insert_symbol(name);
  return 1;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  const Exp exp = make_exp(type, name);
  CHECK_OB(exp);
  const Type t = type_decl_resolve(gwi->env, exp->d.exp_decl.td);
  if(!t)
    ERR_B(0, "type '%s' unknown in union declaration.", type)
  if(isa(t, t_object) > 0)
    SET_FLAG(exp->d.exp_decl.td, ref);
  gwi->union_data.list = new_decl_list(exp, gwi->union_data.list);
  return 1;
}

ANN m_int gwi_union_end(const Gwi gwi, const ae_flag flag) {
  const Stmt stmt = new_stmt_union(gwi->union_data.list, 0);
  stmt->d.stmt_union.flag = flag;
  CHECK_BB(traverse_stmt_union(gwi->env, &stmt->d.stmt_union))
  emit_union_offset(stmt->d.stmt_union.l, stmt->d.stmt_union.o);
  if(GET_FLAG((&stmt->d.stmt_union), member))
    gwi->env->class_def->nspc->offset =
      stmt->d.stmt_union.o + stmt->d.stmt_union.s;
  free_stmt(stmt);
  gwi->union_data.list = NULL;
  gwi->union_data.xid  = NULL;
  return 1;
}

ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  gwi->enum_data.t = type;
  vector_init(&gwi->enum_data.addr);
  return 1;
}

ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  const ID_List list = new_id_list(insert_symbol(name), 0);
  DL_Enum* d = &gwi->enum_data;
  ALLOC_PTR(addr, m_int, i);
  vector_add(&gwi->enum_data.addr, (vtype)addr);
  if(!d->base)
    d->base = list;
  else
    d->curr->next = list;
  d->curr = list;
  return list ? 1 : -1;
}

ANN static void import_enum_end(DL_Enum* d, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&d->addr, i);
    SET_FLAG(value, builtin);
    value->d.ptr = (m_uint*)(addr ? addr : i);
  }
  d->t = NULL;
  d->base = NULL;
  vector_release(&d->addr);
}

ANN m_int gwi_enum_end(const Gwi gwi) {
  DL_Enum* d = &gwi->enum_data;
  const Stmt stmt = new_stmt_enum(d->base, d->t ? insert_symbol(d->t) : NULL);
  if(traverse_stmt_enum(gwi->env, &stmt->d.stmt_enum) < 0) {
    free_id_list(d->base);
    return -1;
  }
  import_enum_end(d, &stmt->d.stmt_enum.values);
  free_stmt(stmt);
  return 1;
}

m_int gwi_add_value(Gwi gwi, const m_str name, Type type, const m_bool is_const, void* value) {
  env_add_value(gwi->env, name, type, is_const, value);
  return 1;
}

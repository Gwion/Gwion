#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "import.h"
#include "importer.h"
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

m_int importer_tmpl_end(Importer importer) {
  importer->templater.n = 0;
  importer->templater.list = NULL;
  return 1;
}

m_int importer_tmpl_ini(Importer importer, m_uint n, const m_str* list) {
  importer->templater.n = n;
  importer->templater.list = (m_str*)list;
  return 1;
}

static void dl_func_init(DL_Func* a, const m_str t, const m_str n, m_uint addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

m_int importer_func_ini(Importer importer, const m_str t, const m_str n, m_uint addr) {
  dl_func_init(&importer->func, t, n, addr);
  return 1;
}

static void dl_func_func_arg(DL_Func* a, const m_str t, const m_str n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

m_int importer_func_arg(Importer importer, const m_str t, const m_str n) {
  if(importer->func.narg == DLARG_MAX - 1)
    CHECK_BB(err_msg(UTIL_, 0,
          "too many arguments for function '%s'.", importer->func.name))
  dl_func_func_arg(&importer->func, t, n);
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

static m_bool mk_xtor(Type type, m_uint d, e_native_func e) {
  VM_Code* code = e == NATIVE_CTOR ? &type->info->pre_ctor : &type->info->dtor;
  m_str name = type->name;
  m_str filename = e == NATIVE_CTOR ? "[ctor]" : "[dtor]";

  SET_FLAG(type, e == NATIVE_CTOR ? ae_flag_ctor : ae_flag_dtor);
  *code = new_vm_code(NULL, SZ_INT, 1, name, filename);
  (*code)->native_func = (m_uint)d;
  (*code)->native_func_type = e;
  return 1;
}

m_int importer_add_type(Importer importer, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  CHECK_BB(env_add_type(importer->env, type))
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

m_int importer_class_ini(Importer importer, Type type, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info)
    CHECK_BB(err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name))
  if(importer->templater.n) {
    type->def = calloc(1, sizeof(struct Class_Def_));
    type->def->types = templater_def(&importer->templater);
    type->def->type = type;
    SET_FLAG(type, ae_flag_template);
  } else
    SET_FLAG(type, ae_flag_scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  CHECK_BB(importer_add_type(importer, type))
  CHECK_BB(import_class_ini(importer->env, type, pre_ctor, dtor))
  return type->xid;
}

m_int importer_class_ext(Importer importer, Type_Decl* td) {
  if(!td->types)
    CHECK_BB(err_msg(TYPE_, 0, "importer_class_ext invoked before "
          "importer_class_ini"))
  VM_Code ctor = importer->env->class_def->info->pre_ctor;
  if(importer->env->class_def->parent ||
      (importer->env->class_def->def && importer->env->class_def->def->ext))
    CHECK_BB(err_msg(TYPE_, 0, "class extend already set"))
  if(td->array && !td->array->exp_list)
    CHECK_BB(err_msg(TYPE_, 0, "class extend array can't be empty"))
  if(!importer->env->class_def->def) {
    Type t = find_type(importer->env, td->xid);
    if(!t)
      CHECK_BB(type_unknown(td->xid, "builtin class extend"))
    CHECK_OB((t = scan_type(importer->env, t, td)))
    if(td->array) {
      CHECK_OB((t = array_type(t, td->array->depth)))
      SET_FLAG(importer->env->class_def, ae_flag_typedef);
    }
    importer->env->class_def->parent = t;
    importer->env->class_def->info->offset = t->info->offset;
    if(t->info->vtable.ptr)
      vector_copy2(&t->info->vtable, &importer->env->class_def->info->vtable);
      importer->env->class_def->info->pre_ctor = new_vm_code(NULL,
          SZ_INT, 1, importer->env->class_def->name, "ext ctor");
    CHECK_OB((importer->emit->code = emit_class_code(importer->emit,
          importer->env->class_def->name)))
    if(td->array)
      CHECK_BB(emit_array_extend(importer->emit, t, td->array->exp_list))
    if(ctor)
      CHECK_BB(emit_ext_ctor(importer->emit, ctor))
    CHECK_BB(emit_class_finish(importer->emit, importer->env->class_def->info))
    free_type_decl(td);
  } else {
// use ctor here too?
    SET_FLAG(td, ae_flag_typedef);
    importer->env->class_def->def->ext = td;
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

m_int importer_class_end(Importer importer) {
  return import_class_end(importer->env);
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
  free(v->t.xid);
}

m_int importer_item_ini(Importer importer, const m_str type, const m_str name) {
  DL_Var* v = &importer->var;
  memset(v, 0, sizeof(DL_Var));
  if(!(v->t.xid = str2list(type, &v->array_depth)))
    CHECK_BB(err_msg(TYPE_, 0, "... during var import '%s.%s'...",
          importer->env->class_def->name, name))
  v->var.xid = insert_symbol(name);
  return 1;
}

m_int importer_item_end(Importer importer, const ae_flag flag, const m_uint* addr) {
  DL_Var* v = &importer->var;
  dl_var_set(v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(importer->env->class_def && GET_FLAG(importer->env->class_def, ae_flag_template)) {
    Type_Decl *type_decl = new_type_decl(v->t.xid, flag, 0);
    Var_Decl var_decl = new_var_decl(v->var.xid, v->var.array, 0);
    Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL, 0);
    Exp exp = new_exp_decl(type_decl, var_decl_list, 0);
    Stmt stmt = new_stmt_exp(exp, 0);
    Stmt_List list = new_stmt_list(stmt, NULL, 0);
    Section* section = new_section_stmt_list(list, 0);
    Class_Body body = new_class_body(section, NULL, 0);
    type_decl->array = v->t.array;
    if(!importer->env->class_def->def->body)
      importer->env->class_def->def->body = importer->body = body;
    else {
      importer->body->next = body;
      importer->body = body;
    }
    return 1;
  }
  CHECK_BB(traverse_decl(importer->env, &v->exp.d.exp_decl))
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
  if(td->types) {
    Type t = find_type(env, td->xid);
    t = scan_type(env, t, td);
  }
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

m_int importer_func_end(Importer importer, ae_flag flag) {
  Func_Def def = import_fun(importer->env, &importer->func, flag);

  CHECK_OB(def)
  if(importer->templater.n) {
    def = calloc(1, sizeof(struct Class_Def_));
    ID_List list = templater_def(&importer->templater);
    def->tmpl = new_func_def_tmpl(list, 1);
    SET_FLAG(def, ae_flag_template);
  }
  if(importer->env->class_def && GET_FLAG(importer->env->class_def, ae_flag_template)) {
    Section* section = new_section_func_def(def, 0);
    Class_Body body = new_class_body(section, NULL, 0);
    if(!importer->env->class_def->def->body)
      importer->env->class_def->def->body = importer->body = body;
    else {
      importer->body->next = body;
      importer->body = body;
    }
    return 1;
  }
  if(traverse_func_def(importer->env, def) < 0) {
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

m_int importer_oper_ini(Importer importer, const m_str l, const m_str r, const m_str t) {
  importer->oper.ret = t;
  importer->oper.rhs = r;
  importer->oper.lhs = l;
  return 1;
}

m_int importer_oper_add(Importer importer, Type (*ck)(Env, void*)) {
  importer->oper.ck = ck;
  return 1;
}

m_int importer_oper_emi(Importer importer, m_bool (*em)(Emitter, void*)) {
  importer->oper.em = em;
  return 1;
}

m_int importer_oper_end(Importer importer, Operator op, const f_instr f) {
  m_bool ret;
  importer->oper.op = op;
  ret = import_op(importer->env, &importer->oper, f);
  importer->oper.ck = NULL;
  importer->oper.em = NULL;
  return ret;
}

m_int importer_fptr_ini(Importer importer, const m_str type, const m_str name) {
  dl_func_init(&importer->func, type, name, 0);
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
m_int importer_fptr_end(Importer importer, ae_flag flag) {
  Stmt stmt = import_fptr(importer->env, &importer->func, flag);
  CHECK_BB(traverse_stmt_fptr(importer->env, &stmt->d.stmt_ptr))
  if(importer->env->class_def)
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

m_int importer_union_ini(Importer importer, const m_str name) {
  if(name)
    importer->union_data.xid = insert_symbol(name);
  return 1;
}

m_int importer_union_add(Importer importer, const m_str type, const m_str name) {
  Exp exp = make_exp(type, name);
  importer->union_data.list = new_decl_list(exp, importer->union_data.list);
  return 1;
}

m_int importer_union_end(Importer importer, ae_flag flag) {
  Stmt stmt = new_stmt_union(importer->union_data.list, 0);
  CHECK_BB(traverse_stmt_union(importer->env, &stmt->d.stmt_union))
  // this is from emit.c. TODO: puts this in a func
  Decl_List l = stmt->d.stmt_union.l;
  while(l) {
    Var_Decl_List var_list = l->self->d.exp_decl.list;
    while(var_list) {
      var_list->self->value->offset = stmt->d.stmt_union.o;
      var_list = var_list->next;
    }
    l = l->next;
  }
  if(GET_FLAG((&stmt->d.stmt_union), ae_flag_member))
    importer->env->class_def->info->offset =
      stmt->d.stmt_union.o + stmt->d.stmt_union.s;
  free_stmt(stmt);
  importer->union_data.list = NULL;
  importer->union_data.xid  = NULL;
  return 1;
}

m_int importer_enum_ini(Importer importer, const m_str type) {
  importer->enum_data.t = type;
  return 1;
}

m_int importer_enum_add(Importer importer, const m_str name) {
  ID_List list = new_id_list(insert_symbol(name), 0);
  DL_Enum* d = &importer->enum_data;

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
    SET_FLAG(value, ae_flag_builtin);
    value->ptr = (m_uint*)i;
  }
  d->t = NULL;
  d->base = NULL;
}

m_int importer_enum_end(Importer importer) {
  DL_Enum* d = &importer->enum_data;
  Stmt stmt = new_stmt_enum(d->base, d->t ? insert_symbol(d->t) : NULL, 0);

  CHECK_OB(stmt)
  if(traverse_stmt_enum(importer->env, &stmt->d.stmt_enum) < 0) {
    free_id_list(d->base);
    return -1;
  }
  import_enum_end(d, &stmt->d.stmt_enum.values);
  free_stmt(stmt);
  return 1;
}

m_int importer_add_value(Importer importer, const m_str name, Type type, const m_bool is_const, void* value) {
  return env_add_value(importer->env, name, type, is_const, value);
}

OP_CHECK(opck_const_lhs) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->lhs->meta != ae_meta_var) {
    if(err_msg(TYPE_, bin->pos, "cannot assign '%s' on types '%s' and'%s'...",
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
          "cannot assign '%s' on types '%s' and'%s'...\n",
          "\t...(reason: --- right-side operand is not mutable)",
          op2str(bin->op), bin->lhs->type->name, bin->rhs->type->name) < 0)
      return &t_null;
  }
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
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

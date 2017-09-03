#include <math.h>
#include <string.h>
#include "err_msg.h"
#include "env.h"
#include "func.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "traverse.h"

#define CHECK_EB(a) if(!env->class_def) { CHECK_BB(err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end")) }
#define CHECK_EO(a) if(!env->class_def) { CHECK_BO(err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end")) }

void free_expression(Exp exp);

void dl_return_push(const char* retval, VM_Shred shred, m_uint size) {
  memcpy(REG(0), retval, size);
  PUSH_REG(shred, size);
}

void dl_func_init(DL_Func* a, const m_str t, const m_str n, m_uint addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

void dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

static m_bool check_illegal(char* curr, char c, m_uint i) {
  if(c != '.') {
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9') || (i == 1 && c == '@'))
      curr[i - 1] = c;
    else
      return 0;
  }
  return 1;
}

static void path_valid_inner(m_str curr) {
  m_int j, size = strlen(curr);
  for(j = (size / 2) + 1; --j;) {
    char s = curr[j];
    curr[j] = curr[size - j - 1];
    curr[size - j - 1] = s;
  }
}

static m_bool path_valid(ID_List* list, char* path, char* curr, m_uint len) {
  char last = '\0';
  m_uint i;
  for(i = len; --i;) {
    char c = path[i - 1];
    if(c != '.' && check_illegal(curr, c, i) < 0)
      CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path))
    if(c == '.' || i == 1) {
      if((i != 1 && last != '.' && last != '\0') ||
          (i ==  1 && c != '.')) {
        path_valid_inner(curr);
        *list = prepend_id_list(curr, *list, 0);
        memset(curr, 0, len + 1);
      } else
        CHECK_BB(err_msg(UTIL_,  0, "path '%s' must not begin or end with '.'", path))
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
  memset(curr, 0, len + 1);

  while(len > 2 && path[len - 1] == ']' && path[len - 2] == '[') {
    depth++;
    len -= 2;
  }
  if(path_valid(&list, path, curr, len) < 0) {
    if(list)
      free_id_list(list);
    return NULL;
  }
  CHECK_OO(list)
  strncpy(curr, path, len);
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

m_int import_class_begin(Env env, Type type, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info)
    CHECK_BB(err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name))
  CHECK_BB(env_add_type(env, type))
  type->info = new_nspc(type->name, "global_nspc");
  type->info->parent = env->curr;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
    mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->obj_size;
    vector_copy2(&type->info->obj_v_table, &type->parent->info->obj_v_table);
  }
  type->owner = env->curr;

  SET_FLAG(type, ae_flag_checked);
  CHECK_BB(env_push_class(env, type))
  return type->xid;
}

m_int import_class_end(Env env) {
  if(!env->class_def)
    CHECK_BB(err_msg(TYPE_, 0, "import: too many class_end called..."))
  env->class_def->obj_size = env->class_def->info->offset;
  CHECK_BB(env_pop_class(env))
  return 1;
}

m_int import_var(Env env, const m_str type, const m_str name, ae_flag flag, m_uint* addr) {
  m_uint array_depth = 0;
  ID_List path;

  CHECK_EB(env->class_def)
  if(!(path = str2list(type, &array_depth)))
    CHECK_BB(err_msg(TYPE_, 0, "... during var import '%s.%s'...", env->class_def->name, name))

  Type_Decl t;
  memset(&t, 0, sizeof(Type_Decl));
  t.xid = path;
  t.flag = flag;
  if(array_depth) {
    t.array = new_array_sub(NULL, 0);
    t.array->depth = array_depth;
  }
  struct Var_Decl_ var;
  memset(&var, 0, sizeof(struct Var_Decl_));
  var.xid = insert_symbol(name);
  if(array_depth) {
    var.array = new_array_sub(NULL, 0);
    var.array->depth = array_depth;
  }
  struct Var_Decl_List_ list;
  memset(&list, 0, sizeof(struct Var_Decl_List_));
  list.self = &var;
  struct Exp_ exp;
  memset(&exp, 0, sizeof(struct Exp_));
  exp.exp_type = ae_exp_decl;
  exp.d.exp_decl.type = &t;
  exp.d.exp_decl.list = &list;
  exp.d.exp_decl.is_static = ((flag & ae_flag_static) == ae_flag_static);
  exp.d.exp_decl.self = &exp;
  var.addr = (void *)addr;
  if(traverse_decl(env, &exp.d.exp_decl) < 0)
    var.value->offset = -1;;
  free(path);
  var.value->flag = flag | ae_flag_builtin;
  return var.value->offset;
}

static Array_Sub make_dll_arg_list_array(Array_Sub array_sub,
  m_uint* array_depth, m_uint array_depth2) {
  m_uint i;
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) {
    array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL, 0);
  }
  return array_sub;
}

static Arg_List make_dll_arg_list(DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  m_int i = 0;

  for(i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Array_Sub array_sub = NULL;
    Type_Decl* type_decl = NULL;
    Var_Decl var_decl    = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2, type_path = str2list(arg->type, &array_depth);
    if(!type_path) {
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_,  0, "...at argument '%i'...", i + 1))
    }
    type_decl = new_type_decl(type_path, 0, 0);
    if((type_path2 = str2list(arg->name, &array_depth2)))
      free_id_list(type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(type_decl);
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_, 0, "array subscript specified incorrectly for built-in module"))
    }
    array_sub = make_dll_arg_list_array(array_sub, &array_depth, array_depth2);
    var_decl = new_var_decl(arg->name, array_sub, 0);
    arg_list = new_arg_list(type_decl, var_decl, arg_list, 0);
  }
  return arg_list;
}

static Func_Def make_dll_as_fun(DL_Func * dl_fun, ae_flag flag) {
  Func_Def func_def = NULL;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  flag |= ae_flag_func | ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0, 0)))
    CHECK_BO(err_msg(TYPE_, 0, "...during @ function import '%s' (type)...", dl_fun->name))
  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL, 0);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }
  name = dl_fun->name;
  arg_list = make_dll_arg_list(dl_fun);
  func_def = new_func_def(flag, type_decl, name, arg_list, NULL, 0);
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->addr;
  return func_def;
}

m_int import_fun(Env env, DL_Func * mfun, ae_flag flag) {
  Func_Def func_def;
  CHECK_OB(mfun) // probably deserve an err msg
  CHECK_BB(name_valid(mfun->name));
  CHECK_EB(env->class_def)
  if(mfun->narg >= DLARG_MAX)
    return -1;
  CHECK_OB((func_def = make_dll_as_fun(mfun, flag)))
  if(traverse_func_def(env, func_def) < 0) {
    free_func_def(func_def);
    return -1;
  }
  return 1;
}

static Type get_type(Env env, const m_str str) {
  m_uint  depth = 0;
  ID_List list = str2list(str, &depth);
  Type    t = list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(list);
  return t ? (depth ? new_array_type(env, depth, t, env->curr) : t) : NULL;
}

m_int import_op(Env env, Operator op, const m_str l, const m_str r, const m_str t,
                const f_instr f, const m_bool global) {
  Type lhs = l ? get_type(env, l) : NULL;
  Type rhs = r ? get_type(env, r) : NULL;
  Type ret = get_type(env, t);
  return env_add_op(env, op, lhs, rhs, ret, f, global);
}

m_bool import_libs(Env env) {
  CHECK_BB(env_add_type(env, &t_void))
  CHECK_BB(env_add_type(env, &t_null))
  CHECK_BB(env_add_type(env, &t_now))
  CHECK_BB(import_int(env))
  CHECK_BB(import_float(env))
  CHECK_BB(import_complex(env))
  CHECK_BB(import_vec3(env))
  CHECK_BB(import_vec4(env))
  CHECK_BB(import_object(env))
  CHECK_BB(import_vararg(env))
  CHECK_BB(import_string(env))
  CHECK_BB(import_shred(env))
  CHECK_BB(import_event(env))
  CHECK_BB(import_ugen(env))
  CHECK_BB(import_array(env))
  env->type_xid = te_last;
  CHECK_BB(import_fileio(env))
  CHECK_BB(import_std(env))
  CHECK_BB(import_math(env))
  CHECK_BB(import_machine(env))
  CHECK_BB(import_soundpipe(env))
  CHECK_BB(import_modules(env))
  return 1;
}

m_bool import_values(Env env) {
  ALLOC_PTR(d_zero, m_float, 0.0);
  ALLOC_PTR(sr,     m_float, (m_float)vm->sp->sr);
  ALLOC_PTR(samp,   m_float, 1.0);
  ALLOC_PTR(ms,     m_float, (m_float)*sr     / 1000.);
  ALLOC_PTR(second, m_float, (m_float)*sr);
  ALLOC_PTR(minute, m_float, (m_float)*sr     * 60.0);
  ALLOC_PTR(hour,   m_float, (m_float)*minute * 60.0);
  ALLOC_PTR(day,    m_float, (m_float)*hour   * 24.0);
  ALLOC_PTR(t_zero, m_float, 0.0);
  ALLOC_PTR(pi, m_float, M_PI);

  env_add_value(env, "d_zero",     &t_dur,   1, d_zero);
  env_add_value(env, "samplerate", &t_dur,   1, sr);
  env_add_value(env, "samp",       &t_dur,   1, samp);
  env_add_value(env, "ms",         &t_dur,   1, ms);
  env_add_value(env, "second",     &t_dur,   1, second);
  env_add_value(env, "minute",     &t_dur,   1, minute);
  env_add_value(env, "day",        &t_dur,   1, hour);
  env_add_value(env, "hour",       &t_dur,   1, day);
  env_add_value(env, "t_zero",     &t_time,  1, t_zero);
  env_add_value(env, "pi",         &t_float, 1, pi);

  return 1;
}

m_bool import_global_ugens(VM* vm, Env env) {
  vm->dac       = new_M_UGen();
  vm->adc       = new_M_UGen();
  vm->blackhole = new_M_UGen();

  assign_ugen(UGEN(vm->dac), 2, 2, 0, vm);
  assign_ugen(UGEN(vm->adc), 2, 2, 0, vm);
  assign_ugen(UGEN(vm->blackhole), 1, 1, 0, vm);
  UGEN(vm->dac)->tick = dac_tick;
  UGEN(vm->adc)->tick = adc_tick;
  vector_add(&vm->ugen, (vtype)UGEN(vm->blackhole));
  vector_add(&vm->ugen, (vtype)UGEN(vm->dac));
  vector_add(&vm->ugen, (vtype)UGEN(vm->adc));

  env_add_value(env, "adc",        &t_ugen, 1, vm->adc);
  env_add_value(env, "dac",        &t_ugen, 1, vm->dac);
  env_add_value(env, "blackhole",  &t_ugen, 1, vm->blackhole);
  return 1;
}

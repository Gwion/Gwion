#include <string.h>
#include "err_msg.h"
#include "env.h"
#include "func.h"
#include "import.h"

#define CHECK_EB(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return -1; }
#define CHECK_EO(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return NULL; }

void free_Expression(Expression exp);

m_bool scan1_Decl_Expression(Env env, Decl_Expression* decl);
m_bool scan2_Decl_Expression(Env env, Decl_Expression* decl);
Type   check_Decl_Expression(Env env, Decl_Expression* decl);

m_bool scan1_Func_Def(Env env, Func_Def f);
m_bool scan2_Func_Def(Env env, Func_Def f);
m_bool check_Func_Def( Env env, Func_Def f );


// should be in type_utils
ID_List str2list(m_str path, m_uint* array_depth )
{
  m_uint len = 0;
  m_int  i, j;
  char curr[256];
  ID_List list = NULL;
  char last = '\0';
  m_uint depth = 0;
  memset(curr, 0, 256);
  while(path[len] != '\0')
    len++;

  while(len > 2 && path[len - 1] == ']' && path[len - 2] == '[') {
    depth++;
    len -= 2;
  }

  for(i = len - 1; i >= 0; i--) {
    char c = path[i];
    if(c != '.') {
      if( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
          || (c == '_') || (c >= '0' && c <= '9') || (i == 0 && c == '@'))
        curr[i] = c;
      else {
        err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path);
        free_ID_List(list);
        return NULL;
      }
    }
    if(c == '.' || i == 0) {
      if((i != 0 && last != '.' && last != '\0') ||
          (i == 0 && c != '.')) {
        m_int size = strlen(curr);
        for(j = 0; j < size / 2; j++) {
          char s = curr[j];
          curr[j] = curr[size - j - 1];
          curr[size - j - 1] = s;
        }
        list = prepend_id_list(curr, list, 0);
        memset(curr, 0, sizeof(curr));
      } else {
        err_msg(UTIL_,  0, "path '%s' must not begin or end with '.'", path);
        free_ID_List(list);
        return NULL;
      }
    }
    last = c;
  }
  path = strndup(path, len);
  list->xid = insert_symbol(path);
  if(array_depth)
    *array_depth = depth;
  free(path);
  return list;
}

static m_bool mk_xtor(Type type, m_uint d, e_native_func e) {
	VM_Code* code;
	m_str name, filename;
	if(e == NATIVE_CTOR) {
      type->has_constructor = 1;
	  name = "ctor";
	  filename = "[internal ctor definition]";
	  code = &type->info->pre_ctor;
	} else {
      type->has_destructor = 1;
	  name = type->name;
	  filename = "[internal dtor definition]";
	  code = &type->info->dtor;
    }
    *code = new_VM_Code(NULL, SZ_INT, 1, name, filename);
    (*code)->native_func = (m_uint)d;
    (*code)->native_func_type = e;
	return 1;
}

Type import_class_begin(Env env, Type type, NameSpace where, f_ctor pre_ctor, f_dtor dtor)
{
  if(type->info) {
    err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name);
    return NULL;
  }
  type->info = new_NameSpace();
  type->info->filename = "global_nspc";
  // add reference
  /*    SAFE_ADD_REF(type->info);*/
  add_ref(type->info->obj);
  type->info->name = type->name;
  type->info->parent = where;
  add_ref(type->info->parent->obj);
  if(pre_ctor)
	type->has_constructor = mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
	type->has_destructor  = mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->obj_size;
    free_Vector(type->info->obj_v_table);
    type->info->obj_v_table = vector_copy(type->parent->info->obj_v_table);
  }

  type->owner = where;
  /*    SAFE_ADD_REF(type->owner);*/
  add_ref(type->owner->obj);
//  type->size = SZ_INT;
//    type->size = 1;
  // set the object size
  type->obj_size = 0; // TODO

  type->is_complete = 1;
//    type_type = type_copy(env, &t_class);
//    type_type->actual_type = type;
  // SAFE_REF_ASSIGN( type_type->actual_type, type );
  vector_append(env->nspc_stack, (vtype)env->curr);
  env->curr = type->info;
  vector_append(env->class_stack, (vtype)env->class_def);
  env->class_def = type;
//    type->obj = new_VM_Object(e_type_obj);
  // ref count
  add_ref(type->obj);
  return type;
}

m_bool import_class_end(Env env)
{
  if(!env->class_def) {
    err_msg(TYPE_, 0, "import: too many class_end called..." );
    return -1;
  }
  env->class_def->obj_size = env->class_def->info->offset;
  env->class_def = (Type)vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = (NameSpace)vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);
  return 1;
}

static m_int import_var(Env env, const m_str type, const m_str name,
   const m_bool is_static, const m_bool is_const, const m_bool is_ref, m_uint* addr, const m_str doc) {
  m_int ret = -1;
  m_uint array_depth;
  ID_List path;
  Type_Decl* type_decl;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Expression exp_decl;
  CHECK_EB(env->class_def)
  if(!(path = str2list(type, &array_depth))) {
    err_msg(TYPE_, 0, "... during mvar import '%s.%s'...", env->class_def->name, name );
    return -1;
  }

  type_decl = new_Type_Decl(path, is_ref, 0);
  if(array_depth) {
    type_decl->array = new_array_sub(NULL, 0);
    type_decl->array->depth = array_depth;
  }
  var_decl = new_Var_Decl(name, NULL, 0);
  if(array_depth) {
    var_decl->array = new_array_sub(NULL, 0);
    var_decl->array->depth = array_depth;
  }
  var_decl_list = new_Var_Decl_List(var_decl, NULL, 0);
  exp_decl = new_Decl_Expression(type_decl, var_decl_list, 0, 0);
  var_decl->addr = (void *)addr;
  if(scan1_Decl_Expression(env, exp_decl->d.exp_decl) < 0)
    goto error;
  if(scan2_Decl_Expression(env, exp_decl->d.exp_decl) < 0)
    goto error;
  var_decl->value->is_const = is_const;
  if(!check_Decl_Expression(env, exp_decl->d.exp_decl))
    goto error;

  if(doc)
    var_decl->value->doc = doc;
  var_decl->value->is_import = 1;
  ret = var_decl->value->offset;
error:
  free_Expression(exp_decl);
  return ret;
}
m_int import_mvar(Env env, const m_str type,
                  const m_str name, const m_bool is_const, const m_bool is_ref, const m_str doc)
{ return import_var(env, type, name, 0, is_const, is_ref, NULL, doc); }
m_int import_svar(Env env, const m_str type,
                  const m_str name, m_bool is_const, m_bool is_ref, m_uint* addr, const m_str doc )
{ return import_var(env, type, name, 1, is_const, is_ref, addr, doc); }

static Arg_List make_dll_arg_list(DL_Func * dl_fun)
{
  Arg_List arg_list    = NULL;
  Type_Decl* type_decl = NULL;
  Var_Decl var_decl    = NULL;
  ID_List type_path    = NULL;
  ID_List type_path2    = NULL;
//  ID_List name_path    = NULL;
  Array_Sub array_sub  = NULL;
  DL_Value* arg        = NULL;
  m_uint array_depth = 0;
  m_uint array_depth2 = 0;
  m_int i = 0, j;

  for(i = vector_size(dl_fun->args) - 1; i >= 0; i--) {
    array_depth = array_depth2 = 0;
    array_sub = NULL;
    arg = (DL_Value*)vector_at(dl_fun->args, i);
    type_path = str2list(arg->type, &array_depth);
    if(!type_path) {
      err_msg(TYPE_,  0, "...at argument '%i'...", i + 1 );
      free_Vector(dl_fun->args);
      if(arg_list)
        free_Arg_List(arg_list);
      return NULL;
    }
    type_decl = new_Type_Decl( type_path, 0, 0 );
    type_path2 = str2list(arg->name, &array_depth2 );
    free_ID_List(type_path2);
    if(array_depth && array_depth2) {
      err_msg(TYPE_,  0, "array subscript specified incorrectly for built-in module" );
      // TODO: cleanup
      free_Type_Decl(type_decl);
      if(arg_list)
        free_Arg_List(arg_list);
      return NULL;
    }
    // TODO: arrays?
    if(array_depth2)
      array_depth = array_depth2;
    if(array_depth) {
      array_sub = new_array_sub( NULL, 0 );
      for(j = 1; j < array_depth; j++ )
        array_sub = prepend_array_sub( array_sub, NULL, 0 );
    }
    var_decl = new_Var_Decl(arg->name, array_sub, 0 );
    arg_list = new_Arg_List( type_decl, var_decl, arg_list, 0 );
    arg_list->doc = arg->doc;
    free_DL_Value(arg); /* could be free(arg) */
  }
  return arg_list;
}

Func_Def make_dll_as_fun(DL_Func * dl_fun, m_bool is_static)
{
  Func_Def func_def = NULL;
  ae_Keyword func_decl = ae_key_func;
  ae_Keyword static_decl = is_static ? ae_key_static : ae_key_instance;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  type_path = str2list(dl_fun->type, &array_depth);
  if(!type_path) {
    // error
    err_msg(TYPE_, 0, "...during function import '%s' (type)...", dl_fun->name);
    goto error;
  }

  type_decl = new_Type_Decl(type_path, 0, 0);
  if(!type_decl) {
    // error
    err_msg(TYPE_, 0, "...during function import '%s' (type)...", dl_fun->name);
    goto error;
  }

  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++ )
      array_sub = prepend_array_sub(array_sub, NULL, 0);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }

  name = dl_fun->name;
  arg_list = make_dll_arg_list(dl_fun);

  func_def = new_Func_Def(func_decl, static_decl, type_decl, name, arg_list, NULL, 0);
  func_def->s_type = ae_func_builtin;
  func_def->dl_func_ptr = (void*)(m_uint)dl_fun->d.mfun;
  free_DL_Func(dl_fun);
  return func_def;
error:
  // clean up
  // if( !func_def ) delete_type_decl( type_decl );
  // else delete_func_def( func_def );
  return NULL;
}

#define CHECK_FN(a) if(a < 0) { if(func_def->func) rem_ref(func_def->func->obj, func_def->func); free_Func_Def(func_def); return NULL;}
static Func import_fun(Env env, DL_Func * mfun, m_bool is_static) {
  Func_Def func_def;
  CHECK_OO(mfun) // probably deserve an err msg
  CHECK_EO(env->class_def)
  CHECK_OO((func_def = make_dll_as_fun(mfun, is_static)))
  CHECK_FN(scan1_Func_Def(env, func_def))
  CHECK_FN(scan2_Func_Def(env, func_def))
  CHECK_FN(check_Func_Def(env, func_def))
  return func_def->func;
}

// those should be defines
Func import_mfun(Env env, DL_Func * fun) { return import_fun(env, fun, 0); }
Func import_sfun(Env env, DL_Func * fun) { return import_fun(env, fun, 1); }

#include <string.h>
#include "err_msg.h"
#include "env.h"
#include "func.h"
#include "import.h"

m_bool scan1_Decl_Expression(Env env, Decl_Expression* decl);
m_bool scan1_Func_Def(Env env, Func_Def f);
m_bool scan2_Func_Def(Env env, Func_Def f);
m_bool check_Func_Def( Env env, Func_Def f );

m_bool scan2_Decl_Expression(Env env, Decl_Expression* decl);
Type check_Decl_Expression(Env env, Decl_Expression* decl);
/*m_bool compile(VM* vm, Emitter emit, Env env, const m_str filename);*/
static ID_List str2list(m_str path, m_uint* array_depth )
{
  m_uint len = 0;
  m_int  i, j;
  char curr[256];
  ID_List list = NULL;
  char last = '\0';
  m_uint depth = 0;
  while(path[len] != '\0')
    len++;

  while( len > 2 && path[len-1] == ']' && path[len-2] == '[' )
  {
    depth++;
    len -= 2;
  }
  
  for( i = len - 1; i >= 0; i-- )
  {
    char c = path[i];
    if( c != '.' )
    {
      if( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' )
        || ( c == '_' ) || ( c >= '0' && c <= '9' ) )
          curr[i] = c;
      else
      {
        err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path);
        free_ID_List( list );
        return NULL;
      }
    }
    if( c == '.' || i == 0 )
    {
      if( (i != 0 && last != '.' && last != '\0') ||
          (i == 0 && c != '.') )
      {
        char s;
        m_int size = strlen(curr);
        for( j = 0; j < size/2; j++ )
        {
          s = curr[j];
          curr[j] = curr[size-j-1];
          curr[size-j-1] = s;
        }
        list = prepend_id_list( curr, list, 0 );
        memset(curr, 0, sizeof(curr));
      }
      else
      {
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
  return list;
}

Type import_class_begin(Env env, Type type, NameSpace where, f_ctor pre_ctor, f_dtor dtor)
{
    Type  type_type = NULL;

    if(type->info)
    {
      err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name);
      return NULL;
    }

    type->info = new_NameSpace(); // done filename
		type->info->filename = "global_nspc";
    // add reference
/*    SAFE_ADD_REF(type->info);*/
    add_ref(type->info->obj);
    type->info->name = type->name;
    type->info->parent = where;
    add_ref(type->info->parent->obj);
    if(pre_ctor)
    {
      type->has_constructor = 1;
      type->info->pre_ctor = new_VM_Code(new_Vector(), sizeof(m_uint), 1, type->name , "[internal ctor definition]");
      type->info->pre_ctor->native_func = (m_uint)pre_ctor;
      type->info->pre_ctor->native_func_type = NATIVE_CTOR;
      type->info->pre_ctor->need_this = 1;
      type->info->pre_ctor->stack_depth = sizeof(m_uint);
    }
    if(dtor)
    {
      type->has_destructor = 1;
      type->info->dtor = new_VM_Code(new_Vector(), sizeof(m_uint), 1, type->name , "[internal dtor definition]");
      type->info->dtor->native_func = (m_uint)dtor;
      type->info->dtor->native_func_type = NATIVE_DTOR;
      type->info->dtor->need_this = 1;
        type->info->dtor->stack_depth = sizeof(m_uint);
    }
    if(type->parent)
    {
      type->info->offset = type->parent->obj_size;
      type->info->obj_v_table = vector_copy(type->parent->info->obj_v_table);
    }

    type->owner = where;
/*    SAFE_ADD_REF(type->owner);*/
    add_ref(type->owner->obj);
    type->size = sizeof(m_uint);
//    type->size = 1;
    // set the object size
    type->obj_size = 0; // TODO

    type->is_complete = 1;
    type_type = type_copy(env, &t_class);
    type_type->actual_type = type;
    // SAFE_REF_ASSIGN( type_type->actual_type, type );
    vector_append(env->nspc_stack, env->curr);
    env->curr = type->info;
    vector_append(env->class_stack, env->class_def);
    env->class_def = type;
    type->obj = new_VM_Object(e_type_obj);
    // ref count
    add_ref(type->obj);
    return type;
}

m_bool import_class_end(Env env) {
  if(!env->class_def)
  {
    err_msg(TYPE_, 0, "import: too many class_end called..." );
    return -1;
  }
  env->class_def->obj_size = env->class_def->info->offset;
  env->class_def = vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);      
  return 1;
}

m_int import_mvar(Env env, const m_str type,
  const m_str name, m_bool is_const, m_bool is_ref, const m_str doc )
{
  m_uint array_depth;
  ID_List path;
  Type_Decl* type_decl;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Expression exp_decl;
  if(!env->class_def)
  {
    err_msg(TYPE_, 0, "import error: import_mvar '%s' invoked between begin/end", name );
    return -1;
  }
  if(!(path = str2list(type, &array_depth)))
  {
    err_msg(TYPE_, 0, "... during mvar import '%s.%s'...", env->class_def->name, name );
    return -1;
  }

  type_decl = new_Type_Decl( path, is_ref, 0 );
  if(array_depth)
  {
    type_decl->array = new_array_sub( NULL, 0 );
    type_decl->array->depth = array_depth;
  }
  var_decl = new_Var_Decl(name, NULL, 0);
  if(array_depth)
  {
    var_decl->array = new_array_sub( NULL, 0 );
    var_decl->array->depth = array_depth;
  }
  var_decl_list = new_Var_Decl_List( var_decl, NULL, 0 );
  exp_decl = new_Decl_Expression( type_decl, var_decl_list, 0, 0 );
  if(scan1_Decl_Expression(env, exp_decl->decl_exp) < 0)
    goto error;
  if(scan2_Decl_Expression(env, exp_decl->decl_exp) < 0)
    goto error;
	var_decl->value->is_const = is_const;
  if(!check_Decl_Expression(env, exp_decl->decl_exp))
    goto error;

  if(doc)
    var_decl->value->doc = doc;

  // cleanup
  free_ID_List(path);
  return var_decl->value->offset;
error:
  free_ID_List(path);
  return -1;
}

m_int import_svar(Env env, const m_str type,
  const m_str name, m_bool is_const, m_bool is_ref, m_uint* addr, const m_str doc )
{
  m_uint depth;
  if(!env->class_def)
  {
    err_msg(TYPE_, 0, "import error: import_svar '%s' invoked between begin/end", name);
    return -1;
  }
  ID_List path = str2list(type, &depth);
  if(!path)
  {
    err_msg(TYPE_,  0, "... during svar import '%s.%s'...",
      env->class_def->name, name );
    return -1;
  }
  Type_Decl* type_decl = new_Type_Decl( path, is_ref, 0 );
  if(depth)
  {
    type_decl->array = new_array_sub( NULL, 0 );
    type_decl->array->depth = depth;
  }
  Var_Decl var_decl = new_Var_Decl( name, NULL, 0);
  if(depth)
  {
    var_decl->array = new_array_sub( NULL, 0 );
    var_decl->array->depth = depth;
  }
  Var_Decl_List var_decl_list = new_Var_Decl_List(var_decl, NULL, 0 );
  Expression exp_decl = new_Decl_Expression(type_decl, var_decl_list , 1, 0);
  var_decl->addr = (void *)addr;

  if(scan1_Decl_Expression(env, exp_decl->decl_exp) < 0)
    goto error;
  if( scan2_Decl_Expression(env, exp_decl->decl_exp) < 0)
    goto error;
  if(!check_Decl_Expression(env, exp_decl->decl_exp))
    goto error;
  if(doc)
    var_decl->value->doc = doc;
  var_decl->value->is_import = 1;
  // cleanup
  free_ID_List( path );
  return 1;
error:
  free_ID_List( path );
  return -1;
}

static Arg_List make_dll_arg_list(DL_Func * dl_fun)
{
  Arg_List arg_list    = NULL;
  Type_Decl* type_decl = NULL;
  Var_Decl var_decl    = NULL;
  ID_List type_path    = NULL;
//  ID_List name_path    = NULL;
  Array_Sub array_sub  = NULL;
  DL_Value* arg        = NULL;
  m_uint array_depth = 0;
  m_uint array_depth2 = 0;
  m_int i = 0,j;

  for(i = vector_size(dl_fun->args) -1; i >= 0; i--)
  {
    array_depth = array_depth2 = 0; 
    array_sub = NULL;
    arg = vector_at(dl_fun->args, i);
    type_path = str2list(arg->type, &array_depth);
    if(!type_path)
    {
      err_msg(TYPE_,  0, "...at argument '%i'...", i+1 );
      free_Vector(dl_fun->args);
/*      free_Arg_List(arg_list);*/
      return NULL;
    }
    type_decl = new_Type_Decl( type_path, 0, 0 );
//    name_path = str2list( arg->name, &array_depth2 );
    if(array_depth && array_depth2)
    {
      err_msg(TYPE_,  0, "array subscript specified incorrectly for built-in module" );
      // TODO: cleanup
      return NULL;
    }
    // TODO: arrays?
    if(array_depth2)
      array_depth = array_depth2;        
    if(array_depth)
    {
      array_sub = new_array_sub( NULL, 0 );
      for(j= 1; j < array_depth; j++ )
        array_sub = prepend_array_sub( array_sub, NULL, 0 );
    }
    var_decl = new_Var_Decl(arg->name, array_sub, 0 );
    arg_list = new_Arg_List( type_decl, var_decl, arg_list, 0 );
    arg_list->doc = arg->doc;
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

  func_decl = ae_key_func;
  static_decl = is_static ? ae_key_static : ae_key_instance;
  type_path = str2list(dl_fun->type, &array_depth);
  type_decl = new_Type_Decl(type_path, 0, 0);

  if(!type_path)
  {
    // error
    err_msg(TYPE_, 0, "...during function import '%s' (type)...", dl_fun->name);
    goto error;
  }

  // array types
  // this allows us to define built-in functions that return array types
  // however doing this without garbage collection is probably a bad idea
  // -spencer
  if(array_depth)
  {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++ )
      array_sub = prepend_array_sub(array_sub, NULL, 0);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }

  name = dl_fun->name;
  arg_list = make_dll_arg_list(dl_fun);
  /*
    if( dl_fun->args.size() > 0 && !arg_list )
    {
        // error
        EM_error2( 0, "...during function import '%s' (arg_list)...",
            dl_fun->name);
        // delete type_decl
        // delete_type_decl( type_decl );
        type_decl = NULL;
        goto error;
    }
*/

  func_def = new_Func_Def( func_decl, static_decl, type_decl, (char *)name,
                           arg_list, NULL, 0 );
  func_def->s_type = ae_func_builtin;
  func_def->dl_func_ptr = (void *)dl_fun->mfun;

  
  return func_def;

 error: 
  // clean up
  // if( !func_def ) delete_type_decl( type_decl );
  // else delete_func_def( func_def );
  return NULL;
}

Func import_mfun(Env env, DL_Func * mfun)
{
  Func_Def func_def = NULL;
  if(!mfun)
    return NULL;
  if(!env->class_def)
  {
    err_msg(TYPE_, 0,
      "import error: import_mfun '%s' invoked between begin/end", mfun->name);
    return NULL;
  }
  func_def = make_dll_as_fun(mfun, 0);
  if(!func_def)
    return NULL;
  if(scan1_Func_Def(env, func_def) < 0)
      return NULL;
  if(scan2_Func_Def(env, func_def) < 0)
      return NULL;
  if(check_Func_Def(env, func_def) < 0)
      return NULL;
  return func_def->func;
}

Func import_sfun(Env env, DL_Func * mfun)
{
  Func_Def func_def = NULL;
  if(!mfun)
    return NULL;
  if(!env->class_def)
  {
    err_msg(TYPE_, 0,
      "import error: import_sfun '%s' invoked between begin/end", mfun->name);
    return NULL;
  }
  func_def = make_dll_as_fun(mfun, 1);
  if(!func_def)
    return NULL;
  if(scan1_Func_Def(env, func_def) < 0)
      return NULL;
  if(scan2_Func_Def(env, func_def) < 0)
      return NULL;
  if(check_Func_Def(env, func_def) < 0)
      return NULL;

  return func_def->func;
}

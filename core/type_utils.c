#define _XOPEN_SOURCE 700
#include <string.h>
#include "err_msg.h"
#include "absyn.h"
#include "context.h"

static m_uint type_xid = te_last;
static m_bool do_type_xid = 0;

int verify_array(Array_Sub array)
{
  if( array->err_num ) {
    if( array->err_num == 1 ) {
      err_msg(UTIL_, array->pos,
              "invalid format for array init [...][...]..." );
      return -1;
    } else if(array->err_num == 2 ) {
      err_msg(UTIL_, array->pos,
              "partially empty array init [...][]..." );
      return -1;
    } else {
      err_msg(UTIL_, array->pos,
              "internal error: unrecognized array error..." );
      return -1;
    }
  }
  return 1;
}

int isa(Type var, Type parent)
{
  if(var->xid == parent->xid)
    return 1;
  if(var->parent)
    return isa(var->parent, parent);
  return -1;
}

int isres(Env env, S_Symbol xid, int pos)
{
  if(!strcmp(S_name(xid), "this"))
    goto error;
  if(!strcmp(S_name(xid), "now"))
    goto error;
  return -1;
error:
  err_msg(UTIL_, 0, "%s is reserved.", S_name(xid));
  return 1;
}

int isprim(Type type)
{
  if(type->array_depth)
    return -1;
  if(isa(type, &t_object) > 0)
    return -1;
  return 1;
}

Type new_Type(Context context)
{
  Type type         = calloc(1, sizeof(struct Type_));
  type->xid         = te_null;
  type->name        = "";
  type->parent      = NULL;
  type->info        = NULL;
  type->owner       = NULL;
  type->func        = NULL;
  type->actual_type = NULL;
  type->array_depth = 0;
  type->is_complete = 0;
  type->def         = NULL;
  type->obj         = new_VM_Object(e_type_obj);
//  context_add_type(context, type, type->obj);
  type->obj->ref    = context->new_types;
//  vector_append(context->new_types, type);
  return type;
}

void free_Type(Type a)
{
  if(!a->is_complete && a->xid == te_user) {
    if(a->info)
      rem_ref(a->info->obj, a->info);
    free(a);
    return;
  }

  if(a->info) {
    rem_ref(a->info->obj, a->info);
  }
  if(a->parent == &t_int || isa(a, &t_class) > 0 || isa(a, &t_function) > 0)
    free(a);
  else if(a->xid > type_xid || isa(a, &t_func_ptr) > 0)
    free(a);
  else if(a->xid == te_user)
    free(a);
}

Type type_copy(Env env, Type type)
{
  Type a         = calloc(1, sizeof(struct Type_));
  a->xid         = type->xid;
  a->name        = type->name;
  a->parent      = type->parent;
  a->info        = type->info;
  a->owner       = type->owner;
  a->func        = type->func;
  a->size        = type->size;
  a->actual_type = type->actual_type;
  a->array_depth = type->array_depth;
  a->is_complete = type->is_complete;
  a->def         = type->def;
  a->obj         = new_VM_Object(e_type_obj);
  return a;
}

Type find_type(Env env, ID_List path)
{
  S_Symbol xid = NULL;
  NameSpace nspc;
  Type t = NULL;
  Type type = namespace_lookup_type(env->curr, path->xid, 1);
  if(!type)
    return NULL;
  nspc = type->info;
  path = path->next;

  while(path) {
    xid = path->xid;
    t = namespace_lookup_type(nspc, xid, 1);
    while( !t && type && type->parent ) {
      /*        t = namespace_lookup_type( type->parent->info, xid, 1);*/
      t = namespace_lookup_type(type->parent->info, xid, -1);
      type = type->parent;
    }
    if(!t) {
      err_msg(UTIL_, path->pos,
              "...(cannot find class '%s' in namespace '%s')",
              S_name(xid), nspc->name);
      return NULL;
    }
    type = t;
    if(type)
      nspc = type->info;
    path = path->next;
  }
  return type;
}

m_bool add_global_value(Env env, m_str name, Type type, m_bool is_const, void* data)
{
  Value v = new_Value(env->global_context, type, name);
  if(!v)
    return -1;
  v->checked = 1;
  v->is_const = is_const;
  v->is_context_global = 1;
  if(data)
    v->ptr = data;
  namespace_add_value(env->global_nspc, insert_symbol(name), v);
//  namespace_add_value(env->curr, insert_symbol(name), v);

  v->owner = env->global_nspc; // ?
  // doc
//  namespace_add_value(env->global_context->nspc, insert_symbol(name), v);
  context_add_value(env->global_context, v, v->obj);
  return 1;
}

m_bool add_global_value_double(Env env, m_str name, Type type, m_float data)
{
  Value v = new_Value(env->global_context, type, name);
  if(!v)
    return -1;
  v->checked = 1;
  v->is_const = 1;
  v->is_context_global = 1;
  m_float* f = calloc(1, sizeof(m_float));
  *f = data;
  v->ptr = (m_uint*)f;
  namespace_add_value(env->global_nspc, insert_symbol(name), v);
  v->owner = env->global_nspc;
//  namespace_add_value(env->curr, insert_symbol(name), v);

  // doc
//  namespace_add_value(env->global_context->nspc, insert_symbol(name), v);
  context_add_value(env->global_context, v, v->obj);
//  namespace_add_value(env->context->nspc, insert_symbol(type->name), v);

  return 1;
}

void start_type_xid()
{
  do_type_xid = 1;
}

m_bool add_global_type(Env env, Type type)
{
  Type v_type = type_copy(env, &t_class);
  v_type->actual_type = type;
  type->obj = new_VM_Object(e_type_obj);
  /*  namespace_add_type(env->global_nspc, insert_symbol(type->name), type);*/
  namespace_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_Value(env->global_context, v_type, type->name);
  v->checked = 1;
  v->is_const = 1;
  v->is_context_global = 1;
//	v->owner = env->curr;
  namespace_add_value(env->curr, insert_symbol(type->name), v);

  // doc
//  namespace_add_type(env->context->nspc, insert_symbol(type->name), type);
  context_add_type(env->global_context, type, type->obj);
  type->owner = env->curr;
  if(do_type_xid) {
    type_xid++;
    type->xid = type_xid;
  }
  return 1;
}

Value find_value(Type type, S_Symbol xid )
{
  Value value = NULL;
  if(!type)
    return NULL;
  if(!type->info)
    return NULL;
  // -1 for base
  if((value = namespace_lookup_value(type->info, xid, -1)))
    return value;
  if(type->parent)
    return find_value(type->parent, xid);
  return NULL;
}

m_str type_path(ID_List path )
{
  char str[256];
  memset(str, 0, sizeof(str));
  while(path) {
    strncat(str, S_name(path->xid), 256 - strlen(str));
    if(path->next)
      strcat(str, ".");
    path = path->next;
  }
  return strdup(str);
}

Kindof kindof(Type type)
{
  if(type->array_depth)
    return Kindof_Int;
  if(isa(type, &t_void) > 0)
    return Kindof_Void;
  else if(isa(type, &t_complex) > 0 || isa(type, &t_polar) > 0)
    return Kindof_Complex;
  if(isa(type, &t_int) > 0 || isa(type, &t_object) > 0)
    return Kindof_Int;
  else if(isa(type, &t_float) > 0 || isa(type, &t_time) > 0 || isa(type, &t_dur) > 0 )
    return Kindof_Float;
  else if(isa(type, &t_vec3) > 0)
    return Kindof_Vec3;
  else if(isa(type, &t_vec4) > 0)
    return Kindof_Vec4;
  return Kindof_Int;
}


Type new_array_type(Env env, Type array_parent, m_uint depth, Type base_type, NameSpace owner_nspc )
{
  Type t = new_Type(env->context);
  t->xid = te_array;
  t->name = base_type->name;
  t->parent = array_parent;
  /*  SAFE_ADD_REF(t->parent);*/
  /*  t->size = array_parent->size;*/
  t->size = SZ_INT;
  t->array_depth = depth;
  t->array_type = base_type;
  t->info = array_parent->info;
  /*  SAFE_ADD_REF(t->array_type);*/
  /*  SAFE_ADD_REF(t->info);*/
  t->owner = owner_nspc;
  /*  SAFE_ADD_REF(t->owner);*/
  return t;
}
Type find_common_anc(Type lhs, Type rhs )
{
  // check to see if either is child of other
  if(isa(lhs, rhs) > 0) return rhs;
  if(isa(rhs, lhs) > 0) return lhs;

  // move up
  Type t = lhs->parent;

  // not at root
  while(t) {
    // check and see again
    if(isa(rhs, t) > 0)
      return t;
    // move up
    t = t->parent;
  }

  // didn't find
  return NULL;
}
m_int str2char( const m_str c, m_int linepos )
{
  if(c[0] == '\\') {
    switch(c[1]) {
    case '0':
      return '\0';
    case '\'':
      return '\'';
    case '\\':
      return '\\';
    case 'a':
      return '\a';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'v':
      return 'v';

    default:
      err_msg(UTIL_, linepos, "unrecognized escape sequence '\\%c'", c[1] );
      return -1;
    }
  } else {
    return c[0];
  }
}

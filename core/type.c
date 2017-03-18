#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <inttypes.h>
#include "err_msg.h"
#include "type.h"
#include "absyn.h"
#include "env.h"
#include "context.h"
#include "func.h"
#include "import.h"
#include "lang.h"
#include "ugen.h"
#include "object.h"
#include "string.h"
#include "bbq.h"

m_bool scan0_Ast(Env, Ast);
m_bool scan1_Ast(Env, Ast);
m_bool scan2_Ast(Env, Ast);

m_bool check_Func_Def(Env env, Func_Def f);
static Type check_Expression(Env env, Expression exp);
static m_bool check_Stmt(Env env, Stmt* stmt);
static Type check_Dot_Member(Env env, Dot_Member* member);
static m_bool check_Stmt_List(Env env, Stmt_List list);
/* static */ Type check_Func_Call1(Env env, Expression exp_func, Expression args, Func *m_func, int pos );
static Type check_Func_Call(Env env, Func_Call* exp_func);
static m_bool check_Class_Def(Env env, Class_Def class_def);
/* static  */ Func find_func_match(Func up, Expression args);


struct Type_ t_void      = { "void",       0, NULL, te_void};
struct Type_ t_complex;
struct Type_ t_function  = { "@function",   sizeof(m_uint),   NULL, te_function };
struct Type_ t_func_ptr  = { "@func_ptr",   sizeof(m_uint),   &t_function, te_func_ptr};
struct Type_ t_dur       = { "dur",         sizeof(m_float),  NULL, te_dur };
struct Type_ t_time      = { "time",        sizeof(m_float),  NULL, te_time };
struct Type_ t_now       = { "@now",        sizeof(m_float),  &t_time, te_now };
struct Type_ t_class     = { "@Class",       SZ_INT,   NULL, te_class };
struct Type_ t_std       = { "Std",          0, NULL, te_std};
struct Type_ t_machine   = { "Machine",      0, NULL, te_machine};
struct Type_ t_array  = { "@Array",     SZ_INT, &t_object, te_array };
struct Type_ t_null  = { "@null",     sizeof(void*), NULL, te_null};
struct Type_ t_vararg = { "@Vararg",    SZ_INT, &t_object, te_vararg};
struct Type_ t_varobj = { "VarObject", SZ_INT, &t_object, te_vararg};

static int so_filter(const struct dirent* dir)
{
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

m_bool import_soundpipe(Env env);

static Vector plugs = NULL;
void start_plug()
{
  plugs = new_Vector();
}

void stop_plug()
{
  m_uint i;
  if(!plugs)
    return;
  for(i = 0; i < vector_size(plugs); i++)
    dlclose((void*)vector_at(plugs, i));
  free_Vector(plugs);
}

Env type_engine_init(VM* vm)
{
  Env env = new_Env();

  /* add primitive types */
  CHECK_BO(add_global_type(env, &t_void))
  CHECK_BO(import_int(env))
  /* coverity[leaked_storage : FALSE] */
  CHECK_BO(import_float(env))
  CHECK_BO(import_complex(env))
  CHECK_BO(import_vec3(env))
  CHECK_BO(import_vec4(env))

  // base object types
  CHECK_BO(import_object(env))
  CHECK_BO(import_string(env))
  CHECK_BO(import_shred(env))
  CHECK_BO(import_event(env))
  CHECK_BO(import_ugen(env))
  CHECK_BO(import_array(env))

//  CHECK_BO(import_io(env))
  start_type_xid();
  // event child
  CHECK_BO(import_fileio(env))

  // libs
  CHECK_BO(import_lib(env))
  CHECK_BO(import_machine(env))

  // SOUNDPIPE
  CHECK_BO(import_soundpipe(env))

  CHECK_BO(import_analys(env))
  // additionnal UGen modules
  CHECK_BO(import_modules(env))

  vm->dac       = new_M_UGen();
  vm->adc       = new_M_UGen();
  vm->blackhole = new_M_UGen();

  assign_ugen(vm->dac->ugen, 2, 2, 0, vm->bbq->sp);
  assign_ugen(vm->adc->ugen, 2, 2, 0, vm->bbq);
  assign_ugen(vm->blackhole->ugen, 1, 1, 0, vm->bbq->sp);
  vm->dac->ugen->tick = dac_tick;
  vm->adc->ugen->tick = adc_tick;
  vector_append(vm->ugen, (vtype)vm->blackhole->ugen);
  vector_append(vm->ugen, (vtype)vm->dac->ugen);
  vector_append(vm->ugen, (vtype)vm->adc->ugen);

  add_global_value(env, "adc",               &t_ugen,   1, vm->adc);
  add_global_value(env, "dac",               &t_ugen,   1, vm->dac);
  add_global_value(env, "blackhole",         &t_ugen,   1, vm->blackhole);
  add_global_value_double(env, "d_zero",     &t_dur,    0.);
  add_global_value_double(env, "samp",       &t_dur,    1.);
  add_global_value_double(env, "samplerate", &t_float,  (m_float)(vm->bbq->sp->sr));
  add_global_value_double(env, "ms",         &t_dur,    (m_float)(vm->bbq->sp->sr) / 1000.);
  add_global_value_double(env, "second",     &t_dur,    (m_float)vm->bbq->sp->sr);
  add_global_value_double(env, "minute",     &t_dur,    (m_float)60 * vm->bbq->sp->sr);
  add_global_value_double(env, "hour",       &t_dur,    (m_float)60 * 60 * vm->bbq->sp->sr);
  add_global_value_double(env, "day",        &t_dur,    (m_float)24 * 60 * 60 * vm->bbq->sp->sr);
  add_global_value_double(env, "week",       &t_dur,    (m_float)7 * 24 * 60 * 60 * vm->bbq->sp->sr);


  /* TODO ? reserved words */

  /* commit */
  namespace_commit(env->global_nspc);

  // doc
  namespace_commit(env->context->nspc);
  map_set(env->known_ctx, (vtype)insert_symbol(env->global_context->filename), (vtype)env->global_context);
  env->global_context->tree = calloc(1, sizeof(struct Ast_));
  env->global_context->tree->doc = "this is the main context, where basic type and global variables are declared";
  // user namespace
  /*  env->user_nspc = new_NameSpace();*/
  /*  env->user_nspc->name = "[user]";*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  /*  add_ref(env->global_nspc->obj);*/
  /*  add_ref(env->user_nspc->obj);*/
  /*  env->curr = env->user_nspc;*/
  // plugins
  //  void* handler;

  start_plug();
  m_str dirname = GWION_ADD_DIR;
  struct dirent **namelist;
  int n;
  n = scandir(GWION_ADD_DIR, &namelist, so_filter, alphasort);
  if (n > 0) {
    while (n--) {
      char c[256];
      sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
      void* handler = dlopen(c, RTLD_LAZY);
      {
        if(!handler) {
//          dlclose(handler);
          m_str err = dlerror();
          err_msg(TYPE_, 0, "error in %s.", err);
          free(err);
          goto next;
        }
      }
      m_bool (*import)(Env) = (m_bool (*)(Env)) (intptr_t)dlsym(handler, "import");
      if(import) {
        if(import(env) > 0)
          vector_append(plugs, (vtype)handler);
        else {
          dlclose(handler);
          goto next;
        }
      } else {
        m_str err = dlerror();
        err_msg(TYPE_, 0, "%s: no import function.", err);
        dlclose(handler);
        /*		if(err)
                free(err); */
        goto next;
      }
next:
      free(namelist[n]);
    }
    free(namelist);
  }
  namespace_commit(env->global_nspc);
  return env;
}

m_bool check_Context(Env env, Context context)
{
#ifdef DEBUG_TYPE
  debug_msg("type", "context");
#endif
  int ret = 1;
  Ast prog = context->tree;
  while(prog && ret > 0) {
    switch(prog->section->type) {
    case ae_section_stmt:
      ret = check_Stmt_List(env, prog->section->d.stmt_list);
      break;

    case ae_section_func:
      ret = check_Func_Def(env, prog->section->d.func_def);
      break;

    case ae_section_class:
      ret = check_Class_Def(env, prog->section->d.class_def);
      break;
    }
    CHECK_BB(ret);
    prog = prog->next;
  }
  /*
     if( !ret )
     {
  // TODO: remove the effects of the context in the env
  // ---> insert code here <----

  // flag the context with error, so more stuff gets deleted
  env->context->has_error = TRUE;
  }
  */

  return 1;
}
m_bool type_engine_check_prog(Env env, Ast ast, m_str filename)
{
  m_bool ret = -1;
  //  Context context = new_Context(ast, strndup(filename, strlen(filename)));
  Context context = new_Context(ast, filename);
  env_reset(env);
  if(load_context(context, env) < 0) {
    ret = -1;
    goto done;
  }

  if(scan0_Ast(env, ast) < 0) {
    ret = -1;
    goto cleanup;
  }
  /*if(scan1_Ast(env, ast) < 0)*/
  /*m_bool ret = */
  ret = scan1_Ast(env, ast);
  if(ret < 0) {
    ret = -3;
    goto cleanup;
  }
  if(scan2_Ast(env, ast) < 0) {
    ret = -1;
    goto cleanup;
  }
  if(check_Context(env, context) < 0 ) {
    ret = -5;
    goto cleanup;
  }
  ret = 1;

cleanup:
//    add_ref(context->obj);
  if(ret > 0) {
    namespace_commit(env->global_nspc);
    map_set(env->known_ctx, (vtype)insert_symbol(context->filename), (vtype)context);
    add_ref(context->obj);
  } else {
    namespace_rollback(env->global_nspc);
    //rem_ref(context->obj, context);
  }
  CHECK_BB(unload_context(context, env)) // no real need to check that
  if(ret < 0) {
    free_Ast(ast);
    //rem_ref(context->nspc->obj, context->nspc);
    rem_ref(context->obj, context); // breaks function pointer for now
    free(filename);
  }
done:
  return ret;
}

static m_bool check_array_subscripts(Env env, Expression exp_list)
{
  Expression exp = exp_list;
  while(exp) {
    if(isa(exp->type, &t_int) < 0) {
      err_msg(TYPE_, exp->pos,
              "incompatible array subscript type '%s'...", exp->type->name);
      return -1;
    }
    exp = exp->next;
  }
  return 1;
}

static m_bool check_Class_Def(Env env, Class_Def class_def)
{
  Type the_class = NULL;
  Type t_parent = NULL;
  m_bool ret = 1;
  Class_Body body = class_def->body;

  if(class_def->ext) {
    if(class_def->ext->extend_id) {
      t_parent = find_type(env, class_def->ext->extend_id);
      if(!t_parent) {
        m_str path = type_path(class_def->ext->extend_id);
        err_msg(TYPE_, class_def->ext->pos,
                "undefined parent class '%s' in definition of class '%s'", path, S_name(class_def->name->xid) );
        free(path);
        return -1;
      }
      if(isprim(t_parent) > 0) {
        err_msg(TYPE_, class_def->ext->pos,
                "cannot extend primitive type '%s'", t_parent->name);
        err_msg(TYPE_, class_def->ext->pos,
                "...(note: primitives types are 'int', 'float', 'time', and 'dur')" );
        return -1;
      }
      if(!t_parent->is_complete) {
        err_msg(TYPE_, class_def->ext->pos,
                "cannot extend incomplete type '%s'", t_parent->name);
        err_msg(TYPE_, class_def->ext->pos,
                "...(note: the parent's declaration must preceed child's)" );
        return -1;
      }
    }
    // TODO: interface
  }


  if( !t_parent )
    t_parent = &t_object;
  the_class = class_def->type;
  the_class->parent = t_parent;
  /*  the_class->ugen_info = t_parent->ugen_info;*/
  the_class->info->offset = t_parent->obj_size;
  free_Vector(the_class->info->obj_v_table);
  the_class->info->obj_v_table = vector_copy(t_parent->info->obj_v_table);
  vector_append(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_append(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body && ret > 0) {
    switch(body->section->type) {
    case ae_section_stmt:
      env->class_def->has_constructor |= (body->section->d.stmt_list->stmt != NULL);
      ret = check_Stmt_List(env, body->section->d.stmt_list);
      break;

    case ae_section_func:
      env->class_def->is_complete = 1;
      ret = check_Func_Def(env, body->section->d.func_def);
      env->class_def->is_complete = 0;
      break;

    case ae_section_class:
      ret = check_Class_Def( env, body->section->d.class_def);
      break;
    }
    body = body->next;
  }
  env->class_def = (Type)vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = (NameSpace)vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);

  if(ret > 0) {
    the_class->obj_size = the_class->info->offset;
    the_class->is_complete = 1;
  } else {

//   scope_rem(env->curr->type, class_def->type);
//namespace_add_type(env->curr, insert_symbol(class_def->type->name), NULL);
//free_Type(the_class);
//free(class_def->type);
//class_def->type = NULL;
    // delete the class definition
    /*      SAFE_RELEASE( class_def->type );*/
//add_ref(class_def->type->obj);
//add_ref(class_def->type->obj);
//    rem_ref(class_def->type->obj, class_def->type);
    // set the thing to NULL
//    the_class = NULL;
  }
  return ret;
}

Type check_Decl_Expression(Env env, Decl_Expression* decl)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "decl");
#endif
  Value value = NULL;
  Type type = NULL;
  Var_Decl_List list = decl->list;
  Var_Decl var_decl;
  while(list) {
    if(env->class_def && !env->class_scope &&
        (value = find_value(env->class_def->parent, list->self->xid))) {
      err_msg(TYPE_, list->self->pos,
              "in class '%s': '%s' has already been defined in parent class '%s'...",
              env->class_def->name, S_name(list->self->xid), value->owner_class->name);
//      add_ref(env->class_def->obj);
//      add_ref(env->class_def->obj);
      return NULL;
    }
    var_decl = list->self;
    value = list->self->value;

    if(!value) {
      err_msg(TYPE_, list->self->pos, "can't declare in GACK");
      return NULL;
    }

    type  = value->m_type;
    if(var_decl->array && var_decl->array->exp_list) {
      if(!check_Expression(env, var_decl->array->exp_list)) {
        free(type->obj);
        free(type);
        return NULL;
      }
      if(check_array_subscripts(env, var_decl->array->exp_list) < 0) {
        free(type->obj);
        free(type);
        return NULL;
      }
    }
    if(value->is_member) {
      value->offset = env->curr->offset;
      value->owner_class->obj_size += type->size;
      /* coverity[var_deref_op : FALSE] */
      env->class_def->obj_size += type->size;
      env->curr->offset += type->size;
    } else if( decl->is_static ) { // static
      if(!env->class_def || env->class_scope > 0 ) {
        err_msg(TYPE_, decl->pos,
                "static variables must be declared at class scope..." );
        return NULL;
      }
      value->is_static = 1;
      // offset
      value->offset = env->class_def->info->class_data_size;
      // move the size
      env->class_def->info->class_data_size += type->size;
      /*      env->class_def->info->class_data_size++;*/
      /*
         if(is_obj && !is_ref)
         {
      // for now - no good for static, since we need separate
      // initialization which we don't have
      err_msg(TYPE_, var_decl->pos,
      "cannot declare static non-primitive objects (yet)..." );
      err_msg(TYPE_, var_decl->pos,
      "...(hint: declare as ref (@) & initialize outside for now)" );
      return NULL;
      }
      */
    }
    list->self->value->checked = 1;
    if(!env->class_def || env->class_scope)
      namespace_add_value(env->curr, list->self->xid, list->self->value);
    list = list->next;
  }
  return decl->m_type;
}

static Type check_array_lit(Env env, Primary_Expression *exp)
{
  Type t = NULL, type = NULL, common = NULL;
  if(verify_array(exp->d.array) < 0)
    return NULL;
  Expression e = exp->d.array->exp_list;

  // can't be []
  if(!e) {
    err_msg(TYPE_, exp->pos, "must provide values/expressions for array [...]" );
    return NULL;
  }

  // go through the array and type check each
  CHECK_OO(check_Expression(env, e))

  // loop
  while( e ) {
    // get the type
    t = e->type;

    // compare
    if(!type) {
      // first
      type = t;
    } else {
      // find common ancestor
      common = find_common_anc(t, type);
      // update type
      if(common) type = common;
      // no common
      else {
        // maybe one is int and other is float
        if(isa(t, &t_int) > 0 && isa(type, &t_float) > 0) {
          // cast from int to float
          e->cast_to = type;
        } else {
          // incompatible
          err_msg(TYPE_, e->pos, "array init [...] contains incompatible types..." );
          return NULL;
        }
      }
    }
    e = e->next;
  }
  t = new_array_type(
        env,  // the env
        &t_array,  // the array base class, usually &t_array
        /*    T_array,  // the array base class, usually &t_array*/
        type->array_depth + 1,  // the depth of the new type
        type->array_depth ? type->array_type : type,  // the 'array_type'
        env->curr  // the owner namespace
      );
  exp->d.array->type = t;
  return t;
}

static Type check_Vec(Env env, Primary_Expression* exp )
{
  Type t = NULL;
  Vec val = exp->d.vec;
  if(val->numdims > 4) {
    err_msg(TYPE_, exp->pos,
            "vector dimensions not supported > 4...\n"
            "    --> format: @(x,y,z,w)" );
    return NULL;
  }
  Expression e = val->args;
  // count
  int count = 1;
  // loop over arguments
  while(e) {
    if(!(t = check_Expression(env, e)))
      return NULL;
    if(isa( t, &t_int) > 0) e->cast_to = &t_float;
    else if(isa( t, &t_float) < 0) {
      err_msg(TYPE_, exp->pos,
              "invalid type '%s' in vector value #%d...\n"
              "    (must be of type 'int' or 'float')", t->name, count );
      return NULL;
    }
    count++;
    e = e->next;
  }
  if(val->numdims < 4)
    return &t_vec3;
  return &t_vec4;
}

static Type check_Primary_Expression(Env env, Primary_Expression* primary)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "primary");
#endif
  Type  t   = NULL;
  Value v   = NULL;
  m_str str;

  switch(primary->type) {
  case ae_primary_var:
    str = S_name(primary->d.var);      // first look for reserved words
    if(!strcmp(str, "this")) { // this
      // in class def
      if( !env->class_def ) {
        err_msg(TYPE_, primary->pos, "keyword 'this' can be used only inside class definition..." );
        return NULL;
      }

      // in member func
      if( env->func && !env->func->is_member ) {
        err_msg(TYPE_, primary->pos, "keyword 'this' cannot be used inside static functions..." );
        return NULL;
      }

      // not assignable
      primary->self->meta = ae_meta_value;
      // whatever the class is
      t = env->class_def;
    } else if(!strcmp(str, "me")) {
      primary->self->meta = ae_meta_value;
      t = &t_shred;
    } else if(!strcmp(str, "now")) {
      /*        primary->self->meta = ae_meta_value;*/
      primary->self->meta = ae_meta_var;
      t = &t_now;
    } else if(!strcmp(str, "NULL") || !strcmp(str, "null")) {
      /*        primary->self->meta = ae_meta_value;*/
      primary->self->meta = ae_meta_var;
      t = &t_null;
    } else if(!strcmp(str, "true") || !strcmp(str, "false") || !strcmp(str, "maybe")) {
      primary->self->meta = ae_meta_value;
      t = &t_int;
    } else {
      v = namespace_lookup_value(env->curr, primary->d.var, 1);
      if(!v)
        v = find_value(env->class_def, primary->d.var);
      if(v) {
        if(env->class_def && env->func) {
          if(env->func->def->static_decl == ae_key_static && v->is_member && !v->is_static ) {
            err_msg(TYPE_, primary->pos, "non-static member '%s' used from static function...", S_name(primary->d.var) );
            return NULL;
          }
        }
      }
      // check me
      if(!v || !v->checked) {
        str = S_name(primary->d.var);
        err_msg(TYPE_, primary->pos, "variable %s not legit at this point.",
                str ? str : "", v);
        return NULL;
      }
      t = v->m_type;
      primary->value = v;
    }
    if(v && v->is_const == 2) // hack for enum
      primary->self->meta = ae_meta_value;
    break;
  case ae_primary_num:
    t = &t_int;
    break;
  case ae_primary_float:
    t = &t_float;
    break;
  case ae_primary_complex:
    if(!primary->d.cmp->im) {
      err_msg(TYPE_, primary->d.cmp->pos, "missing imaginary component of complex value...");
      return NULL;
    }
    if(primary->d.cmp->im->next) {
      err_msg(TYPE_, primary->d.cmp->pos, "extraneous component of complex value...");
      return NULL;
    }
    CHECK_OO(check_Expression(env, primary->d.cmp->re))
    /*      CHECK_OO(check_Expression(env, primary->cmp->im))*/
    if(isa(primary->d.cmp->re->type, &t_float) < 0) {
      if(isa(primary->d.cmp->re->type, &t_int) < 0) {
        err_msg(TYPE_, primary->d.cmp->pos,
                "invalid type '%s' in real component of complex value...\n"
                "    (must be of type 'int' or 'float')", primary->d.cmp->re->type->name);
        return NULL;
      }
      primary->d.cmp->re->cast_to = &t_float;
    }
    if(isa(primary->d.cmp->im->type, &t_float) < 0) {
      if(isa(primary->d.cmp->im->type, &t_int) < 0) {
        err_msg(TYPE_, primary->d.cmp->pos,
                "invalid type '%s' in imaginary component of complex value...\n"
                "    (must be of type 'int' or 'float')", primary->d.cmp->im->type->name);
        return NULL;
      }
      primary->d.cmp->im->cast_to = &t_float;
    }
    t = &t_complex;
    break;
  case ae_primary_polar:
    if(!primary->d.polar->phase) {
      err_msg(TYPE_, primary->d.polar->pos, "missing phase component of polar value...");
      return NULL;
    }
    if(primary->d.polar->phase->next) {
      err_msg(TYPE_, primary->d.polar->pos, "extraneous component of polar value...");
      return NULL;
    }
    CHECK_OO(check_Expression(env, primary->d.polar->mod))
    /*      CHECK_OO(check_Expression(env, primary->polar->phase))*/
    if(isa(primary->d.polar->mod->type, &t_float) < 0) {
      if(isa(primary->d.polar->mod->type, &t_int) < 0) {
        err_msg(TYPE_, primary->d.polar->pos,
                "invalid type '%s' in modulus component of polar value...\n"
                "    (must be of type 'int' or 'float')", primary->d.polar->mod->type->name);
        return NULL;
      }
      primary->d.polar->mod->cast_to = &t_float;
    }
    if(isa(primary->d.polar->phase->type, &t_float) < 0) {
      if(isa(primary->d.polar->phase->type, &t_int) < 0) {
        err_msg(TYPE_, primary->d.polar->pos,
                "invalid type '%s' in phase component of polar value...\n"
                "    (must be of type 'int' or 'float')", primary->d.polar->phase->type->name);
        return NULL;
      }
      primary->d.polar->phase->cast_to = &t_float;
    }
    t = &t_polar;
    break;
  case ae_primary_vec:
    t = check_Vec(env, primary);
    break;
  case ae_primary_nil:
    t = &t_void;
    break;
  case ae_primary_str:
    t = &t_string;
    break;
  case ae_primary_hack:
    if(primary->d.exp->exp_type == Decl_Expression_type) {
      err_msg(TYPE_, primary->pos, "cannot use <<< >>> on variable declarations...\n");
      return NULL;
    }
    CHECK_OO((t = check_Expression(env, primary->d.exp)))
    break;
  case ae_primary_array:
    t = check_array_lit(env, primary);
    break;
  case ae_primary_char:
    t = &t_int;
    break;
  default:
    err_msg(TYPE_, primary->pos, "internal error - unrecognized primary type '%i'...", primary->type);
    return NULL;
  }
  return primary->self->type = t;
}

Type check_Array(Env env, Array* array )
{
  Type t_base, t;
  m_uint depth;
  // verify there are no errors from the parser...
  CHECK_BO(verify_array(array->indices))

  t_base = check_Expression(env, array->base);
  CHECK_OO(t_base)

  if(array->indices->depth > t_base->array_depth) {
    err_msg(TYPE_,  array->pos,
            "array subscripts (%i) exceeds defined dimension (%i)",
            array->indices->depth, t_base->array_depth );
    free(t_base->obj);
    free(t_base);
    return NULL;
  }

  CHECK_OO(check_Expression(env, array->indices->exp_list))

  Expression e = array->indices->exp_list;
  depth = 0;

  while(e) {
    depth++;
    /*    if(isa( e->type, &t_int ) < 0 && isa( e->type, &t_string ) < 0)*/
    if(isa(e->type, &t_int) < 0) {
      err_msg(TYPE_,  e->pos,
              /*        "array index %i must be of type 'int' or 'string', not '%s'",*/
              "array index %i must be of type 'int', not '%s'",
              depth, e->type->name);
    free(t_base->obj);
    free(t_base);
      return NULL;
    }
    e = e->next;
  }

  t = NULL;
  if(depth == t_base->array_depth)
    t = array->base->type->array_type;
  else {
    t = type_copy(env, array->base->type);
    t->array_depth -= depth;
  }
  return t;
}

m_bool compat_func(Func_Def lhs, Func_Def rhs, int pos);
static Type check_op( Env env, Operator op, Expression lhs, Expression rhs, Binary_Expression* binary )
{
#ifdef DEBUG_TYPE
  debug_msg("check", "'%s' %s '%s'", lhs->type->name, op2str(op), rhs->type->name);
#endif

  Type t;

  if(op == op_chuck && isa(binary->rhs->type, &t_func_ptr) > 0) {
    err_msg(TYPE_, binary->pos, "use '@=>' to assign to function pointer.");
    return NULL;
  }

  if(op == op_at_chuck &&  isa(binary->lhs->type, &t_function) > 0 && isa(binary->rhs->type, &t_func_ptr) > 0) {
    /*    m_bool l_member;*/
    Type r_nspc, l_nspc = NULL;
    m_uint i;
    Func f1, f2 = NULL;
    Value v;
    char name[1024];
    Type ret_type;


    if(isa(binary->lhs->type, &t_func_ptr) > 0) {
      err_msg(TYPE_, binary->pos, "can't assign function pointer to function pointer for the moment. sorry.");
      v = namespace_lookup_value(env->curr, binary->lhs->d.exp_primary->d.var, 1);
//f2 = namespace_lookup_func(env->curr, insert_symbol(v->m_type->name), 1);
//Type t = namespace_lookup_type(env->curr, insert_symbol(v->name), 1);
//scope_rem(env->curr->type, insert_symbol(t->name));
//scope_rem(env->curr->type, insert_symbol(t->name));
//t->obj->ref_count--;
//	rem_ref(t->func->obj, t->func);
//	rem_ref(t->obj, t);
//t = NULL;

//      v = namespace_lookup_value(env->curr, binary->rhs->d.exp_primary->var, 1);

//t = namespace_lookup_value(env->curr, binary->rhs->d.exp_primary->var, 1)->m_type;
//scope_rem(env->curr->type, insert_symbol(v->name));
//printf("HERE HERE %p\n", t->obj);
//rem_ref(t->obj, t);

//printf("HERE HERE %p\n", binary->rhs->type->obj);
//binary->lhs->type->obj->ref_count--;
//binary->rhs->type->obj->ref_count--;
//free(binary->lhs->type);
//binary->lhs->type = NULL;
//add_ref(binary->lhs->type->obj);
//rem_ref(binary->lhs->type->obj, binary->lhs->typ);
//printf("here\n");
//free(binary->lhs->type);
//      v = namespace_lookup_value(env->curr, binary->lhs->d.exp_primary->var, 1);
//f2 = namespace_lookup_func(env->curr, insert_symbol(v->m_type->name), 1);
//scope_rem(env->curr->func, f2);
//scope_rem(env->curr->func, f2);
      /*
      f2->obj->ref_count--;
      v->func_ref = NULL;
      f2->def->func = NULL;
      f2->def = NULL;
      rem_ref(f2->obj, f2);
      */
      return NULL;
    }

    if(env->class_def) {
      err_msg(TYPE_, binary->pos, "can't assign function pointer in class for the moment. sorry.");
      v = namespace_lookup_value(env->curr, binary->lhs->d.exp_primary->d.var, 1);
      f2 = namespace_lookup_func(env->curr, insert_symbol(v->m_type->name), 1);
//f2->def = NULL;
//scope_rem(env->curr->func, f2);
//scope_rem(env->curr->func, f2);
//add_ref(f2->obj);
      f2->obj->ref_count--;
      v->func_ref = NULL;
      f2->def->func = NULL;
      f2->def = NULL;
//      rem_ref(f2->obj, f2);
      return NULL;
    }
// was here
    if(binary->rhs->exp_type == Primary_Expression_type) {

      /*      f1 = namespace_lookup_func(env->curr, binary->rhs->d.exp_primary->var, -1);*/
      v = namespace_lookup_value(env->curr, binary->rhs->d.exp_primary->d.var, 1);
      f1 = namespace_lookup_func(env->curr, insert_symbol(v->m_type->name), -1);
      /*f1 = v->func_ref;*/
      r_nspc = NULL; // get owner
      ret_type  = namespace_lookup_type(env->curr, insert_symbol(v->m_type->name), -1);
    } else {
      v = namespace_lookup_value(binary->rhs->d.exp_dot->t_base->info, binary->rhs->d.exp_dot->xid, 1);
      f1 = namespace_lookup_func(env->curr, (insert_symbol(v->m_type->name)), -1);
      r_nspc = (v->owner_class && v->is_member) ? v->owner_class : NULL; // get owner
      ret_type  = namespace_lookup_type(env->curr, (insert_symbol(v->m_type->name)), -1);
    }
    if(binary->lhs->exp_type == Primary_Expression_type) {
      v = namespace_lookup_value(env->curr, binary->lhs->d.exp_primary->d.var, 1);
      f2 = namespace_lookup_func(env->curr, insert_symbol(v->m_type->name), 1);
      l_nspc = NULL; // get owner
      if(!f2) exit(140);
    } else if(binary->lhs->exp_type == Dot_Member_type) {
      v = find_value(binary->lhs->d.exp_dot->t_base, binary->lhs->d.exp_dot->xid);
      f2 = v->func_ref;
      l_nspc = (v->owner_class && v->is_member) ? v->owner_class : NULL; // get owner
    }
    if((r_nspc && l_nspc) && (r_nspc != l_nspc)) {
      err_msg(TYPE_, binary->pos, "can't assign member function to member function pointer of an other class");
      return NULL;
    }
    if(!r_nspc && l_nspc) {
      err_msg(TYPE_, binary->pos, "can't assign member function to non member function pointer");
//.rem_ref(f2->obj, f2);
//f2->obj->ref_count--;
//Type t = find_type(env->curr, insert_symbol(f1->name));
//printf("type %p\n", binary->rhs->type->obj);
//printf("type %p\n", binary->rhs->type->obj);
//rem_ref(ret_type->obj, ret_type);
//rem_ref(binary->rhs->type->obj, binary->rhs->type);
//	rem_ref(f1->value_ref->m_type->obj, f1->value_ref->m_type);
//binary->rhs->type->obj->ref_count--;
      return NULL;
    }
    if(r_nspc && !l_nspc) {
      err_msg(TYPE_, binary->pos, "can't assign non member function to member function pointer");
      return NULL;
    }
    for(i = 0; i <= v->func_num_overloads; i++) {
      if(binary->lhs->exp_type == Primary_Expression_type) {
        m_str c = f2 && f2->def ? S_name(f2->def->name) : NULL;
        sprintf(name, "%s@%li@%s", c, i, env->curr->name);
        f2 = namespace_lookup_func(env->curr, insert_symbol(name), 1);
      }
      /*if(!f1)*/
      /*f1 = namespace_lookup_value(env->curr, binary->rhs->d.exp_primary->var, 1)->func_ref;*/
      if(f1 && f2 && compat_func(f1->def, f2->def, f2->def->pos) > 0) {
        binary->func = f2;
        ret_type = f1->def->ret_type;
        ret_type->func = f2;
        return ret_type;
      }
    }
    err_msg(TYPE_, 0, "not match found for function '%s'", f2 ? S_name(f2->def->name) : "[broken]");
    return NULL;
  }
  // check for arrays
  if((lhs->type->array_depth == rhs->type->array_depth + 1) && op == op_shift_left && isa(lhs->type->array_type, rhs->type) > 0)
    return lhs->type;
  if((lhs->type->array_depth || rhs->type->array_depth) && (op == op_at_chuck && lhs->type->array_depth == rhs->type->array_depth))
    return rhs->type;
  if(isa(binary->rhs->type, &t_function) > 0 && binary->op == op_chuck)
    return check_Func_Call1(env, rhs, lhs, &binary->func, binary->pos);
  if(isa(binary->rhs->type, &t_object) > 0 && isa(binary->lhs->type, &t_null) > 0 && binary->op == op_at_chuck)
    return lhs->type;
  if(isa(binary->lhs->type, &t_varobj) > 0 && binary->op == op_at_chuck)
    return rhs->type;
  if(isa(binary->rhs->type, binary->lhs->type) > 0 && binary->op == op_at_chuck)
    return rhs->type;
  if((t = get_return_type(env, op, lhs->type, rhs->type)))
    return t;
  m_uint i;
  char la[256], ra[256];
  memset(la, 0, 256);
  memset(ra, 0, 256);
  for(i = 0; i < lhs->type->array_depth; i++)
    strcat(la, "[]");
  for(i = 0; i < rhs->type->array_depth; i++)
    strcat(ra, "[]");
  err_msg(TYPE_, 0, "not match found for operator '%s' on types '%s%s' and '%s%s'",
          op2str(op), lhs->type->name, la, rhs->type->name, ra);
  return NULL;
}

static Type check_Binary_Expression( Env env, Binary_Expression* binary )
{
#ifdef DEBUG_TYPE
  debug_msg("check", "binary expression '%p' '%p'", binary->lhs, binary->rhs);
#endif
  Type ret = NULL;
  Expression cl = binary->lhs, cr = binary->rhs;

  CHECK_OO(check_Expression(env, cl))
  CHECK_OO(check_Expression(env, cr))

  switch(binary->op) {
  case op_assign:
    if(cl->meta != ae_meta_var) {
      err_msg(TYPE_, cr->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cr->pos,
              "...(reason: --- left-side operand is not mutable)" );
      return NULL;
    }
    // mark to emit var instead of value
    cl->emit_var = 1;
    break;
  case op_at_chuck:
    if(isa(cl->type, &t_object) > 0 && isa(cr->type, &t_object) > 0) {
      /*      cr->emit_var = cl->emit_var = 1;*/
      cr->emit_var = 1;
      break;
    }
  case op_chuck:
    if(isa(cl->type, &t_ugen) > 0 && isa(cr->type, &t_ugen) > 0) {
      cr->emit_var = cl->emit_var = 0;
      break;
    }
    if(isa(cl->type, &t_string) > 0 && isa(cr->type, &t_string) > 0) {
      cr->emit_var = cl->emit_var = 0;
      break;
    }
  case op_plus_chuck:
  case op_minus_chuck:
  case op_times_chuck:
  case op_divide_chuck:
  case op_modulo_chuck:
  case op_rsand:
  case op_rsor:
  case op_rsxor:
  case op_rsl:
  case op_rsr:
  case op_rand:
  case op_ror:
  case op_req:
  case op_rneq:
  case op_rgt:
  case op_rge:
  case op_rlt:
  case op_rle:
    // make sure mutable
    if(cr->meta != ae_meta_var) {
      err_msg(TYPE_, cl->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cl->pos,
              "...(reason: --- right-side operand is not mutable)" );
      return NULL;
    }
    // mark to emit var instead of value
    cr->emit_var = 1;
    break;
  default:
    break;
  }

  if(binary->op == op_at_chuck) {
    if(isa(binary->lhs->type, &t_void) > 0 && isa(binary->rhs->type, &t_object) > 0)
      return cl->type;
  }

  while(cr) {
    CHECK_OO((ret = check_op(env, binary->op, cl, cr, binary)))
    cr = cr->next;
  }
  return ret;
}

static Type check_Cast_Expression(Env env, Cast_Expression* cast)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "cast expression");
#endif// check the exp
  Type t = check_Expression( env, cast->exp );
  if(!t) return NULL;

  Type t2 = find_type(env, cast->type->xid);
  if(!t2) {
    m_str path = type_path(cast->type->xid);
    err_msg(TYPE_, cast->pos, "unknown type '%s' in cast expression.", path);
    free(path);
    return NULL;
  }

  if(isa(t2, &t_func_ptr) > 0) {
    Value v = namespace_lookup_value(env->curr, cast->exp->d.exp_primary->d.var,  1);
    Func  f = namespace_lookup_func(env->curr, insert_symbol(v->name),  1);
    if(compat_func(t2->func->def, f->def, f->def->pos)) {
      cast->func = f;
      return t2;
    }
  }
  /*  if( t->xid == t2->xid ||*/
  /*    ( t->xid == t_int.xid) && (t2->xid == t_int.xid)*/
  /*  )*/
  if(isa(t, &t_null) > 0 && isa(t2, &t_object) > 0)
    return t2;
  if(isa(t, &t_object) < 0)
	  return isa(t, t2) > 0 ? t2 : NULL;
  // check if cast valid
  Type type = t;
  while(type) {
	if (t2 == type)
		return t2;
	type = type->parent;
  }
  /*  if( !type_engine_check_cast_valid( env, t2, t ) )*/
  err_msg(TYPE_, cast->pos, "invalid cast to '%s' from '%s'...", S_name( cast->type->xid->xid ), t->name );
  return NULL;
}

static Type check_Postfix_Expression(Env env, Postfix_Expression* postfix)
{
  // check the exp
  Type t = check_Expression(env, postfix->exp);
  if( !t ) return NULL;

  // syntax
  switch( postfix->op ) {
  case op_plusplus:
  case op_minusminus:

    // see scan1
    // assignable?
    /*if(((postfix->exp->meta != ae_meta_var) || (postfix->exp->exp_type == Primary_Expression_type)) &&*/
    /*postfix->exp->d.exp_primary->value->is_const) {*/
    /*err_msg(TYPE_, postfix->exp->pos,*/
    /*"postfix operator '%s' cannot be used on non-mutable data-type...",*/
    /*op2str( postfix->op ) );*/
    /*return NULL;*/
    /*}*/

    postfix->exp->emit_var = 1;
    // TODO: mark somewhere we need to post increment

    // check type
    if(isa(t, &t_int) > 0 || isa(t, &t_float) > 0)
      return t;
    break;

  default:
    // no match
    err_msg( TYPE_, postfix->pos,
             "internal compiler error: unrecognized postfix '%i'", postfix->op );
    return NULL;
  }

  // no match
  err_msg( TYPE_, postfix->pos,
           "no suitable resolutation for postfix operator '%s' on type '%s'...",  op2str(postfix->op), t->name );
  return NULL;
}

static Type check_Dur(Env env, Exp_Dur* dur)
{
  Type base = check_Expression(env, dur->base);
  Type unit = check_Expression(env, dur->unit);
  if(!base || !unit)
    return NULL;
  if(isa(base, &t_int) < 0 && isa(base, &t_float) < 0) {
    err_msg(TYPE_, dur->base->pos,
            "invalid type '%s' in prefix of dur expression...\n"
            "    (must be of type 'int' or 'float')", base->name);
    return NULL;
  }
  if(isa(unit, &t_dur) < 0) {
    err_msg( TYPE_, dur->unit->pos,
             "invalid type '%s' in postfix of dur expression...\n"
             "    (must be of type 'dur')", unit->name);
    return NULL;
  }
  return unit;
}

static Func find_func_match_actual(Func up, Expression args, m_bool implicit, m_bool specific)
{
  Expression e;
  Arg_List e1;
  m_uint count;
  Func func;
  int match = -1;
  // see if args is nil
  if(args && args->type == &t_void)
    args = NULL;

  while(up) {
    func = up;
    while(func) {
      e = args;
      e1 = func->def->arg_list;
      count = 1;
      while(e) {
        if(e1 == NULL) {
          if(func->def->is_variadic)
            return func;
          goto moveon;
        }
        match = specific ? e->type == e1->type : isa(e->type, e1->type);
        if(match <= 0) {
          if( implicit && e->type->xid == t_int.xid && e1->type->xid == t_float.xid )
            /*            if(implicit && isa(e->type, &t_int) > 0 && isa(e1->type, &t_float) > 0)*/
            e->cast_to = &t_float;
          /*            else */
          /*              goto moveon; // type mismatch*/
          else if(!(isa(e->type, &t_null) > 0 && isa(e1->type, &t_object) > 0)) /// Hack
            goto moveon; // type mismatch
        }

        e = e->next;
        e1 = e1->next;
        count++;
      }
      if( e1 == NULL ) return func;

moveon:
      func = func->next;
    }
    // go up
    if( up->up )
      up = up->up->func_ref;
    else up = NULL;
  }

  // not found
  return NULL;
}




// move to type_utils ?
Func find_func_match(Func up, Expression args)
{
  Func func;

  func = find_func_match_actual(up, args, 0, 1);
  if(func) return func;


  func = find_func_match_actual(up, args, 1, 1);
  if(func) return func;


  func = find_func_match_actual(up, args, 0, 0);
  if(func) return func;


  func = find_func_match_actual(up, args, 1, 0);
  if(func) return func;

  return NULL;
}

static Type_List mk_type_list(Env env, Type type)
{
  m_uint i;
  NameSpace nspc = type->info;
  Vector v = new_Vector();
  vector_append(v, (vtype)type->name);
  while(nspc && nspc != env->curr && nspc != env->global_nspc) {
    vector_append(v, (vtype)strdup(nspc->name));
    nspc = nspc->parent;
  }
  ID_List id = NULL;
  Type_List list = NULL;
  for(i = vector_size(v); i > 0; i--)
    id = prepend_id_list((m_str)vector_at(v, i - 1), id, 0);
  list = new_type_list(id, NULL, 0);
  free_Vector(v);
  return list;
}

/*static */
static Func_Call* current;
Func find_template_match(Env env, Value v, Func m_func, Type_List types,
                         Expression func, Expression args)
{
  m_uint i;
  Func_Def base;
  Value value;
  if(v->owner_class) {
    vector_append(env->nspc_stack, (vtype)env->curr);
    env->curr = v->owner_class->info;
    vector_append(env->class_stack, (vtype)env->class_def);
    env->class_def = v->owner_class;
    env->class_scope = 0; // should keep former value somewhere
  }
  for(i = 0; i < v->func_num_overloads + 1; i++) {
    char name[256];
    sprintf(name, "%s<template>@%li@%s", v->name, i, env->curr->name);
    if(v->owner_class) {
      value = find_value(v->owner_class, insert_symbol(name));
      if(!value)
        value = v;
    } else
      value = namespace_lookup_value(env->curr, insert_symbol(name), 1);
    if(!value)
      continue;
    base = value->func_ref->def;
    if(!base)
      continue;
    Func_Def def = new_Func_Def(base->func_decl, base->static_decl,
                                base->type_decl, S_name(func->d.exp_primary->d.var),
                                base->arg_list, base->code, func->pos);
    if(base->is_variadic)
      def->is_variadic = 1;
    Type_List list = types;
    ID_List base_t = base->types;
    def->is_template = 1;
    namespace_push_type(env->curr);
    while(base_t) {
      if(!list)
        break;
      ID_List tmp = base_t->next;;
      if(!list->list)
        break;
      namespace_add_type(env->curr, base_t->xid, find_type(env, list->list));
      base_t->next = NULL;
      base_t->next = tmp;

      if(list->next && !base_t->next) {
//        err_msg(TYPE_, def->pos, "'%s' too many argument for template. skipping.", value->name);
        goto next;
      }
      base_t = base_t->next;
      if(!list->next && base_t) {
//        err_msg(TYPE_, def->pos, "'%s' not enough argument for template.", value->name);
        goto next;
        //return NULL;
      }
      list = list->next;
    }
    m_int ret = scan1_Func_Def(env, def);
    namespace_pop_type(env->curr);
    if(ret < 0)
      goto next;
    if(scan2_Func_Def(env, def) < 0)
      goto next;
    if(check_Func_Def(env, def) < 0)
      goto next;
    if(!check_Expression(env, func))
      goto next;
    if(args)
      if(!check_Expression(env, args))
        goto next;
    def->func->next = NULL;
    m_func = find_func_match(def->func, args);
    if(m_func) {
      if(v->owner_class) {
        env->class_def = (Type)vector_back(env->class_stack);
        vector_pop(env->class_stack);
        env->curr = (NameSpace)vector_back(env->nspc_stack);
        vector_pop(env->nspc_stack);
      }
      m_func->is_template = 1;
      m_func->def->base = value->func_ref->def->types;
      return m_func;
    }
next:
    namespace_pop_type(env->curr);
    if(def->func) {
      free_Func(def->func);
    }
    free_Func_Def(def);
  }
  if(v->owner_class) {
    env->class_def = (Type)vector_back(env->class_stack);
    vector_pop(env->class_stack);
    env->curr = (NameSpace)vector_back(env->nspc_stack);
    vector_pop(env->nspc_stack);
  }
  return NULL;
}
/* static */ Type check_Func_Call1(Env env, Expression exp_func, Expression args, Func *m_func, int pos )
{
#ifdef DEBUG_TYPE
  debug_msg("check", "func call");
#endif
  Func func = NULL;
  Func up = NULL;
  Type f;

  exp_func->type = check_Expression(env, exp_func);
  f = exp_func->type;
  // primary func_ptr
  if(exp_func->exp_type == Primary_Expression_type &&
      exp_func->d.exp_primary->value &&
      !exp_func->d.exp_primary->value->is_const) {
    f = namespace_lookup_type(env->curr, insert_symbol(exp_func->d.exp_primary->value->m_type->name), -1);
    /*f = namespace_lookup_type(env->curr, insert_symbol(exp_func->d.exp_primary->value->name), -1);*/
    if(!f) {
      err_msg(TYPE_, exp_func->pos, "trying to call empty func pointer.");
      return NULL;
    }
    if(!f->func) { // func ptr
      up = namespace_lookup_func(env->curr, insert_symbol(exp_func->d.exp_primary->value->m_type->name), -1);
      f->func = up;
    }
  }
  if(!f) {
    err_msg(TYPE_, exp_func->pos, "function call using a non-existing function");
    return NULL;
  }
  if(isa(f, &t_function) < 0) {
    err_msg(TYPE_, exp_func->pos, "function call using a non-function value");
    return NULL;
  }
  up = f->func;

  if(args)
    CHECK_OO(check_Expression(env, args))
    // look for a match
    func = find_func_match(up, args);

  // no func
  if(!func) {
    Value value = NULL;
    if(!f->func) {
      if(exp_func->exp_type == Primary_Expression_type)
        value = namespace_lookup_value(env->curr, exp_func->d.exp_primary->d.var, 1);
      else if(exp_func->exp_type == Dot_Member_type)
        value = find_value(exp_func->d.exp_dot->t_base, exp_func->d.exp_dot->xid);

      if(!value) {
        err_msg(TYPE_, exp_func->pos, "function is template. not enough argument for automatic type guess. this message is incorrect");
        return NULL;
      }

      // template guess
      ID_List list = value->func_ref->def->types;
      m_uint type_number = 0;
      m_uint args_number = 0;

      while(list) {
        type_number++;
        list = list->next;
      }
      list = value->func_ref->def->types;
      Type_List tl[type_number ? type_number : 1];
      if(!type_number)
        tl[0] = NULL;
      while(list) { // iterate through types
        Arg_List arg = value->func_ref->def->arg_list;
        Expression template_arg = args;
        while(arg && template_arg) {
          m_str path = type_path(arg->type_decl->xid);
          if(!strcmp(S_name(list->xid), path)) {
            tl[args_number] = mk_type_list(env, template_arg->type);
            if(args_number)
              tl[args_number - 1]->next = tl[args_number];
            args_number++;
            free(path);
            break;
          }
          free(path);
          arg = arg->next;
          template_arg = template_arg->next;
        }
        list = list->next;
      }
      if(args_number < type_number) {
        err_msg(TYPE_, exp_func->pos, "not able to guess types for template call.");
        return NULL;
      }
      Func f = find_template_match(env, value, func, tl[0], exp_func, args);
      if(f) {
        *m_func = f;
        Type ret_type  = f->def->ret_type;
        current->types = tl[0];
        current->base = value->func_ref->def->types;
        return ret_type;
      }
      err_msg(TYPE_, exp_func->pos, "function is template. automatic type guess not fully implemented yet.\n\
          please provide template types. eg: '<type1, type2, ...>'");
      return NULL;
    }
    m_uint i;
    err_msg(TYPE_, exp_func->pos, "argument type(s) do not match for function. should be :");
    up = f->func;
    while(up) {
      Arg_List e = up->def->arg_list;
      fprintf(stderr, "\t");
      if(!e)
        fprintf(stderr, "\033[32mvoid\033[0m");
      while(e) {
        m_str path = type_path(e->type_decl->xid);
#ifdef COLOR
        fprintf(stderr, " \033[32m%s\033[0m \033[1m%s\033[0m", e->type->name, S_name(e->var_decl->xid));
#else
        fprintf(stderr, " %s %s", e->type->name, S_name(e->var_decl->xid));
#endif
        for(i = 0; i < e->type->array_depth; i++)
          fprintf(stderr, "[]");
        e = e->next;
        if(e)
          fprintf(stderr, ",");
        free(path);
      }
      up = up->next;
      fprintf(stderr, ". (%s)\n", f->name);
      if(up)
        fprintf(stderr, "or :");
    }
    fprintf(stderr, "and not");
    fprintf(stderr, "\n\t");
    Expression e = args;
    while(e) {
#ifdef COLOR
      fprintf(stderr, " \033[32m%s\033[0m", e->type->name);
#else
      fprintf(stderr, " %s", e->type->name);
#endif
      for(i = 0; i < e->type->array_depth; i++)
        fprintf(stderr, "[]");
      e = e->next;
      if(e)
        fprintf(stderr, ",");
    }
    fprintf(stderr, "\n");
    return NULL;
  }
  *m_func = func;
  return func->def->ret_type;
}

static Type check_Func_Call(Env env, Func_Call* exp_func)
{
  if(exp_func->types) {
    Value v;
    if(exp_func->func->exp_type == Primary_Expression_type) {
      v = namespace_lookup_value(env->curr, exp_func->func->d.exp_primary->d.var, 1);
    } else {
      Type t;
      CHECK_OO(check_Expression(env, exp_func->func))
      t = exp_func->func->d.exp_dot->t_base;
      if(isa(t, &t_class) > 0)
        t = t->actual_type;
      v = find_value(t, exp_func->func->d.exp_dot->xid);
      // added 06/12/16
      if(!v) {
        err_msg(TYPE_, exp_func->pos, "unknown template function.");
        return NULL;
      }
      if(!v->func_ref) {
        err_msg(TYPE_, exp_func->pos, "non-function template call.");
        return NULL;
      }
      if(!v->func_ref->def->types) {
        err_msg(TYPE_, exp_func->pos, "template call of non-template function.");
        free_Type_List(exp_func->types);
        exp_func->types = NULL;
        return NULL;
      }
    }
    // primary case
    Func ret = find_template_match(env, v, exp_func->m_func, exp_func->types,
                                   exp_func->func, exp_func->args);
    if(!ret) {
      err_msg(TYPE_, exp_func->pos, "arguments do not match for template call");
      free_Type_List(exp_func->types);
      return NULL;
    }
    exp_func->m_func = ret;
    return ret->def->ret_type;
  }
  current = exp_func;
  return check_Func_Call1(env, exp_func->func, exp_func->args,
                          &exp_func->m_func, exp_func->pos );
}

static m_bool check_Func_Ptr(Env env, Func_Ptr* ptr)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "func pointer '%s'", S_name(ptr->xid));
#endif
  Type t     = ptr->value->is_member ?
               new_Type(env->context) :
               namespace_lookup_type(env->curr, ptr->xid, 1);
  t->size    = SZ_INT;
  t->name    = S_name(ptr->xid);
  t->parent  = &t_func_ptr;
  namespace_add_type(env->curr, ptr->xid, t);
  add_ref(t->obj);
  ptr->m_type = t;
  t->func = ptr->func;
  return 1;
}

/// check exp_unary expression
static Type check_Unary(Env env, Unary_Expression* exp_unary)
{
  Type t = NULL;
  if(exp_unary->exp) {
    if(exp_unary->op == op_new) {
      err_msg(TYPE_, exp_unary->pos, "internal error: exp_unary expression not with 'new'");
      return NULL;
    }
    t = check_Expression(env, exp_unary->exp);
    if(!t)
      return NULL;
  }
  // check code stmt; this is to eventually support sporking of code (added 1.3.0.0)
  if(exp_unary->code)
    CHECK_BO(check_Stmt(env, exp_unary->code))

    switch(exp_unary->op) {
    case op_plusplus:
    case op_minusminus:
      // assignable?
      /* coverity[var_deref_op : FALSE] */
      if((exp_unary->exp->meta != ae_meta_var || exp_unary->exp->exp_type == Primary_Expression_type) &&
          exp_unary->exp->d.exp_primary->value->is_const)
        //if(exp_unary->exp->meta != ae_meta_var)
      {
        err_msg(TYPE_, exp_unary->pos,
                "prefix exp_unary operator '%s' cannot "
                "be used on non-mutable data-types...", op2str( exp_unary->op ) );
        return NULL;
      }

      // assign
      exp_unary->exp->emit_var = 1;

      // check type
      if(isa(t, &t_int) > 0 || isa(t, &t_float) > 0)
        return t;
      // TODO: check overloading
      break;

    case op_minus:
      if(isa(t, &t_int) || isa(t, &t_float) > 0)
        return t;
      break;
    case op_tilda:
    case op_exclamation:
      /* coverity[var_deref_model : FALSE] */
      if(isa(t, &t_int) > 0 || isa(t, &t_object) > 0 || isa(t, &t_float) > 0 || isa(t, &t_time) > 0 || isa(t, 
        &t_dur) > 0)
        return &t_int;
      break;

    case op_spork:
      if(exp_unary->exp && exp_unary->exp->exp_type == Func_Call_type) return &t_shred;
      // spork shred (by code segment)
      else if(exp_unary->code) {
        // TODO : in class
        if(env->func) {
          env->class_scope++;
          namespace_push_value(env->curr);
          int ret = check_Stmt(env, exp_unary->code);
          namespace_pop_value(env->curr);
          env->class_scope--;
          if(ret < 0)
            return NULL;
          else return &t_shred;
          break;
        } else if(check_Stmt(env, exp_unary->code) < 0) {
          err_msg(TYPE_, exp_unary->pos, "problem in evaluating sporked code");
          break;

        }
        return &t_shred;
      }
      // got a problem
      else {
        err_msg(TYPE_,  exp_unary->pos, "only function calls can be sporked..." );
        return NULL;
      }
      break;

    case op_new:
      t = find_type(env, exp_unary->type->xid);
      if(!t) {
        err_msg(TYPE_,  exp_unary->pos,
                "... in 'new' expression ..." );
        return NULL;
      }
      if(exp_unary->array) {
        CHECK_BO(verify_array(exp_unary->array))
        if(!exp_unary->array->exp_list) {
          err_msg(TYPE_, exp_unary->pos, "cannot use empty [] with 'new'..." );
          return NULL;
        }
        CHECK_OO(check_Expression(env, exp_unary->array->exp_list))
        CHECK_BO(check_array_subscripts(env, exp_unary->array->exp_list))
        t = new_array_type(env, &t_array, exp_unary->array->depth, t, env->curr);

      }
      if(isa(t->array_type ? t->array_type : t, &t_object) < 0) {
        err_msg(TYPE_,  exp_unary->pos,
                "cannot instantiate/(new) primitive type '%s'...",
                t->name);
        err_msg(TYPE_,  exp_unary->pos,
                "...(primitive types: 'int', 'float', 'time', 'dur')" );
        if(exp_unary->array)
          free_Array_Sub(exp_unary->array);
        free(t->obj);
        free(t);
        return NULL;
      }
      if( exp_unary->type->ref && !exp_unary->array ) {
        err_msg(TYPE_,  exp_unary->pos,
                "cannot instantiate/(new) single object references (@)..." );
        return NULL;
      }
      return t;
      break;
    // avoid comiler warning
    case op_assign:
    case op_plus:
    case op_times:
    case op_divide:
    case op_percent:
    case op_and:
    case op_or:
    case op_eq:
    case op_neq:
    case op_gt:
    case op_ge:
    case op_lt:
    case op_le:
    case op_shift_left:
    case op_shift_right:
    case op_s_or:
    case op_s_and:
    case op_s_xor:
    case op_typeof:
    case op_sizeof:
    case op_chuck:
    case op_plus_chuck:
    case op_minus_chuck:
    case op_times_chuck:
    case op_divide_chuck:
    case op_modulo_chuck:
    case op_rand:
    case op_ror:
    case op_req:
    case op_rneq:
    case op_rgt:
    case op_rge:
    case op_rlt:
    case op_rle:
    case op_rsl:
    case op_rsr:
    case op_rsand:
    case op_rsor:
    case op_rsxor:
    case op_unchuck:
    case op_rdec:
    case op_rinc:
    case op_runinc:
    case op_rundec:
    case op_at_chuck:
    case op_at_unchuck:
    case op_trig:
    case op_untrig:
      break;
    }
  err_msg(TYPE_, exp_unary->pos,
          "no suitable resolution for prefix exp_unary operator '%s' on type '%s...",
          op2str( exp_unary->op ), t->name );
  return NULL;
}
static Type check_exp_if(Env env, If_Expression* exp_if )
{
#ifdef DEBUG_TYPE
  debug_msg("check", "debug exp if");
#endif
  // check the components
  Type cond     = check_Expression(env, exp_if->cond);
  Type if_exp   = check_Expression(env, exp_if->if_exp);
  Type else_exp = check_Expression(env, exp_if->else_exp);

  // make sure everything good
  if(!cond || !if_exp || !else_exp)
    return NULL;

  // check the type
  if(isa(cond, &t_int) < 0)
    return NULL;

  // make sure the if and else have compatible types
  // TODO: the lesser of two types
  /*  if( !( *if_exp == *else_exp ) )*/
  if(!find_common_anc(if_exp, else_exp)) {
    err_msg(TYPE_, exp_if->pos,
            "incompatible types '%s' and '%s' in if expression...", if_exp->name, else_exp->name);
    return NULL;
  }
  return if_exp;
}

static Type check_Expression(Env env, Expression exp)
{

#ifndef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
  Expression curr = exp;
  while(curr) {
    curr->type = NULL;
    switch(curr->exp_type) {
    case Primary_Expression_type:
      curr->type = check_Primary_Expression(env, curr->d.exp_primary);
      break;
    case Decl_Expression_type:
      curr->type = check_Decl_Expression(env, curr->d.exp_decl);
      break;
    case Unary_Expression_type:
      curr->type = check_Unary(env, curr->d.exp_unary);
      break;
    case Binary_Expression_type:
      curr->type = check_Binary_Expression(env, curr->d.exp_binary);
      break;
    case Postfix_Expression_type:
      curr->type = check_Postfix_Expression(env, curr->d.exp_postfix);
      break;
    case Dur_Expression_type:
      curr->type = check_Dur(env, curr->d.exp_dur);
      break;
    case Cast_Expression_type:
      curr->type = check_Cast_Expression(env, curr->d.exp_cast);
      break;
    case Func_Call_type:
      curr->type = check_Func_Call(env, curr->d.exp_func);
      curr->d.exp_func->ret_type = curr->type;
      break;
    case If_Expression_type:
      curr->type = check_exp_if(env, curr->d.exp_if);
      break;
    case Dot_Member_type:
      curr->type = check_Dot_Member(env, curr->d.exp_dot);
      break;
    case Array_Expression_type:
      curr->type = check_Array(env, curr->d.exp_array);
      break;
    default:
      err_msg(TYPE_, curr->pos, "internal error: unhandled expression type '%i'", curr->exp_type);
      exp->type = NULL;
      break;
    }
    CHECK_OO(curr->type)
    curr = curr->next;
  }
  return exp->type;
#ifndef __clang__
#pragma GCC diagnostic pop
#endif
}

static m_bool check_Enum(Env env, Stmt_Enum stmt)
{
  ID_List list = stmt->list;
  Value v;
  NameSpace nspc = env->class_def ? env->class_def->info : env->curr;
  while(list) {
    v = namespace_lookup_value(nspc, list->xid, 0);
    if(env->class_def) {
      // enum in classes are static
      v->is_static = 1;
      v->offset = env->class_def->info->class_data_size;
      env->class_def->info->class_data_size += t_int.size;
    }
    list = list->next;
  }
  return 1;

}
static m_bool check_Stmt_Code(Env env, Stmt_Code stmt, m_bool push)
{
  m_bool ret;

  env->class_scope++;
  if(push)
    namespace_push_value(env->curr);

  ret = check_Stmt_List( env, stmt->stmt_list );

  if(push)
    namespace_pop_value(env->curr);
  env->class_scope--;

  return ret;
}

static m_bool check_While(Env env, Stmt_While stmt)
{
  CHECK_OB(check_Expression(env, stmt->cond))
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    if(isa(stmt->cond->type, &t_io) > 0)
      break;
    err_msg(TYPE_,  stmt->cond->pos,
            "invalid type '%s' in while condition", stmt->cond->type->name);
    return -1;
  }
  vector_append(env->breaks, (vtype)stmt->self);
  vector_append(env->conts, (vtype)stmt->self);
  CHECK_BB(check_Stmt(env, stmt->body))
  vector_pop(env->breaks);
  vector_pop(env->conts);
  return 1;
}

static m_bool check_Until(Env env, Stmt_Until stmt)
{
  CHECK_OB(check_Expression(env, stmt->cond))

  // ensure that conditional has valid type
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    if(isa(stmt->cond->type, &t_io) > 0)
      break;

    err_msg(TYPE_,  stmt->cond->pos,
            "invalid type '%s' in until condition", stmt->cond->type->name);
    return -1;
  }
  vector_append(env->breaks, (vtype)stmt->self);
  CHECK_BB(check_Stmt(env, stmt->body))
  vector_pop(env->breaks);
  return 1;
}
static m_bool check_For(Env env, Stmt_For stmt)
{
  if(check_Stmt(env, stmt->c1) < 0)
    return -1;
  if(check_Stmt(env, stmt->c2) < 0)
    return -1;
  // check for empty for loop conditional (added 1.3.0.0)
  if( stmt->c2 == NULL ) {
    // error
    err_msg(EMIT_, stmt->pos,
            "empty for loop condition..." );
    err_msg(EMIT_, stmt->pos,
            "...(note: explicitly use 'true' if it's the intent)" );
    err_msg(EMIT_, stmt->pos,
            "...(e.g., 'for( ; true; ){ /*...*/ }')" );
    return -1;
  }

  // ensure that conditional has valid type
  switch( stmt->c2->d.stmt_exp->type->xid ) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    // check for IO
    if(isa( stmt->c2->d.stmt_exp->type, &t_io ) > 0)
      break;

    // error
    err_msg(EMIT_,  stmt->c2->d.stmt_exp->pos,
            "invalid type '%s' in for condition", stmt->c2->d.stmt_exp->type->name);
    return -1;
  }

  // check the post
  if( stmt->c3 && !check_Expression(env, stmt->c3))
    return -1;
  // for break and continue statement
  vector_append(env->breaks, (vtype)stmt->self);
  // check body
  // TODO: restore break stack? (same for other loops)
  if(check_Stmt(env, stmt->body) < 0)
    return -1;

  // remove the loop from the stack
  vector_pop(env->breaks);
  return 1;
}
static m_bool check_Loop(Env env, Stmt_Loop stmt)
{
  Type type = check_Expression(env, stmt->cond);

  if(!type)
    return -1;
  if(isa( type, &t_float))
    stmt->cond->cast_to = &t_int;
  else if(isa( type, &t_int) < 0) { // must be int
    err_msg(TYPE_, stmt->pos,
            "loop * conditional must be of type 'int'..." );
    return -1;
  }
  vector_append(env->breaks, (vtype)stmt->self);
  if(check_Stmt(env, stmt->body) < 0)
    return -1;
  vector_pop(env->breaks);
  return 1;
}

static m_bool check_If(Env env, Stmt_If stmt)
{
  CHECK_OB(check_Expression(env, stmt->cond))
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    // check for IO
    if( isa( stmt->cond->type, &t_io ) > 0)
      break;
    if(isa(stmt->cond->type, &t_object) > 0) // added 12/09/16
      break;
    // error
    err_msg(TYPE_, stmt->cond->pos,
            "invalid type '%s' in if condition", stmt->cond->type->name);
    return -1;
  }
  if(check_Stmt(env, stmt->if_body) < 0)
    return -1;
  if( stmt->else_body )
    if(check_Stmt(env, stmt->else_body) < 0)
      return -1;
  return 1;
}

static m_bool check_Return(Env env, Stmt_Return stmt)
{
  Type ret_type = NULL;
  if(!env->func) {
    err_msg(TYPE_, stmt->pos, "'return' statement found outside function definition");
    return -1;
  }
  if(stmt->val) {
    CHECK_OB((ret_type = check_Expression(env, stmt->val)))
  } else
    ret_type = &t_void;
  if(ret_type->xid == t_null.xid && isprim(env->func->def->ret_type) < 0)
    return 1;
  if(isa(ret_type, env->func->def->ret_type) < 0) {
    err_msg(TYPE_, stmt->pos,
            "invalid return type '%s' -- expecting '%s'",
            ret_type->name, env->func->def->ret_type->name);
    return -1;
  }
  return 1;
}

static m_bool check_Continue(Env env, Stmt_Continue cont)
{
  if(!vector_size(env->breaks)) {
    err_msg(TYPE_,  cont->pos,
            "'continue' found outside of for/while/until..." );
    return -1;
  }
  return 1;
}
static m_bool check_Break(Env env, Stmt_Break cont)
{
  if(!vector_size(env->breaks)) {
    err_msg(TYPE_,  cont->pos,
            "'break' found outside of for/while/until..." );
    return -1;
  }
  return 1;
}

static m_bool check_Switch(Env env, Stmt_Switch a)
{
  Type t = check_Expression(env, a->val);
  if(!t || t->xid !=  t_int.xid) {
    err_msg(TYPE_, a->pos, "invalid type '%s' in switch expression. should be 'int'", t ? t->name : "unknown");
    return -1;
  }
  if(check_Stmt(env, a->stmt) < 0) {
    err_msg(TYPE_, a->pos, "\t... in switch statement");
    return -1;
  }

  return 1;
}

static m_bool check_Case(Env env, Stmt_Case stmt)
{
  Type t = check_Expression(env, stmt->val);
  if(!t) {
    err_msg(TYPE_, stmt->pos, "... in case");
    return -1;
  }
  if(t->xid !=  t_int.xid) {
    err_msg(TYPE_, stmt->pos, "invalid type in '%s' case expression. should be 'int'", t->name);
    return -1;
  }
  return 1;
}

static m_bool check_Goto_Label(Env env, Stmt_Goto_Label stmt)
{
  Map m;
  m_uint* key = env->class_def && !env->func ? (m_uint*)env->class_def : (m_uint*)env->func;
  Stmt_Goto_Label ref;
  if(stmt->is_label)
    return 1;
  m = (Map)map_get(env->curr->label, (vtype)key);
  ref = (Stmt_Goto_Label)map_get(m, (vtype)stmt->name);
  if(!ref) {
    err_msg(TYPE_, stmt->pos, "label '%s' used but not defined", S_name(stmt->name));
    return -1;
  }
  vector_append(ref->data.v, (vtype)stmt);
  return 1;
}

static m_bool check_Stmt(Env env, Stmt* stmt)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "stmt");
#endif
  m_bool ret = 1;
  Decl_List l;
  if( !stmt )
    return 1;
  switch( stmt->type ) {
  case ae_stmt_exp:
    if(stmt->d.stmt_exp)
      ret = (check_Expression(env, stmt->d.stmt_exp) ? 1 : -1);
    break;
  case ae_stmt_code:
    env->class_scope++;
    ret = check_Stmt_Code( env, stmt->d.stmt_code, 1);
    env->class_scope--;
    break;
  case ae_stmt_return:
    ret = check_Return(env, stmt->d.stmt_return);
    break;
  case ae_stmt_break:
    ret = check_Break(env, stmt->d.stmt_break);
    break;
  case ae_stmt_continue:
    ret = check_Continue(env, stmt->d.stmt_continue);
    break;
  case ae_stmt_if:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_If(env, stmt->d.stmt_if);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_while:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_While( env, stmt->d.stmt_while);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_until:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_Until( env, stmt->d.stmt_until);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_for:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_For( env, stmt->d.stmt_for);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_loop:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_Loop( env, stmt->d.stmt_loop);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_switch:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = check_Switch( env, stmt->d.stmt_switch);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_case:
    ret = check_Case(env, stmt->d.stmt_case);
    break;
  case ae_stmt_enum:
    ret = check_Enum(env, stmt->d.stmt_enum);
    break;
  case ae_stmt_gotolabel:
    ret = check_Goto_Label(env, stmt->d.stmt_gotolabel);
    break;
  case ae_stmt_funcptr:
    ret = check_Func_Ptr(env, stmt->d.stmt_funcptr);
    break;
  case ae_stmt_union:
    l = stmt->d.stmt_union->l;
    if(env->class_def) {
      stmt->d.stmt_union->o = env->class_def->obj_size;

    }
    while(l) {
      CHECK_OB(check_Decl_Expression(env, l->self))
      if(l->self->m_type->size > stmt->d.stmt_union->s)
        stmt->d.stmt_union->s = l->self->m_type->size;
      l = l->next;
    }
    ret = 1;
    break;
  default:
    err_msg(TYPE_, stmt->pos, "invalid statement type '%i'", stmt->type);
    return -1;
  }
  return ret;
}

static m_bool check_Stmt_List(Env env, Stmt_List list)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "statement list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(check_Stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

static Type check_Dot_Member(Env env, Dot_Member* member)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "dot member");
#endif
  Value value = NULL;
  Type  the_base = NULL;
  m_bool base_static = 0;
  m_str str;

  member->t_base = check_Expression(env, member->base);
  if(!member->t_base)
    return NULL;
  base_static = member->t_base->xid == t_class.xid;
  the_base = base_static ? member->t_base->actual_type : member->t_base;

  /*  if(the_base == &t_vararg)*/
  /*  {*/
  /*    if(member->xid == insert_symbol("start") ||*/
  /*       member->xid == insert_symbol("end") )*/
  /*    return &t_void;*/
  /*  }*/


  if(!the_base->info && !member->t_base->array_depth) { // ?
    err_msg(TYPE_,  member->base->pos,
            "type '%s' does not have members - invalid use in dot expression of %s",
            the_base->name, S_name(member->xid));
    return NULL;
  }

  str = S_name(member->xid);

  if(!strcmp(str, "this")) {
    if(base_static) {
      err_msg(TYPE_,  member->pos,
              "keyword 'this' must be associated with object instance..." );
      return NULL;
    }
    if( env->func && !env->func->is_member ) {
      err_msg(TYPE_, member->pos,
              "keyword 'this' cannot be used inside static functions..." );
      return NULL;
    }
    return env->class_def;
  }

  value = find_value(the_base, member->xid);
  if(!value) {
    m_uint i;
    /*    m_str s = strdup(the_base->name);*/
    char s[1025];
    memset(s, 0, 1025);
    strncpy(s, the_base->name, 1024);
    for(i = 0; i < the_base->array_depth; i++)
      strcat(s, "[]");
    err_msg(TYPE_,  member->base->pos,
            "class '%s' has no member '%s'", s, S_name(member->xid));
    /*    free(s);*/
    return NULL;
  }
  if(base_static && value->is_member) {
    err_msg(TYPE_, member->pos,
            "cannot access member '%s.%s' without object instance...",
            the_base->name, S_name(member->xid) );
    return NULL;
  }
  if(value->is_const == 2) // for enum
    member->self->meta = ae_meta_value;
  return value->m_type;
}

m_bool compat_func(Func_Def lhs, Func_Def rhs, int pos)
{
  /*
    // make sure public/private/protected/function match
    if(lhs->func_decl != rhs->func_decl) {
      err_msg(TYPE_, pos, "function signatures differ in access modifiers..." );
      err_msg(TYPE_, pos, "(both must be one of public/private/protected/function)..." );
      return -1;
    }
  */
  Arg_List e1 = lhs->arg_list;
  Arg_List e2 = rhs->arg_list;
  //  m_uint count = 1;

  // check arguments against the definition
  while(e1 && e2) {
    // match types
    if( e1->type != e2->type ) {
      //          err_msg(TYPE_, pos, "function signatures differ in argument %i's type...", count );
      return -1;
    }
    e1 = e1->next;
    e2 = e2->next;
    //      count++;
  }
  if(e1 || e2)
    return -1;
  return 1;
}

m_bool check_Func_Def(Env env, Func_Def f)
{
#ifdef DEBUG_TYPE
  debug_msg("check", "func def '%s'", f->func->name);
#endif
  Value value = NULL;
  Func func = NULL;
  Type parent = NULL;
  Value override = NULL;
  Value  v = NULL;
  Value vararg = NULL;
  Func  parent_func = NULL;
  Arg_List arg_list = NULL;
  m_bool parent_match = 0;
  m_str func_name = S_name(f->name);
  //  m_str func_name = strdup(S_name(f->name)); // strdup might be unnnecessary. 27/11/16
  m_uint count = 1;
  m_bool has_code = 0;


  if(f->types) // templating, check at call time
    return 1;

  if(env->func) {
    err_msg(TYPE_, f->pos, "nested function definitions are not (yet) allowed");
    return -1;
  }

  func = f->func;
  value = func->value_ref;

  // look up the value in the parent class
  if(env->class_def)
    override = find_value(env->class_def->parent, f->name);
  else if(value->func_num_overloads) { // check if func has ame args
    m_uint i, j;
    if(!f->types)
      for(i = 0; i <= value->func_num_overloads; i++) {
        char name[1024];
        sprintf(name, "%s@%li@%s", S_name(f->name), i, env->curr->name);
        Func f1 = namespace_lookup_func(env->curr, insert_symbol(name), -1);
        for(j = 1; j <= value->func_num_overloads; j++) {
          if(i != j) {
            sprintf(name, "%s@%li@%s", S_name(f->name), j, env->curr->name);
            Func f2 = namespace_lookup_func(env->curr, insert_symbol(name), -1);
            if(compat_func(f1->def, f2->def, f2->def->pos) > 0) {
              err_msg(TYPE_, f2->def->pos, "global function '%s' already defined for those arguments", S_name(f->name));
              return -1;
            }
          }
        }
      }
  }
  if(env->class_def &&  override) {
    if(isa(override->m_type, &t_function) < 0) {
      err_msg(TYPE_, f->pos, "function name '%s' conflicts with previously defined value...",
              S_name(f->name));
      err_msg(TYPE_, f->pos, "from super class '%s'...", override->owner_class->name);
      goto error;
    }
  }
  if(override) {
    // make reference to parent
    // TODO: ref count
    func->up = override;
  }
  if(!env->class_def && f->static_decl == ae_key_abstract) {
    err_msg(TYPE_, f->pos, "non-class function cannot be declared as 'pure'...");
    err_msg(TYPE_, f->pos, "...at function '%s'", S_name(f->name) );
    goto error;
  }

  if(f->s_type == ae_func_user) has_code = (f->code != NULL);
  else has_code = (f->dl_func_ptr != NULL); // imported

  if(env->class_def && env->class_def->def && env->class_def->def->iface && has_code) {
    err_msg(TYPE_, f->pos, "interface function signatures cannot contain code..." );
    err_msg(TYPE_, f->pos, "...at function '%s'", S_name(f->name));
    goto error;
  }

  if(f->static_decl == ae_key_abstract && has_code) {
    err_msg(TYPE_, f->pos, "'pure' function signatures cannot contain code..." );
    err_msg(TYPE_, f->pos, "...at function '%s'", S_name(f->name));
    goto error;
  }

  if(f->static_decl != ae_key_abstract && !has_code) {
    err_msg(TYPE_, f->pos, "function declaration must contain code..." );
    err_msg(TYPE_, f->pos, "(unless in interface, or is declared 'pure')" );
    err_msg(TYPE_, f->pos, "...at function '%s'", S_name(f->name));
    goto error;
  }

  // TODO if overload, check arguments do not match

  // if overriding super class function, then check signatures
  if(env->class_def) {
    // get parent
    parent = env->class_def->parent;
    while( parent && !parent_match ) {
      if((v = find_value(env->class_def->parent, f->name))) {
        // see if the target is a function
        if(!isa( v->m_type, &t_function)) {
          err_msg(TYPE_, f->pos, "function name '%s' conflicts with previously defined value...",
                  S_name(f->name));
          err_msg(TYPE_, f->pos, "from super class '%s'...", v->owner_class->name );
          goto error;
        }

        // parent func
        parent_func = v->func_ref;

        // go through all overloading
        while(parent_func && !parent_match) {
          // match the prototypes
          if(compat_func(f, parent_func->def, f->pos) < 0) {
            // next
            parent_func = parent_func->next;
            // move on
            continue;
          }
          // see if parent function is static
          if( parent_func->def->static_decl == ae_key_static ) {
            err_msg(TYPE_, f->pos,
                    "function '%s.%s' resembles '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name),
                    v->owner_class->name, S_name(f->name) );
            err_msg(TYPE_, f->pos,
                    "...(reason: '%s.%s' is declared as 'static')",
                    v->owner_class->name, S_name(f->name) );
            goto error;
          }

          // see if function is static
          if(f->static_decl == ae_key_static) {
            err_msg(TYPE_, f->pos,
                    "function '%s.%s' resembles '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name),
                    v->owner_class->name, S_name(f->name));
            err_msg(TYPE_, f->pos,
                    "...(reason: '%s.%s' is declared as 'static')",
                    env->class_def->name, S_name(f->name));
            goto error;
          }

          // see if function is pure
          if(f->static_decl == ae_key_abstract) {
            err_msg(TYPE_, f->pos,
                    "function '%s.%s' resembles '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name),
                    v->owner_class->name, S_name(f->name));
            err_msg(TYPE_, f->pos,
                    "...(reason: '%s.%s' is declared as 'pure')",
                    env->class_def->name, S_name(f->name));
            goto error;
          }

          // make sure returns are equal
          if(isa(f->ret_type, parent_func->def->ret_type) < 0) {
            err_msg(TYPE_, f->pos, "function signatures differ in return type..." );
            err_msg(TYPE_, f->pos,
                    "function '%s.%s' matches '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name),
                    v->owner_class->name, S_name(f->name));
            goto error;
          }
          parent_match = 1;
          func->vt_index = parent_func->vt_index;
          vector_set(env->curr->obj_v_table, func->vt_index, (vtype)func);
          func_name = parent_func->name;
          func->name = func_name;
          value->name = func_name;
        }
      }
      // move to next parent
      parent = parent->parent;
    }
  }
  if(func->is_member && !parent_match) {
    func->vt_index = vector_size(env->curr->obj_v_table);
    vector_append(env->curr->obj_v_table, (vtype)func);
  }
  env->func = func;
  namespace_push_value(env->curr);
  arg_list = f->arg_list;
  while(arg_list) {
    v = arg_list->var_decl->value;
    if(namespace_lookup_value( env->curr, arg_list->var_decl->xid, 0)) {
      err_msg(TYPE_, arg_list->pos, "argument %i '%s' is already defined in this scope",
              count, S_name(arg_list->var_decl->xid) );
      err_msg(TYPE_, arg_list->pos, "in function '%s':", S_name(f->name));
      goto error;
    }
    v->checked  = 1;
    namespace_add_value(env->curr, arg_list->var_decl->xid, v );
    count++;
    arg_list = arg_list->next;
  }

  if(f->is_variadic) {
    vararg = new_Value(env->context, &t_vararg, "vararg");
    vararg->checked = 1;
    namespace_add_value(env->curr, insert_symbol("vararg"), vararg);
  }
  if(f->code && check_Stmt_Code(env, f->code->d.stmt_code, 0) < 0) {
    err_msg(TYPE_, f->pos, "...in function '%s'", S_name(f->name));
    goto error;
  }

  if(f->is_variadic)
    rem_ref(vararg->obj, vararg);
  if(f->s_type == ae_func_builtin) { // is this useful ?
    func->code->stack_depth = f->stack_depth;
    func->code->need_this = 1;
  }
//  else
//    func->code->need_this = 0;
  namespace_pop_value(env->curr);
  env->func = NULL;
  return 1;

error:
  namespace_pop_value(env->curr);
  env->func = NULL;
  return -1;
}

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
#include "string.h"
#include "bbq.h"

m_bool scan0_Ast(Env, Ast);
m_bool scan1_ast(Env, Ast);
m_bool scan2_ast(Env, Ast);

m_bool check_func_def(Env env, Func_Def f);
static Type check_exp(Env env, Exp exp);
static m_bool check_stmt(Env env, Stmt stmt);
static Type check_exp_dot(Env env, Exp_Dot* member);
static m_bool check_stmt_list(Env env, Stmt_List list);
/* static */ Type check_exp_call1(Env env, Exp exp_func, Exp args, Func *m_func, int pos);
static Type check_exp_call(Env env, Exp_Func* exp_func);
static m_bool check_class_def(Env env, Class_Def class_def);
static Func find_func_match(Func up, Exp args);

struct Type_ t_void      = { "void",       0,      NULL,        te_void};
struct Type_ t_function  = { "@function",  SZ_INT, NULL,        te_function };
struct Type_ t_func_ptr  = { "@func_ptr",  SZ_INT, &t_function, te_func_ptr};
struct Type_ t_class     = { "@Class",     SZ_INT, NULL,        te_class };
struct Type_ t_gack      = { "@Gack",      SZ_INT, NULL,             te_gack };

static int so_filter(const struct dirent* dir) {
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

static void add_plugs(VM* vm, Vector plug_dirs) {
  m_uint i;
  Env env = vm->env;
  for(i = 0; i < vector_size(plug_dirs); i++) {
    m_str dirname = (m_str)vector_at(plug_dirs, i);
    struct dirent **namelist;
    int n;
    n = scandir(dirname, &namelist, so_filter, alphasort);
    if(n > 0) {
      while(n--)  {
        char c[strlen(dirname) + strlen(namelist[n]->d_name) + 2];
        sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
        void* handler = dlopen(c, RTLD_LAZY);
        {
          if(!handler) {
            m_str err = dlerror();
            err_msg(TYPE_, 0, "error in %s.", err);
            goto next;
          }
        }
        m_bool(*import)(Env) = (m_bool(*)(Env))(intptr_t)dlsym(handler, "import");
        if(import) {
          if(import(env) > 0)
            vector_add(vm->plug, (vtype)handler);
          else {
            env->class_def = (Type)vector_pop(env->class_stack);
            env->curr = (Nspc)vector_pop(env->nspc_stack);
            dlclose(handler);
            goto next;
          }
        } else {
          m_str err = dlerror();
          err_msg(TYPE_, 0, "%s: no import function.", err);
          dlclose(handler);
          goto next;
        }
next:
        free(namelist[n]);
      }
      free(namelist);
    }
  }
}

Env type_engine_init(VM* vm, Vector plug_dirs) {
  Env env = new_env();

  if(add_global_type(env, &t_void) < 0) goto error;
  if(add_global_type(env, &t_null) < 0) goto error;
  if(import_int(env)       < 0) goto error;
  if(import_float(env)     < 0) goto error;
  if(import_complex(env)   < 0) goto error;
  if(import_vec3(env)      < 0) goto error;
  if(import_vec4(env)      < 0) goto error;
  if(import_object(env)    < 0) goto error;
  if(import_string(env)    < 0) goto error;
  if(import_shred(env)     < 0) goto error;
  if(import_event(env)     < 0) goto error;
  if(import_ugen(env)      < 0) goto error;
  if(import_array(env)     < 0) goto error;
  start_type_xid();
  if(import_fileio(env)    < 0) goto error;
  if(import_lib(env)       < 0) goto error;
  if(import_machine(env)   < 0) goto error;
  if(import_soundpipe(env) < 0) goto error;
  if(import_analys(env)    < 0) goto error;
  if(import_modules(env)   < 0) goto error;

  vm->dac       = new_M_UGen();
  vm->adc       = new_M_UGen();
  vm->blackhole = new_M_UGen();

  assign_ugen(vm->dac->ugen, 2, 2, 0, vm->bbq->sp);
  assign_ugen(vm->adc->ugen, 2, 2, 0, vm->bbq);
  assign_ugen(vm->blackhole->ugen, 1, 1, 0, vm->bbq->sp);
  vm->dac->ugen->tick = dac_tick;
  vm->adc->ugen->tick = adc_tick;
  vector_add(vm->ugen, (vtype)vm->blackhole->ugen);
  vector_add(vm->ugen, (vtype)vm->dac->ugen);
  vector_add(vm->ugen, (vtype)vm->adc->ugen);

  ALLOC_PTR(d_zero, m_float, 0.0);
  ALLOC_PTR(sr,     m_float, (m_float)vm->bbq->sp->sr);
  ALLOC_PTR(samp,   m_float, 1.0);
  ALLOC_PTR(ms,     m_float, (m_float)*sr     / 1000.);
  ALLOC_PTR(second, m_float, (m_float)*sr);
  ALLOC_PTR(minute, m_float, (m_float)*sr     * 60.0);
  ALLOC_PTR(hour,   m_float, (m_float)*minute * 60.0);
  ALLOC_PTR(day,    m_float, (m_float)*hour   * 24.0);
  ALLOC_PTR(t_zero, m_float, 0.0);

  add_global_value(env, "adc",        &t_ugen, 1, vm->adc);
  add_global_value(env, "dac",        &t_ugen, 1, vm->dac);
  add_global_value(env, "blackhole",  &t_ugen, 1, vm->blackhole);
  add_global_value(env, "d_zero",     &t_dur,  1, d_zero);
  add_global_value(env, "samplerate", &t_dur,  1, sr);
  add_global_value(env, "samp",       &t_dur,  1, samp);
  add_global_value(env, "ms",         &t_dur,  1, ms);
  add_global_value(env, "second",     &t_dur,  1, second);
  add_global_value(env, "minute",     &t_dur,  1, minute);
  add_global_value(env, "day",        &t_dur,  1, hour);
  add_global_value(env, "hour",       &t_dur,  1, day);
  add_global_value(env, "t_zero",     &t_time, 1, t_zero);
  /* commit */
  nspc_commit(env->global_nspc);

  // doc
  nspc_commit(env->context->nspc);
  map_set(env->known_ctx, (vtype)insert_symbol(env->global_context->filename), (vtype)env->global_context);
  env->global_context->tree = calloc(1, sizeof(struct Ast_));
  env->global_context->tree->doc = "this is the main context, where basic type and global variables are declared";
  // user nspc
  /*  env->user_nspc = new_nspc();*/
  /*  env->user_nspc->name = "[user]";*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  /*  ADD_REF(env->global_nspc->obj);*/
  /*  ADD_REF(env->user_nspc->obj);*/
  /*  env->curr = env->user_nspc;*/
  // plugins
  //  void* handler;
  vm->env = env;
  add_plugs(vm, plug_dirs);
  nspc_commit(env->curr);
  return env;

error:          // LCOV_EXCL_START
  free(env);
  return NULL;
}              // LCOV_EXCL_STOP

static m_bool check_exp_array_subscripts(Env env, Exp exp_list) {
  Exp exp = exp_list;
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

Type check_exp_decl(Env env, Exp_Decl* decl) {
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
      CHECK_OO(check_exp(env, var_decl->array->exp_list))
      CHECK_BO(check_exp_array_subscripts(env, var_decl->array->exp_list))
    }
    if(GET_FLAG(value, ae_value_member)) {
      value->offset = env->curr->offset;
      value->owner_class->obj_size += type->size;
      env->curr->offset += type->size;
    } else if(decl->is_static) {
      if(!env->class_def || env->class_scope > 0) {
        err_msg(TYPE_, decl->pos,
                "static variables must be declared at class scope...");
        return NULL;
      }
      SET_FLAG(value, ae_value_static);
      value->offset = env->class_def->info->class_data_size;
      env->class_def->info->class_data_size += type->size;
    }
    SET_FLAG(list->self->value, ae_value_checked);
    if(!env->class_def || env->class_scope)
      nspc_add_value(env->curr, list->self->xid, list->self->value);
    list = list->next;
  }
  return decl->m_type;
}

static Type check_exp_prim_array(Env env, Exp_Primary *exp) {
  Exp e;
  Type t = NULL, type = NULL, common = NULL;


  CHECK_BO(verify_array(exp->d.array))
  if(!(e = exp->d.array->exp_list)) {
    err_msg(TYPE_, exp->pos, "must provide values/expressions for array [...]");
    return NULL;
  }
  CHECK_OO(check_exp(env, e))
  while(e) {
    t = e->type;
    if(!type)
      type = t;
    else {
      common = find_common_anc(t, type);
      if(common)
        type = common;
      else {
        if(isa(t, &t_int) > 0 && isa(type, &t_float) > 0) {
          e->cast_to = type;
        } else {
          err_msg(TYPE_, e->pos, "array init [...] contains incompatible types...");
          return NULL;
        }
      }
    }
    e = e->next;
  }
  t = new_array_type(env, type->array_depth + 1,
                     type->array_depth ? type->array_type : type,  env->curr);
  exp->d.array->type = t;
  return t;
}

static Type check_vec(Env env, Exp_Primary* exp) {
  Type t = NULL;
  Vec val = exp->d.vec;
  if(val->numdims > 4) {
    err_msg(TYPE_, exp->pos,
            "vector dimensions not supported > 4...\n"
            "    --> format: @(x,y,z,w)");
    return NULL;
  }
  Exp e = val->args;
  int count = 1;
  while(e) {
    if(!(t = check_exp(env, e)))
      return NULL;
    if(isa(t, &t_int) > 0) e->cast_to = &t_float;
    else if(isa(t, &t_float) < 0) {
      err_msg(TYPE_, exp->pos,
              "invalid type '%s' in vector value #%d...\n"
              "    (must be of type 'int' or 'float')", t->name, count);
      return NULL;
    }
    count++;
    e = e->next;
  }
  if(val->numdims < 4)
    return &t_vec3;
  return &t_vec4;
}

static Type check_exp_primary(Env env, Exp_Primary* primary) {
#ifdef DEBUG_TYPE
  debug_msg("check", "primary");
#endif
  Type  t   = NULL;
  Value v   = NULL;
  m_str str;

  switch(primary->type) {
  case ae_primary_id:
    str = S_name(primary->d.var);
    if(!strcmp(str, "this")) {
      if(!env->class_def) {
        err_msg(TYPE_, primary->pos, "keyword 'this' can be used only inside class definition...");
        return NULL;
      }
      if(env->func && !env->func->is_member) {
        err_msg(TYPE_, primary->pos, "keyword 'this' cannot be used inside static functions...");
        return NULL;
      }
      primary->self->meta = ae_meta_value;
      t = env->class_def;
    } else if(!strcmp(str, "me")) {
      primary->self->meta = ae_meta_value;
      t = &t_shred;
    } else if(!strcmp(str, "now")) {
      primary->self->meta = ae_meta_var;
      t = &t_now;
    } else if(!strcmp(str, "NULL") || !strcmp(str, "null")) {
      primary->self->meta = ae_meta_value;
      t = &t_null;
    } else if(!strcmp(str, "true") || !strcmp(str, "false") || !strcmp(str, "maybe")) {
      primary->self->meta = ae_meta_value;
      t = &t_int;
    } else {
      v = nspc_lookup_value(env->curr, primary->d.var, 1);
      if(!v)
        v = find_value(env->class_def, primary->d.var);
      if(v) {
        if(env->class_def && env->func) {
          if(env->func->def->static_decl == ae_key_static && GET_FLAG(v, ae_value_member) && !GET_FLAG(v, ae_value_static)) {
            err_msg(TYPE_, primary->pos, "non-static member '%s' used from static function...", S_name(primary->d.var));
            return NULL;
          }
        }
      }
      if(!v || !GET_FLAG(v, ae_value_checked)) {
        str = S_name(primary->d.var);
        err_msg(TYPE_, primary->pos, "variable %s not legit at this point.",
                str ? str : "", v);
        return NULL;
      }
      t = v->m_type;
      primary->value = v;
    }
    if(v && GET_FLAG(v, ae_value_const))
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
    CHECK_OO(check_exp(env, primary->d.cmp->re))
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
    CHECK_OO(check_exp(env, primary->d.polar->mod))
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
    t = check_vec(env, primary);
    break;
  case ae_primary_nil:
    t = &t_void;
    break;
  case ae_primary_str:
    t = &t_string;
    break;
  case ae_primary_hack:
    if(primary->d.exp->exp_type == ae_exp_decl) {
      err_msg(TYPE_, primary->pos, "cannot use <<< >>> on variable declarations...\n");
      return NULL;
    }
    CHECK_OO((t = check_exp(env, primary->d.exp)))
	return &t_gack;
    break;
  case ae_primary_array:
    t = check_exp_prim_array(env, primary);
    break;
  case ae_primary_char:
    t = &t_int;
    break;
  }
  return primary->self->type = t;
}

Type check_exp_array(Env env, Exp_Array* array) {
  Type t_base, t;
  m_uint depth;

  CHECK_BO(verify_array(array->indices))
  CHECK_OO((t_base = check_exp(env, array->base)))

  if(array->indices->depth > t_base->array_depth) {
    err_msg(TYPE_,  array->pos,
            "array subscripts (%i) exceeds defined dimension (%i)",
            array->indices->depth, t_base->array_depth);
    return NULL;
  }

  CHECK_OO(check_exp(env, array->indices->exp_list))

  Exp e = array->indices->exp_list;
  depth = 0;

  while(e) {
    depth++;
    if(isa(e->type, &t_int) < 0) {
      err_msg(TYPE_,  e->pos,
              "array index %i must be of type 'int', not '%s'",
              depth, e->type->name);
      return NULL;
    }
    e = e->next;
  }

  t = NULL;

  if(depth != array->indices->depth) {
    err_msg(TYPE_, array->pos, "invalid array acces expression.");
    return NULL;
  }

  if(depth == t_base->array_depth)
    t = array->base->type->array_type;
  else {
    t = type_copy(env, array->base->type);
    t->array_depth -= depth;
  }
  return t;
}

__inline m_bool compat_func(Func_Def lhs, Func_Def rhs, int pos) {
  Arg_List e1 = lhs->arg_list;
  Arg_List e2 = rhs->arg_list;

  while(e1 && e2) {
    if(e1->type != e2->type)
      return -1;
    e1 = e1->next;
    e2 = e2->next;
  }
  if(e1 || e2)
    return -1;
  return 1;
}

static Type check_op(Env env, Operator op, Exp lhs, Exp rhs, Exp_Binary* binary) {
#ifdef DEBUG_TYPE
  debug_msg("check", "'%s' %s '%s'", lhs->type->name, op2str(op), rhs->type->name);
#endif

  Type t;

  if(op == op_at_chuck &&  isa(binary->lhs->type, &t_function) > 0 && isa(binary->rhs->type, &t_func_ptr) > 0) {
    Type r_nspc, l_nspc = NULL;
    m_uint i;
    Func f1, f2 = NULL;
    Value v;
    Type ret_type;
    /*
       if(isa(binary->lhs->type, &t_func_ptr) > 0) {
       err_msg(TYPE_, binary->pos, "can't assign function pointer to function pointer for the moment. sorry.");
    //      v = nspc_lookup_value(env->curr, binary->lhs->d.exp_primary.d.var, 1);
    return NULL;
    }
    */
    if(binary->rhs->exp_type == ae_exp_primary) {
      v = nspc_lookup_value(env->curr, binary->rhs->d.exp_primary.d.var, 1);
      //      f1 = (v->owner_class && v->is_member) ? v->func_ref :nspc_lookup_func(env->curr, insert_symbol(v->m_type->name), -1);
      f1 = v->func_ref ? v->func_ref :nspc_lookup_func(env->curr, insert_symbol(v->m_type->name), -1);
    } else if(binary->rhs->exp_type == ae_exp_dot) {
      v = find_value(binary->rhs->d.exp_dot.t_base, binary->rhs->d.exp_dot.xid);
      //      f1 = (v->owner_class && v->is_member) ? v->func_ref :
      f1 = nspc_lookup_func(binary->rhs->d.exp_dot.t_base->info, insert_symbol(v->m_type->name), -1);
    } else if(binary->rhs->exp_type == ae_exp_decl) {
      v = binary->rhs->d.exp_decl.list->self->value;
      f1 = v->m_type->func;
    } else {
      err_msg(TYPE_, binary->pos, "unhandled function pointer assignement (rhs).");
      return NULL;
    }
    r_nspc = (v->owner_class && GET_FLAG(v, ae_value_member)) ? v->owner_class : NULL; // get owner
    if(binary->lhs->exp_type == ae_exp_primary) {
      v = nspc_lookup_value(env->curr, binary->lhs->d.exp_primary.d.var, 1);
      f2 = nspc_lookup_func(env->curr, insert_symbol(v->m_type->name), 1);
      l_nspc = (v->owner_class && GET_FLAG(v, ae_value_member)) ? v->owner_class : NULL; // get owner
    } else if(binary->lhs->exp_type == ae_exp_dot) {
      v = find_value(binary->lhs->d.exp_dot.t_base, binary->lhs->d.exp_dot.xid);
      f2 = v->func_ref;
      l_nspc = (v->owner_class && GET_FLAG(v, ae_value_member)) ? v->owner_class : NULL; // get owner
      /*    } else if(binary->lhs->exp_type == ae_exp_decl) {
            v = binary->lhs->d.exp_decl->list->self->value;
            f2 = v->m_type->func; */
    } else {
      err_msg(TYPE_, binary->pos, "unhandled function pointer assignement (lhs).");
      return NULL;
    }
    if((r_nspc && l_nspc) && (r_nspc != l_nspc)) {
      err_msg(TYPE_, binary->pos, "can't assign member function to member function pointer of an other class");
      return NULL;
    }
    if(!r_nspc && l_nspc) {
      err_msg(TYPE_, binary->pos, "can't assign member function to non member function pointer");
      return NULL;
    }
    if(r_nspc && !l_nspc) {
      err_msg(TYPE_, binary->pos, "can't assign non member function to member function pointer");
      return NULL;
    }
    if(isa(f1->def->ret_type, f2->def->ret_type) < 0) {
      err_msg(TYPE_, 0, "return type '%s' does not match '%s'\n\t... in pointer assignement",
        f1->def->ret_type->name, f2->def->ret_type->name);
      return NULL;
    }

    m_uint digit = v->func_num_overloads ? (floor(log10(v->func_num_overloads)) + 3) : 3;
    for(i = 0; i <= v->func_num_overloads; i++) {
      if(binary->lhs->exp_type == ae_exp_primary) {
        m_str c = f2 && f2->def ? S_name(f2->def->name) : NULL;
        char name[strlen(c) + strlen(env->curr->name) + digit];
        sprintf(name, "%s@%li@%s", c, i, env->curr->name);
        f2 = nspc_lookup_func(env->curr, insert_symbol(name), 1);
      }
      if(f1 && f2 && compat_func(f1->def, f2->def, f2->def->pos) > 0) {
        binary->func = f2;
        ret_type = f1->value_ref->m_type;
        return ret_type;
      }
    }
    err_msg(TYPE_, 0, "no match found for function '%s'", f2 ? S_name(f2->def->name) : "[broken]");
    return NULL;
  }
  // check for arrays
  if((lhs->type->array_depth == rhs->type->array_depth + 1) && op == op_shift_left && isa(lhs->type->array_type, rhs->type) > 0)
    return lhs->type;
  if((lhs->type->array_depth || rhs->type->array_depth) && (op == op_at_chuck && lhs->type->array_depth == rhs->type->array_depth))
    return rhs->type;
  if(isa(binary->rhs->type, &t_function) > 0 && binary->op == op_chuck)
    return check_exp_call1(env, rhs, lhs, &binary->func, binary->pos);
  if(isa(binary->lhs->type, &t_varobj) > 0 && binary->op == op_at_chuck)
    return rhs->type;
  if(isa(binary->rhs->type, binary->lhs->type) > 0 && binary->op == op_at_chuck)
    return rhs->type;
  if((t = get_return_type(env, op, lhs->type, rhs->type)))
    return t;
  m_uint i;
  m_uint llen  = lhs->type->array_depth*2;
  m_uint rlen  = rhs->type->array_depth*2;
  char la[llen + 1], ra[rlen + 1];
  la[llen] = '\0';
  ra[llen] = '\0';
  for(i = 0; i < lhs->type->array_depth; i++)
    strcat(la, "[]");
  for(i = 0; i < rhs->type->array_depth; i++)
    strcat(ra, "[]");
  err_msg(TYPE_, 0, "no match found for operator '%s' on types '%s%s' and '%s%s'",
          op2str(op), lhs->type->name, la, rhs->type->name, ra);
  return NULL;
}

static Type check_exp_binary(Env env, Exp_Binary* binary) {
#ifdef DEBUG_TYPE
  debug_msg("check", "binary expression '%p' '%p'", binary->lhs, binary->rhs);
#endif
  Type ret = NULL;
  Exp cl = binary->lhs, cr = binary->rhs;

  CHECK_OO(check_exp(env, cl))
  CHECK_OO(check_exp(env, cr))

  switch(binary->op) {
  case op_assign:
    if(cl->meta != ae_meta_var) {
      err_msg(TYPE_, cr->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cr->pos,
              "...(reason: --- left-side operand is not mutable)");
      return NULL;
    }
    cl->emit_var = 1;
    break;
  case op_at_chuck:
    if(cr->meta != ae_meta_var) {
      err_msg(TYPE_, cr->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cr->pos,
              "...(reason: --- rigth-side operand is not mutable)");
      return NULL;
    }
    if(isa(cl->type, &t_array) > 0 && isa(cr->type, &t_array) > 0) {
      if(isa(cl->type->array_type, cr->type->array_type) < 0) {
        err_msg(TYPE_, binary->pos, "array types do not match.");
        return NULL;
      }
      if(cl->type->array_depth != cr->type->array_depth) {
        err_msg(TYPE_, binary->pos, "array depths do not match.");
        return NULL;
      }
      cr->emit_var = 1;
      break;
    }
    if(isa(cl->type, &t_object) > 0 && isa(cr->type, &t_object) > 0) {
      if(isa(cl->type, cr->type) < 0) {
        err_msg(TYPE_, cl->pos, "'%s' @=> '%s' not allowed", cl->type->name, cr->type->name);
        return NULL;
      }
      cr->emit_var = 1;
      break;
    }
  case op_chuck:
    if(isa(cl->type, &t_ugen) > 0 && isa(cr->type, &t_ugen) > 0) {
      cr->emit_var = cl->emit_var = 0;
      break;
    }
    if(isa(cr->type, &t_fileio) > 0) {
      cr->emit_var = 1;
      break;
    }
    if(cr->meta != ae_meta_var && isa(cr->type, &t_function) < 0) {
      err_msg(TYPE_, cl->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cl->pos,
              "...(reason: --- right-side operand is not mutable)");
      return NULL;
    }
    cr->emit_var = 1;
    break;
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
    if(cr->meta != ae_meta_var) {
      err_msg(TYPE_, cl->pos,
              "cannot assign '%s' on types '%s' and'%s'...",
              op2str(binary->op), cl->type->name, cr->type->name);
      err_msg(TYPE_, cl->pos,
              "...(reason: --- right-side operand is not mutable)");
      return NULL;
    }
    cr->emit_var = 1;
    break;
  default:
    break;
  }

  if(binary->op == op_at_chuck) {
    if(isa(binary->lhs->type, &t_null) > 0 && isa(binary->rhs->type, &t_object) > 0)
      return cl->type;
  }

  while(cr) {
    CHECK_OO((ret = check_op(env, binary->op, cl, cr, binary)))
    cr = cr->next;
  }
  return ret;
}

static Type check_exp_cast(Env env, Exp_Cast* cast) {
#ifdef DEBUG_TYPE
  debug_msg("check", "cast expression");
#endif// check the exp
  Type t = check_exp(env, cast->exp);
  if(!t) return NULL;

  Type t2 = find_type(env, cast->type->xid);
  if(!t2) {
    m_str path = type_path(cast->type->xid);
    err_msg(TYPE_, cast->pos, "unknown type '%s' in cast expression.", path);
    free(path);
    return NULL;
  }

  if(isa(t2, &t_func_ptr) > 0) {
    if(isa(t, &t_function) < 0) {
	  err_msg(TYPE_, cast->pos, "can't cast '%s' to '%s'", t->name, t2->name);
      return NULL;

    } else {
      Value v = nspc_lookup_value(env->curr, cast->exp->d.exp_primary.d.var,  1);
      Func  f = isa(v->m_type, &t_func_ptr) > 0 ? v->m_type->func : nspc_lookup_func(env->curr, insert_symbol(v->name),  1);
      if(compat_func(t2->func->def, f->def, f->def->pos)) {
        cast->func = f;
        return t2;
      }
    }
  }
  if(isa(t, &t_float) > 0 && isa(t2, &t_int) > 0)
    return t2;
  if(isa(t, &t_null) > 0 && isa(t2, &t_object) > 0)
    return t2;
  if(isa(t, &t_object) < 0)
    return isa(t, t2) > 0 ? t2 : NULL;
  Type type = t;
  while(type) {
    if(t2 == type)
      return t2;
    type = type->parent;
  }
  err_msg(TYPE_, cast->pos, "invalid cast to '%s' from '%s'...", S_name(cast->type->xid->xid), t->name);
  return NULL;
}

static Type check_exp_postfix(Env env, Exp_Postfix* postfix) {
  Type ret, t = check_exp(env, postfix->exp);

  if(!t) return NULL;
  if(postfix->exp->meta != ae_meta_var) {
     err_msg(TYPE_, postfix->exp->pos,
     "postfix operator '%s' cannot be used on non-mutable data-type...", op2str(postfix->op));
      return NULL;
  }
  postfix->exp->emit_var = 1;
  if(!(ret = get_return_type(env, postfix->op, t, NULL)))
    err_msg(TYPE_, postfix->pos,
     "no suitable resolutation for postfix operator '%s' on type '%s'...",  op2str(postfix->op), t->name);
  return ret;
}

static Type check_exp_dur(Env env, Exp_Dur* dur) {
  Type base = check_exp(env, dur->base);
  Type unit = check_exp(env, dur->unit);
  if(!base || !unit)
    return NULL;
  if(isa(base, &t_int) < 0 && isa(base, &t_float) < 0) {
    err_msg(TYPE_, dur->base->pos,
            "invalid type '%s' in prefix of dur expression...\n"
            "    (must be of type 'int' or 'float')", base->name);
    return NULL;
  }
  if(isa(unit, &t_dur) < 0) {
    err_msg(TYPE_, dur->unit->pos,
            "invalid type '%s' in postfix of dur expression...\n"
            "    (must be of type 'dur')", unit->name);
    return NULL;
  }
  return unit;
}

static Func find_func_match_actual(Func up, Exp args, m_bool implicit, m_bool specific) {
  Exp e;
  Arg_List e1;
  m_uint count;
  Func func;
  int match = -1;
  // see if args is nil
  if(args && isa(args->type, &t_void) > 0)
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
        match = specific ? e->type == e1->type : isa(e->type, e1->type) > 0 && e->type->array_depth == e1->type->array_depth;
        if(match <= 0) {
          if(implicit && e->type->xid == t_int.xid && e1->type->xid == t_float.xid)
            e->cast_to = &t_float;
          else if(!(isa(e->type, &t_null) > 0 && isa(e1->type, &t_object) > 0)) /// Hack
            goto moveon; // type mismatch
        }
        e = e->next;
        e1 = e1->next;
        count++;
      }
      if(e1 == NULL) return func;
moveon:
      func = func->next;
    }
    if(up->up)
      up = up->up->func_ref;
    else up = NULL;
  }
  return NULL;
}

static Func find_func_match(Func up, Exp args) {
  Func func;
  if((func = find_func_match_actual(up, args, 0, 1)) ||
      (func = find_func_match_actual(up, args, 1, 1)) ||
      (func = find_func_match_actual(up, args, 0, 0)) ||
      (func = find_func_match_actual(up, args, 1, 0)));
  return func;
}

static Type_List mk_type_list(Env env, Type type) {
  m_uint i;
  Nspc nspc = type->info;
  Vector v = new_vector();
  vector_add(v, (vtype)type->name);
  while(nspc && nspc != env->curr && nspc != env->global_nspc) {
    vector_add(v, (vtype)S_name(insert_symbol((nspc->name))));
    nspc = nspc->parent;
  }
  ID_List id = NULL;
  Type_List list = NULL;
  for(i = vector_size(v); i > 0; i--)
    id = prepend_id_list((m_str)vector_at(v, i - 1), id, 0);
  list = new_type_list(id, NULL, 0);
  free_vector(v);
  return list;
}

Func find_template_match(Env env, Value v, Func m_func, Type_List types, Exp func, Exp args) {
  m_uint i;
  Func_Def base;
  Value value;
  m_uint digit = floor(log10(v->func_num_overloads + 1)) + 13;

  CHECK_OO(v)
  if(v->owner_class) {
    vector_add(env->nspc_stack, (vtype)env->curr);
    env->curr = v->owner_class->info;
    vector_add(env->class_stack, (vtype)env->class_def);
    env->class_def = v->owner_class;
    env->class_scope = 0; // should keep former value somewhere
  }
  for(i = 0; i < v->func_num_overloads + 1; i++) {
    char name[strlen(v->name) + strlen(env->curr->name) + digit + 20];
    sprintf(name, "%s<template>@%li@%s", v->name, i, env->curr->name);
    if(v->owner_class) {
      value = find_value(v->owner_class, insert_symbol(name));
    } else
      value = nspc_lookup_value(env->curr, insert_symbol(name), 1);
    base = value->func_ref->def;
    Func_Def def = new_func_def(base->func_decl, base->static_decl,
                                base->type_decl, S_name(func->d.exp_primary.d.var),
                                base->arg_list, base->code, func->pos);
    if(base->is_variadic)
      def->is_variadic = 1;
    Type_List list = types;
    ID_List base_t = base->types;
    def->is_template = 1;
    nspc_push_type(env->curr);
    while(base_t) {
      ID_List tmp = base_t->next;;
      if(!list || !list->list)
        break;
      nspc_add_type(env->curr, base_t->xid, find_type(env, list->list));
      base_t->next = tmp;
      if((list->next && !base_t->next) || // too many
        (!list->next && base_t->next)) { // not enough
        nspc_pop_type(env->curr);
        goto next;
      }
      list = list->next;
      base_t = base_t->next;
    }
    m_int ret = scan1_func_def(env, def);
    nspc_pop_type(env->curr);
    if(ret < 0)                        continue;
    if(scan2_func_def(env, def) < 0)   continue;
    if(check_func_def(env, def) < 0)   continue;
    if(!check_exp(env, func))          continue;
    if(args  && !check_exp(env, args)) continue;
    def->func->next = NULL;
    m_func = find_func_match(def->func, args);
    if(m_func) {
      if(v->owner_class) {
        env->class_def = (Type)vector_pop(env->class_stack);
        env->curr = (Nspc)vector_pop(env->nspc_stack);
      }
      m_func->is_template = 1;
      m_func->def->base = value->func_ref->def->types;
      return m_func;
    }
next:
    ;
  }
  if(v->owner_class) {
    env->class_def = (Type)vector_pop(env->class_stack);
    env->curr = (Nspc)vector_pop(env->nspc_stack);
  }
  return NULL;
}

/* static */ Type check_exp_call1(Env env, Exp exp_func, Exp args, Func *m_func, int pos) {
#ifdef DEBUG_TYPE
  debug_msg("check", "func call");
#endif
  Func func = NULL;
  Func up = NULL;
  Type f;
  Value ptr = NULL; // 20/03/17

  exp_func->type = check_exp(env, exp_func);
  f = exp_func->type;
  // primary func_ptr
  if(exp_func->exp_type == ae_exp_primary &&
      exp_func->d.exp_primary.value && !GET_FLAG(exp_func->d.exp_primary.value, ae_value_const)) {
    if(env->class_def && exp_func->d.exp_primary.value->owner_class == env->class_def) {
      err_msg(TYPE_, exp_func->pos, "can't call pointers in constructor.");
      return NULL;
    }
    ptr = exp_func->d.exp_primary.value;
  }
  /*
     else if(exp_func->exp_type == ae_exp_dot) {
     Value v = find_value(exp_func->d.exp_dot.t_base, exp_func->d.exp_dot.xid);
     if(v && v->owner_class == env->class_def) {
     err_msg(TYPE_, exp_func->pos, "can't call pointers in constructor.");
     return NULL;
     }
  //        ptr = exp_func->d.exp_primary.value;
  }
  */
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
    CHECK_OO(check_exp(env, args))
    // look for a match
    func = find_func_match(up, args);
  if(!func) {
    Value value;
    if(!f->func) {
      if(exp_func->exp_type == ae_exp_primary)
        value = nspc_lookup_value(env->curr, exp_func->d.exp_primary.d.var, 1);
      else if(exp_func->exp_type == ae_exp_dot)
        value = find_value(exp_func->d.exp_dot.t_base, exp_func->d.exp_dot.xid);
      else {
        err_msg(TYPE_, exp_func->pos, "unhandled expression type '%lu\' in template call.", exp_func->exp_type);
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
      Type_List tl[type_number];
      while(list) { // iterate through types
        Arg_List arg = value->func_ref->def->arg_list;
        Exp template_arg = args;
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
        env->current->types = tl[0];
        env->current->base = value->func_ref->def->types;
        return ret_type;
      }
      err_msg(TYPE_, exp_func->pos, "function is template. automatic type guess not fully implemented yet.\nplease provide template types. eg: '<type1, type2, ...>'"); // LCOV_EXCL_LINE
      return NULL; //LCOV_EXCL_LINE
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
    Exp e = args;
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
  if(ptr) {
    Func f = malloc(sizeof(struct Func_));
    memcpy(f, func, sizeof(struct Func_));
    f->value_ref = ptr;
    func = f;
    //up->value_ref = exp_func->d.exp_primary.value;
  }
  *m_func = func;
  return func->def->ret_type;
}

static Type check_exp_call(Env env, Exp_Func* exp_func) {
  if(exp_func->types) {
    Value v;
    if(exp_func->func->exp_type == ae_exp_primary) {
      v = nspc_lookup_value(env->curr, exp_func->func->d.exp_primary.d.var, 1);
    } else {
      Type t;
      CHECK_OO(check_exp(env, exp_func->func))
      t = exp_func->func->d.exp_dot.t_base;
      if(isa(t, &t_class) > 0)
        t = t->actual_type;
      v = find_value(t, exp_func->func->d.exp_dot.xid);
      if(!v->func_ref->def->types) {
        err_msg(TYPE_, exp_func->pos, "template call of non-template function.");
        free_type_list(exp_func->types);
        exp_func->types = NULL;
        return NULL;
      }
    }
    Func ret = find_template_match(env, v, exp_func->m_func, exp_func->types,
                                   exp_func->func, exp_func->args);
    if(!ret) {
      err_msg(TYPE_, exp_func->pos, "arguments do not match for template call");
      return NULL;
    }
    exp_func->m_func = ret;
    return ret->def->ret_type;
  }
  env->current = exp_func;
  return check_exp_call1(env, exp_func->func, exp_func->args,
                          &exp_func->m_func, exp_func->pos);
}

static Type check_exp_unary(Env env, Exp_Unary* unary) {
  Type t = NULL;

  if(unary->op != op_new && !unary->code)
    CHECK_OO((t = check_exp(env, unary->exp)))
  if(unary->code)
    CHECK_BO(check_stmt(env, unary->code))

  switch(unary->op) {
    case op_plusplus:
    case op_minusminus:
      if(unary->exp->meta != ae_meta_var) {
        err_msg(TYPE_, unary->pos,
                "prefix unary operator '%s' cannot "
                "be used on non-mutable data-types...", op2str(unary->op));
        return NULL;
      }
      unary->exp->emit_var = 1;
      break;

    case op_minus:
    case op_tilda:
    case op_exclamation:
      unary->self->meta = ae_meta_value;

      break;
    case op_spork:
      if(unary->exp && unary->exp->exp_type == ae_exp_call)
        return &t_shred;
      else if(unary->code) {
        if(env->func) {
          env->class_scope++;
          nspc_push_value(env->curr);
          int ret = check_stmt(env, unary->code);
          nspc_pop_value(env->curr);
          env->class_scope--;
          return (ret > 0) ? &t_shred : NULL;
          break;
        } else if(check_stmt(env, unary->code) < 0) {
          err_msg(TYPE_, unary->pos, "problem in evaluating sporked code"); // LCOV_EXCL_LINE
          break;                                                                // LCOV_EXCL_LINE
        }
        return &t_shred;
      } else {
        err_msg(TYPE_,  unary->pos, "only function calls can be sporked...");
        return NULL;
      }
      break;

    case op_new:
      if(!(t = find_type(env, unary->type->xid))) {
        err_msg(TYPE_,  unary->pos,  "... in 'new' expression ...");
        return NULL;
      }
      if(unary->array) {
        CHECK_BO(verify_array(unary->array))
        CHECK_OO(check_exp(env, unary->array->exp_list))
        CHECK_BO(check_exp_array_subscripts(env, unary->array->exp_list))
        t = new_array_type(env, unary->array->depth, t, env->curr);
      } else if(isa(t, &t_object) < 0) {
        err_msg(TYPE_, unary->pos,
                "cannot instantiate/(new) primitive type '%s'...", t->name);
        err_msg(TYPE_, unary->pos, "...(primitive types: 'int', 'float', 'time', 'dur')");
        return NULL;
      }
      return t;
    default: break;
  }
  if(!(t = get_return_type(env, unary->op, NULL, unary->exp->type)))
    err_msg(TYPE_, unary->pos,
      "no suitable resolution for prefix operator '%s' on type '%s...",
      op2str(unary->op), t ? t->name : "unknown");
  return t;
}

static Type check_exp_if(Env env, Exp_If* exp_if) {
#ifdef DEBUG_TYPE
  debug_msg("check", "debug exp if");
#endif
  Type cond     = check_exp(env, exp_if->cond);
  Type if_exp   = check_exp(env, exp_if->if_exp);
  Type else_exp = check_exp(env, exp_if->else_exp);
  Type ret;
  if(!cond || !if_exp || !else_exp)
    return NULL;

  if(isa(cond, &t_int) < 0 && isa(cond, &t_float) < 0) {
    err_msg(TYPE_, exp_if->pos, "Invalid type '%s' in if expression condition.", cond->name);
    return NULL;
  }
  if(!(ret = find_common_anc(if_exp, else_exp))) {
    err_msg(TYPE_, exp_if->pos,
            "incompatible types '%s' and '%s' in if expression...", if_exp->name, else_exp->name);
    return NULL;
  }
  return ret;
}

static Type check_exp_dot(Env env, Exp_Dot* member) {
#ifdef DEBUG_TYPE
  debug_msg("check", "dot member");
#endif
  Value value;
  Type  the_base;
  m_bool base_static;
  m_str str;

  member->t_base = check_exp(env, member->base);
  if(!member->t_base)
    return NULL;
  base_static = member->t_base->xid == t_class.xid;
  the_base = base_static ? member->t_base->actual_type : member->t_base;

  if(!the_base->info) {
    err_msg(TYPE_,  member->base->pos,
            "type '%s' does not have members - invalid use in dot expression of %s",
            the_base->name, S_name(member->xid));
    return NULL;
  }

  str = S_name(member->xid);
  if(!strcmp(str, "this") && base_static) {
    err_msg(TYPE_,  member->pos,
            "keyword 'this' must be associated with object instance...");
    return NULL;
  }

  if(!(value = find_value(the_base, member->xid))) {
    m_uint i, len = strlen(the_base->name + the_base->array_depth*2 +1);
    char s[len];
    memset(s, 0, len);
    strcpy(s, the_base->name);
    for(i = 0; i < the_base->array_depth; i++)
      strcat(s, "[]");
    err_msg(TYPE_,  member->base->pos,
            "class '%s' has no member '%s'", s, str);
    return NULL;
  }
  if(base_static && GET_FLAG(value, ae_value_member)) {
    err_msg(TYPE_, member->pos,
            "cannot access member '%s.%s' without object instance...",
            the_base->name, str);
    return NULL;
  }
  if(GET_FLAG(value, ae_value_enum)) // for enum
    member->self->meta = ae_meta_value;
  return value->m_type;
}

static m_bool check_stmt_typedef(Env env, Stmt_Ptr ptr) {
#ifdef DEBUG_TYPE
  debug_msg("check", "func pointer '%s'", S_name(ptr->xid));
#endif
  Type t     = nspc_lookup_type(env->curr, ptr->xid, 1);
  t->size    = SZ_INT;
  t->name    = S_name(ptr->xid);
  t->parent  = &t_func_ptr;
  nspc_add_type(env->curr, ptr->xid, t);
  //  ADD_REF(t);
  ptr->m_type = t;
  t->func = ptr->func;
  return 1;
}

static Type check_exp(Env env, Exp exp) {
  Exp curr = exp;
  while(curr) {
    curr->type = NULL;
    switch(curr->exp_type) {
    case ae_exp_primary:
      curr->type = check_exp_primary(env, &curr->d.exp_primary);
      break;
    case ae_exp_decl:
      curr->type = check_exp_decl(env, &curr->d.exp_decl);
      break;
    case ae_exp_unary:
      curr->type = check_exp_unary(env, &curr->d.exp_unary);
      break;
    case ae_exp_binary:
      curr->type = check_exp_binary(env, &curr->d.exp_binary);
      break;
    case ae_exp_postfix:
      curr->type = check_exp_postfix(env, &curr->d.exp_postfix);
      break;
    case ae_exp_dur:
      curr->type = check_exp_dur(env, &curr->d.exp_dur);
      break;
    case ae_exp_cast:
      curr->type = check_exp_cast(env, &curr->d.exp_cast);
      break;
    case ae_exp_call:
      curr->type = check_exp_call(env, &curr->d.exp_func);
      curr->d.exp_func.ret_type = curr->type;
      break;
    case ae_exp_if:
      curr->type = check_exp_if(env, &curr->d.exp_if);
      break;
    case ae_exp_dot:
      curr->type = check_exp_dot(env, &curr->d.exp_dot);
      break;
    case ae_exp_array:
      curr->type = check_exp_array(env, &curr->d.exp_array);
      break;
    }
    CHECK_OO(curr->type)
    curr = curr->next;
  }
  return exp ? exp->type : NULL;
}

static m_bool check_stmt_enum(Env env, Stmt_Enum stmt) {
  ID_List list = stmt->list;
  Value v;
  Nspc nspc = env->class_def ? env->class_def->info : env->curr;
  while(list) {
    v = nspc_lookup_value(nspc, list->xid, 0);
    if(env->class_def) { // enum in classes are static
      SET_FLAG(v, ae_value_static);
      v->offset = env->class_def->info->class_data_size;
      env->class_def->info->class_data_size += t_int.size;
    }
    list = list->next;
  }
  return 1;

}

static m_bool check_stmt_code(Env env, Stmt_Code stmt, m_bool push) {
  m_bool ret;

  env->class_scope++;
  if(push)
    nspc_push_value(env->curr);

  ret = check_stmt_list(env, stmt->stmt_list);

  if(push)
    nspc_pop_value(env->curr);
  env->class_scope--;

  return ret;
}

static m_bool check_stmt_while(Env env, Stmt_While stmt) {
  CHECK_OB(check_exp(env, stmt->cond))
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    err_msg(TYPE_,  stmt->cond->pos,
            "invalid type '%s' in while condition", stmt->cond->type->name);
    return -1;
  }
  vector_add(env->breaks, (vtype)stmt->self);
  vector_add(env->conts, (vtype)stmt->self);
  CHECK_BB(check_stmt(env, stmt->body))
  vector_pop(env->breaks);
  vector_pop(env->conts);
  return 1;
}

static m_bool check_stmt_until(Env env, Stmt_Until stmt) {
  CHECK_OB(check_exp(env, stmt->cond))
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    err_msg(TYPE_,  stmt->cond->pos,
            "invalid type '%s' in until condition", stmt->cond->type->name);
    return -1;
  }
  vector_add(env->breaks, (vtype)stmt->self);
  CHECK_BB(check_stmt(env, stmt->body))
  vector_pop(env->breaks);
  return 1;
}

static m_bool check_stmt_for(Env env, Stmt_For stmt) {
  CHECK_BB(check_stmt(env, stmt->c1))
  CHECK_BB(check_stmt(env, stmt->c2))
  if(!stmt->c2 || !stmt->c2->d.stmt_exp.val) {
    err_msg(EMIT_, stmt->pos, "empty for loop condition...");
    err_msg(EMIT_, stmt->pos, "...(note: explicitly use 'true' if it's the intent)");
    err_msg(EMIT_, stmt->pos, "...(e.g., 'for(; true;){ /*...*/ }')");
    return -1;
  }
  switch(stmt->c2->d.stmt_exp.val->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    err_msg(EMIT_,  stmt->c2->d.stmt_exp.pos,
            "invalid type '%s' in for condition", stmt->c2->d.stmt_exp.val->type->name);
    return -1;
  }

  if(stmt->c3)
    CHECK_OB(check_exp(env, stmt->c3))
    vector_add(env->breaks, (vtype)stmt->self);
  CHECK_BB(check_stmt(env, stmt->body))
  vector_pop(env->breaks);
  return 1;
}

static m_bool check_stmt_loop(Env env, Stmt_Loop stmt) {
  Type type = check_exp(env, stmt->cond);

  CHECK_OB(type)
  if(isa(type, &t_float) > 0)
    stmt->cond->cast_to = &t_int;
  else if(isa(type, &t_int) < 0) { // must be int
    err_msg(TYPE_, stmt->pos,
            "loop * conditional must be of type 'int'...");
    return -1;
  }
  vector_add(env->breaks, (vtype)stmt->self);
  CHECK_BB(check_stmt(env, stmt->body))
  vector_pop(env->breaks);
  return 1;
}

static m_bool check_stmt_if(Env env, Stmt_If stmt) {
  CHECK_OB(check_exp(env, stmt->cond))
  switch(stmt->cond->type->xid) {
  case te_int:
  case te_float:
  case te_dur:
  case te_time:
    break;

  default:
    if(isa(stmt->cond->type, &t_object) > 0) // added 12/09/16
      break;
    err_msg(TYPE_, stmt->cond->pos,
            "invalid type '%s' in if condition", stmt->cond->type->name);
    return -1;
  }
  CHECK_BB(check_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(check_stmt(env, stmt->else_body))
    return 1;
}

static m_bool check_stmt_return(Env env, Stmt_Return stmt) {
  Type ret_type = NULL;
  if(!env->func) {
    err_msg(TYPE_, stmt->pos, "'return' statement found outside function definition");
    return -1;
  }
  if(stmt->val) {
    CHECK_OB((ret_type = check_exp(env, stmt->val)))
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

static m_bool check_stmt_continue(Env env, Stmt_Continue cont) {
  if(!vector_size(env->breaks)) {
    err_msg(TYPE_,  cont->pos,
      "'continue' found outside of for/while/until...");
    return -1;
  }
  return 1;
}

static m_bool check_stmt_break(Env env, Stmt_Break cont) {
  if(!vector_size(env->breaks)) {
    err_msg(TYPE_,  cont->pos,
      "'break' found outside of for/while/until...");
    return -1;
  }
  return 1;
}

static m_bool check_stmt_switch(Env env, Stmt_Switch a) {
  Type t = check_exp(env, a->val);
  if(!t || t->xid !=  t_int.xid) {
    err_msg(TYPE_, a->pos, "invalid type '%s' in switch expression. should be 'int'", t ? t->name : "unknown");
    return -1;
  }
  vector_add(env->breaks, (vtype)a->self);
  if(check_stmt(env, a->stmt) < 0) {
    err_msg(TYPE_, a->val->pos, "\t... in switch statement");
    return -1;
  }
  vector_pop(env->breaks);
  return 1;
}

static m_bool check_stmt_case(Env env, Stmt_Case stmt) {
  Type t = check_exp(env, stmt->val);
  if(!t || t->xid !=  t_int.xid) {
    err_msg(TYPE_, stmt->pos, "invalid type '%s' case expression. should be 'int'", t ? t->name : "unknown");
    return -1;
  }
  return 1;
}

static m_bool check_stmt_gotolabel(Env env, Stmt_Goto_Label stmt) {
  Map m;
  m_uint* key = env->class_def && !env->func ? (m_uint*)env->class_def : (m_uint*)env->func;
  Stmt_Goto_Label ref;
  if(stmt->is_label)
    return 1;
  m = (Map)map_get(env->curr->label, (vtype)key);
  if(!m) {
    err_msg(TYPE_, stmt->pos, "label '%s' used but not defined", S_name(stmt->name));
    return -1;
  }
  ref = (Stmt_Goto_Label)map_get(m, (vtype)stmt->name);
  if(!ref) {
    err_msg(TYPE_, stmt->pos, "label '%s' used but not defined", S_name(stmt->name));
    m_uint i;
    for(i = 0; i < map_size(m); i++) {
      ref = (Stmt_Goto_Label)map_at(m, i);
      free_vector(ref->data.v);
    }
    return -1;
  }
  vector_add(ref->data.v, (vtype)stmt);
  return 1;
}

static m_bool check_stmt_union(Env env, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  if(env->class_def)  {
    stmt->o = env->class_def->obj_size;
  }
  while(l) {
    CHECK_OB(check_exp(env, l->self))
    if(l->self->type->size > stmt->s)
      stmt->s = l->self->type->size;
    l = l->next;
  }
  return 1;
}

static m_bool check_stmt(Env env, Stmt stmt) {
#ifdef DEBUG_TYPE
  debug_msg("check", "stmt");
#endif
  m_bool ret = 1;
  if(!stmt)
    return 1;
  switch(stmt->type) {
  case ae_stmt_exp:
    if(stmt->d.stmt_exp.val)
      ret = (check_exp(env, stmt->d.stmt_exp.val) ? 1 : -1);
    break;
  case ae_stmt_code:
    SCOPE(ret = check_stmt_code(env, &stmt->d.stmt_code, 1))
    break;
  case ae_stmt_return:
    ret = check_stmt_return(env, &stmt->d.stmt_return);
    break;
  case ae_stmt_break:
    ret = check_stmt_break(env, &stmt->d.stmt_break);
    break;
  case ae_stmt_continue:
    ret = check_stmt_continue(env, &stmt->d.stmt_continue);
    break;
  case ae_stmt_if:
    NSPC(ret = check_stmt_if(env, &stmt->d.stmt_if))
    break;
  case ae_stmt_while:
    NSPC(ret = check_stmt_while(env, &stmt->d.stmt_while))
    break;
  case ae_stmt_until:
    NSPC(ret = check_stmt_until(env, &stmt->d.stmt_until))
    break;
  case ae_stmt_for:
    NSPC(ret = check_stmt_for(env, &stmt->d.stmt_for))
    break;
  case ae_stmt_loop:
    NSPC(ret = check_stmt_loop(env, &stmt->d.stmt_loop))
    break;
  case ae_stmt_switch:
    NSPC(ret = check_stmt_switch(env, &stmt->d.stmt_switch))
    break;
  case ae_stmt_case:
    ret = check_stmt_case(env, &stmt->d.stmt_case);
    break;
  case ae_stmt_enum:
    ret = check_stmt_enum(env, &stmt->d.stmt_enum);
    break;
  case ae_stmt_gotolabel:
    ret = check_stmt_gotolabel(env, &stmt->d.stmt_gotolabel);
    break;
  case ae_stmt_funcptr:
    ret = check_stmt_typedef(env, &stmt->d.stmt_ptr);
    break;
  case ae_stmt_union:
    ret = check_stmt_union(env, &stmt->d.stmt_union);
    break;
  }
  return ret;
}

static m_bool check_stmt_list(Env env, Stmt_List list) {
#ifdef DEBUG_TYPE
  debug_msg("check", "statement list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(check_stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

m_bool check_func_def(Env env, Func_Def f) {
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
  m_str func_name;
  m_uint count = 1;

  if(f->types) // templating, check at call time
    return 1;
  func = f->func;
  value = func->value_ref;

  if(env->class_def)
    override = find_value(env->class_def->parent, f->name);
  else if(value->func_num_overloads) {
    m_uint i, j;
    m_uint digit = floor(log10(value->func_num_overloads)) + 3; // add two '@'
    if(!f->types)
      for(i = 0; i <= value->func_num_overloads; i++) {
        char name[strlen(S_name(f->name)) + strlen(env->curr->name) + digit];
        sprintf(name, "%s@%li@%s", S_name(f->name), i, env->curr->name);
        Func f1 = nspc_lookup_func(env->curr, insert_symbol(name), -1);
        for(j = 1; j <= value->func_num_overloads; j++) {
          if(i != j) {
            sprintf(name, "%s@%li@%s", S_name(f->name), j, env->curr->name);
            Func f2 = nspc_lookup_func(env->curr, insert_symbol(name), -1);
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
      return -1;
    }
  }
  if(override)
    func->up = override;
  if(env->class_def) {
    parent = env->class_def->parent;
    while(parent && !parent_match) {
      if((v = find_value(env->class_def->parent, f->name))) {
        parent_func = v->func_ref;
        while(parent_func && !parent_match) {
          if(compat_func(f, parent_func->def, f->pos) < 0) {
            parent_func = parent_func->next;
            continue;
          }
          if(parent_func->def->static_decl == ae_key_static) {
            err_msg(TYPE_, f->pos, "function '%s.%s' resembles '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name), v->owner_class->name, S_name(f->name));
            err_msg(TYPE_, f->pos, "...(reason: '%s.%s' is declared as 'static')",
                    v->owner_class->name, S_name(f->name));
            return -1;
          }
          if(f->static_decl == ae_key_static) {
            err_msg(TYPE_, f->pos, "function '%s.%s' resembles '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name), v->owner_class->name, S_name(f->name));
            err_msg(TYPE_, f->pos, "...(reason: '%s.%s' is declared as 'static')",
                    env->class_def->name, S_name(f->name));
            return -1;
          }
          if(isa(f->ret_type, parent_func->def->ret_type) < 0) {
            err_msg(TYPE_, f->pos, "function signatures differ in return type...");
            err_msg(TYPE_, f->pos, "function '%s.%s' matches '%s.%s' but cannot override...",
                    env->class_def->name, S_name(f->name), v->owner_class->name, S_name(f->name));
            return -1;
          }
          parent_match = 1;
          func->vt_index = parent_func->vt_index;
          vector_set(env->curr->obj_v_table, func->vt_index, (vtype)func);
          free(func->name);
          func_name = strdup(parent_func->name);
          func->name = func_name;
          value->name = func_name;
        }
      }
      parent = parent->parent;
    }
  }
  if(func->is_member && !parent_match) {
    func->vt_index = vector_size(env->curr->obj_v_table);
    vector_add(env->curr->obj_v_table, (vtype)func);
  }
  env->func = func;
  nspc_push_value(env->curr);
  arg_list = f->arg_list;
  while(arg_list) {
    v = arg_list->var_decl->value;
    if(nspc_lookup_value(env->curr, arg_list->var_decl->xid, 0)) {
      err_msg(TYPE_, arg_list->pos, "argument %i '%s' is already defined in this scope",
              count, S_name(arg_list->var_decl->xid));
      err_msg(TYPE_, arg_list->pos, "in function '%s':", S_name(f->name));
      goto error;
    }
    SET_FLAG(v, ae_value_checked);
    nspc_add_value(env->curr, arg_list->var_decl->xid, v);
    count++;
    arg_list = arg_list->next;
  }

  if(f->is_variadic) {
    vararg = new_value(&t_vararg, "vararg");
    SET_FLAG(vararg, ae_value_checked);
    nspc_add_value(env->curr, insert_symbol("vararg"), vararg);
  }
  if(f->code && check_stmt_code(env, &f->code->d.stmt_code, 0) < 0) {
    err_msg(TYPE_, f->type_decl->pos, "...in function '%s'", S_name(f->name));
    goto error;
  }

  if(f->is_variadic)
    REM_REF(vararg);
  if(f->s_type == ae_func_builtin)
    func->code->stack_depth = f->stack_depth;
  nspc_pop_value(env->curr);
  env->func = NULL;
  return 1;

error:
  nspc_pop_value(env->curr);
  env->func = NULL;
  return -1;
}

static m_bool check_class_def(Env env, Class_Def class_def) {
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
                "undefined parent class '%s' in definition of class '%s'", path, S_name(class_def->name->xid));
        free(path);
        return -1;
      }
      if(isprim(t_parent) > 0) {
        err_msg(TYPE_, class_def->ext->pos,
                "cannot extend primitive type '%s'", t_parent->name);
        err_msg(TYPE_, class_def->ext->pos,
                "...(note: primitives types are 'int', 'float', 'time', and 'dur')");
        return -1;
      }
      if(!t_parent->is_complete) {
        err_msg(TYPE_, class_def->ext->pos,
                "cannot extend incomplete type '%s'", t_parent->name);
        err_msg(TYPE_, class_def->ext->pos,
                "...(note: the parent's declaration must preceed child's)");
        return -1;
      }
    }
  }

  if(!t_parent)
    t_parent = &t_object;
  the_class = class_def->type;
  the_class->parent = t_parent;
  the_class->info->offset = t_parent->obj_size;
  free_vector(the_class->info->obj_v_table);
  the_class->info->obj_v_table = vector_copy(t_parent->info->obj_v_table);
  vector_add(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_add(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body && ret > 0) {
    switch(body->section->type) {
    case ae_section_stmt:
      env->class_def->has_constructor |= (body->section->d.stmt_list->stmt != NULL);
      ret = check_stmt_list(env, body->section->d.stmt_list);
      break;

    case ae_section_func:
      env->class_def->is_complete = 1;
      ret = check_func_def(env, body->section->d.func_def);
      env->class_def->is_complete = 0;
      break;

    case ae_section_class:
      ret = check_class_def(env, body->section->d.class_def);
      break;
    }
    body = body->next;
  }
  env->class_def = (Type)vector_pop(env->class_stack);
  env->curr = (Nspc)vector_pop(env->nspc_stack);

  if(ret > 0) {
    the_class->obj_size = the_class->info->offset;
    the_class->is_complete = 1;
  }
  return ret;
}

static m_bool check_ast(Env env, Ast ast) {
#ifdef DEBUG_TYPE
  debug_msg("type", "context");
#endif
  Ast prog = ast;
  while(prog) {
    switch(prog->section->type) {
    case ae_section_stmt:
      CHECK_BB(check_stmt_list(env, prog->section->d.stmt_list))
      break;
    case ae_section_func:
      CHECK_BB(check_func_def(env, prog->section->d.func_def))
      break;
    case ae_section_class:
      CHECK_BB(check_class_def(env, prog->section->d.class_def))
      break;
    }
    prog = prog->next;
  }
  return 1;
}

m_bool type_engine_check_prog(Env env, Ast ast, m_str filename) {
  m_bool ret;
  Context context = new_context(ast, filename);
  env_reset(env);
  CHECK_BB(load_context(context, env))
  if((ret = scan0_Ast(env, ast)) < 0) goto cleanup;
  if((ret = scan1_ast(env, ast)) < 0) goto cleanup;
  if((ret = scan2_ast(env, ast)) < 0) goto cleanup;
  if((ret = check_ast(env, ast)) < 0) goto cleanup;
cleanup:
  if(ret > 0) {
    nspc_commit(env->global_nspc);
    map_set(env->known_ctx, (vtype)insert_symbol(context->filename), (vtype)context);
  } else {
    //    nspc_rollback(env->global_nspc);
  }
  CHECK_BB(unload_context(context, env)) // no real need to check that
  if(ret < 0) {
    free_ast(ast);
    REM_REF(context);
    free(filename);
  }
  return ret;
}

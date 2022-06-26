#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"
#include "gwion.h"
#include "clean.h"
#include "object.h"
#include "instr.h"
#include "operator.h"
#include "import.h"

ANN static inline m_bool freeable(const Type a) {
  return tflag(a, tflag_tmpl) || GET_FLAG(a, global);
}

ANN void free_type(const Type a, struct Gwion_ *const gwion) {
  if (freeable(a)) {
    if (tflag(a, tflag_udef))
      free_union_def(gwion->mp, a->info->udef);
    else if (tflag(a, tflag_cdef))
      class_def_cleaner(gwion, a->info->cdef);
  }
//  if (tflag(a, tflag_cdef) && a->info->parent)
//    type_remref(a->info->parent, gwion);
  if (a->effects.ptr) vector_release(&a->effects);
  if (a->nspc) nspc_remref(a->nspc, gwion);
  if (a->info->tuple) free_tupleform(a->info->tuple, gwion);
  mp_free(gwion->mp, TypeInfo, a->info);
  mp_free(gwion->mp, Type, a);
}

Type new_type(MemPool p, const m_str name, const Type parent) {
  const Type type    = mp_calloc(p, Type);
  type->name         = name;
  type->info         = mp_calloc(p, TypeInfo);
  type->info->parent = parent;
  if (parent) type->size = parent->size;
  type->ref = 1;
  return type;
}

ANN Type type_copy(MemPool p, const Type type) {
  const Type a   = new_type(p, type->name, type->info->parent);
  a->size        = type->size;
  a->array_depth = type->array_depth;
  a->info->gack  = type->info->gack;
  return a;
}

ANN m_bool isa(const restrict Type var, const restrict Type parent) {
  return (var == parent)     ? GW_OK
         : var->info->parent ? isa(var->info->parent, parent)
                             : GW_ERROR;
}

ANN Type find_common_anc(const restrict Type lhs, const restrict Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
}

#define describe_find(name, t)                                                 \
  ANN t find_##name(const Type type, const Symbol xid) {                       \
    if (type->nspc) {                                                          \
      const t val = nspc_lookup_##name##2(type->nspc, xid);                    \
      if (val) return val;                                                     \
    }                                                                          \
    return type->info->parent ? find_##name(type->info->parent, xid) : NULL;   \
  }
describe_find(value, Value)
    // describe_find(func,  Func)

ANN Type array_base(Type type) {
  const Type t = typedef_base(type);
  return t->array_depth ? array_base(t->info->base_type) : t;
}

ANN /*static */ Symbol array_sym(const Env env, const Type src,
                                 const m_uint depth) {
  if (src->array_depth == depth) return insert_symbol(src->name);
  const m_uint total_depth = src->array_depth + depth;
  const Type   t           = array_base_simple(src);
  size_t       len         = strlen(t->name);
  char         name[len + 2 * total_depth + 1];
  strcpy(name, t->name);
  m_uint i = total_depth + 1;
  while (--i) {
    strcpy(name + len, "[]");
    len += 2;
  }
  return insert_symbol(name);
}

ANN Type array_type(const Env env, const Type src, const m_uint depth) {
  const Symbol sym  = array_sym(env, src, depth);
  const Type   type = nspc_lookup_type1(src->info->value->from->owner, sym);
  if (type) return type;
  const size_t tdepth     = depth + src->array_depth;
  const Type   base       = tdepth > 1 ? array_type(env, src, tdepth - 1) : src;
  struct TemplateScan ts  = {.t = base, /*.td=td*/ };
  struct Op_Import    opi = {.op   = insert_symbol("@scan"),
                          .lhs  = env->gwion->type[et_array],
                          .data = (uintptr_t)&ts};
  return op_check(env, &opi);
}

ANN m_bool type_ref(Type t) {
  do {
    if (tflag(t, tflag_empty)) return GW_OK;
    if (tflag(t, tflag_typedef) && tflag(t, tflag_cdef)) {
      if (t->info->cdef->base.ext && t->info->cdef->base.ext->array) {
        if (!t->info->cdef->base.ext->array->exp)
          return GW_OK;
        else {
          const Type type = t->info->parent->info->base_type;
          if (tflag(type, tflag_empty)) return GW_OK;
        }
      }
    }
  } while ((t = t->info->parent));
  return 0;
}

ANN m_uint get_depth(const Type type) {
  m_uint depth = 0;
  Type   t     = type;
  do {
    if (t->array_depth) {
      depth += t->array_depth;
      t = t->info->base_type;
    } else
      t = t->info->parent;
  } while (t);
  return depth;
}

ANN bool is_func(const struct Gwion_ *gwion, const Type t) {
  return isa(actual_type(gwion, t), gwion->type[et_function]) > 0;
}

ANN inline bool is_class(const struct Gwion_ *gwion, const Type t) {
//  return isa(t, gwion->type[et_class]) > 0;
  return t->info->parent ==  gwion->type[et_class];
}

ANN Type actual_type(const struct Gwion_ *gwion, const Type t) {
  return is_class(gwion, t) ? t->info->base_type : t;
}

ANN void inherit(const Type t) {
  const Nspc nspc = t->nspc, parent = t->info->parent->nspc;
  if (!nspc || !parent) return;
  nspc->offset = parent->offset;
  if (parent->vtable.ptr) vector_copy2(&parent->vtable, &nspc->vtable);
}

ANN bool from_global_nspc(const Env env, const Nspc nspc) {
  Nspc global = env->global_nspc;
  while(global) {
    if (nspc == global)
      return true;
    global = global->parent;
  }
  return false;
}

ANN bool type_global(const Env env, Type t) {
  do if(from_global_nspc(env, t->info->value->from->owner)) return true;
  while((t = t->info->value->from->owner_class));
  return false;
}

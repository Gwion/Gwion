#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

static m_str const special_name[] = { "~nonnull", "~force" };
#define SPECIAL_LEN strlen(special_name[0]) + strlen(special_name[1])
static const ae_flag special_flag[] = { ae_flag_nonnull, ae_flag_force };

typedef struct {
  const Type    type;
  Symbol  name;
  ae_flag flag;
  uint st_type;
} SpecialType;


ANN static Type specialtype_create(const Env env, const SpecialType *s) {
  const Type t = type_copy(env->gwion->mp, s->type);
  if(t->nspc)
    ADD_REF(t->nspc)
  t->name = s_name(s->name);
  t->flag = s->type->flag | s->flag;
  t->e->parent = s->type;
  nspc_add_type_front(s->type->e->owner, s->name, t);
  mk_class(env, t);
  return t;
}

static inline int get_flag(const Type t, const ae_flag flag) {
  return (t->flag & flag) == flag;
}


ANN static void specialtype_flag(SpecialType *s, m_str c, const uint i) {
  const ae_flag flag = special_flag[i];
  if(i == s->st_type || get_flag(s->type, flag)) {
    strcat(c, special_name[i]);
    s->flag |= flag;
  }
}

ANN static void specialtype_init(SymTable *st, SpecialType *s) {
  const size_t sz = strlen(s->type->name);
  char c[sz + SPECIAL_LEN + 1];
  strcpy(c, s->type->name);
  m_str flagged = strchr(c, '~');
  if(flagged)
    *flagged = '\0';
  specialtype_flag(s, c, 0);
  specialtype_flag(s, c, 1);
  s->name = insert_symbol(st, c);
}

ANN Type special_type(const Env env, const Type t, const uint st_type) {
  SpecialType s = { .type=t, .st_type=st_type };
  specialtype_init(env->gwion->st, &s);
  return nspc_lookup_type1(t->e->owner, s.name) ?:
    specialtype_create(env, &s);
}


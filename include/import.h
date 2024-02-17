#ifndef __IMPORT
#define __IMPORT

typedef void (*f_xtor)(const M_Object, m_bit *const, const VM_Shred);
typedef void (*f_mfun)(const M_Object, m_bit *const, const VM_Shred);
typedef void (*f_sfun)(const m_bit *,  m_bit *const, const VM_Shred);
typedef void (*f_gack)(const Type,     m_bit *const, const VM_Shred);
typedef void (*f_xfun)();
typedef struct Gwi_ *Gwi;

#define MFUN(a)                                                                \
  ANN void a(const M_Object o NUSED, m_bit *const RETURN NUSED,                \
             const VM_Shred shred NUSED)
#define SFUN(a)                                                                \
  ANN void a(const M_Object o NUSED, m_bit *const RETURN NUSED,                \
             const VM_Shred shred NUSED)
#define CTOR(a)                                                                \
  ANN void a(const M_Object o NUSED, m_bit *const _ NUSED,                     \
             const VM_Shred shred NUSED)
#define DTOR(a)                                                                \
  ANN void a(const M_Object o NUSED, m_bit *const _ NUSED,                     \
             const VM_Shred shred NUSED)
#define GACK(a)                                                                \
  ANN2(2)                                                                      \
  void a(const Type t NUSED, m_bit *const VALUE NUSED,                         \
         const VM_Shred shred NUSED)
#define OP_CHECK(a) ANN Type a(const Env env NUSED, void *data NUSED)
#define OP_EMIT(a)  ANN bool a(const Emitter emit NUSED, void *data NUSED)
#ifdef GWION_BUILTIN
#define GWI_BB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, __FILE__, __LINE__);                                      \
    (void)(a);                                                                 \
  }
#define GWI_B(a)                                                               \
  {                                                                            \
    gwi_set_loc(gwi, __FILE__, __LINE__);                                      \
    (void)(a);                                                                 \
  }
#define GWI_OB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, __FILE__, __LINE__);                                      \
    (void)(a);                                                                 \
  }
#define GWION_IMPORT(a) ANN bool gwimport_##a(const Gwi gwi)
#else
#define GWI_BB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, (m_str)__FILE__, __LINE__);                               \
    CHECK_B(a);                                                                \
  }
#define GWI_B(a)                                                               \
  {                                                                            \
    gwi_set_loc(gwi, (m_str)__FILE__, __LINE__);                               \
    CHECK_B(a);                                                                \
  }
#define GWI_OB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, (m_str)__FILE__, __LINE__);                               \
    CHECK_B(a);                                                                \
  }
#define GWION_IMPORT(a) ANN bool gwimport_##a(const Gwi gwi)
#endif
#define ALLOC_PTR(p, a, b, c)                                                  \
  b *a = (b *)_mp_calloc(p, sizeof(b));                                        \
  *a   = (b)c
#define _CHECK_OP(op, check, func)                                             \
  GWI_B(gwi_oper_add(gwi, opck_##check))                                       \
  GWI_B(gwi_oper_end(gwi, op, func))

#include "import/checker.h"
#include "import/type.h"
#include "import/cdef.h"
#include "import/item.h"
#include "import/tdef.h"
#include "import/udef.h"
#include "import/edef.h"
#include "import/fdef.h"
#include "import/oper.h"
#include "import/special.h"
#include "import/internals.h"

OP_CHECK(opck_const_rhs);
OP_CHECK(opck_unary_meta);
OP_CHECK(opck_unary_meta2);
OP_CHECK(opck_unary);
OP_CHECK(opck_post);
OP_CHECK(opck_rassign);
OP_CHECK(opck_basic_cast);
OP_CHECK(opck_similar_cast);
OP_CHECK(opck_usr_implicit);
OP_EMIT(opem_contract_similar);
OP_CHECK(opck_new);
OP_EMIT(opem_new);

ANN
static inline M_Object new_object_str(const Gwion gwion, const m_str str) {
  loc_t loc = {};
  DECL_OO(const Type, t, = str2type(gwion, str, loc));
  return new_object(gwion->mp, t);
}

static inline Type dict_type(const Gwion gwion, const Type key, const Type val, const loc_t loc) {
   char c[1024];
   const m_str keyname = type2str(gwion, key, loc);
   const m_str valname = type2str(gwion, key, loc);
   sprintf(c, "Dict:[%s,%s]", key->name, val->name);
   free_mstr(gwion->mp, keyname);
   free_mstr(gwion->mp, valname);
   return str2type(gwion, c, loc);
}
#endif

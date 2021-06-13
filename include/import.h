#ifndef __IMPORT
#define __IMPORT

typedef void (*f_xtor)(const M_Object, const m_bit *, const VM_Shred);
typedef void (*f_mfun)(const M_Object, const m_bit *, const VM_Shred);
typedef void (*f_sfun)(const m_bit *, const m_bit *, const VM_Shred);
typedef void (*f_gack)(const Type, const m_bit *, const VM_Shred);
typedef void (*f_xfun)();
typedef struct Gwi_ *Gwi;

#define MFUN(a)                                                                \
  ANN void a(const M_Object o NUSED, const m_bit *RETURN NUSED,                \
             const VM_Shred shred NUSED)
#define SFUN(a)                                                                \
  ANN void a(const M_Object o NUSED, const m_bit *RETURN NUSED,                \
             const VM_Shred shred NUSED)
#define CTOR(a)                                                                \
  ANN void a(const M_Object o NUSED, const m_bit *_ NUSED,                     \
             const VM_Shred shred NUSED)
#define DTOR(a)                                                                \
  ANN void a(const M_Object o NUSED, const m_bit *_ NUSED,                     \
             const VM_Shred shred NUSED)
#define GACK(a)                                                                \
  ANN2(2)                                                                      \
  void a(const Type t NUSED, const m_bit *VALUE NUSED,                         \
         const VM_Shred shred NUSED)
#define OP_CHECK(a) ANN Type a(const Env env NUSED, void *data NUSED)
#define OP_EMIT(a)  ANN m_bool a(const Emitter emit NUSED, void *data NUSED)
#ifdef GWION_BUILTIN
#define GWI_BB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, __FILE__, __LINE__);                                      \
    (void)(a);                                                                 \
  }
#define GWI_OB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, __FILE__, __LINE__);                                      \
    (void)(a);                                                                 \
  }
#define GWION_IMPORT(a) ANN m_bool import_##a(const Gwi gwi)
#else
#define GWI_BB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, (m_str)__FILE__, __LINE__);                               \
    CHECK_BB(a);                                                               \
  }
#define GWI_OB(a)                                                              \
  {                                                                            \
    gwi_set_loc(gwi, (m_str)__FILE__, __LINE__);                               \
    CHECK_OB(a);                                                               \
  }
#define GWION_IMPORT(a) ANN m_bool import(const Gwi gwi)
#endif
#define ALLOC_PTR(p, a, b, c)                                                  \
  b *a = (b *)_mp_calloc(p, sizeof(b));                                        \
  *a   = (b)c
#define _CHECK_OP(op, check, func)                                             \
  GWI_BB(gwi_oper_add(gwi, opck_##check))                                      \
  GWI_BB(gwi_oper_end(gwi, op, func))

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
OP_CHECK(opck_usr_implicit);
OP_CHECK(opck_new);
OP_EMIT(opem_new);

ANN2(1, 3)
static inline M_Object new_object_str(const Gwion gwion, const VM_Shred shred,
                                      const m_str str) {
  struct loc_t_ loc = {};
  DECL_OO(const Type, t, = str2type(gwion, str, loc));
  return new_object(gwion->mp, shred, t);
}
#endif

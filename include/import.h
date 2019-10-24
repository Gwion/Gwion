#ifndef __IMPORT
#define __IMPORT

#define DLARG_MAX 6

typedef void (*f_xtor)(const M_Object, const m_bit*, const VM_Shred);
typedef void (*f_mfun)(const M_Object, const m_bit*, const VM_Shred);
typedef void (*f_sfun)(const m_bit*, const m_bit*, const VM_Shred);
typedef void (*f_gack)(const Type, const m_bit*, const VM_Shred);
typedef void (*f_xfun)();
typedef struct Gwi_* Gwi;

#define MFUN(a) ANN void a(const M_Object o NUSED, const m_bit* RETURN NUSED, const VM_Shred shred NUSED)
#define SFUN(a) ANN void a(const M_Object o NUSED, const m_bit* RETURN NUSED, const VM_Shred shred NUSED)
#define CTOR(a) ANN void a(const M_Object o NUSED, const m_bit* _ NUSED, const VM_Shred shred NUSED)
#define DTOR(a) ANN void a(const M_Object o NUSED, const m_bit* _ NUSED, const VM_Shred shred NUSED)
#define GACK(a) ANN2(2) void a(const Type t NUSED, const m_bit* VALUE NUSED, const VM_Shred shred NUSED)
#define OP_CHECK(a) ANN Type a(const Env env NUSED, void* data NUSED, m_bool* mut NUSED)
#define OP_EMIT(a)  ANN Instr a(const Emitter emit NUSED, void* data NUSED)
#ifdef GWION_BUILTIN
#define GWI_BB(a) { gwi_set_loc(gwi, __FILE__, __LINE__); (void)(a); }
#define GWI_OB(a) { gwi_set_loc(gwi, __FILE__, __LINE__); (void)(a); }
#define GWION_IMPORT(a) ANN m_bool import_##a(const Gwi gwi)
#else
#define GWI_BB(a) { gwi_set_loc(gwi, (m_str)__FILE__, __LINE__); CHECK_BB(a) }
#define GWI_OB(a) { gwi_set_loc(gwi, (m_str)__FILE__, __LINE__); CHECK_OB(a) }
#define GWION_IMPORT(a) ANN m_bool import(const Gwi gwi)
#endif
#define ALLOC_PTR(p, a, b, c) b* a = (b*)_mp_calloc(p, sizeof(b)); *a = (b)c
#define _CHECK_OP(op, check, func)\
    GWI_BB(gwi_oper_add(gwi, opck_##check))\
    GWI_BB(gwi_oper_end(gwi, op, func))
#define GWI_LOC new_loc(gwi->gwion->mp, __LINE__)


ANN VM* gwi_vm(const Gwi);
ANN2(1,2) ANEW Type gwi_mk_type(const Gwi, const m_str, const m_uint, const m_str);
ANN m_int gwi_add_type(const Gwi gwi, Type type);
ANN m_int gwi_set_global_type(const Gwi gwi, const Type type, const type_enum te);
ANN m_bool gwi_gack(const Gwi gwi, const Type type, const f_gack d);
ANN2(1,2)m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor);
ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td);
ANN m_int gwi_class_end(const Gwi gwi);

#include "import/item.h"

ANN m_int gwi_fptr_ini(const Gwi gwi, const m_str __restrict__ type, const __restrict__ m_str name);
ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag);

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name);
ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag);

ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str *name);
ANN m_int gwi_tmpl_end(const Gwi gwi);

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str type, const m_str name);
ANN m_int gwi_union_add(const Gwi gwi, const __restrict__ m_str type, const __restrict__ m_str name);
ANN Type gwi_union_end(const Gwi gwi, const ae_flag flag);

ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type);
ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint value);
ANN Type gwi_enum_end(const Gwi gwi);

ANN m_int gwi_func_ini(const Gwi gwi, const __restrict__ m_str type, const __restrict__ m_str name, const f_xfun addr);
ANN m_int gwi_func_arg(const Gwi gwi, const __restrict__ m_str t, const __restrict__ m_str n);
ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag);

ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const m_str l, const m_str r, const m_str t);
ANN m_int gwi_oper_add(const Gwi gwi, const opck);
ANN m_int gwi_oper_emi(const Gwi gwi, const opem);
ANN2(1) m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f);
ANN m_int gwi_oper_cond(const Gwi, const m_str,  const f_instr, const f_instr);
ANN Type_Decl* str2decl(const Env, const m_str, m_uint* depth, const loc_t);

OP_CHECK(opck_const_rhs);
OP_CHECK(opck_unary_meta);
OP_CHECK(opck_unary_meta2);
OP_CHECK(opck_unary);
OP_CHECK(opck_post);
OP_CHECK(opck_rassign);
OP_CHECK(opck_rhs_emit_var);
OP_CHECK(opck_basic_cast);
OP_CHECK(opck_usr_implicit);
OP_CHECK(opck_new);
OP_EMIT(opem_basic_cast);
OP_EMIT(opem_new);

ANN Type_List str2tl(const Env env, const m_str s, const loc_t);

#define FREEARG(a) ANN void a(Instr instr  NUSED, void *gwion NUSED)
typedef void (*f_freearg)(Instr, void*);
ANN void register_freearg(const Gwi, const f_instr, const f_freearg);
ANN void gwi_reserve(const Gwi, const m_str);
typedef struct SpecialId_* SpecialId;
ANN void gwi_specialid(const Gwi gwi, const m_str id, const SpecialId);
ANN void gwi_set_loc(const Gwi, const m_str, const uint);
#endif

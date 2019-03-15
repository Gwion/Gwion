#ifndef __IMPORT
#define __IMPORT
#define DLARG_MAX 6

typedef void (*f_xtor)(const M_Object o, const m_bit*, const VM_Shred);
//typedef void (*f_xtor)(const M_Object o, const VM_Shred);
typedef void (*f_mfun)(const M_Object o, const m_bit* RETURN, const VM_Shred sh);
//typedef void (*f_sfun)(const m_bit* RETURN, const VM_Shred sh);
typedef void (*f_sfun)(const m_bit*, const m_bit* RETURN, const VM_Shred sh);
typedef void (*f_xfun)();
typedef struct Gwi_* Gwi;

#define MFUN(a) ANN void a(const M_Object o __attribute__((unused)), const m_bit* RETURN __attribute__((unused)), const VM_Shred shred __attribute__((unused)))
#define SFUN(a) ANN void a(const M_Object o __attribute__((unused)), const m_bit* RETURN __attribute__((unused)), const VM_Shred shred __attribute__((unused)))
//#define SFUN(a) ANN void a(const m_bit* mem __attribute__((unused)), const m_bit* RETURN 
//__attribute__((unused)), const VM_Shred shred __attribute__((unused)))
#define CTOR(a) ANN void a(const M_Object o, const m_bit* _ __attribute__((unused)), const VM_Shred shred __attribute__((unused)))
//#define CTOR(a) ANN void a(const M_Object o, const VM_Shred shred __attribute__((unused)))
#define DTOR(a) ANN void a(const M_Object o, const m_bit* _ __attribute__((unused)), const VM_Shred shred __attribute__((unused)))
//#define DTOR(a) ANN void a(const M_Object o, const VM_Shred shred __attribute__((unused)))
#define OP_CHECK(a) ANN Type a(const Env env __attribute__((unused)), void* data __attribute__((unused)))
#define OP_EMIT(a)  ANN m_bool a(const Emitter emit __attribute__((unused)), void* data __attribute__((unused)))
#ifdef GWION_BUILTIN
#define GWION_IMPORT(a) ANN m_bool import_##a(const Gwi gwi)
#else
#define GWION_IMPORT(a) ANN m_bool import(const Gwi gwi)
#endif
#define ALLOC_PTR(a, b, c) b* a = (b*)_mp_alloc(sizeof(b)); *a = (b)c
#define _CHECK_OP(op, check, func)\
    CHECK_BB(gwi_oper_add(gwi, opck_##check))\
    CHECK_BB(gwi_oper_end(gwi, op_##op, func))
#define ERR_N(a,...) { err_msg(a,__VA_ARGS__); return t_null; }

#ifdef GWION_BUILTIN
#define GWIMPORT __declspec(dllexport)
#else
#define GWIMPORT __declspec(dllimport)
#endif

#define gwi_item_end(a, b, c) gwi_item_end(a, (const ae_flag)(b), (const m_uint*)c)
GWION_IMPORT ANN VM* gwi_vm(const Gwi);
GWION_IMPORT ANN2(1,2) ANEW Type gwi_mk_type(const Gwi, const m_str, const m_uint, const Type);
GWION_IMPORT ANN m_int gwi_add_type(const Gwi gwi, Type type);
GWION_IMPORT ANN2(1,2)m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor);
GWION_IMPORT ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td);
GWION_IMPORT ANN m_int gwi_class_end(const Gwi gwi);
GWION_IMPORT 
GWION_IMPORT ANN m_int gwi_item_ini(const Gwi gwi, const m_str type, const m_str name);
GWION_IMPORT ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr);
GWION_IMPORT 
GWION_IMPORT ANN m_int gwi_fptr_ini(const Gwi gwi, const m_str __restrict__ type, const __restrict__ m_str name);
GWION_IMPORT ANN m_int gwi_fptr_end(const Gwi gwi, const ae_flag flag);
GWION_IMPORT 
GWION_IMPORT ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str *name);
GWION_IMPORT ANN m_int gwi_tmpl_end(const Gwi gwi);
GWION_IMPORT 
GWION_IMPORT ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str name);
GWION_IMPORT ANN m_int gwi_union_add(const Gwi gwi, const __restrict__ m_str type, const __restrict__ m_str name);
GWION_IMPORT ANN m_int gwi_union_end(const Gwi gwi, const ae_flag flag);
GWION_IMPORT 
GWION_IMPORT ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type);
GWION_IMPORT ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint value);
GWION_IMPORT ANN m_int gwi_enum_end(const Gwi gwi);
GWION_IMPORT 
GWION_IMPORT ANN m_int gwi_func_ini(const Gwi gwi, const __restrict__ m_str type, const __restrict__ m_str name, const f_xfun addr);
GWION_IMPORT ANN m_int gwi_func_arg(const Gwi gwi, const __restrict__ m_str t, const __restrict__ m_str n);
GWION_IMPORT ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag);
GWION_IMPORT 
GWION_IMPORT ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const m_str l, const m_str r, const m_str t);
GWION_IMPORT ANN m_int gwi_oper_add(const Gwi gwi, Type (*check)(Env, void*));
GWION_IMPORT ANN m_int gwi_oper_emi(const Gwi gwi, m_bool (*check)(Emitter, void*));
GWION_IMPORT ANN2(1) m_int gwi_oper_end(const Gwi gwi, const Operator op, const f_instr f);
GWION_IMPORT 
GWION_IMPORT ANN Type_Decl* str2decl(const Env, const m_str, m_uint* depth);

OP_CHECK(opck_const_rhs);
OP_CHECK(opck_unary_meta);
OP_CHECK(opck_unary);
OP_CHECK(opck_post);
OP_CHECK(opck_rassign);
OP_CHECK(opck_rhs_emit_var);
OP_CHECK(opck_basic_cast);
OP_CHECK(opck_new);
OP_EMIT(opem_basic_cast);
OP_EMIT(opem_new);


ANN /*static */ Type_List str2tl(const Env env, const m_str s, m_uint *depth);

#endif

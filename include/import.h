#define DLARG_MAX 6

typedef void (*f_xtor)(const M_Object o, const VM_Shred sh);
typedef void (*f_mfun)(const M_Object o, char* RETURN, const VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, const VM_Shred sh);
typedef void (*f_xfun)();
typedef struct Gwi_* Gwi;

#define MFUN(a) ANN void a(const M_Object o, char* RETURN, const VM_Shred shred)
#define SFUN(a) ANN void a(char* RETURN, const VM_Shred shred)
#define CTOR(a) ANN void a(const M_Object o, const VM_Shred shred)
#define DTOR(a) ANN void a(const M_Object o, const VM_Shred shred)
#define OP_CHECK(a) ANN Type a(const Env env, void* data)
#define OP_EMIT(a)  ANN m_bool a(const Emitter emit, void* data)
#define IMPORT ANN m_bool import(const Gwi gwi)
#define ALLOC_PTR(a, b, c) b* a = (b*)malloc(sizeof(b)); *a =c
#define _CHECK_OP(op, check, func)\
    CHECK_BB(gwi_oper_add(gwi, opck_##check))\
    CHECK_BB(gwi_oper_end(gwi, op_##op, func))\

ANN VM* gwi_vm(const Gwi);
ANN m_int gwi_add_type(const Gwi gwi, Type type);
__attribute__((nonnull(1,2,3)))
m_int gwi_add_value(const Gwi gwi, const m_str name, Type type, const m_bool is_const, void* data);

__attribute__((nonnull(1,2)))
m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor);
ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td);
ANN m_int gwi_class_end(const Gwi gwi);

ANN m_int gwi_item_ini(const Gwi gwi, const m_str type, const m_str name);
__attribute__((nonnull(1)))
m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr);
#define gwi_item_end(a, b, c) gwi_item_end(a, (ae_flag)(b), (m_uint*)c)

ANN m_int gwi_fptr_ini(const Gwi gwi, const m_str type, const m_str name);
ANN m_int gwi_fptr_end(const Gwi gwi, const ae_flag flag);

ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str *name);
ANN m_int gwi_tmpl_end(const Gwi gwi);

__attribute__((nonnull(1)))
m_int gwi_union_ini(const Gwi gwi, const m_str name);
ANN m_int gwi_union_add(const Gwi gwi, const m_str type, const m_str name);
ANN m_int gwi_union_end(const Gwi gwi, const ae_flag flag);

__attribute__((nonnull(1)))
m_int gwi_enum_ini(const Gwi gwi, const m_str type);
ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint value);
ANN m_int gwi_enum_end(const Gwi gwi);

ANN m_int gwi_func_ini(const Gwi gwi, const m_str type, const m_str name, const f_xfun addr);
ANN m_int gwi_func_arg(const Gwi gwi, const m_str t, const m_str n);
ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag);

__attribute__((nonnull(1)))
m_int gwi_oper_ini(const Gwi gwi, const m_str l, const m_str r, const m_str t);
ANN m_int gwi_oper_add(const Gwi gwi, Type (*check)(Env, void*));
ANN m_int gwi_oper_emi(const Gwi gwi, m_bool (*check)(Emitter, void*));
__attribute__((nonnull(1)))
m_int gwi_oper_end(const Gwi gwi, const Operator op, const f_instr f);

ANN Type_Decl* str2decl(const Env, const m_str, m_uint* depth);

OP_CHECK(opck_const_lhs);
OP_CHECK(opck_unary_meta);
OP_CHECK(opck_unary);
OP_CHECK(opck_post);
OP_CHECK(opck_assign);
OP_CHECK(opck_rassign);
OP_CHECK(opck_rhs_emit_var);
OP_CHECK(opck_basic_cast);
OP_CHECK(opck_spork);
OP_CHECK(opck_new);
OP_EMIT(opem_basic_cast);
OP_EMIT(opem_new);
OP_EMIT(opem_spork);

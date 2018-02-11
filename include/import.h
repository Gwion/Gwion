#define DLARG_MAX 6

typedef void (*f_xtor)(M_Object o, VM_Shred sh);
typedef void (*f_mfun)(M_Object o, char* RETURN, VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, VM_Shred sh);
typedef void (*f_xfun)();
typedef struct Gwi_* Gwi;

#define MFUN(a) void a(M_Object o, char* RETURN, VM_Shred shred)
#define SFUN(a) void a(char* RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define OP_CHECK(a) Type a(Env env, void* data)
#define OP_EMIT(a) m_bool a(Emitter emit, void* data)
#define IMPORT m_bool import(Gwi gwi)
#define ALLOC_PTR(a, b, c) b* a = (b*)malloc(sizeof(b)); *a =c

m_int gwi_add_type(Gwi gwi, Type type);
m_int gwi_add_value(Gwi gwi, const m_str name, Type type, const m_bool is_const, void* data);

m_int gwi_class_ini(Gwi gwi, Type type, const f_xtor pre_ctor, const f_xtor dtor);
m_int gwi_class_ext(Gwi gwi, Type_Decl* td);
m_int gwi_class_end(Gwi gwi);

m_int gwi_item_ini(Gwi gwi, const m_str type, const m_str name);
m_int gwi_item_end(Gwi gwi, const ae_flag flag, const m_uint* addr);
#define gwi_item_end(a, b, c) gwi_item_end(a, (ae_flag)(b), (m_uint*)c)

m_int gwi_fptr_ini(Gwi gwi, const m_str type, const m_str name);
m_int gwi_fptr_end(Gwi gwi, const ae_flag flag);

m_int gwi_tmpl_ini(Gwi gwi, const m_uint n, const m_str *name);
m_int gwi_tmpl_end(Gwi gwi);

m_int gwi_union_ini(Gwi gwi, const m_str name);
m_int gwi_union_add(Gwi gwi, const m_str type, const m_str name);
m_int gwi_union_end(Gwi gwi, ae_flag flag);

m_int gwi_enum_ini(Gwi gwi, const m_str type);
m_int gwi_enum_add(Gwi gwi, const m_str name, const m_uint value);
m_int gwi_enum_end(Gwi gwi);

m_int gwi_func_ini(Gwi gwi, const m_str type, const m_str name, f_xfun addr);
m_int gwi_func_arg(Gwi gwi, const m_str t, const m_str  n);
m_int gwi_func_end(Gwi gwi, const ae_flag flag);

m_int gwi_oper_ini(Gwi gwi, const m_str l, const m_str r, const m_str t);
m_int gwi_oper_add(Gwi gwi, Type (*check)(Env, void*));
m_int gwi_oper_emi(Gwi gwi, m_bool (*check)(Emitter, void*));
m_int gwi_oper_end(Gwi gwi, const Operator op, const f_instr f);

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

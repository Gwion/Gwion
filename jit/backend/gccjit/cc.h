#include <libgccjit.h>
struct JitCC_ {
  gcc_jit_context* ctx;
  gcc_jit_context* parent;
  gcc_jit_function* f;
  gcc_jit_block* block;
  gcc_jit_rvalue* shred;
  struct Map_ type;
  struct Map_ field;
};

#define LVAL gcc_jit_lvalue*
#define RVAL gcc_jit_rvalue*
#define FIELD gcc_jit_field*
#define STRUCT gcc_jit_struct*
#define TYPE gcc_jit_type*
#define GET_TYPE(a) cc_get_type(cc, #a)
#define BLOCK gcc_jit_block*
#define NEW_BLOCK(a) gcc_jit_function_new_block(cc->f, (a))
#define BLOCK_RET(a, b) { gcc_jit_block_end_with_void_return((a), NULL); cc->block = (b); }
#define BLOCK_JMP(a, b) { gcc_jit_block_end_with_jump((a), NULL, (b)); }
#define BLOCK_CND(a, b, c, d) gcc_jit_block_end_with_conditional((a), NULL, (b), (c), (d));
#define AS_RVAL(a) gcc_jit_lvalue_as_rvalue((a))
#define EVAL(a, b) gcc_jit_block_add_eval((a), NULL, (b))
#define ASSIGN(a, b, c) gcc_jit_block_add_assignment((a), NULL, (b), (c))
#define ARRAY_ACCESS(a,b) gcc_jit_context_new_array_access(cc->ctx, NULL, (a), (b))
#define CAST(a, b) gcc_jit_context_new_cast(cc->ctx, NULL, (a), (b))
#define CALL(a, b, c)gcc_jit_context_new_call_through_ptr(cc->ctx, NULL, (a), (b), (c))
#define FPTR(a, b, c, d) gcc_jit_context_new_function_ptr_type(cc->ctx, NULL, (a), (b), (c), (d))
#define COMP(a, b, c) gcc_jit_context_new_comparison(cc->ctx, NULL, GCC_JIT_COMPARISON_##a, (b), (c))
#define ADDR(a) gcc_jit_lvalue_get_address((a), NULL)
// beware arch
#define ICONST(type, a) gcc_jit_context_new_rvalue_from_long(cc->ctx, (type), (a))
#define PCONST(type, a) gcc_jit_context_new_rvalue_from_ptr(cc->ctx, (type), (void*)(a))
void push_reg(CC, m_int);
void push_reg2(CC, RVAL);
void push_mem(CC, m_int);
void push_mem2(CC, RVAL);
gcc_jit_type* cc_get_type(CC, const m_str);
gcc_jit_field* cc_get_field(CC, const m_str);
LVAL cc_deref_field(CC cc, RVAL val, const m_str s);
void cc_release(CC cc, RVAL obj);
void cc_release2(CC cc, RVAL obj);

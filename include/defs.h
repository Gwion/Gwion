#ifndef __DEF
#define __DEF

#define MEM_STEP 16
#define SIZEOF_MEM 0x1 << MEM_STEP
#define SIZEOF_REG 0x1 << 14

#define CHECK_BB(f) if(f < 0) return -1;
#define CHECK_OB(f) if(!f)    return -1;
#define CHECK_BO(f) if(f < 0) return NULL;
#define CHECK_OO(f) if(!f)    return NULL;

#include "generated.h"
/* pointer types */
typedef struct Ast_       * Ast;
typedef struct Func_Def_  * Func_Def;
typedef struct Class_Def_ * Class_Def;
typedef struct Array_Sub_ * Array_Sub;

typedef struct VM_Object_ * VM_Object;
typedef struct NameSpace_ * NameSpace;
typedef struct Scope_     * Scope;
typedef struct Type_      * Type;
typedef struct Value_     * Value;
typedef struct Func_      * Func;
typedef struct Context_   * Context;
typedef struct Env_       * Env;

typedef struct Instr_     * Instr;
typedef struct ID_List_   * ID_List;
typedef struct Type_List_ * Type_List;

typedef struct Emitter_   * Emitter;
typedef struct M_Object_  * M_Object;
typedef struct VM_Array_  * VM_Array;
typedef struct UGen_      * UGen;

typedef enum {
  te_int, te_float, te_dur, te_time, te_now, te_string, te_void, te_function, te_func_ptr,
  te_object, te_class, te_user, te_null,
  te_io, te_fileio, te_polar, te_complex, te_vec3, te_vec4, te_array, te_shred, te_machine, te_event,
  te_std,
  te_template,
  te_ugen,
  te_vararg,
  te_last
} te_type;
typedef enum { ae_func_spec_none, ae_func_spec_dtor, ae_func_spec_op } ae_func_spec;

typedef enum { Kindof_Void, Kindof_Int, Kindof_Float, Kindof_Complex, Kindof_Vec3, Kindof_Vec4, Kindof_Ptr} Kindof;

typedef enum {
// arithmetic
  op_assign, op_plus, op_minus, op_times, op_divide, op_percent,
// logical
  op_and, op_or, op_eq, op_neq,
  op_gt, op_ge, op_lt, op_le,
  // bitwise
  op_shift_left, op_shift_right,
  op_s_or, op_s_and, op_s_xor,
  // unary op
  op_plusplus, op_minusminus, op_exclamation, op_tilda,
  op_new, op_spork, op_typeof, op_sizeof,
// reverse arithmetic
  op_chuck, op_plus_chuck, op_minus_chuck, op_times_chuck, op_divide_chuck, op_modulo_chuck,
// reverse logical
  op_rand, op_ror, op_req, op_rneq,
  op_rgt, op_rge, op_rlt, op_rle,
// reverse bitwise
  op_rsl, op_rsr, op_rsand, op_rsor, op_rsxor,
// unchuck and others
  op_unchuck, op_rinc, op_rdec, op_runinc, op_rundec,
// at
  op_at_chuck, op_at_unchuck,
// trig
  op_trig, op_untrig
} Operator;
// more
enum {
  ae_op_arrow_left, ae_op_arrow_right
};
#define INSTR(a) void a(VM* vm, VM_Shred shred, Instr instr)
#define MFUN(a) void a(M_Object o,  DL_Return * RETURN, VM_Shred shred)
#define SFUN(a) void a(DL_Return * RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Env env)
// should be in lang.h, maybe
// string
#define STRING(o) *((m_str*)((M_Object)o)->d.data + o_string_data)
// shred
#define ME(o) *((VM_Shred*)((M_Object)o)->d.data + o_shred_me)
// event
#define EV_SHREDS(o) *((Vector*)((M_Object)o)->d.data + o_event_shred)
// fileio
//#define IO_DIR(o)	  *((DIR**)((M_Object)o)->d.data + o_fileio_dir)
#define IO_DIR(o) *(DIR**)(((M_Object)o)->d.data + o_fileio_dir)
#define IO_FILE(o)	*(FILE**)(((M_Object)o)->d.data + o_fileio_file)
#define IO_ASCII(o)	*(m_uint*)(((M_Object)o)->d.data + o_fileio_ascii)

/*
#define SZ_INT sizeof(m_uint)
#define SZ_FLOAT sizeof(SPFLOAT)
#define SZ_COMPLEX (sizeof(SPFLOAT) * 2)
#define SZ_VEC3 (sizeof(SPFLOAT) * 3)
#define SZ_VEC4 (sizeof(SPFLOAT) * 4)
*/
#define NEXT_INT     (s)  *(m_int*)    (s->mem + SZ_INT + RETURN->offset); RETURN->offset += SZ_INT    )
#define NEXT_UINT    (s)  *(m_uint*)   (s->mem + SZ_INT + RETURN->offset); RETURN->offset += SZ_INT    )
#define NEXT_FLOAT   (s)  *(m_float*)  (s->mem + SZ_INT + RETURN->offset); RETURN->offset += SZ_FLOAT  )
#define NEXT_OBJECT  (s)  *(M_Object*) (s->mem + SZ_INT + RETURN->offset); RETURN->offset += SZ_INT    )
#define NEXT_COMPLEX (s)  *(m_complex*)(s->mem + SZ_INT + RETURN->offset); RETURN->offset += SZ_COMPLEX)


#ifndef GW_PLUG_DIR
#define GW_PLUG_DIR "/usr/lib/Gwion/plug"
#endif

#ifdef DEBUG_STACK
#define PUSH_MEM(a, b) \
{\
  a->mem_index += b; \
  a->mem += b;\
}
#define POP_MEM(a, b) \
{\
  a->mem_index -= b; \
  a->mem -= b;\
}
#define PUSH_REG(a, b) \
{\
  a->reg_index += b; \
  a->reg += b;\
}
#define POP_REG(a, b) \
{\
  a->reg_index -= b; \
  a->reg -= b;\
}
#else
#define PUSH_MEM(a, b) \
  a->mem += b;
#define POP_MEM(a, b) \
  a->mem -= b;
#define PUSH_REG(a, b) \
  a->reg += b;
#define POP_REG(a, b) \
  a->reg -= b;
#endif
#endif

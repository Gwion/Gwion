#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "instr.h"
#include "context.h"
#include "func.h"
#include "traverse.h"
#include "mpool.h"

#define INSTR_RECURS (f_instr)1
#define INSTR_INLINE (f_instr)2

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_ref;
  m_bool is_obj;
} Local;
POOL_HANDLE(Local, 128)
POOL_HANDLE(Frame, 8)
POOL_HANDLE(Code,  8)
POOL_HANDLE(Instr, 2048)
POOL_HANDLE(VM_Array_Info, 64)

#define IS_REF 1 << 1
#define IS_OBJ 1 << 2

#ifdef OPTIMIZE
__attribute__((returns_nonnull))
ANN static inline f_instr get_goto(Env env) {
  return SAFE_FLAG(env->func, ae_flag_inline) ? InlineGoto : Goto;
}
#else
#define get_goto(a) Goto
#endif

#ifdef GWCOV
#define COVERAGE(a) if(emit->coverage)coverage(emit, a->pos);


ANN static void coverage(const Emitter emit, const m_uint pos) {
  fprintf(emit->cov_file, "%" INT_F " ini\n", pos);
  Instr cov = emitter_add_instr(emit, InstrCoverage);
  cov->m_val = pos;
}
#else
#define COVERAGE(a)
#endif

#ifdef GWCGRAPH
#define GWCGRAPH_INI\
  m_uint index = emit_alloc_local(emit, 2*SZ_INT, 0);\
  if(emit->profile) {\
    Instr start = emitter_add_instr(emit, gwcgraph_ini);\
    start->m_val = index;\
  }
#define GWCGRAPH_END(name)\
  if(emit->profile) {\
    Instr instr = emitter_add_instr(emit, gwcgraph_end);\
    instr->m_val = index;\
    instr->m_val2 = (m_uint)name;\
    *(FILE**)instr->ptr = emit->call_file;\
  }
static INSTR(gwcgraph_ini) {
  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
  *(struct timespec*)MEM(instr->m_val) = ts;
}
static INSTR(gwcgraph_end) {
  struct timespec end, start = *(struct timespec*)MEM(instr->m_val);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
  fprintf(*(FILE**)instr->ptr, "%s %s %lu %lu %lu %lu\n",
    (m_str)instr->m_val2,
    shred->code->name,
    start.tv_sec, start.tv_nsec,
    end.tv_sec, end.tv_nsec);
}
#else
#define GWCGRAPH_INI
#define GWCGRAPH_END(a)
#endif

ANEW static Frame* new_frame() {
  Frame* frame = mp_alloc(Frame);
  vector_init(&frame->stack);
  return frame;
}

ANN static void free_frame(Frame* a) {
  LOOP_OPTIM
  for(vtype i = vector_size(&a->stack) + 1; --i;)
    if(vector_at(&a->stack, i - 1))
      mp_free(Local, (Local*)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  mp_free(Frame, a);
}

ANEW ANN static Local* frame_alloc_local(Frame* frame, const m_uint size, const m_uint flag) {
  Local* local = mp_alloc(Local);
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_ref = (flag & IS_REF) == IS_REF;
  local->is_obj = (flag & IS_OBJ) == IS_OBJ;
  frame->curr_offset += local->size;
  vector_add(&frame->stack, (vtype)local);
  return local;
}

ANN static void frame_push_scope(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN static void frame_pop_scope(Frame* frame, Vector v) {
  m_uint i;
  while((i = vector_size(&frame->stack)) && vector_back(&frame->stack)) {
    Local* local = (Local*)vector_pop(&frame->stack);
    frame->curr_offset -= local->size;
    vector_add(v, (vtype)local);
  }
  vector_pop(&frame->stack);
}

ANN static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool add_ref);
ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop);
ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List list);
ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member);
ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def);

ANEW ANN Emitter new_emitter(const Env env) {
  Emitter emit = (Emitter)xcalloc(1, sizeof(struct Emitter_));
  vector_init(&emit->stack);
  vector_init(&emit->codes);
  emit->env = env;
  return emit;
}

ANN void free_emitter(Emitter a) {
#ifdef GWCGRAPH
  if(a->profile)
    fclose(a->call_file);
#endif
  free_env(a->env);
  vector_release(&a->stack);
  vector_release(&a->codes);
  free(a);
}

ANEW Instr new_instr() { return mp_alloc(Instr); }
ANN void free_instr(Instr instr) { mp_free(Instr, instr); }

__attribute__((returns_nonnull))
ANN2(1) Instr emitter_add_instr(const Emitter emit, const f_instr f) {
  Instr instr = mp_alloc(Instr);
  instr->execute = f;
  vector_add(&emit->code->code, (vtype)instr);
  return instr;
}

ANEW static Code* new_code() {
  Code* code = mp_alloc(Code);
  code->stack_depth = 0;
  vector_init(&code->code);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  code->frame = new_frame();
  return code;
}

ANN static void free_code(Code* code) {
  vector_release(&code->code);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  free_frame(code->frame);
  free(code->name);
  mp_free(Code, code);
}

ANN static void emit_pop_scope(const Emitter emit) { GWDEBUG_EXE
  m_uint i;
  struct Vector_ v;
  vector_init(&v);
  frame_pop_scope(emit->code->frame, &v);
  for(i = 0; i < vector_size(&v); i++) {
    Local* l = (Local*)vector_at(&v, i);
    if(l->is_obj) {
      Instr instr = emitter_add_instr(emit, Release_Object);
      instr->m_val = l->offset;
    }
    mp_free(Local, l);
  }
  vector_release(&v);
}

ANN static inline void emit_push_scope(const Emitter emit) { GWDEBUG_EXE
  frame_push_scope(emit->code->frame);
}

ANN static inline m_uint emit_code_size(const Emitter emit) { GWDEBUG_EXE
  return vector_size(&emit->code->code);
}

ANN static inline m_uint emit_code_offset(const Emitter emit) { GWDEBUG_EXE
  return emit->code->frame->curr_offset;
}

ANN static inline m_uint emit_alloc_local(const Emitter emit, const m_uint size, const m_uint flag) { GWDEBUG_EXE
  Local* l = frame_alloc_local(emit->code->frame, size, flag);
  return l->offset;
}

ANN static m_bool emit_pre_ctor_inner(const Emitter emit, const Type type) { GWDEBUG_EXE
  Instr instr = emitter_add_instr(emit, Pre_Constructor);
  instr->m_val = (m_uint)type->info->pre_ctor;
  instr->m_val2 = (m_uint)emit_code_offset(emit);
  return 1;
}

ANN static m_bool emit_pre_ctor(const Emitter emit, const Type type) { GWDEBUG_EXE
  if(type->parent)
    emit_pre_ctor(emit, type->parent);
  if(type->info->pre_ctor)
    emit_pre_ctor_inner(emit, type);
  if(GET_FLAG(type, ae_flag_template) && GET_FLAG(type, ae_flag_builtin)) {
    const m_str name = get_type_name(type->name, 0);
    const Type t = nspc_lookup_type1(type->info->parent, insert_symbol(name));
    if(t->info->pre_ctor)
      emit_pre_ctor_inner(emit, t);
  }
  return 1;
}

ANN static m_bool emit_pre_constructor_array(const Emitter emit, const Type type) { GWDEBUG_EXE
  Instr top, bottom;
  const m_uint start_index = emit_code_size(emit);
  top = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Top);
  *(Type*)top->ptr = type;
  emit_pre_ctor(emit, type);
  bottom = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = emit_code_size(emit);
  bottom->m_val = start_index;
  emitter_add_instr(emit, Instr_Pre_Ctor_Array_Post);
  return 1;
}
ANN void free_array_info(VM_Array_Info* info) { mp_free(VM_Array_Info, info); }
ANN VM_Array_Info* emit_array_extend_inner(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  const Type base = array_base(t);
  CHECK_BO(emit_exp(emit, e, 0))
  VM_Array_Info* info = mp_alloc(VM_Array_Info);
  info->depth = t->array_depth;
  info->type = t;
  info->base = base;
  Instr alloc = emitter_add_instr(emit, Instr_Array_Alloc);
  *(VM_Array_Info**)alloc->ptr = info;
  if(isa(base, t_object) > 0) {
    CHECK_BO(emit_pre_constructor_array(emit, base))
    info->is_obj = 1;
  }
  return info;
}

static INSTR(pop_array_class) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  M_Object tmp = *(M_Object*)REG(0);
  ARRAY(obj) = ARRAY(tmp);
  free(tmp->data);
  free_object(tmp);
  ADD_REF(obj->type_ref) // add ref to typedef array type
}

ANN m_bool emit_ext_ctor(const Emitter emit, const VM_Code code) { GWDEBUG_EXE
  emitter_add_instr(emit, Reg_Dup_Last);
  Instr push_f = emitter_add_instr(emit, Reg_Push_Imm);
  Instr offset = emitter_add_instr(emit, Reg_Push_Imm);
  push_f->m_val = SZ_INT;
  *(VM_Code*)push_f->ptr = code;
  offset->m_val = SZ_INT;
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  emitter_add_instr(emit, Func_Member);
  return 1;
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  CHECK_OB(emit_array_extend_inner(emit, t, e))
  emitter_add_instr(emit, pop_array_class);
  return 1;
}


ANN2(1,2) m_bool emit_instantiate_object(const Emitter emit, const Type type,
    const Array_Sub array, const m_bool is_ref) {
  if(type->array_depth) {
    VM_Array_Info* info = emit_array_extend_inner(emit, type, array->exp);
    CHECK_OB(info)
    info->is_ref = is_ref;
  } else if(isa(type, t_object) > 0 && !is_ref) {
    Instr instr = emitter_add_instr(emit, Instantiate_Object);
    *(Type*)instr->ptr = type;
    emit_pre_ctor(emit, type);
  }
  return 1;
}

ANN static void prepare_this_exp(Exp base, Exp dot) {
  memset(base, 0, sizeof(struct Exp_));
  memset(dot, 0, sizeof(struct Exp_));
  base->meta = ae_meta_var;
  base->exp_type = ae_exp_primary;
  base->d.exp_primary.primary_type = ae_primary_id;
  base->d.exp_primary.d.var = insert_symbol("this");
  base->d.exp_primary.self = base;
  dot->exp_type = ae_exp_dot;
  dot->meta = ae_meta_var;
  dot->d.exp_dot.base = base;
  dot->d.exp_dot.self = dot;
}

ANN static m_bool emit_symbol_owned(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  struct Exp_ base, dot;
  prepare_this_exp(&base, &dot);
  base.type = v->owner_class;
  dot.type = v->type;
  dot.d.exp_dot.t_base = v->owner_class;
  dot.d.exp_dot.xid = prim->d.var;
  dot.emit_var = prim->self->emit_var;
  CHECK_BB(emit_exp_dot(emit, &dot.d.exp_dot))
  return 1;
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  Instr instr = emitter_add_instr(emit, Reg_Push_Imm);

  if(GET_FLAG(v, ae_flag_func)) {
    instr->m_val = SZ_INT;
    *(Func*)instr->ptr = v->d.func_ref;
  } else if(!prim->self->emit_var && isa(v->type, t_object) < 0 && !GET_FLAG(v,ae_flag_enum)) { GWDEBUG_EXE
    instr->m_val = v->type->size;
    if(v->d.ptr)
      memcpy(instr->ptr, v->d.ptr, v->type->size);
  } else {
    instr->m_val = v->type->size;
    *(m_uint*)instr->ptr = (prim->self->emit_var ? (m_uint)&v->d.ptr : (m_uint)v->d.ptr);
  }
  return 1;
}

ANN static m_bool emit_symbol(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  Instr instr;
  if(v->owner_class)
    return emit_symbol_owned(emit, prim);
  if(GET_FLAG(v, ae_flag_builtin) || GET_FLAG(v, ae_flag_enum))
    return emit_symbol_builtin(emit, prim);
  instr = emitter_add_instr(emit, prim->self->emit_var ?
      Reg_Push_Mem_Addr : Reg_Push_Mem);
  instr->m_val  = v->offset;
  instr->m_val2 = v->type->size;
  *(m_uint*)instr->ptr = GET_FLAG(v, ae_flag_global);
  return 1;
}

ANEW ANN VM_Code emit_code(const Emitter emit) { GWDEBUG_EXE
  Code* c = emit->code;
  const VM_Code code = new_vm_code(&c->code, c->stack_depth,
      GET_FLAG(c, ae_flag_member), c->name);
  if(GET_FLAG(c, ae_flag_recurs))
    SET_FLAG(code, ae_flag_recurs);
  vector_add(&emit->codes, (vtype)code);
  free_code(c);
  return code;
}

ANN static m_bool emit_exp_prim_array(const Emitter emit, const Array_Sub array) { GWDEBUG_EXE
  m_uint count = 0;
  Exp e = array->exp;
  CHECK_BB(emit_exp(emit, e, 0))
  while(e) {
    count++;
    e = e->next;
  }
  const Type type = array->type;
  const Type base = array_base(type);
  Instr instr = emitter_add_instr(emit, Instr_Array_Init);
  VM_Array_Info* info = mp_alloc(VM_Array_Info);
  info->type = type;
  info->base = base;
  info->length = count;
  *(VM_Array_Info**)instr->ptr = info;
  instr->m_val2 = base->size;
  return 1;
}

ANN static m_uint get_depth(Type t) {
  m_uint depth = 0;
  do depth += t->array_depth;
  while((t = t->parent));
  return depth;
}

ANN static m_bool emit_exp_array(const Emitter emit, const Exp_Array* array) { GWDEBUG_EXE
  const m_uint is_var = array->self->emit_var;
  const m_uint depth = get_depth(array->base->type) - array->self->type->array_depth;
  CHECK_BB(emit_exp(emit, array->base, 0))
  CHECK_BB(emit_exp(emit, array->array->exp, 0))
  if(depth == 1) {
    Instr instr = emitter_add_instr(emit, Instr_Array_Access);
    instr->m_val = is_var;
    instr->m_val2 = is_var ? SZ_INT : array->self->type->size;
  } else {
    Instr instr = emitter_add_instr(emit, Instr_Array_Access_Multi);
    instr->m_val = depth;
    instr->m_val2 = (is_var || array->self->type->array_depth) ?
      SZ_INT : array_base(array->base->type)->size;
    *(m_uint*)instr->ptr = is_var || array->self->type->array_depth;
  }
  return 1;
}

ANN static m_bool emit_exp_prim_vec(const Emitter emit, const Vec* vec, const ae_Exp_Primary_Type t) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, vec->exp, 0));
  m_int n = (t == ae_primary_vec ? 3 : 2) - vec->dim;
  while(n-- > 0) {
    Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_FLOAT;
  }
  return 1;
}

ANN static m_bool emit_exp_prim_id(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->d.var == insert_symbol("this"))
    emitter_add_instr(emit, Reg_Push_This);
  else if(prim->d.var == insert_symbol("me"))
    emitter_add_instr(emit, Reg_Push_Me);
  else if(prim->d.var == insert_symbol("now"))
    emitter_add_instr(emit, Reg_Push_Now);
  else if(prim->d.var == insert_symbol("false") ||
      prim->d.var == insert_symbol("null") ||
      prim->d.var == insert_symbol("NULL")) {
    Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_INT;
  }
  else if(prim->d.var == insert_symbol("true")) {
    Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
    instr->m_val = SZ_INT;
    *(m_uint*)instr->ptr = 1;
  }  else if(prim->d.var == insert_symbol("maybe"))
    emitter_add_instr(emit, Reg_Push_Maybe);
  else
    emit_symbol(emit, prim);
  return 1;
}

ANN static m_bool emit_exp_prim_num(const Emitter emit, const m_uint i) { GWDEBUG_EXE
  Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = i;
  return 1;
}

ANN static m_bool emit_exp_prim_float(const Emitter emit, const m_float f) { GWDEBUG_EXE
  Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_FLOAT;
  *(m_float*)instr->ptr = f;
  return 1;
}

ANN static m_bool emit_exp_prim_char(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = str2char(prim->d.chr, prim->self->pos);
  return 1;
}

ANN static m_bool emit_exp_prim_str(const Emitter emit, const m_str str) { GWDEBUG_EXE
  Instr instr = emitter_add_instr(emit, Reg_Push_Str);
  instr->m_val = (m_uint)str;
  return 1;
}

ANN static m_bool emit_exp_prim_gack(const Emitter emit, const Exp exp) { GWDEBUG_EXE
  Instr instr;
  Vector v = new_vector();
  Exp e = exp;
  while(e) {
    vector_add(v, (vtype)e->type);
    if(e->type != emit->env->class_def)
      ADD_REF(e->type);
    e = e->next;
  }
  if(emit_exp(emit, exp, 0) < 0) { GWDEBUG_EXE
    free_vector(v);
    CHECK_BB(err_msg(EMIT_, exp->pos, "\t... in 'gack' expression."))
  }
  instr = emitter_add_instr(emit, Gack);
  *(Vector*)instr->ptr = v;
  return 1;
}

ANN static m_bool emit_exp_primary2(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_str:
      return emit_exp_prim_str(emit, prim->d.str);
    case ae_primary_array:
      return emit_exp_prim_array(emit, prim->d.array);
    case ae_primary_hack:
      return emit_exp_prim_gack(emit, prim->d.exp);
    case ae_primary_nil:
    default:
      break;
  }
  return 1;
}

ANN static m_bool emit_exp_primary1(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_complex:
    case ae_primary_polar:
    case ae_primary_vec:
      return emit_exp_prim_vec(emit, &prim->d.vec, prim->primary_type);
    default:
      return emit_exp_primary2(emit, prim);
  }
  return 1;
}

#ifdef OPTIMIZE
ANN static m_bool emit_exp_prim_constprop(Emitter emit, const Exp_Primary* prim) {
  Instr instr = emitter_add_instr(emit, ConstPropSet);
  instr->m_val = prim->value->offset;
  instr->m_val2 = prim->d.num;
  *(m_bool*)instr->ptr = prim->self->emit_var;
  return 1;
}
#endif

ANN static m_bool emit_exp_primary(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_id:
      return emit_exp_prim_id(emit, prim);
    case ae_primary_num:
      return emit_exp_prim_num(emit, prim->d.num);
    case ae_primary_float:
      return emit_exp_prim_float(emit, prim->d.fnum);
    case ae_primary_char:
      return emit_exp_prim_char(emit, prim);
#ifdef OPTIMIZE
    case ae_primary_constprop:
      return emit_exp_prim_constprop(emit, prim);
#endif

    default:
      return emit_exp_primary1(emit, prim);
  }
  return 1;
}

ANN static m_bool emit_dot_static_data(const Emitter emit, const Value v, const m_bool emit_var) { GWDEBUG_EXE
  Instr alloc, push = emitter_add_instr(emit, Reg_Push_Imm);
  push->m_val = SZ_INT;
  *(Type*)push->ptr = v->owner_class;
  alloc = emitter_add_instr(emit, Dot_Static_Data);
  alloc->m_val2 = emit_var ? SZ_INT : v->type->size;
  *(m_uint*)alloc->ptr = emit_var;
  alloc->m_val = v->offset;
  return 1;
}

ANN static m_bool decl_static(const Emitter emit, const Var_Decl var_decl, const m_bool is_ref) {
  Value v = var_decl->value;
  Code* code = emit->code;

  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->type, var_decl->array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, 1))
  emitter_add_instr(emit, Assign_Object);
  emit->code = code;
  return 1;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Var_Decl var_decl, const m_bool is_ref) { GWDEBUG_EXE
  const Value value = var_decl->value;

  if(isa(value->type, t_object) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  CHECK_BB(emit_dot_static_data(emit, value, 1))
  return 1;
}

ANN static Instr emit_exp_decl_global(const Emitter emit, const Value v, const m_uint flag) { GWDEBUG_EXE
  v->offset = emit_alloc_local(emit, v->type->size, flag);
  return emitter_add_instr(emit, Alloc_Word);
}

ANN static m_bool emit_exp_decl_non_static(const Emitter emit, const Var_Decl var_decl,
  const m_bool is_ref, const m_bool emit_var) { GWDEBUG_EXE
  const Value value = var_decl->value;
  const Type type = value->type;
  Instr alloc;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = array && array->exp;
  const m_bool is_obj = isa(type, t_object) > 0 || var_decl->array;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  if(GET_FLAG(value, ae_flag_member))
    alloc = emitter_add_instr(emit, Alloc_Member);
  else
    alloc = emit_exp_decl_global(emit, value, (is_ref ? IS_REF : 0) |
        (is_obj ? IS_OBJ : 0));
  alloc->m_val2 = type->size;
  alloc->m_val = value->offset;
  *(m_uint*)alloc->ptr = ((is_ref && !array) || isa(type, t_object) < 0)  ? emit_var : 1;
  if(is_obj) {
    if(GET_FLAG(type, ae_flag_typedef | ae_flag_ref)) {
if(!(type->def && type->def->ext && GET_FLAG(type->def->ext, ae_flag_typedef)))
      return 1;
    }
    if((is_array) || !is_ref) {
      Instr assign = emitter_add_instr(emit, Assign_Object);
      assign->m_val = emit_var;
    }
    if(is_array)
      ADD_REF(type);
  }
  return 1;
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def class_Def);

ANN static m_bool emit_exp_decl_template(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  if(!GET_FLAG(decl->type, ae_flag_emit)) {
    CHECK_BB(template_push_types(emit->env, decl->base->tmpl->list.list, decl->td->types))
    CHECK_BB(emit_class_def(emit, decl->type->def))
    nspc_pop_type(emit->env->curr);
  }
  return 1;
}

ANN static m_bool emit_exp_decl(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  const m_bool ref = GET_FLAG(decl->td, ae_flag_ref) || type_ref(decl->type);
  const m_bool var = decl->self->emit_var;

  if(GET_FLAG(decl->type, ae_flag_template))
    CHECK_BB(emit_exp_decl_template(emit, decl))
  while(list) {
    const m_bool r = GET_FLAG(list->self->value, ae_flag_ref) + ref;

    if(isa(list->self->value->type, t_func_ptr) > 0)
      REM_REF(list->self->value->type)

    if(GET_FLAG(decl->td, ae_flag_static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, r))
    else
      CHECK_BB(emit_exp_decl_non_static(emit, list->self, r, var))
    list = list->next;
  }
  return 1;
}

ANN static m_uint vararg_size(const Exp_Func* exp_func, const Vector kinds) {
  Exp e = exp_func->args;
  Arg_List l = exp_func->m_func->def->arg_list;
  m_uint size = 0;
  while(e) {
    if(!l) {
      size += e->type->size;
      if(e->type->size)
        vector_add(kinds, e->type->size);
    } else
      l = l->next;
    e = e->next;
  }
  return size;
}

ANN static m_bool emit_func_arg_vararg(const Emitter emit, const Exp_Func* exp_func) { GWDEBUG_EXE
  Instr instr;
  Vector kinds = new_vector();

  instr = emitter_add_instr(emit, MkVararg);
  instr->m_val = vararg_size(exp_func, kinds);
  instr->m_val2 = (m_uint)kinds;
  return 1;
}

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Func* exp_func) { GWDEBUG_EXE
  if(exp_func->args)
    CHECK_BB(emit_exp(emit, exp_func->args, 1))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic))
    CHECK_BB(emit_func_arg_vararg(emit, exp_func))
  return 1;
}

ANN static m_bool emit_exp_call_helper(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  if(exp_func->args && !spork)
    CHECK_BB(emit_func_args(emit, exp_func))
  CHECK_BB(emit_exp(emit, exp_func->func, 0))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) {
    // handle empty call to variadic functions
    emitter_add_instr(emit, MkVararg);
    Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_INT;
  }
  return 1;
}

ANN static m_bool emit_exp_call_template(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  const Func_Def def = exp_func->m_func->def;
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = exp_func->m_func->value_ref->owner_class;
  vector_add(&emit->env->nspc_stack, (vtype)emit->env->curr);
  emit->env->curr = exp_func->m_func->value_ref->owner;
  SET_FLAG(def, ae_flag_template);
  CHECK_BB(template_push_types(emit->env, def->tmpl->list, exp_func->tmpl->types))
  CHECK_BB(traverse_func_def(emit->env, def))
  CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  nspc_pop_type(emit->env->curr);
  env_pop_class(emit->env);
  UNSET_FLAG(exp_func->m_func, ae_flag_checked);
  return 1;
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  if(!exp_func->tmpl)
    CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  else
    CHECK_BB(emit_exp_call_template(emit, exp_func, spork))
  return emit_exp_call1(emit, exp_func->m_func);
}

ANN m_bool emit_exp_binary_ptr(const Emitter emit, const Exp rhs) { GWDEBUG_EXE
  Value v = NULL;
  Instr instr = emitter_add_instr(emit, assign_func);
  if(rhs->exp_type == ae_exp_dot) {
    Type t = rhs->d.exp_dot.t_base;
    if(isa(t, t_class) > 0)
      t = t->d.base_type;
    v = find_value(t, rhs->d.exp_dot.xid);
    if(GET_FLAG(v, ae_flag_member))
        instr->m_val = 1;
  } else if(rhs->exp_type == ae_exp_primary) {
    if(GET_FLAG(rhs->d.exp_primary.value, ae_flag_member)) {
      v = rhs->d.exp_primary.value;
      instr->m_val = 1;
    }
  } else if(rhs->exp_type == ae_exp_decl)
      v = rhs->d.exp_decl.list->self->value;
  else return -1;
  instr->m_val2 = v ? v->offset : 0;
  return 1;
}

ANN static m_bool emit_binary_func(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  CHECK_BB(emit_exp(emit, lhs, 1))
  if(GET_FLAG(rhs->type->d.func->def, ae_flag_variadic)) {
    Exp_Func exp;
    exp.args = lhs;
    exp.m_func = rhs->type->d.func;
    emit_func_arg_vararg(emit, &exp);
  }
  CHECK_BB(emit_exp(emit, rhs, 1))
  return emit_exp_call1(emit, bin->func);
}

ANN static m_bool emit_exp_binary(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  struct Op_Import opi = { bin->op, lhs->type, rhs->type, NULL,
    NULL, NULL, (uintptr_t)bin };

  if(bin->op == op_chuck && isa(rhs->type, t_function) > 0)
    return emit_binary_func(emit, bin);
  CHECK_BB(emit_exp(emit, lhs, 1))
  CHECK_BB(emit_exp(emit, rhs, 1))
  CHECK_BB(op_emit(emit, &opi))
  return 1;
}

ANN static m_bool exp_exp_cast(const Emitter emit, const Exp_Cast* cast) {
  struct Op_Import opi = { op_dollar, cast->exp->type, cast->self->type, NULL,
    NULL, NULL, (uintptr_t)cast};
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  (void)op_emit(emit, &opi);
  return 1;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix* post) { GWDEBUG_EXE
  struct Op_Import opi;
  memset(&opi, 0, sizeof(struct Op_Import));
  opi.op = post->op;
  opi.lhs = post->exp->type;
  opi.data = (uintptr_t)post;
  CHECK_BB(emit_exp(emit, post->exp, 1))
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_dur(const Emitter emit, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, dur->base, 0))
  if(isa(dur->base->type, t_int) > 0)
    emitter_add_instr(emit, Cast_i2f);
  CHECK_BB(emit_exp(emit, dur->unit, 0))
  emitter_add_instr(emit, float_times);
  return 1;
}

ANN static Func emit_get_func(const Nspc nspc, const Func f) { GWDEBUG_EXE
  return isa(f->value_ref->type, t_func_ptr) > 0 ||
         isa(f->value_ref->type, t_class) > 0 ?
         nspc_lookup_func1(f->value_ref->owner, f->def->name) :
         nspc_lookup_func2(nspc, insert_symbol(f->name));
}

ANN static m_bool emit_exp_call_code_template(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  CHECK_BB(template_push_types(env, class_def->tmpl->list.list, class_def->tmpl->base))
  CHECK_BB(traverse_class_def(env, class_def))
  nspc_pop_type(env->curr);
  return 1;
}

ANN static m_bool emit_exp_call1_code(const Emitter emit, const Func func) { GWDEBUG_EXE
  if(!emit_get_func(emit->env->curr, func)) { GWDEBUG_EXE //template with no list
    Instr code;

    if(func->value_ref->owner_class &&
        GET_FLAG(func->value_ref->owner_class, ae_flag_template))
      CHECK_BB(emit_exp_call_code_template(emit->env,
            func->value_ref->owner_class->def))
    else if(!GET_FLAG(func->def, ae_flag_template)) {
      code = emitter_add_instr(emit, INSTR_RECURS);
      code->m_val = (m_uint)func;
      SET_FLAG(emit->code, ae_flag_recurs);
      return 1;
    }
    if(emit_func_def(emit, func->def) < 0)
      CHECK_BB(err_msg(EMIT_, 0, "can't emit func.")) // LCOV_EXCL_LINE
    code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code = func->def->func->code;
  } else {
    Instr code = emitter_add_instr(emit, Reg_Push_Code);
    code->m_val = func->value_ref->offset;
    code->m_val2 = func->value_ref->owner_class ? 1 : 0;
  }
  return 1;
}

ANN static m_bool emit_exp_call1_offset(const Emitter emit) { GWDEBUG_EXE
  Instr offset = emitter_add_instr(emit, Reg_Push_Imm);
  offset->m_val = SZ_INT;
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  return 1;
}

ANN m_bool emit_exp_call1_builtin(const Emitter emit, const Func func) { GWDEBUG_EXE
  Instr call;

  Type t = func->value_ref->type;
  if(isa(t, t_class) > 0)
    t = t->d.base_type;
  if(isa(t, t_func_ptr) < 0)
  if(!func->code || !func->code->native_func)
    CHECK_BB(err_msg(EMIT_, func->def->td->pos,
          "missing native func. are you trying to spork?"))
  call = emitter_add_instr(emit, GET_FLAG(func, ae_flag_member) ?
    Func_Member : Func_Static);
  call->m_val = func->def->ret_type->size;
  return 1;
}

ANN static m_bool emit_exp_call1_op(const Emitter emit, const Arg_List list) { GWDEBUG_EXE
  Instr call    = emitter_add_instr(emit, Call_Binary);
  call->m_val   = emit->code->stack_depth;
  call->m_val2  = (m_uint)list->type;
  *(Type*)call->ptr     = list->next ? list->next->type : NULL;
  return 1;
}

ANN static m_bool emit_exp_call1_usr(const Emitter emit) { GWDEBUG_EXE
  Instr call = emitter_add_instr(emit, Instr_Exp_Func);
  call->m_val = emit->code->stack_depth;
  return 1;
}

ANN m_bool emit_exp_call1(const Emitter emit, const Func func) { GWDEBUG_EXE
  GWCGRAPH_INI
#ifdef OPTIMIZE
  if(GET_FLAG(func, ae_flag_inline)) {
    SET_FLAG(func, ae_flag_recurs);
    SET_FLAG(emit->code, ae_flag_recurs);
    Instr instr = emitter_add_instr(emit, INSTR_INLINE);
    *(Func*)instr->ptr = func;
    instr->m_val = emit_code_offset(emit);
    return 1;
  }
#endif
  if(!func->code ||
      (GET_FLAG(func, ae_flag_ref) &&!GET_FLAG(func, ae_flag_builtin)))
    CHECK_BB(emit_exp_call1_code(emit, func))
  else {
    Instr code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code;
  }
  CHECK_BB(emit_exp_call1_offset(emit))
  if(GET_FLAG(func->def, ae_flag_builtin))
    CHECK_BB(emit_exp_call1_builtin(emit, func))
  else if(GET_FLAG(func->def, ae_flag_op))
    CHECK_BB(emit_exp_call1_op(emit, func->def->arg_list))
  else
    CHECK_BB(emit_exp_call1_usr(emit))
  GWCGRAPH_END(func->name)
  return 1;
}

ANN2(1,2) static m_bool emit_exp_spork_finish(const Emitter emit, const VM_Code code, const Func f,
    const m_uint arg_size, const m_uint stack_depth) {
  Instr push_code, spork;

  push_code = emitter_add_instr(emit, Reg_Push_Imm);
  push_code->m_val = SZ_INT;
  *(VM_Code*)push_code->ptr = code;
  spork = emitter_add_instr(emit, Spork);
  spork->m_val = arg_size;
  spork->m_val2 = (m_uint)f;
  *(m_uint*)spork->ptr = stack_depth; // only for some sporked expressions
  return 1;
}

ANN static m_uint emit_exp_spork_size(Exp e) { GWDEBUG_EXE
  m_uint size = 0;
  do size += e->cast_to ? e->cast_to->size : e->type->size;
  while((e = e->next));
  return size;
}

ANN m_bool emit_exp_spork(const Emitter emit, const Exp_Func* exp) { GWDEBUG_EXE
  Instr op;
  VM_Code code;
  m_uint size;

  CHECK_BB(emit_func_args(emit, exp))
  CHECK_BB(emit_exp(emit, exp->func, 0))
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  emitter_add_instr(emit, start_gc);
  if(GET_FLAG(exp->m_func, ae_flag_member))
    SET_FLAG(emit->code, ae_flag_member);
#ifdef OPTIMIZE
  if(GET_FLAG(exp->m_func, ae_flag_inline))
    SET_FLAG(emit->code, ae_flag_recurs);
#endif
  char c[11 + num_digit(exp->self->pos)];
  sprintf(c, "spork~exp:%i", exp->self->pos);
  emit->code->name = code_name_set(c, emit->filename);
  op = emitter_add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_exp_call1(emit, exp->m_func))
  emitter_add_instr(emit, stop_gc);
  emitter_add_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;
  code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  size = exp->args ? emit_exp_spork_size(exp->args) : 0;
  CHECK_BB(emit_exp_spork_finish(emit, code, NULL, size, 0)) // last arg migth have to be 'emit_code_offset(emit)'
  return 1;
}

ANN m_bool emit_exp_spork1(const Emitter emit, const Stmt stmt) { GWDEBUG_EXE
  Instr op;
  VM_Code code;
  const Symbol sporked = insert_symbol("sporked");
  const ID_List list = new_id_list(sporked, stmt->pos);
  const Func f = new_func("sporked", new_func_def(0, new_type_decl(list, 0, stmt->pos), sporked, NULL, stmt));

  if(emit->env->class_def)
    emitter_add_instr(emit, Reg_Push_This);
  Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  push->m_val = SZ_INT;
  *(Func*)push->ptr = f;

  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  if(emit->env->class_def) {
    SET_FLAG(f, ae_flag_member);
    SET_FLAG(emit->code, ae_flag_member);
  }
  char c[12 + num_digit(stmt->pos)];
  sprintf(c, "spork~code:%i", stmt->pos);
  emit->code->name = code_name_set(c, emit->filename);
  op = emitter_add_instr(emit, Mem_Push_Imm);
  emit_push_scope(emit);

  emitter_add_instr(emit, start_gc);
  CHECK_BB(emit_stmt(emit, stmt, 0))
  emitter_add_instr(emit, stop_gc);
  emit_pop_scope(emit);
  emitter_add_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;
  f->code = code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  CHECK_BB(emit_exp_spork_finish(emit, code, f, 0, emit->env->func ? emit->env->func->def->stack_depth : 0))
  return 1;
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary* unary) { GWDEBUG_EXE
  struct Op_Import opi;
  memset(&opi, 0, sizeof(struct Op_Import));
  opi.op = unary->op;
  opi.data = (uintptr_t)unary;
  if(unary->exp) {
    CHECK_BB(emit_exp(emit, unary->exp, 1))
    opi.rhs = unary->exp->type;
  }
  return op_emit(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter emit, const Type from, const Type to) { GWDEBUG_EXE
  struct Op_Import opi = { op_implicit, from, to, NULL,
         NULL, NULL, (m_uint)from };
  op_emit(emit, &opi);
  return 1;
}

ANN static Instr emit_flow(const Emitter emit, const Type type, const f_instr f1, const f_instr f2) { GWDEBUG_EXE
  Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  if(isa(type, t_int) > 0) {
    push->m_val = SZ_INT;
    return emitter_add_instr(emit, f1);
  }
  push->m_val = SZ_FLOAT;
  return emitter_add_instr(emit, f2);
}

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If* exp_if) { GWDEBUG_EXE
  m_bool ret;
  Instr op = NULL, op2 = NULL;
  nspc_push_value(emit->env->curr);
  CHECK_BB(emit_exp(emit, exp_if->cond, 0))
  CHECK_OB((op = emit_flow(emit, exp_if->cond->type, Branch_Eq_Int, Branch_Eq_Float)))
  CHECK_BB((ret = emit_exp(emit, exp_if->if_exp, 0)))
  CHECK_OB((op2 = emitter_add_instr(emit, get_goto(emit->env))))
  op->m_val = emit_code_size(emit);
  ret = emit_exp(emit, exp_if->else_exp, 0);
  nspc_pop_value(emit->env->curr);
  CHECK_OB(ret)
  op2->m_val = emit_code_size(emit);
  return ret;
}

#ifdef OPTIMIZE
ANN static void emit_exp_constprop(const Emitter emit, const Exp e) {
  if(!e->emit_var) {
    if(e->exp_type == ae_exp_constprop) {
      const Instr instr = emitter_add_instr(emit, Reg_Push_Mem);
      instr->m_val = e->d.exp_primary.value->offset;
      instr->m_val2 = SZ_INT;
      *(m_uint*)instr->ptr = GET_FLAG(e->d.exp_primary.value, ae_flag_global);
      return;
    }
    const Instr instr = emitter_add_instr(emit, ConstPropGet);
    instr->m_val2 = e->d.exp_primary.value->offset;
    instr->m_val = e->d.exp_primary.d.num;
    *(m_uint*)instr->ptr = 1;
  } else {
    const Instr instr = emitter_add_instr(emit, ConstPropSet);
    instr->m_val = e->d.exp_primary.value->offset;
    *(m_bool*)instr->ptr = 1;
    instr->m_val2 = e->d.exp_primary.d.num;
  }
}
#endif

ANN2(1) static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool ref) { GWDEBUG_EXE
  do {
    switch(exp->exp_type) {
      case ae_exp_decl:
        CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))       break;
      case ae_exp_primary:
        CHECK_BB(emit_exp_primary(emit, &exp->d.exp_primary)) break;
      case ae_exp_unary:
        CHECK_BB(emit_exp_unary(emit, &exp->d.exp_unary))     break;
      case ae_exp_binary:
        CHECK_BB(emit_exp_binary(emit, &exp->d.exp_binary))   break;
      case ae_exp_post:
        CHECK_BB(emit_exp_post(emit, &exp->d.exp_post)) break;
      case ae_exp_cast:
        CHECK_BB(exp_exp_cast(emit, &exp->d.exp_cast))        break;
      case ae_exp_dot:
        CHECK_BB(emit_exp_dot(emit, &exp->d.exp_dot))         break;
      case ae_exp_call:
        CHECK_BB(emit_exp_call(emit, &exp->d.exp_func, 0))    break;
      case ae_exp_array:
        CHECK_BB(emit_exp_array(emit, &exp->d.exp_array))     break;
      case ae_exp_if:
        CHECK_BB(emit_exp_if(emit, &exp->d.exp_if))           break;
      case ae_exp_dur:
        CHECK_BB(emit_exp_dur(emit, &exp->d.exp_dur))         break;
#ifdef OPTIMIZE
      case ae_exp_constprop2:
      case ae_exp_constprop:
        emit_exp_constprop(emit, exp);
        break;
#endif
    }
    if(exp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp->type, exp->cast_to))
      if(ref && isa(exp->type, t_object) > 0 && isa(exp->type, t_void) < 0) {
        Instr ref = emitter_add_instr(emit, Reg_Add_Ref);
        ref->m_val = exp->emit_var;
      }
  } while((exp = exp->next));
  return 1;
}

ANN static m_bool emit_stmt_if(const Emitter emit, const Stmt_If stmt) { GWDEBUG_EXE
  Instr op = NULL, op2 = NULL;

  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  COVERAGE(stmt->cond)
  CHECK_OB((op = emit_flow(emit, isa(stmt->cond->type, t_object) > 0 ? t_int : stmt->cond->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->if_body, 1))
  emit_pop_scope(emit);
  op2 = emitter_add_instr(emit, get_goto(emit->env));
  op->m_val = emit_code_size(emit);

  if(stmt->else_body) {
    emit_push_scope(emit);
    CHECK_BB(emit_stmt(emit, stmt->else_body, 1))
    emit_pop_scope(emit);
  }
  op2->m_val = emit_code_size(emit);
  emit_pop_scope(emit);
  return 1;
}

//ANN static m_bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt, const m_bool push) { GWDEBUG_EXE
ANN static m_bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt) { GWDEBUG_EXE
  return stmt->stmt_list ? emit_stmt_list(emit, stmt->stmt_list) : 1;
}

ANN static void emit_func_release(const Emitter emit) { GWDEBUG_EXE
  const Vector v = &emit->code->frame->stack;
  for(m_uint i = vector_size(v); i; i--) {
    const Local* l = (Local*)vector_at(v, i - 1);
    if(l && l->is_obj) {
      Instr rel = emitter_add_instr(emit, Release_Object);
      rel->m_val = l->offset;
    }
  }
}

#ifdef OPTIMIZE
ANN static m_bool optimize_taill_call(const Emitter emit, const Exp_Func* e) {
  Exp arg = e->args;
  CHECK_BB(emit_exp(emit, e->args, 0))
  Instr instr = emitter_add_instr(emit, PutArgsInMem);
  while(arg) {
    instr->m_val += arg->type->size;
    arg = arg->next;
  }
  emitter_add_instr(emit, get_goto(emit->env));
  return 1;
}
#define OPTIMIZE_TCO\
  if(stmt->val->exp_type == ae_exp_call && emit->env->func == stmt->val->d.exp_func.m_func)\
    return optimize_taill_call(emit, &stmt->val->d.exp_func);
#else
#define OPTIMIZE_TCO
#endif


ANN static m_bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(stmt->val) {
    OPTIMIZE_TCO
    CHECK_BB(emit_exp(emit, stmt->val, 0))
  }
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, get_goto(emit->env)));
  return 1;
}

ANN static m_bool emit_stmt_continue(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
  vector_add(&emit->code->stack_cont, (vtype)emitter_add_instr(emit, get_goto(emit->env)));
  return 1;
}

ANN static m_bool emit_stmt_break(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
  vector_add(&emit->code->stack_break, (vtype)emitter_add_instr(emit, get_goto(emit->env)));
  return 1;
}

ANN static void emit_push_stack(const Emitter emit) { GWDEBUG_EXE
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

ANN static void emit_pop_stack(const Emitter emit, const m_uint index) { GWDEBUG_EXE
  while(vector_size(&emit->code->stack_cont) && vector_back(&emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(&emit->code->stack_cont);
    instr->m_val = index;
  }
  while(vector_size(&emit->code->stack_break) && vector_back(&emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(&emit->code->stack_break);
    instr->m_val = emit_code_size(emit);
  }
  emit_pop_scope(emit);
  vector_pop(&emit->code->stack_cont);
  vector_pop(&emit->code->stack_break);
}

ANN static m_bool emit_stmt_while(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  m_bool ret = 1;
  const m_uint index = emit_code_size(emit);
  Instr op, goto_;

  emit_push_scope(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  COVERAGE(stmt->cond)
  CHECK_OB((op = emit_flow(emit, stmt->cond->type, Branch_Eq_Int, Branch_Eq_Float)))

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  goto_ = emitter_add_instr(emit, get_goto(emit->env));
  goto_->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static m_bool emit_stmt_do_while(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  m_bool ret = 1;
  Instr op;
  const m_uint index = emit_code_size(emit);

  emit_push_scope(emit);
  emit_push_stack(emit);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Neq_Int, Branch_Neq_Float)))
  op->m_val = index;
  emit_pop_stack(emit, index);
  return ret;
}

ANN static m_bool emit_stmt_until(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  Instr op;
  m_uint index;

  emit_push_scope(emit);
  index = emit_code_size(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Neq_Int, Branch_Neq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  Instr _goto = emitter_add_instr(emit, get_goto(emit->env));
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_do_until(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  Instr op;
  m_uint index;

  emit_push_scope(emit);
  index = emit_code_size(emit);
  emit_push_stack(emit);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  op->m_val = index;
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) { GWDEBUG_EXE
  Instr  op = NULL;
  m_uint index;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  index = emit_code_size(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c2, 0))
  CHECK_OB((op = emit_flow(emit, stmt->c2->d.stmt_exp.val->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  m_uint action_index = emit_code_size(emit);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3, 0))
    Exp e = stmt->c3;
    m_uint num_words = 0;
    while(e) {
      num_words += e->type->size;
      e = e->next;
    }
    if(num_words) {
      Instr pop = emitter_add_instr(emit, Reg_Pop_Word);
      pop->m_val = num_words;
    }
  }
  Instr _goto = emitter_add_instr(emit, get_goto(emit->env));
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, action_index);
  return 1;
}

ANN static m_bool emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt) { GWDEBUG_EXE
  m_uint start, offset, index;
  Instr s1, s2, loop, tgt, end;

  CHECK_BB(emit_exp(emit, stmt->exp, 0))
  s1 = emitter_add_instr(emit, Mem_Set_Imm);
  s2 = emitter_add_instr(emit, Mem_Set_Imm);
  start  = emit_code_size(emit);
  emit_push_stack(emit);
  loop = emitter_add_instr(emit, AutoLoopStart);
  offset = emit_alloc_local(emit, 2*SZ_INT, 0);
  stmt->v->offset = offset + SZ_INT;
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  index = emit_code_size(emit);
  end = emitter_add_instr(emit, AutoLoopEnd);
  tgt = emitter_add_instr(emit, get_goto(emit->env));
  end->m_val2 = emit_code_size(emit);
  tgt->m_val = start;
  loop->m_val = offset;
  end->m_val = offset;
  s1->m_val = offset;
  s2->m_val = offset + SZ_INT;
  if(stmt->is_ptr) {
    *(Type*)loop->ptr = stmt->v->type;
    *(Type*)end->ptr = stmt->v->type;
  }
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) { GWDEBUG_EXE
  Instr init, op, deref, dec, _goto, push;
  m_int* counter;
  m_uint index;

  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  counter = (m_int*)xcalloc(1, SZ_INT);
  init = emitter_add_instr(emit, Init_Loop_Counter);
  init->m_val = (m_uint)counter;
  index = emit_code_size(emit);
  emit_push_stack(emit);
  deref = emitter_add_instr(emit, Reg_Push_Deref);
  deref->m_val = (m_uint)counter;

  CHECK_OB((push = emitter_add_instr(emit, Reg_Push_Imm)))
  push->m_val = SZ_INT;
  op = emitter_add_instr(emit, Branch_Eq_Int);
  dec = emitter_add_instr(emit, Dec_int_Addr);
  dec->m_val = (m_uint)counter;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  _goto = emitter_add_instr(emit, get_goto(emit->env));
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_gotolabel(const Emitter emit, const Stmt_Goto_Label stmt) { GWDEBUG_EXE
  m_uint i, size;
  Stmt_Goto_Label label;
  if(!stmt->is_label)
    stmt->data.instr = emitter_add_instr(emit, get_goto(emit->env));
  else {
    if(emit->cases && !strcmp(s_name(stmt->name), "default")) {
      if(emit->default_case_index != -1)
        CHECK_BB(err_msg(EMIT_, stmt->self->pos, "default case already defined"))
        emit->default_case_index = emit_code_size(emit);
      return 1;
    }
    if(!stmt->data.v.ptr) {
      emit->cases = NULL; // check me (mem leak?)
      CHECK_BB(err_msg(EMIT_, stmt->self->pos, "illegal case"))
    }
    size = vector_size(&stmt->data.v);
    if(!size) {
      vector_release(&stmt->data.v);
      CHECK_BB(err_msg(EMIT_, stmt->self->pos, "label '%s' defined but not used.", s_name(stmt->name)))
    }
    LOOP_OPTIM
    for(i = size + 1; --i;) {
      label = (Stmt_Goto_Label)vector_at(&stmt->data.v, i - 1);
      label->data.instr->m_val = emit_code_size(emit);
    }
    vector_release(&stmt->data.v);
  }
  return 1;
}

ANN static m_bool emit_stmt_switch(const Emitter emit, const Stmt_Switch stmt) { GWDEBUG_EXE
  Instr instr, _break;
  vector_add(&emit->code->stack_break, (vtype)NULL);
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  if(emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "swith inside an other switch. this is not allowed for now"))
    emit->default_case_index = -1;
  emitter_add_instr(emit, start_gc);
  instr = emitter_add_instr(emit, Branch_Switch);
  emit->cases = new_map();
  *(Map*)instr->ptr = emit->cases;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->stmt, 1))
  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? (m_uint)emit->default_case_index : emit_code_size(emit);
  emit->default_case_index = -1;
  while(vector_size(&emit->code->stack_break) && vector_back(&emit->code->stack_break)) {
    _break = (Instr)vector_pop(&emit->code->stack_break);
    _break->m_val = emit_code_size(emit);
  }
  vector_pop(&emit->code->stack_break);
  emitter_add_instr(emit, stop_gc);
  emit->cases = NULL;
  return 1;
}

ANN static m_bool primary_case(const Exp_Primary* prim, m_int* value) {
  if(prim->primary_type == ae_primary_num)
    *value = prim->d.num;
  else if(prim->d.var == insert_symbol("true"))
    *value = 1;
  else if(prim->d.var == insert_symbol("false"))
    *value = 0;
  else if(prim->d.var == insert_symbol("maybe"))
    CHECK_BB(err_msg(EMIT_, prim->self->pos, "'maybe' is not constant."))
  else {
    if(!GET_FLAG(prim->value, ae_flag_const))
      CHECK_BB(err_msg(EMIT_, prim->self->pos,
            "value is not constant."))
    *value = (m_uint)prim->value->d.ptr; // assume enum.
  }
  return 1;
}

ANN static m_int get_case_value(const Stmt_Exp stmt, m_int* value) {
  if(stmt->val->exp_type == ae_exp_primary)
    CHECK_BB(primary_case(&stmt->val->d.exp_primary, value))
  else {
    Type t = isa(stmt->val->d.exp_dot.t_base, t_class) > 0 ?
        stmt->val->d.exp_dot.t_base->d.base_type :
        stmt->val->d.exp_dot.t_base;
    Value v = find_value(t, stmt->val->d.exp_dot.xid);
    *value = GET_FLAG(v, ae_flag_enum) ? !GET_FLAG(v, ae_flag_builtin) ?
      (m_uint)t->info->class_data[v->offset] : (m_uint)v->d.ptr : *(m_uint*)v->d.ptr;
  }
  return 1;
}

ANN static m_bool emit_stmt_case(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  m_int value = 0;
  if(!emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "case found outside switch statement. this is not allowed for now"))
  CHECK_BB(get_case_value(stmt, &value))
  if(map_get(emit->cases, (vtype)value))
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "duplicated cases value %i", value))
  map_set(emit->cases, (vtype)value, (vtype)emit_code_size(emit));
  return 1;
}

ANN static m_bool emit_stmt_fptr(const Emitter emit __attribute__((unused)), const Stmt_Ptr ptr) { GWDEBUG_EXE
  if(GET_FLAG(ptr, ae_flag_static))
    ADD_REF(ptr->func)
  return 1;
}

ANN static m_bool emit_stmt_type(const Emitter emit, const Stmt_Typedef stmt) { GWDEBUG_EXE
  return stmt->type->def ? emit_class_def(emit, stmt->type->def) : 1;
}

ANN static m_bool emit_stmt_enum(const Emitter emit, const Stmt_Enum stmt) { GWDEBUG_EXE
  m_uint i;
  LOOP_OPTIM
  for(i = 0; i < vector_size(&stmt->values); i++) {
    Value v = (Value)vector_at(&stmt->values, i);
    if(!emit->env->class_def) {
      m_uint offset = emit_alloc_local(emit, SZ_INT, 0);
      v->offset = offset;
      v->d.ptr = (m_uint*)i;
    } else
      emit->env->class_def->info->class_data[v->offset] = i;
  }
  return 1;
}

ANN void emit_union_offset(Decl_List l, const m_uint o) { GWDEBUG_EXE
  do {
    Var_Decl_List v = l->self->d.exp_decl.list;
    do v->self->value->offset = o;
    while((v = v->next));
  } while((l = l->next));
}

ANN static m_bool emit_stmt_union(const Emitter emit, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;

  if(stmt->xid) {
    if(!stmt->value->type->info->class_data)
      stmt->value->type->info->class_data =
        (m_bit*)xcalloc(1, stmt->value->type->info->class_data_size);
    Type_Decl *type_decl = new_type_decl(new_id_list(stmt->xid, stmt->self->pos),
        0, emit->env->class_def ? ae_flag_member : 0);
    type_decl->flag = stmt->flag;
    Var_Decl var_decl = new_var_decl(stmt->xid, NULL, 0);
    Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL);
    Exp exp = new_exp_decl(type_decl, var_decl_list, 0);
    exp->d.exp_decl.type = stmt->value->type;
    var_decl->value = stmt->value;
    CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))
    if(!emit->env->class_def)
      ADD_REF(stmt->value);
    free_exp(exp);
    env_push_class(emit->env, stmt->value->type);
  }
  else if(!GET_FLAG(l->self->d.exp_decl.list->self->value, ae_flag_member))
    stmt->o = emit_alloc_local(emit, stmt->s, IS_REF);
  emit_union_offset(stmt->l, stmt->o);
  if(stmt->xid) {
    Instr instr = emitter_add_instr(emit, Reg_Pop_Word);
    instr->m_val = SZ_INT;
    env_pop_class(emit->env);
  }
  return 1;
}

ANN static m_uint get_decl_size(Var_Decl_List a) {
  m_uint size = 0;
  do size += a->self->value->type->size;
  while((a = a->next));
  return size;
}

ANN static m_bool emit_stmt_exp(const Emitter emit, const struct Stmt_Exp_* exp, const m_bool pop) { GWDEBUG_EXE
  int ret;
  if(!exp->val)
    return 1;
  ret = emit_exp(emit, exp->val, 0);
  if(ret > 0 && pop && exp->val->type && exp->val->type->size > 0) {
    Exp e = exp->val;
    if(e->exp_type == ae_exp_primary &&
        e->d.exp_primary.primary_type == ae_primary_hack)
      e = e->d.exp_primary.d.exp;
    while(e) {
      Instr instr = emitter_add_instr(emit, Reg_Pop_Word);
      instr->m_val = (e->exp_type == ae_exp_decl ?
        get_decl_size(e->d.exp_decl.list) : e->type->size);
      e = e->next;
    }
  }
 return ret;
}
#ifdef CURSES
static INSTR(dbg_pos) {
  gw_pos(instr->m_val);
}
#endif
ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) { GWDEBUG_EXE
  if(stmt->stmt_type != ae_stmt_if || stmt->stmt_type != ae_stmt_while)
    COVERAGE(stmt)
#ifdef CURSES
  {
    Instr instr = emitter_add_instr(emit, dbg_pos);
    instr->m_val = stmt->pos;
  }
#endif
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      return emit_stmt_exp(emit, &stmt->d.stmt_exp, pop);
    case ae_stmt_code:
//      return emit_stmt_code(emit, &stmt->d.stmt_code, 1);
      return emit_stmt_code(emit, &stmt->d.stmt_code);
    case ae_stmt_if:
      return emit_stmt_if(emit, &stmt->d.stmt_if);
    case ae_stmt_return:
      return emit_stmt_return(emit, &stmt->d.stmt_exp);
    case ae_stmt_break:
      return emit_stmt_break(emit, stmt);
    case ae_stmt_continue:
      return emit_stmt_continue(emit, stmt);
    case ae_stmt_while:
      return stmt->d.stmt_flow.is_do ? emit_stmt_do_while(emit, &stmt->d.stmt_flow) :
            emit_stmt_while(emit, &stmt->d.stmt_flow);
    case ae_stmt_until:
      return stmt->d.stmt_flow.is_do ? emit_stmt_do_until(emit, &stmt->d.stmt_flow) :
            emit_stmt_until(emit, &stmt->d.stmt_flow);
    case ae_stmt_for:
      return emit_stmt_for(emit, &stmt->d.stmt_for);
    case ae_stmt_auto:
      return emit_stmt_auto(emit, &stmt->d.stmt_auto);
    case ae_stmt_loop:
      return emit_stmt_loop(emit, &stmt->d.stmt_loop);
    case ae_stmt_gotolabel:
      return emit_stmt_gotolabel(emit, &stmt->d.stmt_gotolabel);
    case ae_stmt_case:
      return emit_stmt_case(emit, &stmt->d.stmt_exp);
    case ae_stmt_enum:
      return emit_stmt_enum(emit, &stmt->d.stmt_enum);
    case ae_stmt_switch:
      return emit_stmt_switch(emit, &stmt->d.stmt_switch);
    case ae_stmt_funcptr:
      return emit_stmt_fptr(emit, &stmt->d.stmt_ptr);
    case ae_stmt_typedef:
      return emit_stmt_type(emit, &stmt->d.stmt_type);
    case ae_stmt_union:
      return emit_stmt_union(emit, &stmt->d.stmt_union);
  }
  return 1;
}

ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(emit_stmt(emit, l->stmt, 1))
  while((l = l->next));
  return 1;
}

ANN static m_bool is_special(const Type t) {
  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0 ||
     isa(t, t_vec3)    > 0 || isa(t, t_vec4)  > 0  ||
     isa(t, t_vararg)  > 0)
    return 1;
  return -1;
}

ANN static m_bool emit_dot_static_import_data(const Emitter emit, const Value v, const m_bool emit_addr) { GWDEBUG_EXE
  Instr func_i;

  if(v->d.ptr && GET_FLAG(v, ae_flag_builtin)) { // from C
    if(GET_FLAG(v, ae_flag_enum)) {
      func_i = emitter_add_instr(emit, Reg_Push_Imm);
      *(m_uint*)func_i->ptr = (m_uint)v->d.ptr;
      func_i->m_val = SZ_INT;
    } else {
      func_i = emitter_add_instr(emit, Dot_Static_Import_Data);
      func_i->m_val = (isa(v->type, t_object) > 0 ?
        (m_uint)&v->d.ptr : (m_uint)v->d.ptr);
      func_i->m_val2 = emit_addr ? SZ_INT : v->type->size;
      *(m_uint*)func_i->ptr = emit_addr;
    }
  } else { // from code
    Instr push_i = emitter_add_instr(emit, Reg_Push_Imm);
    func_i = emitter_add_instr(emit, Dot_Static_Data);
    push_i->m_val = SZ_INT;
    *(Type*)push_i->ptr = v->owner_class;
    func_i->m_val = (m_uint)v->offset;
    func_i->m_val2 = emit_addr ? SZ_INT : v->type->size;
    *(m_uint*)func_i->ptr = emit_addr;
  }
  return 1;
}

ANN static m_bool emit_complex_member(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  Instr instr;
  Exp base = member->base;

  base->emit_var = 1;
  CHECK_BB(emit_exp(emit, base, 0))
  if(!strcmp((isa(base->type, t_complex) > 0  ? "re" : "phase") ,
        s_name(member->xid)))
    instr = emitter_add_instr(emit, complex_real);
  else
    instr = emitter_add_instr(emit, complex_imag);
  instr->m_val = member->self->emit_var;
  return 1;
}

ANN static m_bool emit_vec_func(const Emitter emit, const Value v) { GWDEBUG_EXE
  Instr instr;
  emitter_add_instr(emit, Reg_Dup_Last);
  instr = emitter_add_instr(emit, member_function);
  *(Vector*)instr->ptr = &v->owner_class->info->vtable;
  instr->m_val = v->d.func_ref->vt_index;
  return 1;
}

ANN static m_bool emit_vec_member(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  Instr instr;
  Value v;

  member->base->emit_var = 1;
  CHECK_BB(emit_exp(emit, member->base, 0))
  v = find_value(member->base->type, member->xid);
  if(GET_FLAG(v, ae_flag_func))
    return emit_vec_func(emit, v);
  instr = emitter_add_instr(emit, vec_member);
  instr->m_val2 = v->offset;
  instr->m_val = member->self->emit_var;
  return 1;
}

ANN static m_bool emit_vararg_start(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start already used. this is an error"))
  emit->env->func->variadic = emitter_add_instr(emit, Vararg_start);
  emit->env->func->variadic->m_val = offset;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  return 1;
}

ANN static m_bool emit_vararg_end(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(!emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error"))
  Instr instr = emitter_add_instr(emit, Vararg_end);
  instr->m_val = offset;
  instr->m_val2 = emit->env->func->variadic->m_val2;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  *(m_uint*)emit->env->func->variadic->ptr = 1;
  return 1;
}

ANN static m_bool emit_vararg(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  Instr instr;
  m_uint offset = emit->env->class_def ? SZ_INT : 0;
  Arg_List l = emit->env->func->def->arg_list;
  const m_str str = s_name(member->xid);
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(str, "start"))
    return emit_vararg_start(emit, offset);
  if(!strcmp(str, "end"))
    return emit_vararg_end(emit, offset);
  instr = emitter_add_instr(emit, Vararg_Member);
  instr->m_val = offset;
  instr->m_val2 = member->self->type->size;
  return 1;
}

ANN static m_bool emit_exp_dot_special(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t = member->t_base;

  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0)
    return emit_complex_member(emit, member);
  else if(isa(t, t_vec3) > 0 || isa(t, t_vec4) > 0)
    return emit_vec_member(emit, member);
  return emit_vararg(emit, member);
}

ANN static m_bool emit_dot_static_func(const Emitter emit, const Type type, const Func func) { GWDEBUG_EXE
  Instr push_i = emitter_add_instr(emit, Reg_Push_Imm);
  Instr func_i = emitter_add_instr(emit, Dot_Static_Func);
  push_i->m_val = SZ_INT;
  *(Type*)push_i->ptr = type;
  func_i->m_val = (m_uint)func;
  return 1;
}

ANN static m_bool emit_member_func(const Emitter emit, const Exp_Dot* member, const Func func) { GWDEBUG_EXE
  if(GET_FLAG(func, ae_flag_member)) {
    Instr func_i;
    if(emit_exp(emit, member->base, 0) < 0)
      CHECK_BB(err_msg(EMIT_, member->self->pos, "... in member function")) // LCOV_EXCL_LINE
    emitter_add_instr(emit, Reg_Dup_Last);
    func_i = emitter_add_instr(emit, Exp_Dot_Func);
    func_i->m_val = func->vt_index;
  }
  return 1;
}

ANN static m_bool emit_member(const Emitter emit, const Value v, const m_bool emit_addr) { GWDEBUG_EXE
  Instr func_i = emitter_add_instr(emit, Exp_Dot_Data);
  func_i->m_val = v->offset;
  func_i->m_val2 = v->type->size;
  *(m_uint*)func_i->ptr = emit_addr;
  return 1;
}

ANN static m_bool emit_exp_dot_instance(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base;
  const Value value = find_value(t_base, member->xid);
  const m_bool emit_addr = member->self->emit_var;
  if(isa(member->self->type, t_func_ptr) > 0) { // function pointer
    if(GET_FLAG(value, ae_flag_member)) { // member
      if(emit_exp(emit, member->base, 0) < 0)
        CHECK_BB(err_msg(EMIT_, member->self->pos, "... in member function")) // LCOV_EXCL_LINE
      emitter_add_instr(emit, Reg_Dup_Last);
      return emit_member(emit, value, emit_addr);
    } else
      return emit_dot_static_data(emit, value, emit_addr);
  } else if(isa(member->self->type, t_function) > 0) { // function
    const Func func = value->d.func_ref;
    if(GET_FLAG(func, ae_flag_member))
      return emit_member_func(emit, member, func);
    else
      return emit_dot_static_func(emit, t_base, func);
  } else { // variable
    if(GET_FLAG(value, ae_flag_member)) { // member
      CHECK_BB(emit_exp(emit, member->base, 0))
      return emit_member(emit, value, emit_addr);
    } else // static
      CHECK_BB(emit_dot_static_import_data(emit, value, emit_addr))
  }
  return 1;
}

ANN static m_bool emit_exp_dot_static(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base->d.base_type;
  const Value value = find_value(t_base, member->xid);

  if(isa(member->self->type, t_func_ptr) > 0)
    return emit_dot_static_import_data(emit, value, member->self->emit_var);
  if(isa(member->self->type, t_function) > 0)
    return emit_dot_static_func(emit, t_base, value->d.func_ref);
  return emit_dot_static_import_data(emit, value, member->self->emit_var);
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  if(is_special(member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  if(isa(member->t_base, t_class) > 0)
    return emit_exp_dot_static(emit, member);
  return emit_exp_dot_instance(emit, member);
}

ANN static m_bool emit_func_def_global(const Emitter emit, const Value value) { GWDEBUG_EXE
  Instr set_mem = emitter_add_instr(emit, Mem_Set_Imm);
  set_mem->m_val = value->offset = emit_alloc_local(emit, value->type->size, IS_REF);
  *(Func*)set_mem->ptr = value->d.func_ref;
  return 1;
}

ANN static m_bool emit_func_def_init(const Emitter emit, const Func func) { GWDEBUG_EXE
  const Type t = emit->env->class_def;
  char c[(t ? strlen(t->name) + 1 : 0) + strlen(func->name) + 6];
  emit->env->func = func;
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  sprintf(c, "%s%s%s(...)", t ? t->name : "", t ? "." : "", func->name);
  emit->code->name = code_name_set(c, emit->filename);
  if(GET_FLAG(func, ae_flag_member))
    SET_FLAG(emit->code, ae_flag_member);
  return 1;
}

ANN static m_bool emit_func_def_flag(const Emitter emit, const Func func) { GWDEBUG_EXE
  if(GET_FLAG(func, ae_flag_member)) {
    emit->code->stack_depth += SZ_INT;
    emit_alloc_local(emit, SZ_INT, IS_REF);
  }
  return 1;
}

ANN static m_bool emit_func_def_args(const Emitter emit, Arg_List a) { GWDEBUG_EXE
  do {
    Value value = a->var_decl->value;
    m_uint offset, size = value->type->size;
    m_bool obj = isa(value->type, t_object) > 0 ? IS_OBJ : 0;
    m_bool ref = GET_FLAG(a->td, ae_flag_ref) ? IS_REF : 0;
    emit->code->stack_depth += size;
    offset = emit_alloc_local(emit, size, ref | obj);
    value->offset = offset;
  } while((a = a->next));
  return 1;
}

ANN static void emit_func_def_ensure(const Emitter emit, const m_uint size) { GWDEBUG_EXE
  if(size) {
    Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
    instr->m_val = size;
  }
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, get_goto(emit->env)));
}

ANN static m_bool emit_func_def_return(const Emitter emit) { GWDEBUG_EXE
  m_uint i, val = emit_code_size(emit);
  LOOP_OPTIM
  for(i = vector_size(&emit->code->stack_return) + 1; --i; ) {
    Instr instr = (Instr)vector_at(&emit->code->stack_return, i-1);
    instr->m_val = val;
  }
  vector_clear(&emit->code->stack_return);
  emit_func_release(emit);
  emitter_add_instr(emit, Func_Return);
  return 1;
}

ANN static m_bool emit_func_def_code(const Emitter emit, const Func func) { GWDEBUG_EXE
  func->code = emit_code(emit);
  if(GET_FLAG(func->def, ae_flag_dtor)) {
    emit->env->class_def->info->dtor = func->code;
    ADD_REF(func->code)
  } else if(GET_FLAG(func->def, ae_flag_op)) {
    const Arg_List a = func->def->arg_list;
    const m_bool is_unary = GET_FLAG(func->def, ae_flag_unary);
    const Type l = is_unary ? NULL : a->type;
    const Type r = is_unary ? a->type : a->next ? a->next->type : NULL;
    struct Op_Import opi = { name2op(s_name(func->def->name)), l, r, NULL,
      NULL, NULL, 0 };
    opi.data = (m_uint)func;
    operator_set_func(&opi);
  }
  return 1;
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  if(func_def->arg_list)
    CHECK_BB(emit_func_def_args(emit, func_def->arg_list))
  if(GET_FLAG(func_def, ae_flag_variadic)) {
    emit_alloc_local(emit, SZ_INT, IS_REF);
    emit->code->stack_depth += SZ_INT;
  }
  CHECK_BB(emit_stmt(emit, func_def->d.code, 0))
  emit_func_def_ensure(emit, func_def->ret_type->size);
  return 1;
}

ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  const Func func = get_func(emit->env, func_def);
  if(tmpl_list_base(func_def->tmpl)) { // don't check template definition
    func_def->flag &= ~ae_flag_template;
    return 1;
  }
  if(!emit->env->class_def)
    CHECK_BB(emit_func_def_global(emit, func->value_ref))
  else if(GET_FLAG(emit->env->class_def, ae_flag_builtin) && GET_FLAG(emit->env->class_def, ae_flag_template))
    return 1;
  CHECK_BB(emit_func_def_init(emit, func))
  CHECK_BB(emit_func_def_flag(emit, func))
  emit_push_scope(emit);
  CHECK_BB(emit_func_def_body(emit, func_def))
  if(GET_FLAG(func_def, ae_flag_variadic) && (!emit->env->func->variadic ||
      !*(m_uint*)emit->env->func->variadic->ptr))
    CHECK_BB(err_msg(EMIT_, func_def->td->pos, "invalid variadic use"))
  CHECK_BB(emit_func_def_return(emit))
  emit_pop_scope(emit);
  CHECK_BB(emit_func_def_code(emit, func))
  emit->env->func = NULL;
  emit->code = (Code*)vector_pop(&emit->stack);
  return 1;
}

ANN static m_bool emit_section(const Emitter emit, const Section* section) { GWDEBUG_EXE
  const ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(emit_stmt_list(emit, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(emit_func_def(emit, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(emit_class_def(emit, section->d.class_def))
  return 1;
}

ANN Code* emit_class_code(const Emitter emit, const m_str name) { GWDEBUG_EXE
  const m_uint len = strlen(name) + 7;
  char c[len];
  Code* code = new_code();
  snprintf(c, len, "class %s", name);
  code->name = code_name_set(c, emit->filename);
  SET_FLAG(code, ae_flag_member);
  code->stack_depth += SZ_INT;
  return code;

}

ANN void emit_class_finish(const Emitter emit, const Nspc nspc) { GWDEBUG_EXE
  emitter_add_instr(emit, Func_Return);
  nspc->pre_ctor = emit_code(emit);
}

ANN static m_bool emit_class_push(const Emitter emit, const Type type) { GWDEBUG_EXE
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
  vector_add(&emit->stack, (vtype)emit->code);
  return 1;
}

ANN static void emit_class_pop(const Emitter emit) { GWDEBUG_EXE
  emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
  emit->code = (Code*)vector_pop(&emit->stack);
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def class_def) { GWDEBUG_EXE
  const Type type = class_def->type;
  const Nspc nspc = type->info;
  Class_Body body = class_def->body;

  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext && ((!GET_FLAG(type->parent, ae_flag_emit) &&
      GET_FLAG(class_def->ext, ae_flag_typedef)) || class_def->ext->types)) {
    Type base = class_def->ext->array ?
             array_base(type->parent) : type->parent;
    CHECK_BB(emit_class_def(emit, base->def))
  }
  if(nspc->class_data_size)
    nspc->class_data = (m_bit*)xcalloc(1, nspc->class_data_size);
  CHECK_BB(emit_class_push(emit, type))
  CHECK_OB((emit->code = emit_class_code(emit, type->name)))

  if(class_def->ext && class_def->ext->array)
      CHECK_BB(emit_array_extend(emit, type->parent,
            class_def->ext->array->exp))
  emit_alloc_local(emit, SZ_INT, IS_REF | IS_OBJ);
  while(body) {
    CHECK_BB(emit_section(emit, body->section))
    body = body->next;
  }
  emit_class_finish(emit, nspc);
  emit_class_pop(emit);
  SET_FLAG(type, ae_flag_emit);
  return 1;
}

ANN static void emit_free_stack(const Emitter emit) { GWDEBUG_EXE
  m_uint i, j;
  LOOP_OPTIM
  for(i = 0;  i < vector_size(&emit->stack); i++) {
    Code* code = (Code*)vector_at(&emit->stack, i);
    LOOP_OPTIM
    for(j = 0; j < vector_size(&code->code); j++)
      free_instr((Instr)vector_at(&code->code, j));
     free_code(code);
  }
  LOOP_OPTIM
  for(i = 0; i < emit_code_size(emit); i++)
    free_instr((Instr)vector_at(&emit->code->code, i));
  free_code(emit->code);
}

ANN static m_bool emit_ast_inner(const Emitter emit, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(emit_section(emit, ast->section))
  while((ast = ast->next));
  return 1;
}

ANN static void handle_code(const VM_Code c) {
  LOOP_OPTIM
  for(m_uint i = 0; i < vector_size(c->instr); i++) {
    Instr instr = (Instr)vector_at(c->instr, i);
    if(instr->execute == INSTR_RECURS) {
      instr->execute = Reg_Push_Ptr;
      Func func = (Func)instr->m_val;
//        if(!func->code)
//          CHECK_BB(err_msg(EMIT_, 0, "function not emitted yet"))
      *(VM_Code*)instr->ptr = func->code;
    }
#ifdef OPTIMIZE
else if(instr->execute == INSTR_INLINE) {
  const Func f = *(Func*)instr->ptr;
  const m_uint j = i + 1, c_size = vector_size(c->instr), f_size = vector_size(f->code->instr),
    size = c_size + f_size;
  instr->execute = InlineStart;
  *(VM_Code*)instr->ptr = f->code;
  instr->m_val2 = j;
  Instr* code = (Instr*)xmalloc((OFFSET + size) * SZ_INT);
  Instr inlinestop = new_instr();
  inlinestop->execute = InlineStop;
  inlinestop->m_val = instr->m_val;
  inlinestop->m_val2 = f->code->stack_depth;
  code[i + f_size + OFFSET] = inlinestop;
  memcpy(code + OFFSET, c->instr->ptr + OFFSET, j * SZ_INT);
  memcpy(code + OFFSET + j + f_size, c->instr->ptr + j + OFFSET,  (c_size - j) * SZ_INT);
  memcpy(code + OFFSET + j, f->code->instr->ptr + OFFSET, (f_size - 1)* SZ_INT);
  free(c->instr->ptr);
  c->instr->ptr = (m_uint*)code;
  VLEN(c->instr) = size;
  VCAP(c->instr) = size + OFFSET;
    }
#endif
  }
}

ANN /* static */ void code_optim(const Emitter emit) {
  for(m_uint i = vector_size(&emit->codes) + 1; --i;) {
    const VM_Code c = (VM_Code)vector_at(&emit->codes, i - 1);
    if(GET_FLAG(c, ae_flag_recurs))
      handle_code(c);
  }
  vector_clear(&emit->codes);
}

ANN m_bool emit_ast(const Emitter emit, Ast ast, const m_str filename) { GWDEBUG_EXE
  int ret;
  emit->filename = filename;
#ifdef GWCOV
  if(emit->coverage) {
    char cov_filename[strlen(filename) + 3];
    sprintf(cov_filename, "%sda", filename);
    emit->cov_file = fopen(cov_filename, "a");
  }
#endif
  emit->code = new_code();
  vector_clear(&emit->stack);
  emit_push_scope(emit);
  emitter_add_instr(emit, start_gc);
  ret = emit_ast_inner(emit, ast);
  emitter_add_instr(emit, stop_gc);
  if(emit->cases) {
    free_map(emit->cases);
    emit->cases = NULL;
  }
  emit_pop_scope(emit);
  code_optim(emit);
  if(ret < 0) { // should free all stack.
    gw_err("in file '%s'\n", filename);
    emit_free_stack(emit);
    free(filename);
    free_ast(ast);
  }
#ifdef GWCOV
  if(emit->coverage)
    fclose(emit->cov_file);
#endif
  return ret;
}

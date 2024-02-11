#ifndef EMIT_H
#define EMIT_H

typedef struct {
  m_bit *ptr;
  uint32_t reg;
  uint32_t limit;
} MaybeVal;

typedef struct Frame_ {
  struct Vector_ stack;
  struct Map_    handlers;
  struct Vector_ defer;
  m_uint         value_count;
  MP_Vector *maybe_stack;
  uint16_t       curr_offset;
  uint16_t       try_top;
} Frame;

typedef struct VMValue_ {
  const Type     t;
  const uint16_t offset;
  const uint16_t start;
  uint16_t end;
} VMValue;

typedef struct Code_ {
  Frame *          frame;
  struct Vector_   instr;
  struct Vector_   stack_cont;
  struct Vector_   stack_break;
  struct Vector_   stack_return;
  struct M_Vector_ live_values;
  m_str            name;
  uint16_t         stack_depth;
} Code;

struct EmitterInfo_ {
  struct Vector_ pure;
  char *         escape;
  VM_Code (*emit_code)(const Emitter);
  VM_Code  code;
  bool     debug;
  bool     dump;
};

typedef struct EmitterStatus {
  uint16_t             this_offset;   // reset
  uint16_t unroll;
  uint16_t line;
  uint32_t effect; // offset of last throw effect
  bool in_return;
} EmitterStatus;

struct Emitter_ {
  Env                  env;
  Code *               code;
  struct Gwion_ *      gwion;
  struct EmitterInfo_ *info;
  struct Vector_       stack;
  Func                 locale;
  EmitterStatus status;
};

ANEW ANN Emitter new_emitter(MemPool);
ANN void         free_emitter(MemPool, Emitter);
ANN bool       emit_ast(const Env env, Ast *ast);
ANN bool       emit_func_def(const Emitter emit, const Func_Def fdef);
ANN bool emit_exp_call1(const Emitter, const Func, const m_uint size, const bool is_static);
ANN2(1)
Instr emit_add_instr(const Emitter, const f_instr)
    __attribute__((returns_nonnull));
ANN Code * emit_class_code(const Emitter, const m_str);
ANN bool emit_array_extend(const Emitter, const Type, Exp*);
ANN void   emit_class_finish(const Emitter, const Nspc);
ANN2(1, 2)
bool     emit_instantiate_object(const Emitter, const Type, const Array_Sub,
                                   const bool);
ANN m_uint emit_code_offset(const Emitter emit);
ANN m_uint emit_local(const Emitter emit, const Type t);
ANN m_uint emit_localn(const Emitter emit, const Type t);
ANN void* emit_localx(const Emitter emit, const Type t);
ANN m_uint emit_local_exp(const Emitter emit, Exp*);
ANN bool emit_exp_spork(const Emitter, const Exp_Unary *);
ANN bool emit_exp(const Emitter, Exp*);

ANN void emit_object_addref(const Emitter emit, const m_int size,
                             const bool emit_var);
ANN void emit_struct_addref(const Emitter emit, const Type t, const m_int size,
                             const bool emit_var);
ANN static inline void emit_compound_addref(const Emitter emit, const Type t,
                                             const m_int  size,
                                             const bool emit_var) {
  return !tflag(t, tflag_struct) ? emit_object_addref(emit, size, emit_var)
                                 : emit_struct_addref(emit, t, size, emit_var);
}

ANN Instr emit_kind(Emitter, const m_uint val, const m_uint size, const bool addr,
                    const f_instr func[]);
ANN Instr emit_regpushimm(Emitter, const m_uint, const m_uint, const bool);
ANN Instr emit_regpushmem(Emitter, const m_uint, const m_uint , const bool);
ANN Instr emit_regpushbase(Emitter, const m_uint, const m_uint, const bool);
ANN Instr emit_dotstatic(Emitter, const m_uint, const m_uint, const bool);
ANN Instr emit_dotmember(Emitter, const m_uint, const m_uint, const bool);
ANN Instr emit_structmember(Emitter, const m_uint, const m_uint, const bool);
ANN Instr emit_unionmember(Emitter, const m_uint, const m_uint, const bool);

void emit_fast_except(const Emitter emit, const ValueFrom *vf, const loc_t loc);
ANN static inline m_uint emit_code_size(const Emitter emit) {
  return vector_size(&emit->code->instr);
}

ANN void emit_push_scope(const Emitter emit);
ANN void emit_pop_scope(const Emitter emit);

ANN bool ensure_emit(const Emitter, const Type);
ANN bool emit_ensure_func(const Emitter emit, const Func f);
ANN m_bool get_emit_var(const Emitter emit, const Type t, bool is_var);

ANN static inline void emit_regmove(const Emitter emit, const m_uint i) {
  const Instr instr = emit_add_instr(emit, RegMove);
  instr->m_val = i;
}

ANN static inline void emit_pushfunc(const Emitter emit, const Func f) {
  if(f->code) {
    const Instr instr = emit_add_instr(emit, RegPushImm);
    instr->m_val = (m_uint)f->code;
  } else {
    const Instr instr = emit_add_instr(emit, SetFunc);
    instr->m_val = (m_uint)f;
  }
}

ANN static inline void emit_pushimm(const Emitter emit, const m_uint value) {
  const Instr instr = emit_add_instr(emit, RegPushImm);
  instr->m_val = value;
}

#define mk_emit_instr(name, instruction)                                                           \
ANN static inline Instr emit_##name(const Emitter emit, const m_uint value, const m_uint value2) { \
  const Instr instr = emit_add_instr(emit, instruction);                                           \
  instr->m_val  = value;                                                                           \
  instr->m_val2 = value2;                                                                          \
  return instr;                                                                                    \
 }
mk_emit_instr(memsetimm, MemSetImm);
mk_emit_instr(regsetimm, RegSetImm);
mk_emit_instr(regtomem, Reg2Mem);
mk_emit_instr(regtomem4, Reg2Mem4);
mk_emit_instr(regpushmem4, RegPushMem4);

ANN void emit_struct_release(const Emitter, const Type , const m_uint offset);

ANN void comptime_ini(const Emitter emit, const m_str name);
ANN2(1) void comptime_end(const Emitter emit, const size_t size, void*);

ANEW Code *new_code(const Emitter emit, const m_str name);
ANN VM_Code finalyze(const Emitter emit, const f_instr exec);
ANN static inline void emit_push_code(const Emitter emit, const m_str name) {
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code(emit, name);
  if (emit->info->debug) emit_add_instr(emit, DebugLine);
}

ANN static inline void emit_pop_code(const Emitter emit) {
  emit->code = (Code *)vector_pop(&emit->stack);
}
#endif

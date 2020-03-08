#ifndef __SPECIALID
#define __SPECIALID

struct SpecialId_;
typedef Type (*idck)(const Env, const Exp_Primary*);
typedef Instr (*idem)(const Emitter, const Exp_Primary*);

struct SpecialId_ {
  Type type;
  idck ck;
  f_instr exec;
  idem em;
  m_bool is_const;
};

#define ID_CHECK(a)  ANN Type a(const Env env NUSED, const Exp_Primary* prim NUSED)
#define ID_EMIT(a)  ANN Instr a(const Emitter emit NUSED, const Exp_Primary* prim NUSED)

ANN static inline Type specialid_type(const Env env,
    struct SpecialId_ *spid, const Exp_Primary* prim) {
  exp_self(prim)->type = spid->type;
  if(spid->is_const)
    exp_self(prim)->meta = ae_meta_value;
  return spid->ck ? spid->ck(env, prim) : spid->type;
}

ANN static inline Instr specialid_instr(const Emitter emit,
    struct SpecialId_ *spid, const Exp_Primary* prim) {
  return spid->exec ? emit_add_instr(emit, spid->exec) : spid->em(emit, prim);
}

ANN struct SpecialId_* specialid_get(const Gwion, const Symbol);
#endif

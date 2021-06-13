#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"

enum Kind { KIND_INT, KIND_FLOAT, KIND_OTHER, KIND_ADDR, KIND_END };

static inline enum Kind kindof(const m_uint size, const uint emit_var) {
  if (emit_var) return KIND_ADDR;
  return size == SZ_INT ? KIND_INT : size == SZ_FLOAT ? KIND_FLOAT : KIND_OTHER;
}

ANN Instr emit_kind(Emitter emit, const m_uint size, const bool addr,
                    const f_instr func[]) {
  const enum Kind kind  = kindof(size, addr);
  const Instr     instr = emit_add_instr(emit, func[kind]);
  instr->m_val2         = size;
  return instr;
}

static const f_instr regpushimm[KIND_END] = {RegPushImm, RegPushImm2,
                                             RegPushImm3, RegPushImm4};

static const f_instr regpushmem[KIND_END] = {RegPushMem, RegPushMem2,
                                             RegPushMem3, RegPushMem4};

static const f_instr regpushbase[KIND_END] = {RegPushBase, RegPushBase2,
                                              RegPushBase3, RegPushBase4};

static const f_instr dotstatic[KIND_END] = {DotStatic, DotStatic2, DotStatic3,
                                            RegPushImm};

static const f_instr dotmember[KIND_END] = {DotMember, DotMember2, DotMember3,
                                            DotMember4};

static const f_instr structmember[KIND_END] = {
    StructMember, StructMemberFloat, StructMemberOther, StructMemberAddr};

static const f_instr unionmember[KIND_END] = {UnionMember, UnionMember2,
                                              UnionMember3, UnionMember4};

// function factory
#define kind_func(name)                                                        \
  ANN Instr emit_##name(Emitter emit, const m_uint size, const bool addr) {    \
    return emit_kind(emit, size, addr, name);                                  \
  }

kind_func(regpushimm);
kind_func(regpushmem);
kind_func(regpushbase);
kind_func(dotstatic);
kind_func(dotmember);
kind_func(structmember);
kind_func(unionmember);

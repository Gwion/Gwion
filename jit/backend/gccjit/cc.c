#include <stdlib.h>
#include <string.h>
#include <libgccjit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "cc.h"
#include "ctrl.h"
#include "ctrl_private.h"
#include "thread.h"
#include "backend.h"

static void gccjit_no(JitThread jt, Instr instr) {
  CC cc = jt->cc;
  TYPE ptype = GET_TYPE(pointer);
  RVAL jinstr = PCONST(ptype, instr);
  TYPE arg_types[] = { ptype, ptype };
  RVAL args[] = { cc->shred, jinstr };
  TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
  RVAL func = PCONST(fptr_type, instr->execute);
  EVAL(cc->block, CALL(func, 2, args));
}

static void gccjit_ex(JitThread jt) {
  CC cc = jt->cc;
  LVAL vm   = cc_deref_field(cc, cc->shred, "shred:vm");
  LVAL sh   = cc_deref_field(cc, AS_RVAL(vm), "vm:shreduler");
  LVAL curr = cc_deref_field(cc, AS_RVAL(sh), "shreduler:curr");
  RVAL null = PCONST(GET_TYPE(shred), NULL);
  RVAL cond = COMP(EQ, AS_RVAL(curr), null);
  BLOCK b_ex = NEW_BLOCK("exception occured");
  BLOCK b_end = NEW_BLOCK("end");
  BLOCK_CND(cc->block, cond, b_ex, b_end);
  BLOCK_RET(b_ex, b_end);
}

static void gccjit_pc(JitThread jt, struct ctrl* ctrl) {
  CC cc = jt->cc;
  LVAL pc = cc_deref_field(cc, cc->shred, "shred:pc");
  RVAL val = ICONST(GET_TYPE(m_uint), ctrl_idx(ctrl));
  ASSIGN(cc->block, pc, val);
}

static void gccjit_ini(JitThread jt) {
  CC cc = jt->cc;
  cc->ctx = gcc_jit_context_new_child_context(cc->parent);
  gcc_jit_param *shred =
    gcc_jit_context_new_param(cc->ctx, NULL, GET_TYPE(shred), "shred");
  gcc_jit_param* arg[] = { shred /*, instr*/ };
  cc->f = gcc_jit_context_new_function (cc->ctx, NULL,
    GCC_JIT_FUNCTION_EXPORTED, GET_TYPE(void), "greet", 1, arg, 0);
  cc->block = NEW_BLOCK("initial");
  cc->shred = gcc_jit_param_as_rvalue(shred);
}

static void gccjit_end(JitThread jt/*, const m_str str*/) {
  CC cc = jt->cc;
  BLOCK_RET(cc->block, NULL);
  gcc_jit_result *result = gcc_jit_context_compile(cc->ctx);
  if(!result) {
    fprintf (stderr, "NULL result");
    exit (1);
  }
  typedef void (*fn_type) (const char *);
  fn_type greet = (fn_type)gcc_jit_result_get_code (result, "greet");
  if(!greet) {
    fprintf (stderr, "NULL greet");
    exit (1);
  }
//  char c[strlen(str) + 10];
//  sprintf(c, "/tmp/%s.dot", str);
//  gcc_jit_function_dump_to_dot(cc->f, c);
// to instr
  const Instr base = (Instr)jt->base;
  base->execute = JitExec;
  base->m_val = (m_uint)greet;
  base->m_val2 = (m_uint)jt;
  *(Instr*)base->ptr = jt->top;
  jt->top = NULL;
  jt->base = NULL;
  gcc_jit_context_release(cc->ctx);
}

static void gccjit_ctrl(struct Jit* j) {
  jit_ctrl_import_instr(j);
  jit_ctrl_import_int(j);
  jit_ctrl_import_float(j);
//  jit_ctrl_import_complex(j);
//  jit_ctrl_import_vec(j);
//  jit_ctrl_import_object(j);
  jit_ctrl_import_array(j);
  jit_ctrl_import_string(j);
  jit_ctrl_import_event(j);
  jit_ctrl_import_ugen  (j);
//  jit_ctrl_import_ptr(j);
//  jit_ctrl_import_gack(j);
  jit_ctrl_import_vararg(j);
}

static void gccjit_code(struct Jit* j) {
  jit_code_import_instr(j);
//  jit_code_import_array(j);
//  jit_code_import_object(j);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no   = gccjit_no;
  be->pc   = gccjit_pc;
  be->ex   = gccjit_ex;
  be->ini  = gccjit_ini;
  be->end  = gccjit_end;
  be->ctrl = gccjit_ctrl;
  be->code = gccjit_code;
  be->free = NULL; // TODO
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}

INSTR(JitExec){
  void (*f)(VM_Shred) = (void(*)(VM_Shred))instr->m_val;
  f(shred);
}

CC new_cc(){
  CC cc = (CC)xmalloc(sizeof(struct JitCC_));
  cc->ctx = cc->parent = gcc_jit_context_acquire();

  gcc_jit_context_set_int_option(cc->ctx, GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL, 1);
/*
gcc_jit_context_add_command_line_option(cc->ctx, "-Ofast");
gcc_jit_context_add_command_line_option(cc->ctx, "-msse4");
*/
//gcc_jit_context_add_command_line_option(cc->ctx, "-fno-signed-zeros");
//gcc_jit_context_add_command_line_option(cc->ctx, "-ffinite-math-only");
//gcc_jit_context_add_command_line_option(cc->ctx, "-ffast-math");
//gcc_jit_context_add_command_line_option(cc->ctx, "-fno-trapping-math");
gcc_jit_context_add_command_line_option(cc->ctx, "-fstrict-aliasing");
gcc_jit_context_add_command_line_option(cc->ctx, "-mtune=native");
gcc_jit_context_add_command_line_option(cc->ctx, "-fno-sanitize=all");
gcc_jit_context_add_command_line_option(cc->ctx, "-fdelete-null-pointer-checks");
//gcc_jit_context_add_command_line_option(cc->ctx, "-flto");
  map_init(&cc->type);
  map_init(&cc->field);
  import_core_type(cc);
  import_other_type(cc);
  return cc;
}

static inline void clear_map(Map m) {
  for(m_uint i= 0; i < map_size(m); ++i)
    xfree((m_str)VKEY(m, i));
  map_release(m);
}

void free_cc(CC cc) {
  gcc_jit_context_release(cc->parent);
  clear_map(&cc->type);
  clear_map(&cc->field);
  xfree(cc);
}

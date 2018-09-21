#include <stdlib.h>
#include <libgccjit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "cc.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"

static void gccjit_no(JitThread jt, Instr instr) {
  CC cc = jt->cc;
  gcc_jit_type* void_type = gcc_jit_context_get_type(cc->ctx, GCC_JIT_TYPE_VOID);
  gcc_jit_type* pointer_type = gcc_jit_context_get_type(cc->ctx, GCC_JIT_TYPE_VOID_PTR);
  gcc_jit_rvalue *jinstr = gcc_jit_context_new_rvalue_from_ptr(cc->ctx, pointer_type, instr);
  gcc_jit_type *arg_types[] = { pointer_type, pointer_type };
  gcc_jit_rvalue *args[] = { cc->shred, jinstr };
  gcc_jit_type* fptr_type = gcc_jit_context_new_function_ptr_type(cc->ctx, NULL,
    void_type, 2, arg_types, 1);
  gcc_jit_rvalue *func = gcc_jit_context_new_rvalue_from_ptr(cc->ctx,
    fptr_type, instr->execute);
  gcc_jit_block_add_eval (
    cc->block, NULL,
      gcc_jit_context_new_call_through_ptr(
      cc->ctx,
      NULL,
      func,
      2, args));
//  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)instr);
}
static void gccjit_ex(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}

static void gccjit_pc(JitThread jt, struct ctrl* ctrl) {
//  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)ctrl);
}

static void gccjit_ini(JitThread jt) {
  CC cc = jt->cc;
  cc->ctx = gcc_jit_context_acquire();
  gcc_jit_type* void_type = gcc_jit_context_get_type(cc->ctx, GCC_JIT_TYPE_VOID);
  gcc_jit_type* pointer_type = gcc_jit_context_get_type(cc->ctx, GCC_JIT_TYPE_VOID_PTR);
  gcc_jit_param *shred =
    gcc_jit_context_new_param (cc->ctx, NULL, pointer_type, "shred");
//  gcc_jit_param *instr =
//    gcc_jit_context_new_param (cc->ctx, NULL, pointer_type, "instr");
  gcc_jit_param* arg[] = { shred/*, instr*/ };
  gcc_jit_function *func = gcc_jit_context_new_function (cc->ctx, NULL,
    GCC_JIT_FUNCTION_EXPORTED, void_type, "greet", 1, arg, 0);
  cc->block = gcc_jit_function_new_block (func, NULL);
  cc->shred = gcc_jit_param_as_rvalue(shred);
//  printf("%-12s %p\n", __func__, (void*)jt);
  jt->compiling = 1;
}

static void gccjit_end(JitThread jt) {
  CC cc = jt->cc;
  gcc_jit_block_end_with_void_return(cc->block, NULL);
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
// to instr
  const Instr base = (Instr)jt->base;
  base->execute = JitExec;
  base->m_val = (m_uint)greet;
  base->m_val2 = (m_uint)jt;
  *(Instr*)base->ptr = jt->top;
  jt->top = NULL;
  jt->base = NULL;
//

  gcc_jit_context_release(cc->ctx);
//  printf("%-12s %p\n", __func__, (void*)jt);
  jt->compiling = 0;
}

static void gccjit_ctrl(JitThread j) {
  jit_code_import_instr(j);
//  jit_code_import_int(j);
//  jit_code_import_float(j);
//  jit_code_import_complex(j);
//  jit_code_import_vec(j);
//  jit_code_import_object(j);
//  jit_code_import_array(j);
//  jit_code_import_string(j);
//  jit_code_import_event(j);
//  jit_code_import_ugen  (j);
//  jit_code_import_ptr(j);
//  jit_code_import_gack(j);
//  jit_code_import_vararg(j);
//  printf("%-12s %p\n", __func__, (void*)jt);
}

static void gccjit_code(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
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
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
INSTR(JitExec){ 
//puts(__func__);
  void (*f)(VM_Shred) = (void(*)(VM_Shred))instr->m_val;
  f(shred);
}

CC new_cc(){
  CC cc = xcalloc(1, sizeof(struct JitCC_));
  return cc;
}

void free_cc(CC cc) {
  xfree(cc);
}
void free_jit_instr(JitThread jt, Instr instr) {}

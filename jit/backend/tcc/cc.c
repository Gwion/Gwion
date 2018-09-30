#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <libtcc.h>
#include "defs.h"
#include "mpool.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "cc.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"

/**/
static void update_pc(const VM_Shred shred) {
  ++shred->pc;
}
/**/

void cc_add(CC cc, const m_str str) {
  const size_t len = strlen(str);
  cc->str = (m_str)xrealloc(cc->str, cc->len + len + 1);
  strncpy(cc->str + cc->len, str, len);
  cc->str[(cc->len += len)] = '\0';
}
extern m_str dev_get(const m_str);
static void tcc_no(JitThread jt, Instr instr) {
  const m_str str = dev_get(instr->execute);
  cc_add_symbol(jt->cc, str, instr->execute);
  cc_add(jt->cc, str);
  cc_add(jt->cc, "(shred, *(Instr*)instr->ptr);");
  xfree(str);
//  if(instr->execute == EOC)
//cc_add(jt->cc, "  vm_shred_exit(shred);");
//    cc_add(jt->cc, "shred->vm_ref->shreduler->curr = NULL;\n");
}
static void tcc_ex(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}

ANN static void to_instr(const JitThread jt){
  CC cc = jt->cc;
  const Instr base = (Instr)jt->base;
  cc_add(cc, "}\n\0");
//puts(cc->str);
pthread_mutex_lock(&cc->mutex);
//
//if (tcc_compile_string(cc->s, cc->str) == -1)
//        exit(6);
pthread_mutex_unlock(&cc->mutex);
xfree(cc->str);
  base->execute = JitExec;
    /* relocate the code */
    if (tcc_relocate(cc->s, TCC_RELOCATE_AUTO) < 0)
        exit(7);
//  base->m_val = (m_uint)tcc_get_symbol(cc->s, "foo");
  base->m_val = (m_uint)cc->s;
vector_add(&cc->code, cc->s);
//printf("! instr %p val %p\n", base, base->m_val);
  base->m_val2 = (m_uint)jt;
  *(Instr*)base->ptr = jt->top;
  jt->top = NULL;
  jt->base = NULL;
}

static void tcc_end(JitThread jt) {
  CC cc = jt->cc;
  to_instr(jt);
  //printf("%-12s %p\n", __func__, (void*)jt);
  cc->str = NULL;
}

static void tcc_ini(JitThread jt) {
  CC cc = jt->cc;
//  printf("%-12s %p\n", __func__, (void*)jt);
  cc->s = tcc_new();
tcc_set_output_type(cc->s, TCC_OUTPUT_MEMORY);
tcc_add_include_path(cc->s, "include");
tcc_add_include_path(cc->s, "../Soundpipe/h");
//tcc_add_include_path(cc->s, "/usr/include");
//tcc_add_include_path(cc->s, "../tcc/include");
//  tcc_add_symbol(cc->s, "update_pc", update_pc);
  cc->str = strdup("#include </home/djay/src/tcc-0.9.26/include/stdarg.h>"
"\n#include </home/djay/src/tcc-0.9.26/include/stddef.h>\n"
"#define _SYS_SYSMACROS_H_OUTER\n"
"//#include <complex.h>\n"
"#include \"absyn.h\"\n"
"#include \"instr.h\"\n"
"#include \"shreduler_private.h\"\n"
"typedef char* m_complex;\n"
"\nvoid foo(VM_Shred shred, Instr instr) {\n");
  cc->len = strlen(cc->str);
//  cc_add(cc->s, "void main(VM_Shred shred, Instr instr) {\n");
}


static void tcc_pc(JitThread jt, struct ctrl* ctrl) {
  printf("%-12s %p\n", __func__, (void*)j);
}

static void tcc_code(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

static void tcc_ctrl(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

void cc_add_symbol(CC cc, const m_str str, const uintptr_t ptr) {
  if(!tcc_get_symbol(cc->s, str))
    tcc_add_symbol(cc->s, str, (void*)ptr);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no = tcc_no;
  be->pc = tcc_pc;
  be->ex = tcc_ex;
  be->end = tcc_end;
  be->code = tcc_code;
  be->ctrl = tcc_ctrl;
  be->free = NULL;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
INSTR(JitExec){ puts(__func__);
//printf("pc %lu\n", shred->pc);
TCCState* s= (TCCState*)instr->m_val;
void(*f)(VM_Shred, Instr) = 
(void(*)(VM_Shred, Instr))(uintptr_t)tcc_get_symbol(s, "foo");
f(shred, instr); }

void free_cc(CC cc) {
//  tcc_delete(instr->m_val);
  for(m_uint i = 0; i < vector_size(&cc->code); ++i)
    tcc_delete((TCCState*)vector_at(&cc->code, i));
  vector_release(&cc->code);
  pthread_mutex_destroy(&cc->mutex);
  xfree(cc);
}

CC new_cc(void) {
  CC cc = (CC)xmalloc(sizeof(struct JitCC_));
  cc->s = NULL;
  cc->len = 0;
  cc->str = NULL;
  pthread_mutex_init(&cc->mutex, NULL);
  vector_init(&cc->code);
  return cc;
}

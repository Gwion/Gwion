#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "context.h"
#include "compile.h"
#include "traverse.h"
#include "shreduler_private.h"

static inline int _bind_cr(int count, int key) {
  printf("\n(...)");
  return 0;
}

static inline int _bind_accept(int count, int key) {
  printf("\n"); return rl_done = 1;
}

static inline VM_Shred repl_shred() {
  VM_Code code = new_vm_code(NULL, 0, 0, "repl");
  VM_Shred shred = new_vm_shred(code);
  return shred;
}

ANN static void eval(VM* vm, VM_Shred shred, const m_str line) {
  if(shred == vm->shreduler->list) {
    gw_err("shred[%"UINT_F"] is running.please use ':spork'\n");
    return;
  }
  FILE* f = fmemopen(line, strlen(line), "r");
  Ast ast = parse("repl", f);
  m_str str;
  if(!ast)
    goto close;
  str = strdup("repl");
  if(traverse_ast(vm->emit->env, ast) < 0)
    goto close;
  if(emit_ast(vm->emit, ast, str) < 0)
    goto close;
  ((Instr)vector_back(&vm->emit->code->code))->execute = EOC2;
  vm->emit->code->name = str;
  if(shred->code)
    free_vm_code(shred->code);
  shred->code = emit_code(vm->emit);
  free_ast(ast);
  vm_add_shred(vm, shred);
close:
  fclose(f);
}

ANN static void repl_finish(VM* vm, VM_Shred shred) {
  if(shred->code->instr) {
    ((Instr)vector_back(shred->code->instr))->execute = EOC;
    shred->next_pc = vector_size(shred->code->instr) - 2;
    vm_add_shred(vm, shred);
  } else
    shreduler_remove(vm->shreduler, shred, 1);
}

ANN static VM_Shred repl_cmd(VM* vm, VM_Shred shred, const m_str line) {
  if(*line == '+')
    compile(vm, line+1);
  else if(*line == '-') {
    m_str endptr;
    m_uint i, index = strtol(line + 1, &endptr, 10) - 1;
    for(i = 0; i < vector_size(&vm->shred); i++) {
      VM_Shred sh = (VM_Shred)vector_at(&vm->shred, i);
      if(sh->xid == index) {
       shreduler_remove(vm->shreduler, sh, 1);
       break;
      }
    }
  }
  else if(strcmp(line, "spork")) {
     gw_err("unknown command '%s'\n", line);
     return shred;
  }
  VM_Shred old = shred;
  shred = repl_shred();
  old->parent = shred;
  if(!shred->child.ptr)
    vector_init(&shred->child);
  vector_add(&shred->child, (vtype)old);
  memcpy(shred->_mem, old->_mem, SIZEOF_MEM);
  memcpy(shred->_reg, old->_reg, SIZEOF_REG);
  return shred;
}

ANN static VM_Shred handle_line(VM* vm, VM_Shred shred, const m_str line) {
  add_history(line);
  if(*line == ':')
    shred = repl_cmd(vm, shred, line + 1);
  else
    eval(vm, shred, line);
  return shred;
}

ANN static void* repl_process(void* data) {
  VM* vm = (VM*)data;
  VM_Shred shred = repl_shred();
  Context ctx = new_context(NULL, "repl ctx");
  load_context(ctx, vm->emit->env);
  read_history(NULL);
  while(vm->is_running) {

    char prompt[128];
    char prompt1[128];
    if(shred->xid) {
      snprintf(prompt, 128, "\033[30;3;1m[%"UINT_F"]\033[32m=>\033[0m ", shred->xid);
      snprintf(prompt1, 128, "[%"UINT_F"]=> ", shred->xid);
    } else {
      snprintf(prompt, 128, "\033[30;3;1m[!]\033[32m=>\033[0m ");
      snprintf(prompt1, 128, "[!]=> ");
    }
//    char* line = readline("\033[32;1m=>\033[0m ");
    char* line = readline(prompt);
    rl_prompt = prompt1;
    if(!line)
      break;
    if(strlen(line))
      shred = handle_line(vm, shred, line);
    free(line);
  }
  repl_finish(vm, shred);
  write_history(NULL);
  unload_context(ctx, vm->emit->env);
  REM_REF(ctx);
  return NULL;
}

ANN void repl_init(VM* vm, pthread_t* p) {
  rl_bind_key('\n', _bind_cr);\
  rl_bind_key('\r', _bind_cr);\
  rl_bind_keyseq("\\C-g", _bind_accept);\
  pthread_create(p, NULL, repl_process, vm);
#ifndef __linux__
  pthread_detach(*p);
#endif
}

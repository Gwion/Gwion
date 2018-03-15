#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "instr.h"
#include "import.h"
#include "compile.h"

#define RAND_LEN 12

static void randstring(VM* vm, m_uint length, m_str str) {
  char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
  size_t stringLen = 26 * 2 + 10 + 2;
  m_uint n;
  for(n = 0; n < length; n++) {
    unsigned int key = sp_rand(vm->sp) % stringLen;
    str[n] = string[key];
  }
  str[length] = '\0';
}

static const m_str get_prefix(VM_Shred shred) {
  M_Object prefix_obj = *(M_Object*)MEM(SZ_INT);
  if(!prefix_obj)
    return ".";
  release(prefix_obj, shred);
  return STRING(prefix_obj);
}

static void get_filename(VM_Shred shred, m_str c, m_str prefix) {
  char filename[RAND_LEN + 1];

  randstring(shred->vm_ref, 12, filename);
  sprintf(c, "%s/%s", prefix, filename);
}

static m_bool check_code(VM_Shred shred, m_str c) {
  m_bool ret;
  m_str s;
  Ast ast;
  FILE* f = fopen(c, "r");
  if(!f)
    return - 1;
  if(!(ast = parse(c, f))) {
    fclose(f);
    return - 1;
  }
  s = strdup(c);
  if((ret = type_engine_check_prog(shred->vm_ref->emit->env,  ast, s)) > 0) {
    free_ast(ast);
    free(s);
  }
  fclose(f);
  return ret;
}

static m_bool code_to_file(VM_Shred shred, m_str filename) {
  M_Object code_obj = *(M_Object*)MEM(SZ_INT * 2);
  FILE* file;

  CHECK_OB(code_obj)
  release(code_obj, shred);
  CHECK_OB((file = fopen(filename, "w")))
  fprintf(file, "%s\n", STRING(code_obj));
  fclose(file);
  return 1;
}

static SFUN(machine_add) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    return;
  m_str str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  *(m_uint*)RETURN = compile(shred->vm_ref, str);
}

static SFUN(machine_check) {
  const m_str prefix = get_prefix(shred);
  char c[strlen(prefix) + 17];

  *(m_uint*)RETURN = 0;
  get_filename(shred, c, prefix);
  if(code_to_file(shred, c) < 0)
    return;
  *(m_uint*)RETURN = check_code(shred, c) > 0 ? 1 : 0;
}

static SFUN(machine_compile) {
  const m_str prefix = get_prefix(shred);
  char c[strlen(prefix) + 17];

  *(m_uint*)RETURN = 0;
  get_filename(shred, c, prefix);
  if(code_to_file(shred, c) < 0)
    return;
  *(m_uint*)RETURN = compile(shred->vm_ref, c);
}

static SFUN(machine_shreds) {
  int i;
  VM* vm = shred->vm_ref;
  VM_Shred sh;
  Type t = array_type(t_int, 1);
  M_Object obj = new_M_Array(t, SZ_INT, vector_size(&vm->shred), 1);
  for(i = 0; i < vector_size(&vm->shred); i++) {
    sh = (VM_Shred)vector_at(&vm->shred, i);
    m_vector_set(ARRAY(obj), i, &sh->xid);
  }
  vector_add(&shred->gc, (vtype)obj);
  *(M_Object*)RETURN = obj;
}

m_bool import_machine(Gwi gwi) {
  Type t_machine;
  CHECK_OB((t_machine = gwi_mk_type(gwi, "Machine", 0, NULL)))
  CHECK_BB(gwi_class_ini(gwi,  t_machine, NULL, NULL))
  gwi_func_ini(gwi, "void",  "add", machine_add);
  gwi_func_arg(gwi,       "string",  "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int[]", "shreds", machine_shreds);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int",  "check", machine_check);
  gwi_func_arg(gwi,      "string",  "prefix");
  gwi_func_arg(gwi,      "string",  "code");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "void", "compile", machine_compile);
  gwi_func_arg(gwi,      "string",  "prefix");
  gwi_func_arg(gwi,      "string",  "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

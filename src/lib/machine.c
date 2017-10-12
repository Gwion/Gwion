#include "type.h"
#include "instr.h"
#include "import.h"
#include "compile.h"

#define RAND_LEN 12

struct Type_ t_machine   = { "Machine",      0, NULL, te_machine};

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
  m_str s;
  Ast ast;

  CHECK_OB((ast = parse(c)))
  s = strdup(c);
  CHECK_BB(type_engine_check_prog(shred->vm_ref->emit->env,  ast, s))
  free(s);
  free_ast(ast);
  return 1;
}

static m_bool code_to_file(VM_Shred shred, m_str filename) {
  M_Object code_obj = *(M_Object*)MEM(SZ_INT * 2);
  FILE* file;

  CHECK_OB(code_obj)
  CHECK_OB((file = fopen(filename, "w")))
  fprintf(file, "%s\n", STRING(code_obj));
  release(code_obj, shred);
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
  compile(shred->vm_ref, c);
}

static SFUN(machine_shreds) {
  int i;
  VM* vm = shred->vm_ref;
  VM_Shred sh;
  M_Object obj = new_M_Array(SZ_INT, vector_size(&vm->shred), 1);
  for(i = 0; i < vector_size(&vm->shred); i++) {
    sh = (VM_Shred)vector_at(&vm->shred, i);
    i_vector_set(ARRAY(obj), i, sh->xid);
  }
  vector_add(&shred->gc, (vtype)obj);
  *(M_Object*)RETURN = obj;
}

m_bool import_machine(Importer importer) {
  CHECK_BB(importer_class_ini(importer,  &t_machine, NULL, NULL))
  importer_func_ini(importer, "void",  "add", (m_uint)machine_add);
  importer_func_arg(importer,       "string",  "filename");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int[]", "shreds", (m_uint)machine_shreds);
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int",  "check", (m_uint)machine_check);
  importer_func_arg(importer,      "string",  "prefix");
  importer_func_arg(importer,      "string",  "code");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "void", "compile", (m_uint)machine_compile);
  importer_func_arg(importer,      "string",  "prefix");
  importer_func_arg(importer,      "string",  "filename");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  CHECK_BB(importer_class_end(importer))
  return 1;
}

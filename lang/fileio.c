#include <sys/types.h>
#include <dirent.h>
#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"
#include <unistd.h>
#include "object.h"


struct Type_ t_io        = { "IO",         sizeof(m_uint), &t_event, te_io };
struct Type_ t_fileio    = { "FileIO",     sizeof(m_uint), &t_io,    te_fileio };

m_int o_fileio_dir;
m_int o_fileio_file;
m_int o_fileio_ascii;
m_int o_fileio_line;

#define IO_LINE(o) *(m_str*)(o->d.data + o_fileio_line)

CTOR(fileio_ctor)
{
  IO_FILE(o)  = NULL;
  IO_ASCII(o) = 1;
}

DTOR(fileio_dtor)
{
  if(IO_DIR(o))
    closedir(IO_DIR(o));
  else if(IO_FILE(o))
    fclose(IO_FILE(o));
}

INSTR(int_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int to file");
#endif
  POP_REG(shred, SZ_INT)
  char c[256];
  M_Object o = *(M_Object*)shred->reg;
  sprintf(c, "%i", *(int*)(shred->reg - SZ_INT));
  fwrite(c,  strlen(c), 1, IO_FILE(o));
}

INSTR(float_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float to file");
#endif
  POP_REG(shred, SZ_INT)
  char c[256];
  M_Object o = *(M_Object*)shred->reg;
  sprintf(c, "%f", *(m_float*)(shred->reg - SZ_INT));
  fwrite(c,  strlen(c), 1, IO_FILE(o));
}

INSTR(string_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)shred->reg;
  m_str str;
  M_Object lhs = *(M_Object*)(shred->reg - SZ_INT);
  str = STRING(lhs);
  m_uint i = fwrite(str,  strlen(str), 1, IO_FILE(o));
  *(m_uint*)shred->reg = i;
//  jmp_u(shred->reg, 1);
  PUSH_REG(shred, SZ_INT)
}

INSTR(file_to_int)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => int");
#endif
  POP_REG(shred, SZ_INT)
  int ret;
  M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  if(IO_ASCII(o)) {
    (void)fscanf(IO_FILE(o), "%i", &ret);
    *(m_uint*)shred->reg = (**(m_uint**)(shred->reg) = ret);
  } else exit(89);
}

INSTR(file_to_float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => float");
#endif
  POP_REG(shred, SZ_INT)
  /*  m_float ret;*/
  float ret;
  M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  if(IO_ASCII(o)) {
    (void)fscanf(IO_FILE(o), "%f", &ret);
    *(m_float*)(shred->reg - SZ_INT) = (**(m_float**)(shred->reg) = ret);
  } else exit(89);
}

m_bool inputAvailable(FILE* f)
{
  int fd = fileno(f);
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
//  FD_SET(STDIN_FILENO, &fds);
//  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  FD_SET(fd, &fds);
  select(fd + 1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds));
}

INSTR(file_to_string)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => string");
#endif
  POP_REG(shred, SZ_INT)
  /*  char ret[1024];*/
  M_Object o    = *(M_Object*)(shred->reg - SZ_INT);
  M_Object s    = **(M_Object**)(shred->reg);
  m_str    c    = strdup("\n");
  m_uint   size = 0;
//  if(IO_ASCII(o))
  {
    if(inputAvailable(IO_FILE(o)))
      (void)getline(&c, &size, IO_FILE(o));
    STRING(s) = c;
    *(M_Object*)(shred->reg - SZ_INT) = s;
  }
}

MFUN(file_nl)
{
  RETURN->v_uint = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

MFUN(file_open)
{
  DIR* dir;
  M_Object lhs = *(M_Object*)(shred->mem + SZ_INT * 2);
  M_Object rhs = *(M_Object*)(shred->mem + SZ_INT);
  m_str filename = STRING(rhs);
  m_str mode = STRING(lhs);

  if(IO_DIR(o)) {
    closedir(IO_DIR(o));
    IO_DIR(o) = NULL;
  } else if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  if((dir = opendir(filename))) {
    IO_DIR(o) = dir;
    RETURN->v_uint = 2;
    return;
  }
  IO_FILE(o) = fopen(filename, mode);
  IO_DIR(o) = NULL;
  RETURN->v_uint = IO_FILE(o) ? 1 : 0;
}

MFUN(file_close)
{
  if(IO_DIR(o)) {
    RETURN->v_uint = closedir(IO_DIR(o));
    IO_DIR(o) = NULL;
    return;
  }
  if(IO_FILE(o))
    fclose(IO_FILE(o));
  RETURN->v_uint = !IO_FILE(o) ? 1 : 0;
}

MFUN(file_remove)
{
  RETURN->v_uint = remove(STRING(*(M_Object*)(shred->mem + SZ_INT)));
}

SFUN(file_list)
{
  m_uint i;
  struct dirent **namelist;
  m_int n = scandir(STRING(*(M_Object*)(shred->mem + SZ_INT)), &namelist, NULL, alphasort);
  if(n < 0) {
    RETURN->v_uint = 0;
    return;
  }
  M_Object ret = new_M_Array(SZ_INT, n);
  for(i = 0; i < n; i++) {
    i_vector_set(ret->d.array, i, (m_uint)new_String(namelist[i]->d_name));
    free(namelist[i]);
  }
  free(namelist);
  RETURN->v_uint = (m_uint)ret;
}
m_bool import_fileio(Env env)
{
  DL_Func* fun;

  // hack
  CHECK_BB(add_global_type(env, &t_io))
  CHECK_OB(import_class_begin(env, &t_io, env->global_nspc, NULL, NULL))
  env->class_def->doc = "io operations";
  CHECK_BB(import_class_end(env))
  // !hack


  CHECK_BB(add_global_type(env, &t_fileio))
  CHECK_OB(import_class_begin(env, &t_fileio, env->global_nspc, fileio_ctor, NULL))
  env->class_def->doc = "read/write files";

  // import vars
  o_fileio_dir = import_mvar(env, "int",  "@dir",   0, 0, "place for the directory");
  CHECK_BB(o_fileio_dir)
  o_fileio_file = import_mvar(env, "int", "@file",  0, 0, "place for the file");
  CHECK_BB(o_fileio_file)
  o_fileio_ascii = import_mvar(env, "int", "ascii", 0, 0, "ascii or binary");
  CHECK_BB(o_fileio_file)
  o_fileio_line = import_mvar(env, "int", "@line",  0, 0, "place for the file");
  CHECK_BB(o_fileio_line)


  // import funcs
  fun = new_DL_Func("int", "nl", (m_uint)file_nl);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "open", (m_uint)file_open);
  dl_func_add_arg(fun, "string", "filename");
  dl_func_add_arg(fun, "string", "mode");
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "close", (m_uint)file_close);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "remove", (m_uint)file_remove);
  dl_func_add_arg(fun, "string", "filename");
  CHECK_OB(import_sfun(env, fun))
  fun = new_DL_Func("string[]", "list", (m_uint)file_list);
  dl_func_add_arg(fun, "string", "filename");
  CHECK_OB(import_sfun(env, fun))

  // import operators
  CHECK_BB(add_binary_op(env, op_chuck,     &t_int,    &t_fileio, &t_int,     int_to_file, 1))
  CHECK_BB(add_binary_op(env, op_chuck,     &t_fileio, &t_int,    &t_int,     file_to_int, 1))
  CHECK_BB(add_binary_op(env, op_chuck,     &t_float,  &t_fileio, &t_int,     float_to_file, 1))
  CHECK_BB(add_binary_op(env, op_chuck,     &t_fileio, &t_float,  &t_float,   file_to_float, 1))
  CHECK_BB(add_binary_op(env, op_chuck,     &t_string, &t_fileio,  &t_int,    string_to_file, 1))
  CHECK_BB(add_binary_op(env, op_chuck,     &t_fileio, &t_string,  &t_string, file_to_string, 1))

  CHECK_BB(import_class_end(env))

  M_Object gw_stdin = new_M_Object();
  initialize_object(gw_stdin, &t_fileio);
  IO_FILE(gw_stdin) = fdopen(STDIN_FILENO, "r");
//  IO_FILE(gw_stdin) = stdin;
  add_global_value(env, "cin", &t_fileio,   1, gw_stdin);

  M_Object cout = new_M_Object();
  initialize_object(cout, &t_fileio);
  IO_FILE(cout) = fdopen(STDOUT_FILENO, "w");
  M_Object cerr = new_M_Object();
  initialize_object(cerr, &t_fileio);
  IO_FILE(cerr) = fdopen(STDERR_FILENO, "w");
  add_global_value(env, "cout",  &t_fileio,   1, cout);
  add_global_value(env, "cerr",  &t_fileio,   1, cerr);

  return 1;
}

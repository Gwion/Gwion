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


struct Type_ t_io      = { "IO",       SZ_INT, &t_event, te_io };
struct Type_ t_fileio  = { "FileIO",   SZ_INT, &t_io,    te_fileio };
struct Type_ t_cout    = { "@Cout",    SZ_INT, &t_fileio,    te_fileio };
struct Type_ t_cerr    = { "@Cerr",    SZ_INT, &t_fileio,    te_fileio };
struct Type_ t_cin     = { "@Cin",     SZ_INT, &t_fileio,    te_fileio  };

static M_Object gw_cin, gw_cout, gw_cerr;
m_int o_fileio_dir;
m_int o_fileio_file;
m_int o_fileio_ascii;
m_int o_fileio_line;

#define IO_LINE(o) *(m_str*)(o->d.data + o_fileio_line)
#define CHECK_FIO(o)   if(!IO_FILE(o)) { err_msg(INSTR_, 0, "trying to write an empty file."); Except(shred); }
CTOR(fileio_ctor)
{
  IO_DIR(o)  = NULL;
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

DTOR(static_fileio_dtor)
{
//exit(2);
  IO_FILE(o) = NULL;
//  IO_FILE(o) = NULL;
}

INSTR(int_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)shred->reg;
  if(o != gw_cout && o != gw_cerr) {
    o = **(M_Object**)shred->reg;
    release(o, shred);
  }
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%li", *(m_int*)(shred->reg - SZ_INT));
  *(M_Object*)(shred->reg - SZ_INT) = o;
}

INSTR(float_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)shred->reg;
  if(o != gw_cout && o != gw_cerr) {
    o = **(M_Object**)shred->reg;
    release(o, shred);
  }
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%f", *(m_float*)(shred->reg - SZ_FLOAT));
  PUSH_REG(shred, SZ_FLOAT);
  *(M_Object*)(shred->reg - SZ_INT) = o;
}

INSTR(string_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)shred->reg;
  if(o != gw_cout && o != gw_cerr) {
    o = **(M_Object**)shred->reg;
    release(o, shred);
  }
  M_Object lhs = *(M_Object*)(shred->reg - SZ_INT);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%s", lhs ? STRING(lhs) : NULL);
  *(M_Object*)(shred->reg -SZ_INT)= o;
  release(lhs, shred);
}

INSTR(object_to_file)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)shred->reg;
  if(o != gw_cout && o != gw_cerr) {
    o = **(M_Object**)shred->reg;
    release(o, shred);
  }
  M_Object lhs = *(M_Object*)(shred->reg - SZ_INT);
  fprintf(IO_FILE(o), "%p", (void*)lhs);
  *(M_Object*)(shred->reg -SZ_INT)= o;
  release(lhs, shred);
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
    if(fscanf(IO_FILE(o), "%i", &ret) < 0) {
	err_msg(INSTR_, 0, "problem while reading file.");
      Except(shred);
    }
    *(m_uint*)(shred->reg - SZ_INT)= (**(m_uint**)(shred->reg) = ret);
  } else {
	err_msg(INSTR_, 0, "trying to read from empty file.");
	Except(shred);
  }
  release(o, shred);
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
    if(fscanf(IO_FILE(o), "%f", &ret) < 0) {
      Except(shred);
    }
    *(m_float*)(shred->reg - SZ_FLOAT) = (**(m_float**)(shred->reg) = ret);
  } else {
	err_msg(INSTR_, 0, "trying to read from empty file.");
	Except(shred);
  }
  release(o, shred);
}
/*
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
*/
INSTR(file_to_string)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => string");
#endif
  POP_REG(shred, SZ_INT)
  /*  char ret[1024];*/
  M_Object o    = *(M_Object*)(shred->reg - SZ_INT);
  M_Object s    = **(M_Object**)(shred->reg);
  char c[1025];
  if(IO_ASCII(o))
  {
//    if(inputAvailable(IO_FILE(o)))
    if(fscanf(IO_FILE(o), "%s1024", c) < 0) {
//      if(getline(&c, &size, IO_FILE(o)) < 0) {
        Except(shred);
        return;
      }
    STRING(s) = S_name(insert_symbol(c));
    *(M_Object*)(shred->reg - SZ_INT) = s;
  }
  release(o, shred);
  release(s, shred);
}

MFUN(file_nl)
{
  RETURN->d.v_uint = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

MFUN(file_open)
{
  DIR* dir;
  M_Object lhs = *(M_Object*)(shred->mem + SZ_INT * 2);
  M_Object rhs = *(M_Object*)(shred->mem + SZ_INT);
  m_str filename = STRING(rhs);
  m_str mode = STRING(lhs);
  release(rhs, shred);
  release(lhs, shred);

  if(IO_DIR(o)) {
    closedir(IO_DIR(o));
    IO_DIR(o) = NULL;
  } else if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  if((dir = opendir(filename))) {
    IO_DIR(o) = dir;
    RETURN->d.v_uint = 2;
    return;
  }
  IO_FILE(o) = fopen(filename, mode);
  IO_DIR(o) = NULL;
  RETURN->d.v_uint = IO_FILE(o) ? 1 : 0;
}

MFUN(file_close)
{
  if(IO_DIR(o)) {
    RETURN->d.v_uint = closedir(IO_DIR(o));
    IO_DIR(o) = NULL;
    return;
  }
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  RETURN->d.v_uint = !IO_FILE(o) ? 1 : 0;
}

SFUN(file_remove)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  if(!obj)
	return;
  release(obj, shred);
  RETURN->d.v_uint = remove(STRING(*(M_Object*)(shred->mem + SZ_INT)));
}

SFUN(file_list)
{
  m_uint i;
  struct dirent **namelist;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str str;
  if(!obj)
    return;
  str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  m_int n = scandir(str, &namelist, NULL, alphasort);
  if(n < 0) {
    RETURN->d.v_uint = 0;
    return;
  }
  Type t = new_array_type(shred->vm_ref->env, &t_array, 1, &t_string, shred->vm_ref->env->curr);
  M_Object ret = new_M_Array(SZ_INT, n, 1);
  ret->type_ref = t;
  t->obj->ref_count = 1;
  for(i = 0; i < n; i++) {
    M_Object string = new_String(namelist[i]->d_name);
    string->ref = 1;
    i_vector_set(ret->d.array, i, (m_uint)string);
    free(namelist[i]);
  }
  free(namelist);
  RETURN->d.v_uint = (m_uint)ret;
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
  CHECK_OB(import_class_begin(env, &t_fileio, env->global_nspc, fileio_ctor, fileio_dtor))
  env->class_def->doc = "read/write files";

  // import vars
  o_fileio_dir = import_mvar(env, "int",  "@dir",   0, 0, "place for the directory");
  CHECK_BB(o_fileio_dir)
  o_fileio_file = import_mvar(env, "int", "@file",  0, 0, "place for the file");
  CHECK_BB(o_fileio_file)
  o_fileio_ascii = import_mvar(env, "int", "ascii", 0, 0, "ascii or binary");
  CHECK_BB(o_fileio_ascii)
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
  CHECK_BB(add_binary_op(env, op_chuck, &t_int,    &t_fileio, &t_fileio, int_to_file, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_float,  &t_fileio, &t_fileio, float_to_file, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_string, &t_fileio, &t_fileio, string_to_file, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_object, &t_fileio, &t_fileio, object_to_file, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_null,   &t_fileio, &t_fileio, object_to_file, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_fileio, &t_string, &t_string, file_to_string, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_fileio, &t_int,    &t_int,    file_to_int, 1, 0))
  CHECK_BB(add_binary_op(env, op_chuck, &t_fileio, &t_float,  &t_float,  file_to_float, 1, 0))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_cout))
  CHECK_OB(import_class_begin(env, &t_cout, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_cerr))
  CHECK_OB(import_class_begin(env, &t_cerr, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_cin))
  CHECK_OB(import_class_begin(env, &t_cin, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  gw_cin = new_M_Object();
  initialize_object(gw_cin, &t_cin);

  gw_cout = new_M_Object();
  initialize_object(gw_cout, &t_cout);
  IO_FILE(gw_cout) = stdout;
  gw_cerr = new_M_Object();
  initialize_object(gw_cerr, &t_cerr);
  IO_FILE(gw_cerr) = stderr;
  add_global_value(env, "cin",  &t_fileio, 1, gw_cin);
  add_global_value(env, "cout", &t_fileio, 1, gw_cout);
  add_global_value(env, "cerr", &t_fileio, 1, gw_cerr);
  return 1;
}

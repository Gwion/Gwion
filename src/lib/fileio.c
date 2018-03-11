#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <complex.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "lang.h"

#define CHECK_FIO(o)   if(!o || !IO_FILE(o)) Except(shred, "EmptyFileException");

struct Type_ t_fileio  = { "FileIO", SZ_INT, &t_event  };
struct Type_ t_cout    = { "@Cout",  SZ_INT, &t_fileio };
struct Type_ t_cerr    = { "@Cerr",  SZ_INT, &t_fileio };
struct Type_ t_cin     = { "@Cin",   SZ_INT, &t_fileio };

m_int o_fileio_file;
static M_Object gw_cin, gw_cout, gw_cerr;

CTOR(fileio_ctor) {
  IO_FILE(o)  = NULL;

}

DTOR(fileio_dtor) {
  if(IO_FILE(o))
    fclose(IO_FILE(o));
}

DTOR(static_fileio_dtor) {
  IO_FILE(o) = NULL;
}

static INSTR(int_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "%" INT_F "", *(m_int*)REG(- SZ_INT));
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(float_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "%f", *(m_float*)REG(- SZ_FLOAT));
  PUSH_REG(shred, SZ_FLOAT);
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(string_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(lhs, shred);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "%s", lhs ? STRING(lhs) : NULL);
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(object_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(lhs, shred);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "%p", (void*)lhs);
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(complex_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  m_complex lhs = *(m_complex*)REG(- SZ_COMPLEX);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "#(%f, %f)", creal(lhs), cimag(lhs));
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(polar_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  m_complex lhs = *(m_complex*)REG(- SZ_COMPLEX);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "%%(%f, %f * pi)", creal(lhs), cimag(lhs) / M_PI);
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(vec3_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  m_vec3 lhs = *(m_vec3*)REG(- SZ_VEC3);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "@(%f, %f, %f)", lhs.x, lhs.y, lhs.z);
  POP_REG(shred, SZ_FLOAT)
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(vec4_to_file) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  m_vec4 lhs = *(m_vec4*)REG(- SZ_VEC4);
  CHECK_FIO(o)
  release(o, shred);
  fprintf(IO_FILE(o), "@(%f, %f, %f, %f)", lhs.x, lhs.y, lhs.z, lhs.w);
  POP_REG(shred, 2*SZ_FLOAT)
  *(M_Object*)REG(- SZ_INT) = o;
}

static INSTR(file_to_int) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  int ret;
  M_Object o = *(M_Object*)REG(- SZ_INT);
  if(!o) {
    Except(shred, "EmptyFileException");
  }
  release(o, shred);
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%i", &ret) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    *(m_uint*)REG(- SZ_INT) = (**(m_uint**)REG(0) = ret);
  } else
    Except(shred, "EmptyFileException");
}

static INSTR(file_to_float) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  float ret;
  M_Object o = *(M_Object*)REG(-SZ_INT);
  if(!o) {
    Except(shred, "EmptyFileException");
  }
  release(o, shred);
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%f", &ret) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    *(m_float*)REG(- SZ_FLOAT) = (**(m_float**)REG(0) = ret);
  } else
    Except(shred, "EmptyFileException");
  POP_REG(shred, SZ_FLOAT)
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
static INSTR(file_to_string) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  M_Object o    = *(M_Object*)REG(- SZ_INT);
  M_Object s    = **(M_Object**)REG(0);
  if(!o) {
    Except(shred, "EmptyFileException");
  }
  if(!s) {
    Except(shred, "EmptyStringException");
  }
  if(IO_FILE(o)) {
    char c[1025];
    if(fscanf(IO_FILE(o), "%1024s", c) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    STRING(s) = s_name(insert_symbol(c));
    *(M_Object*)REG(- SZ_INT) = s;
  }
  release(o, shred);
  release(s, shred);
}

static MFUN(file_nl) {
  *(m_uint*)RETURN = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

static MFUN(file_open) {
  M_Object lhs = *(M_Object*)MEM(SZ_INT * 2);
  M_Object rhs = *(M_Object*)MEM(SZ_INT);
  if(!lhs || !rhs)
    Except(shred, "invalid arguments to FileIO.open()");
  m_str filename = STRING(rhs);
  m_str mode = STRING(lhs);
  release(rhs, shred);
  release(lhs, shred);
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  IO_FILE(o) = fopen(filename, mode);
  *(m_uint*)RETURN = IO_FILE(o) ? 1 : 0;
}

static MFUN(file_close) {
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  *(m_uint*)RETURN = !IO_FILE(o) ? 1 : 0;
}

static SFUN(file_remove) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    return;
  release(obj, shred);
  *(m_uint*)RETURN = remove(STRING(*(M_Object*)MEM(SZ_INT)));
}

static SFUN(file_list) {
  m_uint i;
  struct dirent **namelist;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str str;
  if(!obj)
    return;
  str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  m_int n = scandir(str, &namelist, NULL, alphasort);
  if(n < 0) {
    *(m_uint*)RETURN = 0;
    return;
  }
  Type t = array_type(&t_string, 1);
  M_Object ret = new_M_Array(t, SZ_INT, n, 1);
  vector_add(&shred->gc, (vtype)ret);
  for(i = 0; i < n; i++) {
    M_Object string = new_String(NULL, namelist[i]->d_name);
    m_vector_set(ARRAY(ret), i, &string);
    free(namelist[i]);
  }
  free(namelist);
  *(m_uint*)RETURN = (m_uint)ret;
}

m_bool import_fileio(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_fileio, fileio_ctor, fileio_dtor))

  // import vars
	gwi_item_ini(gwi, "int", "@file");
  o_fileio_file = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_fileio_file)

  // import funcs
  gwi_func_ini(gwi, "int", "nl", file_nl);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "open", file_open);
  gwi_func_arg(gwi, "string", "filename");
  gwi_func_arg(gwi, "string", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "close", file_close);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "remove", file_remove);
  gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  gwi_func_ini(gwi, "string[]", "list", file_list);
  gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  // import operators
  CHECK_BB(gwi_oper_ini(gwi, "int",    "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, int_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "float",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, float_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "complex",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, complex_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "polar",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, polar_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "Vec3",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec3_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "Vec4",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec4_to_file))
  CHECK_BB(gwi_oper_ini(gwi,"string", "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, string_to_file))
  CHECK_BB(gwi_oper_ini(gwi,"Object", "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, object_to_file))
  CHECK_BB(gwi_oper_ini(gwi,"@null",  "FileIO", "FileIO"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, object_to_file))
  CHECK_BB(gwi_oper_ini(gwi, "FileIO", "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, file_to_string))
  CHECK_BB(gwi_oper_ini(gwi, "FileIO", "int",    "int"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, file_to_int))
  CHECK_BB(gwi_oper_ini(gwi, "FileIO", "float",  "float"))
  CHECK_BB(gwi_oper_add(gwi, opck_rhs_emit_var))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, file_to_float))

  CHECK_BB(gwi_class_ini(gwi,  &t_cout, NULL, static_fileio_dtor))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_class_ini(gwi,  &t_cerr, NULL, static_fileio_dtor))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_class_ini(gwi,  &t_cin, NULL, static_fileio_dtor))
  CHECK_BB(gwi_class_end(gwi))

  gw_cin = new_M_Object(NULL);
  initialize_object(gw_cin, &t_cin);
  EV_SHREDS(gw_cin) = new_vector();
  gw_cout = new_M_Object(NULL);
  initialize_object(gw_cout, &t_cout);
  IO_FILE(gw_cout) = stdout;
  EV_SHREDS(gw_cout) = new_vector();
  gw_cerr = new_M_Object(NULL);
  initialize_object(gw_cerr, &t_cerr);
  IO_FILE(gw_cerr) = stderr;
  EV_SHREDS(gw_cerr) = new_vector();
//  gwi_add_value(gwi,  "cin",  &t_fileio, 1, gw_cin);
//  gwi_add_value(gwi,  "cout", &t_fileio, 1, gw_cout);
//  gwi_add_value(gwi,  "cerr", &t_fileio, 1, gw_cerr);
  gwi_item_ini(gwi, "FileIO", "cin");
  gwi_item_end(gwi, ae_flag_const, gw_cin);
  gwi_item_ini(gwi, "FileIO", "cout");
  gwi_item_end(gwi, ae_flag_const, gw_cout);
  gwi_item_ini(gwi, "FileIO", "cerr");
  gwi_item_end(gwi, ae_flag_const, gw_cerr);
  return 1;
}

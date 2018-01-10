#include <dirent.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "lang.h"

#define CHECK_FIO(o)   if(!o || !IO_FILE(o)) { if(o)release(o, shred); err_msg(INSTR_, 0, "trying to write an empty file."); Except(shred, "EmptyFileException"); }

struct Type_ t_fileio  = { "FileIO", SZ_INT, &t_event,  te_fileio };
struct Type_ t_cout    = { "@Cout",  SZ_INT, &t_fileio, te_fileio };
struct Type_ t_cerr    = { "@Cerr",  SZ_INT, &t_fileio, te_fileio };
struct Type_ t_cin     = { "@Cin",   SZ_INT, &t_fileio, te_fileio };

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

INSTR(int_to_file) {
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  release(o, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%" INT_F "", *(m_int*)REG(- SZ_INT));
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(float_to_file) {
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  o = **(M_Object**)REG(0);
  release(o, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%f", *(m_float*)REG(- SZ_FLOAT));
  PUSH_REG(shred, SZ_FLOAT);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(string_to_file) {
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(o, shred);
  release(lhs, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%s", lhs ? STRING(lhs) : NULL);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(object_to_file) {
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(o, shred);
  release(lhs, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%p", (void*)lhs);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(file_to_int) {
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
  } else {
    release(o, shred);
    Except(shred, "EmptyFileException");
  }
}

INSTR(file_to_float) {
  POP_REG(shred, SZ_INT)
  /*  m_float ret;*/
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
  } else {
    release(o, shred);
    Except(shred, "EmptyFileException");
  }
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
INSTR(file_to_string) {
  POP_REG(shred, SZ_INT)
  M_Object o    = *(M_Object*)REG(- SZ_INT);
  M_Object s    = **(M_Object**)REG(0);
  if(!o) {
    release(s, shred);
    Except(shred, "EmptyFileException");
  }
  if(!s) {
    release(o, shred);
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

MFUN(file_nl) {
  *(m_uint*)RETURN = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

MFUN(file_open) {
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

MFUN(file_close) {
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  *(m_uint*)RETURN = !IO_FILE(o) ? 1 : 0;
}

SFUN(file_remove) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    return;
  release(obj, shred);
  *(m_uint*)RETURN = remove(STRING(*(M_Object*)MEM(SZ_INT)));
}

SFUN(file_list) {
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
  M_Object ret = new_M_Array(SZ_INT, n, 1);
  vector_add(&shred->gc, (vtype)ret);
  ret->type_ref = t;
  for(i = 0; i < n; i++) {
    M_Object string = new_String(NULL, namelist[i]->d_name);
    i_vector_set(ARRAY(ret), i, (m_uint)string);
    free(namelist[i]);
  }
  free(namelist);
  *(m_uint*)RETURN = (m_uint)ret;
}

m_bool import_fileio(Importer importer) {
  CHECK_BB(importer_class_ini(importer,  &t_fileio, fileio_ctor, fileio_dtor))

  // import vars
	importer_item_ini(importer, "int", "@file");
  o_fileio_file = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_fileio_file)

  // import funcs
  importer_func_ini(importer, "int", "nl", (m_uint)file_nl);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "open", (m_uint)file_open);
  importer_func_arg(importer, "string", "filename");
  importer_func_arg(importer, "string", "mode");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "close", (m_uint)file_close);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "remove", (m_uint)file_remove);
  importer_func_arg(importer, "string", "filename");
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  importer_func_ini(importer, "string[]", "list", (m_uint)file_list);
  importer_func_arg(importer, "string", "filename");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  // import operators
  CHECK_BB(importer_oper_ini(importer, "int",    "FileIO", "FileIO"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, int_to_file, 1))
  CHECK_BB(importer_oper_ini(importer, "float",  "FileIO", "FileIO"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, float_to_file, 1))
  CHECK_BB(importer_oper_ini(importer,"string", "FileIO", "FileIO"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, string_to_file, 1))
  CHECK_BB(importer_oper_ini(importer,"Object", "FileIO", "FileIO"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, object_to_file, 1))
  CHECK_BB(importer_oper_ini(importer,"@null",  "FileIO", "FileIO"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, object_to_file, 1))
  CHECK_BB(importer_oper_ini(importer, "FileIO", "string", "string"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, file_to_string, 1))
  CHECK_BB(importer_oper_ini(importer, "FileIO", "int",    "int"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, file_to_int, 1))
  CHECK_BB(importer_oper_ini(importer, "FileIO", "float",  "float"))
  CHECK_BB(importer_oper_add(importer, opck_rhs_emit_var))
  CHECK_BB(importer_oper_end(importer, op_chuck, file_to_float, 1))
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer,  &t_cout, NULL, static_fileio_dtor))
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer,  &t_cerr, NULL, static_fileio_dtor))
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer,  &t_cin, NULL, static_fileio_dtor))
  CHECK_BB(importer_class_end(importer))

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
  importer_add_value(importer,  "cin",  &t_fileio, 1, gw_cin);
  importer_add_value(importer,  "cout", &t_fileio, 1, gw_cout);
  importer_add_value(importer,  "cerr", &t_fileio, 1, gw_cerr);
  return 1;
}

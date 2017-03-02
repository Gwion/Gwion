/* use this to generate include/generated.h */
#include <stdio.h>
#include <stdarg.h>
#ifdef USE_DOUBLE
#define GW_DOUBLE
#undef USE_DOUBLE
#endif
#include <soundpipe.h>

#include "err_msg.h"

static const char* filename = "include/generated.h";

#ifdef USE_DOUBLE
static const char* type	 = "double";
static const char* other = "float ";
#define SZ sizeof(double)
#else
static const char* type	 = "float ";
static const char* other = "double";
#define SZ sizeof(float)
#endif

int main(int argc , char** argv)
{
  FILE* file;
  debug_msg("config", "creating Gwion header.");
  if(SZ != sizeof(SPFLOAT)) {
    err_msg(INSTR_, 0, "sizes do not match. please recompile soundpipe to use %s.", other);
    return 1;
  }
  if(argc >= 2)
	filename = argv[1];
  file = fopen(filename, "w");
  if(!file) {
    err_msg(INSTR_, 0, "can't open '%s' for writing.", filename);
    return 1;
  }
  fprintf(file,
          "/* common typedefs */\ntypedef long          int m_int;\ntypedef long unsigned int m_uint;\n\
typedef short         int m_bool;\ntypedef %s            m_float;\ntypedef char *            m_str;\n\
typedef struct { m_float x, y, z; }  VEC3_T;\ntypedef struct { m_float x, y, z, w; } VEC4_T;\n\
typedef _Complex %s m_complex;\n", type, type);
  fprintf(file, "#define SZ_INT     %lu\n", sizeof(long unsigned int));
  fprintf(file, "#define SZ_FLOAT   %lu\n", sizeof(SPFLOAT));
  fprintf(file, "#define SZ_COMPLEX %lu\n", sizeof(SPFLOAT) * 2);
  fprintf(file, "#define SZ_VEC3    %lu\n", sizeof(SPFLOAT) * 3);
  fprintf(file, "#define SZ_VEC4    %lu\n", sizeof(SPFLOAT) * 4);
  fclose(file);
  debug_msg("config", "config written to '%s'.", filename);
  return 0;
}

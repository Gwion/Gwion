/* use this to generate include/generated.h */
#include <stdio.h>
#include <inttypes.h>

#ifdef USE_DOUBLE
#define GW_DOUBLE
#undef USE_DOUBLE
#endif
#include <soundpipe.h>

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

int main(int argc, char** argv) {
  FILE* file;
  fprintf(stderr, "creating Gwion header.\n");
  if(SZ != sizeof(SPFLOAT)) {
    fprintf(stderr, "sizes do not match. please recompile soundpipe to use %s.\n", other);
    return 1;
  }
  if(argc >= 2)
    filename = argv[1];
  file = fopen(filename, "w");
  if(!file) {
    fprintf(stderr, "can't open '%s' for writing.\n", filename);
    return 1;
  }
  fprintf(file,
          "#include <inttypes.h>\n/* common typedefs */\ntypedef intptr_t m_int;\ntypedef uintptr_t m_uint;\n\
typedef short         int m_bool;\ntypedef %s            m_float;\ntypedef char *            m_str;\n\
typedef struct { m_float x, y, z; }  m_vec3;\ntypedef struct { m_float x, y, z, w; } m_vec4;\n\
typedef _Complex %s m_complex;\n", type, type);
  fprintf(file, "#define SZ_INT     %lu\n", sizeof(uintptr_t));
  fprintf(file, "#define SZ_FLOAT   %lu\n", sizeof(SPFLOAT));
  fprintf(file, "#define SZ_COMPLEX %lu\n", sizeof(SPFLOAT) * 2);
  fprintf(file, "#define SZ_VEC3    %lu\n", sizeof(SPFLOAT) * 3);
  fprintf(file, "#define SZ_VEC4    %lu\n", sizeof(SPFLOAT) * 4);
  fclose(file);
  fprintf(stderr, "config written to '%s'.\n", filename);
  return 0;
}

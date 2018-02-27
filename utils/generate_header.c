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
#define SZ sizeof(double)
#else
static const char* type	 = "float ";
#define SZ sizeof(float)
#endif

int main(int argc, char** argv) {
  FILE* file;
  if(SZ != sizeof(SPFLOAT)) {
    fprintf(stderr, "sizes do not match.\n"
        "please recompile soundpipe to use %s.\n", type);
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
  fprintf(file, "#define SZ_INT     %zu\n", sizeof(uintptr_t));
  fprintf(file, "#define SZ_FLOAT   %zu\n", sizeof(SPFLOAT));
  fprintf(file, "#define SZ_COMPLEX %zu\n", sizeof(SPFLOAT) * 2);
  fprintf(file, "#define SZ_VEC3    %zu\n", sizeof(SPFLOAT) * 3);
  fprintf(file, "#define SZ_VEC4    %zu\n", sizeof(SPFLOAT) * 4);
  if(sizeof(uintptr_t) == sizeof(unsigned int)) {
    fprintf(file, "#define  INT_F \"i\"\n");
    fprintf(file, "#define UINT_F \"u\"\n");
  } else {
    fprintf(file, "#define  INT_F \"li\"\n");
    fprintf(file, "#define UINT_F \"lu\"\n");
  }
  fclose(file);
  return 0;
}

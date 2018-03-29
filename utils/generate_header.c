/* use this to generate include/generated.h */
#include <stdio.h>
#include <string.h>
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
typedef unsigned char m_bit;\n\
typedef        int m_bool;\ntypedef %s            m_float;\ntypedef char *            m_str;\n\
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
  if(strcmp(type, "double"))
    fprintf(file, "#define creal crealf\n#define cimag cimagf\n"
                  "#define abs fabs\n"
                  "#define cos cosf\n#define sin sinf\n#define tan tanf\n"
                  "#define acos acosf\n#define asin asinf\n#define atan atanf\n"
                  "#define cosh coshf\n#define sinh sinhf\n#define tanh tanhf\n"
                  "#define acosh acoshf\n#define asinh asinhf\n#define atanh atanhf\n"
                  "#define atan2 atan2f\n#define hypot hypotf\n"
                  "#define pow powf\n#define sqrt sqrtf\n"
                  "#define exp expf\n#define log logf\n"
                  "#define log2 log2f\n#define log10 log10f\n"
                  "#define floor floorf\n#define ceil ceilf\n"
                  "#define round roundf\n#define trunc truncf\n"
                  "#define fmod fmodf\n#define remainder remainderf\n");
  fclose(file);
  return 0;
}

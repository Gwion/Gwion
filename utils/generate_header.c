/* use this to generate include/generated.h */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static const char* filename = "include/generated.h";

#ifdef USE_DOUBLE
static const char* type	 = "double";
#define SZ sizeof(double)
#else
static const char* type	 = "float ";
#define SZ sizeof(float)
#endif

static void include(void) {
  printf("#include <stdlib.h>\n#include <inttypes.h>\n");
}

static void base_type(void) {
  printf("/* common typedefs */\ntypedef intptr_t m_int;\n"
      "typedef uintptr_t m_uint;\ntypedef unsigned char m_bit;\n"
      "typedef int m_bool;\ntypedef %s m_float;\ntypedef char * m_str;\n",
      type);
} 

static void compound_type(void) {
  printf("typedef struct { m_float x, y, z; } m_vec3;\n"
      "typedef struct { m_float x, y, z, w; } m_vec4;\n"
      "typedef _Complex %s m_complex;\n", type);
}

static void size(void) {
  printf("#define SZ_INT     %zu\n#define SZ_FLOAT   %zu\n"
    "#define SZ_COMPLEX %zu\n#define SZ_VEC3    %zu\n#define SZ_VEC4    %zu\n",
    sizeof(uintptr_t), SZ, SZ * 2, SZ * 3, SZ * 4);
}

static void format(void) {
  if(sizeof(uintptr_t) == sizeof(unsigned int))
    printf("#define  INT_F \"i\"\n#define UINT_F \"u\"\n");
  else
    printf("#define  INT_F \"li\"\n#define UINT_F \"lu\"\n#define abs labs\n");
}

static void math_func(void) {
  printf("#define creal crealf\n#define cimag cimagf\n#define fabs fabsf"
      "\n#define cos cosf\n#define sin sinf\n#define tan tanf\n"
      "#define acos acosf\n#define asin asinf\n#define atan atanf\n"
      "#define cosh coshf\n#define sinh sinhf\n#define tanh tanhf\n"
      "#define acosh acoshf\n#define asinh asinhf\n#define atanh atanhf\n"
      "#define atan2 atan2f\n#define hypot hypotf\n#define pow powf\n"
      "#define sqrt sqrtf\n#define exp expf\n#define log logf\n"
      "#define log2 log2f\n#define log10 log10f\n#define floor floorf\n"
      "#define ceil ceilf\n#define round roundf\n#define trunc truncf\n"
      "#define fmod fmodf\n#define remainder remainderf\n");
}

static void minval(void) {
  if(sizeof(uintptr_t) > SZ)
    printf("#define SZ_MINVAL SZ_INT");
  else
    printf("#define SZ_MINVAL SZ_FLOAT");
}

int main(int argc, char** argv) {
  include();
  base_type();
  compound_type();
  size();
  format();
  if(strcmp(type, "double"))
    math_func();
  minval();
  return 0;
}

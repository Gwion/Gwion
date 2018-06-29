#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
#define jit_describe_2file(name, decl, offset, pop, sig, format, ...) \
JIT_CODE(name##_to_file) { GWDEBUG_EXE                                \
  CJval reg = push_reg(j, -SZ_INT);                                   \
  decl                                                                \
  CJval o = JLOADR(reg, 0, void_ptr);                                 \
  CJval null = JCONST(void_ptr, 0);                                   \
  CJval condo = JINSN(eq, o, null);                                  \
  INIT_LABEL(lblo);                                                   \
  JINSN(branch_if_not, condo, &lblo);                                \
  cc_except(j, "NullPtrException");                                   \
  JINSN(label, &lblo);                                               \
  CJval arg[] = { o, j->shred };                                      \
  CALL_NATIVE(_release, "vpp", arg);                                  \
  CJval data = JLOADR(o, JOFF(M_Object, data), void_ptr);             \
  CJval f = JLOADR(data, o_fileio_file, void_ptr);                    \
  CJval condf = JINSN(eq, f, null);                                  \
  INIT_LABEL(lblf);                                                   \
  JINSN(branch_if_not, condf, &lblf);                                \
  cc_except(j, "EmptyFileException");                                 \
  JINSN(label, &lblf);                                               \
  CJval fmt = JCONST(void_ptr, format);                               \
  CJval parg[] = { f, fmt, __VA_ARGS__ };                             \
  CALL_NATIVE(fprintf, "ipp"sig, parg);                               \
  pop;                                                                \
  JSTORER(reg, -SZ_INT, o);                                           \
}
jit_describe_2file(int,
  CJval lhs = JLOADR(reg, -SZ_INT, nint);,
  SZ_INT,, "I", "%"INT_F, lhs)
jit_describe_2file(float,
  CJval lhs = JLOADR(reg, -SZ_FLOAT, float);,
  SZ_INT, push_reg(j, SZ_FLOAT), "f", "%f", lhs)
jit_describe_2file(complex,
  CJval real = JLOADR(reg, -SZ_COMPLEX, float);
  CJval imag = JLOADR(reg, -SZ_FLOAT, float);,
  SZ_COMPLEX,, "ff", "#(%.4f, %.4f)", real, imag)
jit_describe_2file(polar,
  CJval real = JLOADR(reg, -SZ_COMPLEX, float);
  CJval  src = JLOADR(reg, -SZ_FLOAT, float);
  CJval   pi = JCONSTF(M_PI);
  CJval imag = JINSN(div, src, pi);,
  SZ_COMPLEX,, "ff", "%%(%.4f, %.4f)", real, imag)
jit_describe_2file(vec3,
  CJval x = JLOADR(reg, -SZ_VEC3, float);
  CJval y = JLOADR(reg, -SZ_COMPLEX, float);
  CJval z = JLOADR(reg, -SZ_FLOAT, float);,
  SZ_VEC3, push_reg(j, -SZ_FLOAT), "fff", "@(%.4f, %.4f, %.4f)", x, y, z)
jit_describe_2file(vec4,
  CJval x = JLOADR(reg, -SZ_VEC4, float);
  CJval y = JLOADR(reg, -SZ_VEC3, float);
  CJval z = JLOADR(reg, -SZ_COMPLEX, float);
  CJval w = JLOADR(reg, -SZ_FLOAT, float);,
  SZ_VEC3, push_reg(j, -SZ_COMPLEX), "ffff", "@(%.4f, %.4f, %.4f, %.4f)", x, y, z, w)
jit_describe_2file(object,
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);,
  SZ_INT, cc_release(j, lhs), "p", "%p", lhs)
jit_describe_2file(string,
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval sarg[] = { lhs };
  CJval str = CALL_NATIVE(safe_string, "pp", sarg);,
  SZ_INT, cc_release(j, lhs), "p", "%s", str)
*/

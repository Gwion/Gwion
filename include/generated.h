/* common typedefs */
typedef long          int m_int;
typedef long unsigned int m_uint;
typedef short         int m_bool;
typedef double            m_float;
typedef char *            m_str;
typedef struct { m_float x, y, z; }  VEC3_T;
typedef struct { m_float x, y, z, w; } VEC4_T;
#define SZ_INT     8
#define SZ_FLOAT   8
#define SZ_COMPLEX 16
#define SZ_VEC3    24
#define SZ_VEC4    32

/* use this to generate include/generated.h */
#include <stdio.h>
#include <soundpipe.h>

static const char* filename = "include/generated.h";

#ifdef USE_DOUBLE
static const char* type	= "double";
#define SZ sizeof(double)
#else
static const char* type	= "float ";
#define SZ sizeof(float)
#endif

int main()
{
	FILE* file;
	if(SZ != sizeof(SPFLOAT))
	{
		fprintf(stderr, "sizes do not match. please recomile soundpipe to use %s.\n", type);
		return 1;
	}
	file = fopen(filename, "w");
	if(!file)
	{
		fprintf(stderr, "can't open '%s' for writing.", filename);
		return 1;
	}
	fprintf(file,
"/* common typedefs */\ntypedef long          int m_int;\ntypedef long unsigned int m_uint;\n\
typedef short         int m_bool;\ntypedef %s            m_float;\ntypedef char *            m_str;\n\
typedef struct { m_float x, y, z; }  VEC3_T;\ntypedef struct { m_float x, y, z, w; } VEC4_T;\n", type);
	fprintf(file, "#define SZ_INT     %lu\n", sizeof(long unsigned int));
	fprintf(file, "#define SZ_FLOAT   %lu\n", sizeof(SPFLOAT));
	fprintf(file, "#define SZ_COMPLEX %lu\n", sizeof(SPFLOAT) * 2);
	fprintf(file, "#define SZ_VEC3    %lu\n", sizeof(SPFLOAT) * 3);
	fprintf(file, "#define SZ_VEC4    %lu\n", sizeof(SPFLOAT) * 4);
	fclose(file);
	return 0;
}

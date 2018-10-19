#ifdef JIT
#define NPLUG_VEC 5
#else
#define NPLUG_VEC 2
#endif

typedef struct Vector_ PlugInfo[NPLUG_VEC];
void plug_ini(PlugInfo, Vector);
void plug_end(PlugInfo);

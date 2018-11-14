#ifndef __PLUG
#define __PLUG
#define NPLUG_VEC 2
typedef struct Vector_ PlugInfo[NPLUG_VEC];
void plug_ini(PlugInfo, Vector);
void plug_end(PlugInfo);
#endif

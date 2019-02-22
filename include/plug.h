#ifndef __PLUG
#define __PLUG

enum plug_t {
  GWPLUG_DL,
  GWPLUG_IMPORT,
  GWPLUG_MODULE,
  GWPLUG_LAST
};

typedef struct Vector_ PlugInfo[GWPLUG_LAST];
void plug_ini(PlugInfo, Vector);
void plug_end(const Gwion gwion, PlugInfo);
#endif

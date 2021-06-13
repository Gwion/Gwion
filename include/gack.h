#ifndef __GACK
#define __GACK
ANN void gack(const VM_Shred, const m_uint);

#define INTERP_PRINTF(fmt, ...)                                                \
  {                                                                            \
    m_str ret = *(m_str *)(VALUE - SZ_INT);                                    \
    gw_asprintf(shred->info->mp, &ret, fmt, ##__VA_ARGS__);                    \
    *(m_str *)(VALUE - SZ_INT) = ret;                                          \
  }

ANN2(2) int gw_asprintf(MemPool mp, char **str, const char *fmt, ...);
#endif

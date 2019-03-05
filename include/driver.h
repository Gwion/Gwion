#ifndef __DRIVER
#define __DRIVER

struct _driver;
typedef struct DriverInfo_ {
  uint32_t   sr;
  m_str arg;
  void (*func)(struct _driver*);
  void (*run)(const VM*);
  void* data;
  uint8_t in, out;
} DriverInfo;

typedef struct _driver {
  m_bool (*ini)(VM* vm, DriverInfo*);
  void   (*run)(VM* vm, struct DriverInfo_*);
  void   (*del)(VM* vm, struct DriverInfo_*);
} Driver;

void dummy_driver(Driver*);
ANN struct BBQ_* new_bbq(DriverInfo*);
#endif

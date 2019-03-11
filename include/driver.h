#ifndef __DRIVER
#define __DRIVER
struct Driver_;

typedef void (*f_diset)(struct Driver_*);
typedef void (*f_dirun)(const VM*);

typedef struct DriverInfo_ {
  uint32_t   sr;
  m_str arg;
  f_diset func;
  f_dirun run;
  void* data;
  struct Driver_* driver;
  uint8_t in, out;
} DriverInfo;

typedef m_bool (*f_drvini)(VM*, DriverInfo*);
typedef void   (*f_drvrun)(VM* vm, struct DriverInfo_*);
typedef void   (*f_drvdel)(VM* vm, struct DriverInfo_*);

typedef struct Driver_ {
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
} Driver;

void dummy_driver(Driver*);
ANN struct BBQ_* new_bbq(DriverInfo*);
#endif

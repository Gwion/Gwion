#ifndef __DRIVER
#define __DRIVER
typedef struct BBQ_ DriverInfo;
typedef m_bool (*f_drvini)(VM*, struct BBQ_*);
typedef void   (*f_drvrun)(VM*, struct BBQ_*);
typedef void   (*f_drvdel)(VM*, struct BBQ_*);

typedef struct Driver_ {
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
  void* data;
} Driver;

ANN void dummy_driver(Driver*);
ANN void bbq_alloc(DriverInfo*);
#endif

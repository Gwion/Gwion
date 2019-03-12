#ifndef __DRIVER
#define __DRIVER

typedef m_bool (*f_drvini)(VM*, struct BBQ_*);
typedef void   (*f_drvrun)(VM*, struct BBQ_*);
typedef void   (*f_drvdel)(VM*, struct BBQ_*);

typedef struct DriverData_ {
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
  void* data;
} DriverData;

ANN void dummy_driver(DriverData*);
ANN void bbq_alloc(struct BBQ_*);
#endif

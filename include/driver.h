#ifndef __DRIVER
#define __DRIVER

typedef m_bool (*f_drvini)(VM*, struct BBQ_*);
typedef void   (*f_drvrun)(VM*, struct BBQ_*);
typedef void   (*f_drvdel)(VM*, struct BBQ_*);
#define DRVRUN(a) void a(VM *vm, struct BBQ_* di)
#define DRVDEL(a) void a(VM *vm, struct BBQ_* di)
typedef struct DriverData_ {
  void* data;
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
} DriverData;

ANN void dummy_driver(DriverData*);
ANN void bbq_alloc(struct BBQ_*);
#endif

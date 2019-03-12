#ifndef __DRIVER
#define __DRIVER

typedef m_bool (*f_drvini)(VM*, struct BBQ_*);
typedef void   (*f_drvrun)(VM*, struct BBQ_*);
typedef void   (*f_drvdel)(VM*, struct BBQ_*);

#define DRVINI(a) ANN m_bool a(VM *vm __attribute__((unused)), struct BBQ_* di __attribute__((unused)))
#define DRVRUN(a) ANN void   a(VM *vm __attribute__((unused)), struct BBQ_* di __attribute__((unused)))
#define DRVDEL(a) ANN void   a(VM *vm __attribute__((unused)), struct BBQ_* di __attribute__((unused)))

typedef struct DriverData_ {
  void* data;
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
} DriverData;

ANN void dummy_driver(DriverData*);
ANN void bbq_alloc(struct BBQ_*);
#endif

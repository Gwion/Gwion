#ifndef __AST_DATA
#define __AST_DATA

typedef struct DataTable_* DataTable;
ANN DataTable new_datatable(MemPool p, size_t sz);
ANN void datatable_reset(DataTable);
ANN void free_datatable(DataTable);

ANN struct Type_*   dataget_type  (DataTable, void*);
//ANN Type   dataget_cast  (Data, void*);
//ANN Value  dataget_val   (Data, void*);
//ANN Func   dataget_func  (Data, void*);
//ANN Nspc   dataget_nspc  (Data, void*);
//ANN Vector dataget_vec   (Data, void*);
//ANN Instr  dataget_instr (Data, void*);

ANN void   dataset_type  (DataTable, void*, struct Type_*);
//ANN void   dataset_cast  (Data, void*);
//ANN void   dataset_val   (Data, void*);
//ANN void   dataset_func  (Data, void*);
//ANN void   dataset_nspc  (Data, void*);
//ANN void   dataset_vec   (Data, void*);
//ANN void   dataset_instr (Data, void*);
#endif

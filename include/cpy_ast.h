#ifndef __CPY_AST
#define __CPY_AST
ANN Func_Def cpy_func_def(MemPool, Func_Def);
ANN struct Func_Base_* cpy_func_base(MemPool, struct Func_Base_*);
ANN Class_Def cpy_class_def(MemPool, Class_Def);
ANN Type_List cpy_type_list(MemPool p, const Type_List src);
#endif

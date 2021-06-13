#ifndef _TMPL_INFO
#define _TMPL_INFO
struct tmpl_info {
  Symbol           name;
  Specialized_List list;
  const Type_Decl *td;
  Type             ret;
  Type             base;
  struct Vector_   type;
  struct Vector_   size;
  uint8_t          index;
};

ANN Type tmpl_exists(const Env env, struct tmpl_info *const info);
#endif

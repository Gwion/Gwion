#ifndef _TMPL_INFO
#define _TMPL_INFO
struct tmpl_info {
  Symbol           name;
  ID_List          list;
  const Type_Decl* td;
  Type             ret;
  Type             base;
  struct Vector_   type;
  struct Vector_   size;
  uint8_t index;
};
#endif

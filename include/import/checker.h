#ifndef __IMPORT_CHECKER
#define __IMPORT_CHECKER

struct func_checker { // name_checker ?
  m_str name;
  ID_List tmpl;
  const ae_flag flag;
};

struct array_checker {
  m_str str;
  Exp base, exp;
  m_uint depth;
  loc_t pos;
  m_bool is_exp;
};

ANN void func_checker_clean(const Gwi gwi, struct func_checker *ck);
ANN Type_Decl* import_td(const Gwi gwi, const m_str name, const m_bool is_exp);
ANN m_bool check_typename_def(const Gwi gwi, struct func_checker *ck);
ANN ID_List tmpl_valid(const Gwi gwi, const m_str str);
#endif

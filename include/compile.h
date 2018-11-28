#ifndef __COMPILE
#define __COMPILE
//m_bool check_filename(struct Gwion_* vm, const m_str filename);
m_bool check_string(struct Gwion_* vm, const m_str filename, const m_str data);
//m_bool check_file(struct Gwion_* vm, const m_str filename, FILE* file);
m_uint compile_filename(struct Gwion_* vm, const m_str filename);
m_uint compile_string(struct Gwion_* vm, const m_str filename, const m_str data);
//m_uint compile_file(struct Gwion_* vm, const m_str filename, FILE* file);
#endif

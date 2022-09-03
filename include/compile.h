#ifndef __COMPILE
#define __COMPILE
m_uint compile_filename_values(struct Gwion_ *vm, const m_str filename, MP_Vector*);
m_uint compile_string_values(struct Gwion_ *vm, const m_str filename,
                      const m_str data, MP_Vector*);
m_uint compile_file_values(struct Gwion_ *vm, const m_str filename, FILE *file, MP_Vector*);
m_uint compile_filename_xid_values(struct Gwion_ *vm, const m_str filename,
                            const m_uint xid, MP_Vector*);
m_uint compile_string_xid_values(struct Gwion_ *vm, const m_str filename,
                          const m_str data, const m_uint xid, MP_Vector*);
m_uint compile_file_xid_values(struct Gwion_ *vm, const m_str filename, FILE *file,
                        const m_uint xid, MP_Vector*);
#define compile_filename(a, b) compile_filename_values(a, b, NULL)
#define compile_string(a, b, c) compile_string_values(a, b, c, NULL)
#define compile_file(a, b, c) compile_file_values(a, b, c, NULL)
#define compile_filename_xid(a, b, c) compile_filename_xid_values(a, b, c, NULL)
#define compile_string_xid(a, b, c, d) compile_string_xid_values(a, b, c, d, NULL)
#define compile_file_xid(a, b, c) compile_file_xid_values(a, b, c, NULL)
#endif

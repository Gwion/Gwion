#ifndef __COMPILE
#define __COMPILE
m_uint compile_filename(struct Gwion_ *vm, const m_str filename);
m_uint compile_string(struct Gwion_ *vm, const m_str filename,
                      const m_str data);
m_uint compile_file(struct Gwion_ *vm, const m_str filename, FILE *file);
m_uint compile_filename_xid(struct Gwion_ *vm, const m_str filename,
                            const m_uint xid);
m_uint compile_string_xid(struct Gwion_ *vm, const m_str filename,
                          const m_str data, const m_uint xid);
m_uint compile_file_xid(struct Gwion_ *vm, const m_str filename, FILE *file,
                        const m_uint xid);
#endif

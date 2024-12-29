#ifndef __COMPILE
#define __COMPILE
m_uint compile_filename_values(struct Gwion_ *vm, const char * filename, Vector args, ValueList*);
m_uint compile_string_values(struct Gwion_ *vm, const char * filename,
                      const char * data, Vector args, ValueList*);
m_uint compile_file_values(struct Gwion_ *vm, const char * filename, FILE *file, Vector args, ValueList*);
m_uint compile_filename_xid_values(struct Gwion_ *vm, const char * filename,
                            Vector args, const m_uint xid, ValueList*);
m_uint compile_string_xid_values(struct Gwion_ *vm, const char * filename,
                          const char * data, Vector args, const m_uint xid, ValueList*);
m_uint compile_file_xid_values(struct Gwion_ *vm, const char * filename, FILE *file,
                        Vector args, const m_uint xid, ValueList*);
ANN2(1,2,3) m_uint compile_string_xid_values_comments(struct Gwion_ *gwion, const char *filename,
                              const char *data, Vector args, const m_uint xid, ValueList *values, CommentList **comments);
#define compile_filename(a, b, c) compile_filename_values(a, b, c, NULL)
#define compile_string(a, b, c, d) compile_string_values(a, b, c, d, NULL)
#define compile_file(a, b, c, d) compile_file_values(a, b, c, d, NULL)
#define compile_filename_xid(a, b, c, d) compile_filename_xid_values(a, b, c, d, NULL)
#define compile_string_xid(a, b, c, d, e) compile_string_xid_values(a, b, c, d, e, NULL)
#define compile_file_xid(a, b, c, d) compile_file_xid_values(a, b, c, d, NULL)
#endif

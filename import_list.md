## NOTES:
  Tmpl* gwi tmpl is used to build tmpl for udef, fdef, tdef


src/import/cleaner.c
src/import/import_cdef.c
src/import/import_checker.c
src/import/import_enum.c

## src/import/import_internals.c
  nothing to see here apparently

## src/import/import_oper.c
  nothing to see here apparently

src/import/import_special.c
src/import/import_tdef.c
src/import/import_type.c
src/import/import_udef.c


## src/import/import_fdef.c
  * name
  * sym
  * type decl (for ret_type)
  * arg list (formerly mpv, now arg_list)
  * flags
  * TODO: fflags
  * tmpl (what for? assume decl)
  * tags???? must be an error

## src/import/import_item.c
  * exp (for value?)

## src/import/import_prim.c
  the fuck
  check->m_val = info->array.type->actual_size * CHAR_BIT;

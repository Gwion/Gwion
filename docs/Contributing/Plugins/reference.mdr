# Plugin Reference

There are some general macros essential to interfacing with Gwion in general.

The return codes of functions are of type `m_bool` and consist of the following values:

Name | Value | Description
---- | ----- | -----------
`GW_OK` | 1 | Success
`GW_PASS` | 0 | Ignore the result and continue
`GW_ERROR` | -1 | An error occured

The following macros are sugar for handling error codes. They can exit the scope of the function. It si recommended to use these rather than writing:
```c
if (!a) return NULL;
```
and co.

Name | Value | Description
---- | ----- | -----------
`CHECK_BB` | `if (f < 0) return GW_ERROR;` | Takes an expression that evaluates to `m_bool`. Potentially exits function returning `GW_ERROR`.
`CHECK_BO` | `if (f < 0) return NULL;` | Takes an expression that evaluates to `m_bool`. Potentially exits function returning `NULL`.
`CHECK_OB` | `if (!f) return GW_ERROR;` | Takes an expression that evaluates to a pointer. Potentially exits function returning `GW_ERROR`.
`CHECK_OO` | `if (!f) return NULL;` | Takes an expression that evaluates to a pointer. Potentially exits function returning `NULL`.

Likewise there are the `DECL_XX` macros which specialize to failing if a declaration fails. Their general syntax is as follows:
```c
DECL_XX(type, var_name, = value);
```

These macros are specific to creating plugins.

`MFUN(name)`         signature for a member function
`SFUN(name)`         signature for a static function
`CTOR(name)`         signature for a constructor
`DTOR(name)`         signature for a destructor
`GACK(name)`         signature for a pretty print function
`TICK(name)`         signature for a UGen *tick* function
`GWION_IMPORT(name)` *main* function of a plugin/library

in said *main function*, one should use GWI_XX instead of CHECK_XX
since it takes care of position and might optimize some checks away (in gwion's internals)

`OP_CHECK(name)`     define a type checking function. should return
   correct type in success
   error type on failure
   NULL to continue type checking


`OP_EMIT(name)`     define an  emitter function. returns an `m_bool`

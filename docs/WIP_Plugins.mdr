# Writing a Gwion plugin

> THIS IS OUTDATED. please look at the source code in src/lib/ instead

  * [getting started]

## Getting started
use the script

### headers
```
#include "vm.h"
#include "instr.h"
#include "import.h
```
<!-- TODO: verify this -->

### Class
Define the type:
```
struct Type_ t_mytype = { "MyType", SZ_INT, &t_object};
```
> every type extending t_object should have SZ_INT

### Handling Constructors and Destructors
#### CTOR
```
CTOR(mytype_ctor) {
  /* constructor code here */
}
```
#### DTOR
```
DTOR(mytype_dtor) {
  /* destructor code here */
}
```

those macros provide two variables:
  * `o`:     the *M_Object* for the (con/des)tructor
  * `shred`: the *VM_Shred* for the (con/des)tructor

```
CHECK_BB(import_class_begin(env, &t_mytpe, env->global_nspc, mytype_ctor, mytype_dtor))
```
#### variable
declare a `m_int`. coding convention require
  * a leading *_o*
  * a following *_type_*
```c
m_int o_mytype_myvaroffset;
```
#### function
```c
/* declare a member function */
MFUN(mytype_memberfunction) {
  /* code here */
}

SFUN(mtype_staticfunction) {
  /* code here */
}
```

#### operator

### Import function


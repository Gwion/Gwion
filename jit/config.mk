jit_dir := jit
jit_src := $(wildcard ${jit_dir}/src/*.c)
CFLAGS += -Ijit -DJIT
#CFLAGS += -DJIT_DEV

#jit_backend := naive
#jit_backend := gccjit
#jit_backend := dummy
jit_backend := libjit
#jit_backend := tcc
#jit_backend := dynasm

CFLAGS += -Ijit/include
CFLAGS += -Ijit/backend/${jit_backend}
jit_src += $(wildcard jit/backend/${jit_backend}/*.c)
include jit/backend/${jit_backend}/jit.mk

jit_obj := $(jit_src:.c=.o)
GW_OBJ += ${jit_obj}

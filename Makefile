ifeq (,$(wildcard util/config.mk))
$(shell cp util/config.mk.orig util/config.mk)
endif
ifeq (,$(wildcard config.mk))
$(shell cp config.mk.orig config.mk)
endif
include util/config.mk
include config.mk

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(@F:.o=.Td)

# initialize source lists
src_src := $(wildcard src/*.c)
lib_src := $(wildcard src/lib/*.c)
oo_src := $(wildcard src/oo/*.c)
vm_src := $(wildcard src/vm/*.c)
parse_src := $(wildcard src/parse/*.c)
util_src := $(wildcard src/util/*.c)
emit_src := $(wildcard src/emit/*.c)
opt_src := $(wildcard opt/*.c)

# add boolean
ifeq (${USE_GWCOV}, 1)
CFLAGS += -DGWCOV
endif

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif
ifeq (${USE_OPTIMIZE}, 1)
util_src += ${opt_src}
CFLAGS+= -DOPTIMIZE
endif
ifeq (${USE_JIT}, 1)
include jit/config.mk
endif
ifeq (${USE_NOMEMOIZE}, 1)
CFLAGS += -DNOMEMOIZE
endif

ifeq (${USE_VMBENCH}, 1)
CFLAGS += -DVMBENCH
LDFLAGS += -lbsd
endif

ifeq (${BUILD_ON_WINDOWS}, 1)
CFLAGS += -DBUILD_ON_WINDOWS -D_XOPEN_SOURCE=700
else
CFLAGS += -ldl
endif

# add directories
CFLAGS+=-DGWPLUG_DIR=\"${GWPLUG_DIR}\"

# initialize object lists
src_obj := $(src_src:.c=.o)
lib_obj := $(lib_src:.c=.o)
ast_obj := $(ast_src:.c=.o)
parse_obj := $(parse_src:.c=.o)
emit_obj := $(emit_src:.c=.o)
oo_obj := $(oo_src:.c=.o)
vm_obj := $(vm_src:.c=.o)
util_obj := $(util_src:.c=.o)

GW_OBJ=${src_obj} ${ast_obj} ${parse_obj} ${emit_obj} ${oo_obj} ${vm_obj} ${util_obj} ${lib_obj}

ifeq ($(shell uname), Linux)
LDFLAGS+=-lrt
endif

CCFG="${CFLAGS}"
LDCFG="${LDFLAGS}"

# hide this from gwion -v
CFLAGS += -DGWION_BUILTIN

LDFLAGS += ast/libgwion_ast.a util/libgwion_util.a

all: options util/libgwion_util.a ast/libgwion_ast.a ${GW_OBJ} ${jit_obj}
	$(info link ${PRG})
	@${CC} ${GW_OBJ} ${jit_obj} ${LDFLAGS} -o ${PRG}

config.mk:
	$(info generating config.mk)
	@cp config.mk.orig config.mk

util/libgwion_util.a:
	@make -C util

ast/libgwion_ast.a:
	@make -C ast

options:
	$(info CC      : ${CC})
	$(info CFLAGS  : ${CFLAGS})
	$(info LDFLAGS : ${LDFLAGS})

clean:
	$(info cleaning ...)
	@rm -f */*.o */*/*.o */*.gw.* */*/*.gw.* */*/*.gcda */*/*.gcno gwion

src/arg.o:
	$(info compile $(<:.c=) (with arguments defines))
	@${CC} $(DEPFLAGS) ${CFLAGS} -c src/arg.c -o src/arg.o -DLDFLAGS='${LDCFG}' -DCFLAGS='${CCFG}'
	@mv -f $(DEPDIR)/$(@F:.o=.Td) $(DEPDIR)/$(@F:.o=.d) && touch $@
	@echo $@: config.mk >> $(DEPDIR)/$(@F:.o=.d)

.c.o: $(DEPDIR)/%.d
	$(info compile $(<:.c=))
	@${CC} $(DEPFLAGS) ${CFLAGS} -c $< -o $(<:.c=.o)
	@mv -f $(DEPDIR)/$(@F:.o=.Td) $(DEPDIR)/$(@F:.o=.d) && touch $@
	@echo $@: config.mk >> $(DEPDIR)/$(@F:.o=.d)

install: ${PRG}
	install ${PRG} ${PREFIX}/bin

uninstall:
	rm ${PREFIX}/${PRG}

test:
	@bash help/test.sh tests/* examples

include $(wildcard .d/*.d)

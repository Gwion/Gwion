ifeq (,$(wildcard util/config.mk))
$(shell cp util/config.mk.orig util/config.mk)
endif
ifeq (,$(wildcard config.mk))
$(shell cp config.mk.orig config.mk)
endif
include util/config.mk
include config.mk

SEVERITY ?= 10
GWION_TEST_DIR ?= /tmp

GWION_PACKAGE=gwion
CFLAGS += -DGWION_PACKAGE='"${GWION_PACKAGE}"'

GIT_BRANCH=$(shell git branch | grep "*" | cut -d" " -f2)

src := $(wildcard src/*.c)
src += $(wildcard src/*/*.c)

test_dir_all := $(wildcard tests/*)
test_ignore = tests/import
test_dir := $(filter-out $(test_ignore), $(test_dir_all))
test_dir += examples

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif

ifneq (${BUILD_ON_WINDOWS}, 1)
LDFLAGS += -ldl -lpthread
endif

src_obj := $(src:.c=.o)
lib_obj := $(filter-out src/main.o, ${src_obj})

CFLAGS  += -Iinclude

ifeq (${BUILD_ON_WINDOWS}, 1)
ifeq (${CC}, gcc)
CFLAGS += -export-dynamic
LDFLAGS += -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--out-implib=libgwion.dll.a
CFLAGS += -Wl,-export-dynamic
LDFLAGS += -Wl,-export-dynamic
LDFLAGS += -lm
endif
else
LDFLAGS += -rdynamic
LDFLAGS += -lm
endif

ifeq ($(shell uname), Linux)
LDFLAGS += -lrt
endif

CFLAGS += -DGWION_BUILTIN

#GWLIBS = libgwion.a ast/libgwion_ast.a ast/libgwion_grammar.a util/libgwion_util.a
GWLIBS = libgwion.a ast/libgwion_ast.a util/libgwion_util.a
_LDFLAGS = ${GWLIBS} ${LDFLAGS}

all: options-show util/libgwion_util.a ast/libgwion_ast.a libgwion.a src/main.o
	$(info link ${PRG})
	@${CC} src/main.o -o ${PRG} ${_LDFLAGS} ${LIBS}

options-show:
	@$(call _options)

libgwion.a: ${lib_obj}
	@${AR} ${AR_OPT}

util/libgwion_util.a:
	@+GWION_PACKAGE= make -s -C util

util: util/libgwion_util.a
	@(info build util)

ast/libgwion_ast.a:
	@+GWION_PACKAGE= make -s -C ast

ast: ast/libgwion_ast.a
	@(info build ast)

afl:
	touch src/parse/{scan*.c,check.c} src/emit/emit.c src/main.c
	PRG=gwion-fuzz CC=afl-clang-fast make
	touch src/parse/{scan*.c,check.c} src/emit/emit.c src/main.c

clean:
	$(info cleaning ...)
	@rm -f src/*.o src/*/*.o gwion libgwion.a src/*.gcno src/*.gcda

install: ${PRG}
	$(info installing ${GWION_PACKAGE} in ${PREFIX})
	@install ${PRG} ${DESTDIR}/${PREFIX}/bin
	@sed 's/PREFIX/$\{PREFIX\}/g' scripts/gwion-config > gwion-config
	@install gwion-config ${DESTDIR}/${PREFIX}/bin/gwion-config
	@install scripts/gwion-pkg ${DESTDIR}/${PREFIX}/bin/gwion-pkg
	@rm gwion-config
	@mkdir -p ${DESTDIR}/${PREFIX}/include/gwion
	@cp -r include/* ${DESTDIR}/${PREFIX}/include/gwion

uninstall:
	$(info uninstalling ${GWION_PACKAGE} from ${PREFIX})
	@rm -rf ${DESTDIR}/${PREFIX}/bin/${PRG}
	@rm -rf ${DESTDIR}/${PREFIX}/include/gwion

test:
	@bash scripts/test.sh ${test_dir}

include $(wildcard .d/*.d)
include util/locale.mk

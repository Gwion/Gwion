GWION_PACKAGE=gwion

ifeq (,$(wildcard util/Makefile))
  $(warning "missing git submodules, please run:")
  $(error "git submodules update --init --recursive")
endif
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
test_dir_all := $(wildcard tests/*)
test_ignore = tests/plug test/driver tests/module
test_dir := $(filter-out $(test_ignore), $(test_dir_all))
test_dir += examples

src := $(wildcard src/*.c)
src += $(wildcard src/*/*.c)

CFLAGS += -Iutil/include -Iutil/libtermcolor/include -Iast/include -Ilibcmdapp/src -D_GNU_SOURCE
CFLAGS += -Iast/libprettyerr/src -Ifmt/include

# add commit hash to version for now
CFLAGS += -DGWION_VERSION="\"$(shell git log -n1 --format="%h%m%cs")\""

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif

ifneq (${BUILD_ON_WINDOWS}, 1)
LDFLAGS += -lpthread
ifeq ($(shell uname -s), NetBSD)
else ifeq ($(shell uname -s), OpenBSD)
else
LDFLAGS += -ldl
endif
endif

ifeq (${USE_HELGRIND}, 1)
CFLAGS += -DUSE_HELGRIND
endif

src_obj := $(src:.c=.o)
gcda := $(src:.c=.gcda)
gcno := $(src:.c=.gcno)
lib_obj := $(filter-out src/main.o, ${src_obj})
almost_obj := $(filter-out src/vm/vm.o, ${src_obj})

CFLAGS  += -Iinclude

ifeq ($(shell uname), Linux)
LDFLAGS += -lrt
endif

ALMOST_LIBS := libcmdapp/libcmdapp.a fmt/libgwion-fmt.a
ALMOST_LIBS += ast/libgwion_ast.a ast/libprettyerr/libprettyerr.a
ALMOST_LIBS += util/libgwion_util.a util/libtermcolor/libtermcolor.a
GWLIBS := lib${PRG}.a ${ALMOST_LIBS}
LDFLAGS := ${GWLIBS} ${LDFLAGS}

# we are not pedantic
CFLAGS += -Wno-pedantic

CFLAGS += -DGWION_BUILTIN

all: options-show ${PRG}

${PRG}: ${GWLIBS} src/main.o
	@$(info link ${PRG})
	@${CC} src/main.o -o ${PRG} ${LDFLAGS} ${LIBS}

options-show:
	@$(call _options)
	@$(info libs: ${GWLIBS})

almost_gwion: ${almost_obj} ${ALMOST_LIBS}

lto:
	@${MAKE} USE_LTO=1 almost_gwion
	@${MAKE} USE_LTO=0

lib${PRG}.a: ${lib_obj}
	@${AR} ${AR_OPT}

util/libtermcolor/libtermcolor.a:
	@+${MAKE} BUILD_ON_WINDOWS=${BUILD_ON_WINDOWS} -s -C util/libtermcolor static

util/libgwion_util.a:
	@+${MAKE} -s -C util

util: util/libgwion_util.a
	@(info build util)

ast/libgwion_ast.a: util/libgwion_util.a
	@+ ${MAKE} -s -C ast

libcmdapp/libcmdapp.a:
	@+CFLAGS=-I$(shell pwd)/util/libtermcolor/include ${MAKE} -s -C libcmdapp static

fmt/libgwion-fmt.a:
	@+${MAKE} -s -C fmt libgwion-fmt.a

ast/libprettyerr/libprettyerr.a:
	@+CFLAGS=-I$(shell pwd)/util/libtermcolor/include ${MAKE} -s -C ast/libprettyerr static

ast: ast/libgwion_ast.a
	@(info build ast)

afl: gwion-fuzz

gwion-fuzz:
	@touch src/parse/{scan*.c,check.c} src/emit/emit.c src/main.c
	@+PRG=gwion-fuzz CC=afl-clang-fast CFLAGS=-D__FUZZING__ ${MAKE}
	@touch src/parse/{scan*.c,check.c} src/emit/emit.c src/main.c

clean_core:
	@rm -f core.* *vgcore.*

clean-all: clean
		${MAKE} -s -C libcmdapp clean
		${MAKE} -s -C fmt clean
		${MAKE} -s -C ast clean
		${MAKE} -s -C ast/libprettyerr clean
		${MAKE} -s -C util clean
		${MAKE} -s -C util/libtermcolor clean

update: clean-all
	bash scripts/update.sh

clean: clean_core
	$(info cleaning ...)
	@rm -f src/*.o src/*/*.o gwion lib${PRG}.a ${gcno} ${gcda}

install: all translation-install
	$(info installing ${GWION_PACKAGE} in ${PREFIX})
	@mkdir -p ${DESTDIR}/${PREFIX}/bin
	@mkdir -p ${DESTDIR}/${PREFIX}/lib
	@mkdir -p ${DESTDIR}/${PREFIX}/include
	@mkdir -p ${DESTDIR}/${PREFIX}/share
	@install ${PRG} ${DESTDIR}/${PREFIX}/bin
	@install lib${PRG}.a ${DESTDIR}/${PREFIX}/lib
	@PREFIX=${PREFIX} sed 's#PREFIX#${PREFIX}#g' scripts/gwion-config > gwion-config
	@install gwion-config ${DESTDIR}/${PREFIX}/bin/gwion-config
	@install scripts/gwion-pkg ${DESTDIR}/${PREFIX}/bin/gwion-pkg
	@rm gwion-config
	@mkdir -p ${DESTDIR}/${PREFIX}/include/gwion
	@cp -r include/* ${DESTDIR}/${PREFIX}/include/gwion
	@${MAKE} -s -C util install
	@${MAKE} -s -C ast install

uninstall: translation-uninstall
	$(info uninstalling ${GWION_PACKAGE} from ${PREFIX})
	@rm ${DESTDIR}/${PREFIX}/bin/${PRG}
	@rm ${DESTDIR}/${PREFIX}/bin/gwion-config
	@rm ${DESTDIR}/${PREFIX}/bin/gwion-pkg
	@rm ${DESTDIR}/${PREFIX}/lib/lib${PRG}.a
	@rm ${DESTDIR}/${PREFIX}/include/gwion/*.h
	@rmdir --ignore-fail-on-non-empty ${DESTDIR}/${PREFIX}/include/gwion

test: ${PRG}
	@bash scripts/test.sh ${test_dir}

scan:
	@rm -rf scan_output
	@scan-build -o ./scan_output make USE_DEBUG=1
	@scan-view ./scan_output/*

include $(wildcard .d/*.d)
include util/locale.mk

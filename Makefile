ifeq (,$(wildcard util/config.mk))
$(shell cp util/config.mk.orig util/config.mk)
endif
ifeq (,$(wildcard config.mk))
$(shell cp config.mk.orig config.mk)
endif
include util/config.mk
include config.mk

GWION_PACKAGE=gwion
CFLAGS += -DGWION_PACKAGE='"${GWION_PACKAGE}"'

GIT_BRANCH=$(shell git branch | grep "*" | cut -d" " -f2)

# initialize source lists
src_src := $(wildcard src/*.c)
lib_src := $(wildcard src/lib/*.c)
oo_src := $(wildcard src/oo/*.c)
vm_src := $(wildcard src/vm/*.c)
parse_src := $(wildcard src/parse/*.c)
util_src := $(wildcard src/util/*.c)
emit_src := $(wildcard src/emit/*.c)

test_dir_all := $(wildcard tests/*)
test_ignore = tests/benchmark tests/import
test_dir := $(filter-out $(test_ignore), $(test_dir_all))
test_dir += examples

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif

ifeq (${BUILD_ON_WINDOWS}, 1)
CFLAGS += -DBUILD_ON_WINDOWS -D_XOPEN_SOURCE=700
else
LDFLAGS += -ldl -lpthread
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
src=${src_src} ${ast_src} ${parse_src} ${emit_src} ${oo_src} ${vm_src} ${util_src} ${lib_src}
gwlib_obj := $(filter-out src/main.o, ${GW_OBJ})

CFLAGS  += -Iinclude

ifeq (${BUILD_ON_WINDOWS}, 1)
ifeq (${CC}, gcc)
LDFLAGS += -lm
endif
else
LDFLAGS += -lm
endif

ifeq ($(shell uname), Linux)
LDFLAGS += -lrt -rdynamic
endif

INSTALLED_INCLUDE = -I${PREFIX}/include/gwion -I${PREFIX}/include/gwion/util -I${PREFIX}/include/gwion/ast
CCFG="${INSTALLED_INCLUDE} ${CFLAGS}"
LDCFG="${LDFLAGS}"

# hide this from gwion -v
CFLAGS += -DGWION_BUILTIN

GWLIBS = libgwion.a ast/libgwion_ast.a util/libgwion_util.a
_LDFLAGS = ${GWLIBS} ${LDFLAGS}

all: options-show util/libgwion_util.a ast/libgwion_ast.a libgwion.a src/main.o
	$(info link ${PRG})
	@${CC} src/main.o -o ${PRG} ${_LDFLAGS} ${LIBS}

options-show:
	@$(call _options)

libgwion.a: ${gwlib_obj}
	${AR} ${AR_OPT}

util/libgwion_util.a:
	@make -C util

ast/libgwion_ast.a:
	@make -C ast

clean:
	$(info cleaning ...)
	@rm -f */*.o */*/*.o */*.gw.* */*/*.gw.* */*/*.gcda */*/*.gcno gwion libgwion.a src/*.gcno src/*.gcda

src/arg.o:
	$(info compile $(<:.c=) (with arguments defines))
	@${CC} $(DEPFLAGS) ${CFLAGS} ${CICFLAGS} -c src/arg.c -o src/arg.o -DLDFLAGS='${LDCFG}' -DCFLAGS='${CCFG}'
	@mv -f $(DEPDIR)/$(@F:.o=.Td) $(DEPDIR)/$(@F:.o=.d) && touch $@
	@echo $@: config.mk >> $(DEPDIR)/$(@F:.o=.d)

install: ${PRG}
	$(info installing ${GWION_PACKAGE} in ${PREFIX})
	@install ${PRG} ${DESTDIR}/${PREFIX}/bin
	@sed "s#PREFIX#${PREFIX}#g" scripts/gwion-config > gwion-config
	@install gwion-config ${DESTDIR}/${PREFIX}/bin/gwion-config
	@install scripts/gwion-pkg ${DESTDIR}/${PREFIX}/bin/gwion-pkg
	@rm gwion-config
	@mkdir -p ${DESTDIR}/${PREFIX}/include/gwion
	@cp include/*.h ${DESTDIR}/${PREFIX}/include/gwion

uninstall:
	$(info uninstalling ${GWION_PACKAGE} from ${PREFIX})
	@rm -rf ${DESTDIR}/${PREFIX}/bin/${PRG}
	@rm -rf ${DESTDIR}/${PREFIX}/include/gwion

test:
	@bash scripts/test.sh ${test_dir}

coverity:
	[ -z "$(git ls-remote --heads $(git remote get-url origin) coverity_scan)" ] || git push origin :coverity_scan
	git show-ref --verify --quiet refs/heads/master && git branch -D coverity_scan || echo ""
	git checkout -b coverity_scan
	git push --set-upstream origin coverity_scan
	git checkout ${GIT_BRANCH}

include $(wildcard .d/*.d)
include util/intl.mk
include docs.mk

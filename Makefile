PRG=gwion
LDFLAGS += -lsoundpipe
LDFLAGS += -lm
LDFLAGS += -ldl -rdynamic

CFLAGS += -Iinclude -std=c99 -O3 -mfpmath=sse -mtune=native
CFLAGS += -fno-strict-aliasing -Wall -pedantic
CFLAGS += -D_GNU_SOURCE

core_src := $(wildcard  src/*.c)
lang_src := $(wildcard lang/*.c)
ugen_src := $(wildcard ugen/*.c)
drvr_src := driver/driver.c driver/dummy.c

core_obj := $(core_src:.c=.o)
lang_obj := $(lang_src:.c=.o)
ugen_obj := $(ugen_src:.c=.o)
ast_obj = ast/absyn.o ast/parser.o ast/lexer.o

include config.mk
include driver.mk
drvr_obj := $(drvr_src:.c=.o)

ifeq ($(shell uname), Linux)
LDFLAGS+=-lrt
ifeq (${COVERAGE}, 1)
ifeq (${CC}, gcc)
CFLAGS+= --coverage
LDFLAGS+= -lgcov
endif
endif
endif

ifeq (${CC}, gcc)
LDFLAGS += -lpthread
else
CFLAGS  += -pthread
LDFLAGS += -lpthread
endif

ifeq (${GWION_HAS_DIR}, 1)
CFLAGS += -DGWION_DOC_DIR=${GWION_DOC_DIR}
CFLAGS += -DGWION_API_DIR=${GWION_API_DIR}
CFLAGS += -DGWION_TOK_DIR=${GWION_TOK_DIR}
CFLAGS += -DGWION_TAG_DIR=${GWION_TAG_DIR}
CFLAGS += -DGWION_PLUG_DIR=${GWION_PLUG_DIR}
endif

ifeq (${USE_DOUBLE}, 1)
CFLAGS += -DUSE_DOUBLE -DSPFLOAT=double
endif

faster: check_driver include/generated.h
	make soundpipe_import
	make -C ast
	make -j 8 all

check_driver:
	[ ${DRIVER_OK} -eq 1 ] || exit 1

all: config.mk core lang ugen drvr
	${CC} ${core_obj} ${lang_obj} ${ugen_obj} ${drvr_obj} ${ast_obj} ${LDFLAGS} -o ${PRG}

default: soundpipe_import config.mk include/generated.h core lang ugen drvr
	@make -C ast
	${CC} ${core_obj} ${lang_obj} ${ugen_obj} ${drvr_obj} ${ast_obj} ${LDFLAGS} -o ${PRG}


config.mk: config.def.mk
	cp config.def.mk config.mk

include/generated.h:
	${CC} ${CFLAGS} util/generate_header.c -o util/generate_header
	./util/generate_header
	rm ./util/generate_header

core: ${core_obj}
lang: ${lang_obj}
ugen: ${ugen_obj}
drvr: ${drvr_obj}

mostly_clean:
	@rm -f core.* vgcore.* src/*.o lang/*.o driver/*.o parser.c lexer.c *.output *.h ugen/*.o ugen/soundpipe.c */*.gcno
	@rm -f include/generated.h
#	@which astyle > /dev/null && astyle -q -p -s2 --style=kr src/*.c ugen/*.c ast/*.c lang/*.c include/*.h
#	@rm -rf src/*.orig lang/*.orig ast/*.orig driver/*.orig ugen/*.orig
#	@rm -rf doc/html doc/latex
	@make -s -C ast clean

clean: mostly_clean
	@rm -f ${PRG}

soundpipe_import: util/import.lua
	lua util/import.lua ${SOUNDPIPE_DATA_DIR} > ugen/soundpipe.c

.c.o:
	${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

install:
	@echo "'gwion' is in pre-alpha stage, no install for now."

coverity:
	@git ls-remote --heads origin coverity_scan | wc -l || git push origin :coverity_scan
#	@git show-ref --heads --quiet coverity_scan && git branch -D coverity_scan
#	@git branch coverity_scan
	@git checkout --orphan coverity_scan
	@cp .travis.yml util/travis.yml
	@cp  util/coverity.yml .travis.yml
	@git add .travis.yml
	@git commit -m"push to Travis to coverity"
	@git push --set-upstream origin coverity_scan
	@git checkout dev

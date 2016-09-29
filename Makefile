PRG=Gwion
LDFLAGS = -lsoundpipe
LDFLAGS += -g -lm -pthread -lasound -lsndfile -fno-strict-aliasing
LDFLAGS += -std=c99 -O3 -mfpmath=sse -mtune=core2 -freg-struct-return -pipe \
  -levdev -ldl -rdynamic -lrt -lcgraph -lgvc
CFLAGS+=-Iinclude -I/usr/include/libevdev-1.0
CC = cc

core_src := $(wildcard src/*.c)
lang_src := $(wildcard lang/*.c)
ugen_src := $(wildcard ugen/*.c)
drvr_src := $(wildcard driver/*.c)

core_obj := $(core_src:.c=.o)
lang_obj := $(lang_src:.c=.o)
ugen_obj := $(ugen_src:.c=.o)
drvr_obj := $(drvr_src:.c=.o)
ast_obj = ast/absyn.o ast/parser.o ast/lexer.o

include config.mk

STAMP=$(shell cat .parser.stamp)
NEW_STAMP=$(shell stat -c %X gwion.y )

silent:
	@make -s default

default: config.mk core lang ugen drvr
	@make -C ast
	@${CC} ${core_obj} ${lang_obj} ${ugen_obj} ${drvr_obj} ${ast_obj} ${LDFLAGS} -o ${PRG}

core: ${core_obj}
lang: ${lang_obj}
ugen: ${ugen_obj}
drvr: ${drvr_obj}

clean:
	@rm -f core.* src/*.o lang/*.o driver/*.o parser.c lexer.c *.output *.h ugen/*.o
	@make -s -C ast clean

soundpipe_import:
	@cd ../import_sp; sh make_sp.sh; cd ../src

.c.o:
	@${CC} ${DEF} ${CFLAGS} -c $< -o $(<:.c=.o)

install:
	cp Gwion ~/bin

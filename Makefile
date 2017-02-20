##################
# handle options #
##################
PRG ?= gwion
CC ?= gcc
YACC ?= yacc
LEX ?= flex
DATA ?= ../Soundpipe/modules/data
PREFIX ?= /usr/local

LDFLAGS += -lm -ldl -rdynamic
CFLAGS += -Iinclude -std=c99 -O3 -mfpmath=sse -mtune=native -fno-strict-aliasing -Wall -pedantic -D_GNU_SOURCE


# handle directories
GWION_API_DIR ?= ${PREFIX}/lib/Gwion/api
GWION_DOC_DIR ?= ${PREFIX}/lib/Gwion/doc
GWION_TAG_DIR ?= ${PREFIX}/lib/Gwion/tag
GWION_TOK_DIR ?= ${PREFIX}/lib/Gwion/tok
GWION_ADD_DIR ?= ${PREFIX}/lib/Gwion/add
CFLAGS+=-DGWION_API_DIR=${GWION_API_DIR} -DGWION_DOC_DIR=${GWION_DOC_DIR} -DGWION_TAG_DIR=${GWION_TAG_DIR} -DGWION_TOK_DIR=${GWION_TOK_DIR} -DGWION_ADD_DIR=${GWION_ADD_DIR} 

# initialize source lists
core_src := $(wildcard src/*.c)
lang_src := $(wildcard lang/*.c)
ugen_src := $(wildcard ugen/*.c)
drvr_src := driver/driver.c driver/dummy.c

# handle libraries
SNDFILE_D ?= 1
ifeq (${SNDFILE_D}, on)
LDFLAGS+=-lsndfile
CFLAGS+=-DHAVE_SNDFILE
drvr_src+=driver/sndfile.c
endif

# initialize object lists
core_obj := $(core_src:.c=.o)
lang_obj := $(lang_src:.c=.o)
ugen_obj := $(ugen_src:.c=.o)
drvr_obj := $(drvr_src:.c=.o)
eval_obj := ast/absyn.o ast/parser.o ast/lexer.o

# os specific
LDFLAGS+=-lrt

# recipes
all: ${core_obj} ${lang_obj} ${eval_obj} ${ugen_obj}

style:
	@which astyle > /dev/null && astyle -q -p -s2 --style=kr */*.c */*.h
	@rm -rf */*.orig

mostly_clean: style
	@rm -f */*.o ast/parser.c ast/lexer.c ugen/soundpipe.c
	@rm -f include/generated.h

clean: mostly_clean
	@rm -f ${PRG}

import: util/import.lua
	lua util/import.lua ${DATA} > ugen/soundpipe.c

.c.o:
	${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

parser.c:
	${YACC} ${CFLAGS} -o parser.c -dvx gwion.y

lexer.c:
	${LEX}  ${CFLAGS} -o lexer.c gwion.l

install:
	@echo "'gwion' is in pre-alpha stage, no install for now."

tests: faster
    sh util/test.sh

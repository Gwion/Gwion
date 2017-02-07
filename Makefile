PRG=gwion
LDFLAGS = -lsoundpipe
LDFLAGS += -lm -pthread -lsndfile
LDFLAGS += -ldl -rdynamic -lrt

CFLAGS += -Iinclude -g -std=c99 -O3 -mfpmath=sse -mtune=native
CFLAGS += -fno-strict-aliasing -Wall -pedantic
CFLAGS += -D_GNU_SOURCE


core_src := $(wildcard  src/*.c)
lang_src := $(wildcard lang/*.c)
ugen_src := $(wildcard ugen/*.c)
drvr_src := driver/driver.c driver/sndfile.c driver/dummy.c driver/raw.c

core_obj := $(core_src:.c=.o)
lang_obj := $(lang_src:.c=.o)
ugen_obj := $(ugen_src:.c=.o)
drvr_obj := $(drvr_src:.c=.o)
ast_obj = ast/absyn.o ast/parser.o ast/lexer.o

include config.mk
include driver.mk

ifdef ($GWION_DOC_DIR)
CFLAGS += -DGWION_DOC_DIR=${GWION_DOC_DIR}
endif

ifdef ($GWION_API_DIR)
CFLAGS += -DGWION_API_DIR=${GWION_API_DIR}
endif

ifdef ($GWION_TOK_DIR)
CFLAGS += -DGWION_TOK_DIR=${GWION_TOK_DIR}
endif

ifdef ($GWION_TAG_DIR)
CFLAGS += -DGWION_TAG_DIR=${GWION_TAG_DIR}
endif

ifdef ($GWION_PLUG_DIR)
CFLAGS += -DGWION_PLUG_DIR=${GWION_PLUG_DIR}
endif

ifeq (${USE_DOUBLE}, 1)
CFLAGS += -DUSE_DOUBLE -DSPFLOAT=double
endif

faster: include/generated.h
	@make soundpipe_import
	@make -C ast
	@make -j 8 all

all: config.mk core lang ugen drvr
	${CC} ${core_obj} ${lang_obj} ${ugen_obj} ${drvr_obj} ${ast_obj} ${LDFLAGS} -o ${PRG}

default: config.mk include/generated.h core lang ugen drvr
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

drvr:
	${CC} ${CFLAGS} -c driver/driver.c -o driver/driver.o
	${CC} ${CFLAGS} -c driver/dummy.c -o driver/dummy.o
	${CC} ${CFLAGS} -c driver/sndfile.c -o driver/sndfile.o
	${CC} ${CFLAGS} -c driver/raw.c -o driver/raw.o
ifeq (${ALSA_D}, 1)
	${CC} -I include ${CFLAGS} -c driver/alsa.c -o driver/alsa.o
endif
ifeq (${JACK_D}, 1)
	${CC} -I include ${CFLAGS} -c driver/jack.c -o driver/jack.o
endif
ifeq (${SOUNDIO_D}, 1)
	${CC} -I include ${CFLAGS} -c driver/soundio.c -o driver/soundio.o
endif
ifeq (${PORTAUDIO_D}, 1)
	${CC} -I include ${CFLAGS} -c driver/portaudio.c -o driver/portaudio.o
endif

ifndef $SOUNDPIPE_DATA_DIR
export SOUNDPIPE_DATA_DIR=../Soundpipe/modules/data
endif
mostly_clean:
	@rm -f core.* vgcore.* src/*.o lang/*.o driver/*.o parser.c lexer.c *.output *.h ugen/*.o ugen/soundpipe.c
	@rm -f include/generated.h
	@which astyle > /dev/null && astyle -q -p -s2 --style=kr src/*.c ugen/*.c
	@rm -rf src/*.orig lang/*.orig ast/*.orig driver/*.orig ugen/*.orig
	@rm -rf doc/html doc/latex
	@make -s -C ast clean

clean: mostly_clean
	@rm -f ${PRG}

soundpipe_import: util/import.lua
	@lua util/import.lua ${SOUNDPIPE_DATA_DIR} > ugen/soundpipe.c



.c.o:
	${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

install:
	@echo "'gwion' is in pre-alpha stage, no install for now."

coverity:
#	@git branch -D coverity_scan
#	@git branch -D origin/coverity_scan
	@git branch coverity_scan1
	@git checkout coverity_scan1
	@cp .travis.yml travis.yml
	@cp  util/coverity.yml .travis.yml
	@git add .travis.yml
	@git commit -m"push to Travis to coverity"

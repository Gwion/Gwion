PRG=gwion
LDFLAGS = -lsoundpipe
LDFLAGS += -lm -pthread -lsndfile
LDFLAGS += -ldl -rdynamic -lrt

CFLAGS += -Iinclude -g -std=c99 -O3 -mfpmath=sse -mtune=native -freg-struct-return
CFLAGS += -fno-strict-aliasing -Wall
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

ifeq (${USE_DOUBLE}, 1)
CFLAGS += -DUSE_DOUBLE -DSPFLOAT=double
endif

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

clean:
	@rm -f core.* vgcore.* src/*.o lang/*.o driver/*.o parser.c lexer.c *.output *.h ugen/*.o
	@rm -f ${PRG}
	@rm -f include/generated.h
	@which astyle && astyle -p -s2 --style=kr src/*.c
	rm -rf src/*.orig lang/*.orig ast/*.orig driver/*.orig
	@make -s -C ast clean

soundpipe_import: import.lua
	@lua import.lua ../Soundpipe/modules/data > ugen/soundpipe.c



.c.o:
	${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

install:
	@echo "'gwion' is in pre-alpha stage, no install for now."

docs:
	@doxygen doc/Gwion.dox

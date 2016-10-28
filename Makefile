PRG=gwion
LDFLAGS = -lsoundpipe
LDFLAGS += -lm -pthread -lsndfile
LDFLAGS += -ldl -rdynamic -lrt

CFLAGS += -Iinclude -g -std=c99 -O3 -mfpmath=sse -mtune=core2 -freg-struct-return
CFLAGS += -fno-strict-aliasing -Wall
CFLAGS += -D_GNU_SOURCE
CFLAGS += -DSPFLOAT=double

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

silent:
	@make -s default

default: config.mk core lang ugen drvr
	@make -C ast
	${CC} ${core_obj} ${lang_obj} ${ugen_obj} ${drvr_obj} ${ast_obj} ${LDFLAGS} -o ${PRG}

config.mk: config.def.mk
	cp config.def.mk config.mk

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
	@rm -f core.* src/*.o lang/*.o driver/*.o parser.c lexer.c *.output *.h ugen/*.o
	@rm -f ${PRG}
	@make -s -C ast clean

soundpipe_import: import.lua
	l@ua import.lua ../Soundpipe/modules/data > ugen/soundpipe.c

.c.o:
	${CC} ${DEF} ${CFLAGS} -c $< -o $(<:.c=.o)

install:
	cp Gwion ~/bin

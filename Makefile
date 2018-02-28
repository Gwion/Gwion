include config.mk

# initialize source lists
src_src := $(wildcard src/*.c)
lib_src := $(wildcard src/lib/*.c)
oo_src := $(wildcard src/oo/*.c)
vm_src := $(wildcard src/vm/*.c)
ast_src := $(wildcard src/ast/*.c)
parse_src := $(wildcard src/parse/*.c)
util_src := $(wildcard src/util/*.c)
emit_src := $(wildcard src/emit/*.c)
drvr_src := src/drvr/driver.c

# add libraries
ifeq (${DUMMY_D}, 1)
CFLAGS +=-DHAVE_DUMMY
drvr_src +=src/drvr/dummy.c
endif
ifeq (${SPA_D}, 1)
CFLAGS +=-DHAVE_SPA
drvr_src +=src/drvr/spa.c
endif
ifeq (${SNDFILE_D}, 1)
LDFLAGS += -lsndfile
CFLAGS +=-DHAVE_SNDFILE
drvr_src +=src/drvr/sndfile.c
endif
ifeq (${ALSA_D}, 1)
LDFLAGS += -lasound
CFLAGS +=-DHAVE_ALSA
drvr_src +=src/drvr/alsa.c
endif
ifeq (${JACK_D}, 1)
LDFLAGS += -ljack
CFLAGS +=-DHAVE_JACK
drvr_src +=src/drvr/jack.c
endif
ifeq (${PORTAUDIO_D}, 1)
LDFLAGS += -lportaudio
CFLAGS +=-DHAVE_PORTAUDIO
drvr_src +=src/drvr/portaudio.c
endif
ifeq (${SOUNDIO_D}, 1)
LDFLAGS += -lsoundio
CFLAGS +=-DHAVE_SOUNDIO
drvr_src +=src/drvr/soundio.c
endif
ifeq (${PULSE_D}, 1)
LDFLAGS += -lpulse-simple
CFLAGS +=-DHAVE_PULSE
drvr_src +=src/drvr/pulse.c
endif

# add boolean
ifeq (${USE_COVERAGE}, 1)
CFLAGS += -ftest-coverage -fprofile-arcs
endif
ifeq (${USE_COVERAGE}, 1)
LDFLAGS += --coverage
endif
ifeq (${USE_GWCOV}, 1)
CFLAGS += -DGWCOV
endif
ifeq (${USE_MEMCHECK}, 1)
CFLAGS += -g
endif
ifeq (${USE_DOUBLE}, 1)
CFLAGS +=-DUSE_DOUBLE -DSPFLOAT=double
else
CFLAGS+=-DSPFLOAT=float
endif

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif

# add definitions
CFLAGS+= -DD_FUNC=${D_FUNC}

# add directories
CFLAGS+=-DGWION_ADD_DIR=\"${GWION_ADD_DIR}\"

# add soundpipe
LDFLAGS += ${SOUNDPIPE_LIB}
CFLAGS  += ${SOUNDPIPE_INC}

# initialize object lists
src_obj := $(src_src:.c=.o)
lib_obj := $(lib_src:.c=.o)
ast_obj := $(ast_src:.c=.o)
parse_obj := $(parse_src:.c=.o)
emit_obj := $(emit_src:.c=.o)
oo_obj := $(oo_src:.c=.o)
vm_obj := $(vm_src:.c=.o)
util_obj := $(util_src:.c=.o)
drvr_obj := $(drvr_src:.c=.o)
TOOL_OBJ=src/util/map.o src/util/vector.o src/util/symbol.o src/util/err_msg.o src/util/absyn.c src/ast/lexer.o src/ast/parser.o src/parse/op_utils.o
GW_OBJ=${src_obj} ${lib_obj} ${ast_obj} ${parse_obj} ${emit_obj} ${oo_obj} ${vm_obj} ${util_obj} ${drvr_obj}

CCFG="${CFLAGS}"
LDCFG="${LDFLAGS}"
ifeq ($(shell uname), Linux)
LDFLAGS+=-lrt
endif

all: config.mk generated.h options ${GW_OBJ}
	$(info link ${PRG})
	@${CC} ${GW_OBJ} ${LDFLAGS} -o ${PRG}

config.mk:
	$(info generating config.mk)
	@cp config.mk.orig config.mk

generated.h: include/generated.h
	$(info generating generated.h)
	@cc utils/generate_header.c ${CFLAGS} -o generate_header
	@./generate_header
	@rm generate_header

options:
	$(info CFLAGS  : ${CFLAGS})
	$(info LDFLAGS  : ${LDFLAGS})

clean:
	$(info cleaning ...)
	@rm -f */*.o */*/*.o */*.gw.* */*/*.gw.* */*/*.gcda */*/*.gcno gwion

src/arg.o:
	$(info compile $(<:.c=) (with arguments defines))
	@${CC} ${CFLAGS} -c src/arg.c -o src/arg.o -DLDFLAGS='${LDCFG}' -DCFLAGS='${CCFG}'

.c.o:
	$(info compile $(<:.c=))
	@${CC} ${CFLAGS} -c $< -o $(<:.c=.o)

install: directories
	cp ${PRG} ${PREFIX}

uninstall:
	rm ${PREFIX}/${PRG}

test:
	@bash utils/test.sh tests/sh severity=11 test/sh examples severity=10 tests/error tests/tree tests/ugen_coverage test/bug

parser:
	$(info generating parser)
	@${YACC} -o src/ast/parser.c --defines=include/parser.h utils/gwion.y

lexer:
	$(info generating lexer)
	@${LEX}  -o src/ast/lexer.c utils/gwion.l

gwcov: utils/gwcov.o
	$(info compiling gwcov)
	@${CC} utils/gwcov.o -o gwcov ${LDFLAGS}

gwlint: ${TOOL_OBJ} utils/gwlint.o
	$(info compiling gwlint)
	@${CC} ${CFLAGS} ${TOOL_OBJ} -o gwlint -DGWLINT utils/gwlint.o ${LDFLAGS}

gwtag: ${TOOL_OBJ} utils/gwtag.o
	$(info compiling gwlint)
	@${CC} ${CFLAGS} ${TOOL_OBJ} -o gwtag -DGWLINT utils/gwtag.o ${LDFLAGS}

directories:
	mkdir -p ${PREFIX} ${GWION_ADD_DIR}

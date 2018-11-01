include util/config.mk
include config.mk

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(@F:.o=.Td)

# initialize source lists
src_src := $(wildcard src/*.c)
lib_src := $(wildcard src/lib/*.c)
oo_src := $(wildcard src/oo/*.c)
vm_src := $(wildcard src/vm/*.c)
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

ifeq (${SNDFILE_D}, 1)
LDFLAGS += -lsndfile
CFLAGS +=-DHAVE_SNDFILE
drvr_src +=src/drvr/sndfile.c
else
CFLAGS +=-DNO_LIBSNDFILE
endif
ifeq (${PLOT_D}, 1)
CFLAGS +=-DHAVE_PLOT
drvr_src +=src/drvr/plot.c
endif
ifeq (${SLES_D}, 1)
LDFLAGS += -lOpenSLES
CFLAGS +=-DHAVE_SLES
drvr_src +=src/drvr/sles.c
endif
# add boolean
ifeq (${USE_GWCOV}, 1)
CFLAGS += -DGWCOV
endif

ifeq (${DEBUG_STACK}, 1)
CFLAGS += -DDEBUG_STACK
endif
ifeq (${USE_OPTIMIZE}, 1)
util_src += opt/optim.c
CFLAGS+= -DOPTIMIZE
endif
ifeq (${USE_JIT}, 1)
include jit/config.mk
endif

ifeq (${USE_VMBENCH}, 1)
CFLAGS += -DVMBENCH
LDFLAGS += -lbsd
endif

# add definitions
CFLAGS+= -DD_FUNC=${D_FUNC}

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
drvr_obj := $(drvr_src:.c=.o)

GW_OBJ=${src_obj} ${ast_obj} ${parse_obj} ${emit_obj} ${oo_obj} ${drvr_obj} ${vm_obj} ${util_obj} ${lib_obj}

ifeq ($(shell uname), Linux)
LDFLAGS+=-lrt
endif

CCFG="${CFLAGS}"
LDCFG="${LDFLAGS}"

# hide this from gwion -v
CFLAGS += -DGWION_BUILTIN

LDFLAGS += util/libgwion_ast.a

all: include/generated.h options ${GW_OBJ} ${jit_obj}
	@make -C util
	$(info link ${PRG})
	@${CC} ${GW_OBJ} ${jit_obj} ${LDFLAGS} -o ${PRG}

config.mk:
	$(info generating config.mk)
	@cp config.mk.orig config.mk

options:
	$(info CC      : ${CC})
	$(info CFLAGS  : ${CFLAGS})
	$(info LDFLAGS : ${LDFLAGS})

clean:
	$(info cleaning ...)
	@rm -f */*.o */*/*.o */*.gw.* */*/*.gw.* */*/*.gcda */*/*.gcno gwion

src/arg.o:
	$(info compile $(<:.c=) (with arguments defines))
	@${CC} $(DEPFLAGS) ${CFLAGS} -c src/arg.c -o src/arg.o -DLDFLAGS='${LDCFG}' -DCFLAGS='${CCFG}'
	@mv -f $(DEPDIR)/$(@F:.o=.Td) $(DEPDIR)/$(@F:.o=.d) && touch $@
	@echo $@: config.mk >> $(DEPDIR)/$(@F:.o=.d)

.c.o: $(DEPDIR)/%.d
	$(info compile $(<:.c=))
	@${CC} $(DEPFLAGS) ${CFLAGS} -c $< -o $(<:.c=.o)
	@mv -f $(DEPDIR)/$(@F:.o=.Td) $(DEPDIR)/$(@F:.o=.d) && touch $@
	@echo $@: config.mk >> $(DEPDIR)/$(@F:.o=.d)

install:
	install ${PRG} ${PREFIX}

uninstall:
	rm ${PREFIX}/${PRG}

test:
	@bash help/test.sh tests/* examples

include $(wildcard .d/*.d)

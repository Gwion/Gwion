# here you set the default driver
D_FUNC?=alsa_driver
CFLAGS+=-DD_FUNC=${D_FUNC}
# one of:
# mandatory (shipped with gwion)
#  sndfile_driver dummy_driver silent_driver
# optionnal (depends on libs)
#  alsa_driver jack_driver soundio_driver portaudio_driver

# set those value to 1 or uncomment if you want the driver
# set to 0 or comment thoes you don't want/need/have
# if DD_FUNC is one of the optional driver, this driver has to be set to 1
ALSA_D      ?= 1
#JACK_D      ? = 1
#PORTAUDIO_D ?= 1
#SOUNDIO_D   ?= 1
SPA_D       ?= 1

# colorize output
#CFLAGS+=-DCOLOR

#various debug flags
#CFLAGS+=-DDEBUG_COMPILE
#CFLAGS+=-DDEBUG_OPERATOR
#CFLAGS+=-DDEBUG_TYPE
#CFLAGS+=-DDEBUG_SCAN1
#CFLAGS+=-DDEBUG_SCAN2
#CFLAGS+=-DDEBUG_EMIT
#CFLAGS+=-DDEBUG_VM
#CFLAGS+=-DDEBUG_INSTR
#CFLAGS+=-DDEBUG_SHREDULER


#uncomment this to set the float type to double
USE_DOUBLE=1

# if any debug flag is set, we need -DDEBUG
ifeq ($(findstring DEBUG,$(CFLAGS)), DEBUG)
CFLAGS+=-DDEBUG
endif

#this must point to a valid soundpipe data directory
SOUNDPIPE_DATA_DIR?=../Soundpipe/modules/data

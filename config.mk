# here you set the default driver
CFLAGS+=-DD_FUNC=sio_driver

# set those value to 1 if you want the driver
# comment thoes you don't want/need/have
ALSA_D      = 1
JACK_D      = 1
PORTAUDIO_D = 1
SOUNDIO_D   = 1
SPA_D       = 1

#vrious debug flags
#CFLAGS+=-DDEBUG_COMPILE
#CFLAGS+=-DDEBUG_OPERATOR
#CFLAGS+=-DDEBUG_TYPE
#CFLAGS+=-DDEBUG_SCAN1
#CFLAGS+=-DDEBUG_SCAN2
#CFLAGS+=-DDEBUG_EMIT
#CFLAGS+=-DDEBUG_VM
#CFLAGS+=-DDEBUG_INSTR
#CFLAGS+=-DCOLOR
#CFLAGS+=-DDEBUG_SHREDULER

# don't forget it if you want debug mode
#CFLAGS+=-DDEBUG # this one should be set automatically

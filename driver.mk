ifeq (${ALSA_D}, 1)
  LDFLAGS+=-lasound
  CFLAGS+=-DHAVE_ALSA
	drvr_obj+=driver/alsa.o
endif

ifeq (${JACK_D}, 1)
  LDFLAGS+=-ljack
  CFLAGS+=-DHAVE_JACK
	drvr_obj+=driver/jack.o
endif

ifeq (${PORTAUDIO_D}, 1)
  LDFLAGS+=-lportaudio
  CFLAGS+=-DHAVE_PORTAUDIO
	drvr_obj+=driver/portaudio.o
endif

ifeq (${SOUNDIO_D}, 1)
  LDFLAGS+=-lsoundio
  CFLAGS+=-DHAVE_SOUNDIO
	drvr_obj+=driver/soundio.o
endif

ifeq (${SPA_D}, 1)
  CFLAGS+=-DHAVE_SPA
	drvr_obj+=driver/spa.o
endif

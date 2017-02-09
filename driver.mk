ifeq (${SNDFILE_D}, 1)
  LDFLAGS+=-lsndfile
  CFLAGS+=-DHAVE_SNDFILE
  drvr_src+=driver/sndfile.c
endif

ifeq (${SPA_D}, 1)
  CFLAGS+=-DHAVE_SPA
  drvr_src+=driver/raw.c
endif

ifeq (${ALSA_D}, 1)
  LDFLAGS+=-lasound
  CFLAGS+=-DHAVE_ALSA
  drvr_src+=driver/alsa.c
endif

ifeq (${JACK_D}, 1)
  LDFLAGS+=-ljack
  CFLAGS+=-DHAVE_JACK
  drvr_src+=driver/jack.c
endif

ifeq (${PORTAUDIO_D}, 1)
  LDFLAGS+=-lportaudio
  CFLAGS+=-DHAVE_PORTAUDIO
  drvr_src+=driver/portaudio.c
endif

ifeq (${SOUNDIO_D}, 1)
  LDFLAGS+=-lsoundio
  CFLAGS+=-DHAVE_SOUNDIO
  drvr_src+=driver/soundio.c
endif


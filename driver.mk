DRIVER_OK=0
ifeq (${D_FUNC}, raw_driver)
SPA_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, sndfile_driver)
SNDFILE_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, alsa_driver)
ALSA_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, jack_driver)
JACK_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, pa_driver)
PORTAUDIO_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, sio_driver)
SOUNDIO_D=1
DRIVER_OK=1
endif

ifeq (${D_FUNC}, dummy_driver)
DRIVER_OK=1
endif
ifeq (${D_FUNC}, silent_driver)
DRIVER_OK=1
endif

############################


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


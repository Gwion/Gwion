#!/bin/bash

format definition:
line starting with '#' are comments
line starting with '##' are headers

line starting with 'opt:' are options (e.g;: use-double, help)
line starting with 'arg:' arg options who need arguments (e.g.: --default-driver)
line starting with 'bool:' are boolean options (e.g;: --debug, --no-debug)
line starting with 'opt:' are options (e.g;: use-double, help)


Libraries [ soundpipe ]:
  * line starting with 'inc:' include for library
  * line starting with 'lib:' libary path

  => '$ lib: xxx, include, lib'

Drivers [ spa sndfile alsa jack portaudio soundio]:
  * need 'bool' --[with|without]-xxx
  * need 'inc'
  * need 'lib'

  => '$ drv: xxx, inc, lib, on/off'

Directories [soundpipe_data_dir plug api tag tok doc]:
  * use default value.

  => '$ arg: api, "/usr/lib/Gwion/api"'

Debug:

  => '$ dbg: xxx' # of by default.

---------
drv: spa, , 
drv: sndfile, , -lsndfile
drv: alsa, /usr/include/alsa, -lasound
drv: jack, /usr/include/jack, -ljack
drv: portaudio, , -lportaudio
drv: soundio, /usr/include/soundio, -lsoundio

#!/bin/sh

sudo apt-get install valgrind libsndfile1 libsndfile1-dev

# Soundpipe
pushd Soundpipe
if [ "$GW_FLOAT_TYPE" = "double" ]
then
    sed -i 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
fi
make
sudo make install
popd

if [ "$GW_FLOAT_TYPE" = "double" ]
then
    sed -i 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
fi


sed -i "s/CFLAGS+=-DD_FUNC=alsa_driver/CFLAGS+=-DD_FUNC=dummy_driver/" config.def.mk
sed -i    "s/ALSA_D/#ALSA_D/" config.def.mk
sudo mkdir -p /usr/lib/Gwion/{doc,api,plug,tags,tok}
sudo chmod -R a+rw /usr/lib/Gwion

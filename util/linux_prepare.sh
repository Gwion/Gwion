#!/bin/sh

sudo apt-get install valgrind libsndfile1 libsndfile1-dev

# Soundpipe
pushd Soundpipe
if [ "$GW_FLOAT_TYPE" = "double" ]
then
    sed -i 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
fi
make
export LIBARY_PATH=$PWD
#sudo make install
popd

if [ "$GW_FLOAT_TYPE" = "double" ]
then
    sed -i 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
fi


sed -i "s/CFLAGS+=-DD_FUNC=alsa_driver/CFLAGS+=-DD_FUNC=dummy_driver/" config.def.mk
sed -i "s/ALSA_D/#ALSA_D/" config.def.mk

#sudo mkdir -p /usr/lib/Gwion/{doc,api,plug,tags,tok}
#sudo chmod -R a+rw /usr/lib/Gwion

# use local directories as default location is not writable in Travis
#export GWION_DOC_DIR="./doc"
#export GWION_API_DIR="./api"
#export GWION_TOK_DIR="./tok"
#export GWION_TAG_DIR="./tag"
#export GWION_PLUG_DIR="./plug"

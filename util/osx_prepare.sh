#!/bin/sh
brew install valgrind libsndfile

pushd Soundpipe
if [ "$GW_FLOAT_TYPE" = "double" ]
then
	sed -i '' 's/#USE_DOUBLE/USE_DOUBLE/'                                     config.def.mk
fi
make
sudo make install
popd


if [ "$GW_FLOAT_TYPE" = "double" ]
then
	sed -i '' 's/#USE_DOUBLE/USE_DOUBLE/'                                     config.def.mk
fi

#remove -lrt
sed -i '' 's/-lrt//'                                                      Makefile

#remove alsa driver
sed -i '' "s/CFLAGS+=-DD_FUNC=alsa_driver/CFLAGS+=-DD_FUNC=dummy_driver/" config.def.mk
sed -i '' "s/ALSA_D/#ALSA_D/"                                             config.def.mk

#export doc dirs
export GWION_DOC_DIR="./doc"
export GWION_API_DIR="./api"
export GWION_TOK_DIR="./tok"
export GWION_TAG_DIR="./tag"
export GWION_PLUG_DIR="./plug"

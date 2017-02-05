#!/bin/sh
brew unlink bison
brew install valgrind libsndfile bison


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
sed -i '' "s/-lsoundpipe/Soundpipe\/libsoundpipe.a/" Makefile
#sed -i '' "s/-o parser.c -dv gwion.y -x/-o parser.c -dv gwion.y/" ast/Makefile

#export doc dirs
export GWION_DOC_DIR="./doc"
export GWION_API_DIR="./api"
export GWION_TOK_DIR="./tok"
export GWION_TAG_DIR="./tag"
export GWION_PLUG_DIR="./plug"

#echo 'export PATH="/usr/local/opt/bison/bin:$PATH"' >> ~/.bash_profile
#echo 'export LDFLAGS:  -L/usr/local/opt/bison/lib' >> ~/.bash_profile
which bison

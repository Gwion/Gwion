#!/bin/bash

# TODO: output error message
check_variable() {
	if [ -z "$1" ]
	then
		echo "unknown variable $2."
		exit 1
	fi
}

check_variable "$BISON_VERSION"  BISON_VERSION
check_variable "$TRAVIS_OS_NAME" TRAVIS_OS_NAME
check_variable "$SP_BRANCH"      SP_BRANCH
check_variable "$GWION_DOC_DIR"  GWION_DOC_DIR
check_variable "$GWION_API_DIR"  GWION_API_DIR
check_variable "$GWION_TOK_DIR"  GWION_TOK_DIR
check_variable "$GWION_TAG_DIR"  GWION_TAG_DIR
check_variable "$GWION_PLUG_DIR" GWION_PLUG_DIR
check_variable "$GW_FLOAT_TYPE"  GW_FLOAT_TYPE

SED() {
	if [ "$TRAVIS_OS_NAME" = "osx" ]
	then echo "sed -i ''"
	else echo "sed -i"
	fi
}

brew_dependencies() {
	brew install libsndfile # needed for soundpipe
	brew install valgrind   # needed for test
	brew install lua        # needed for importing soundpipe
}

install_bison() {
	wget "http://ftp.gnu.org/gnu/bison/$BISON_VERSION.tar.gz"
	tar -xzf bison-3.0.4.tar.gz
	pushd "$BISON_VERSION"
    ./configure --prefix="$PWD/../bison"
    make install
    popd
}

install_bats() {
	git clone https://github.com/sstephenson/bats.git
}

install_soundpipe() {
	git clone -b "$SP_BRANCH" https://github.com/paulbatchelor/Soundpipe.git
	pushd Soundpipe
#	[ "$GW_FLOAT_TYPE" = "double" ] && $(SED) 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
    $(SED) "s/-DSPFLOAT=float/-DSPFLOAT=${SPFLOAT}/" Makefile
    if [ "$SP_BRANCH" = "master" ]
    then
		wget https://gist.githubusercontent.com/fennecdjay/a5dbc54342bcf6f0c8d5f9a03355580b/raw/f7a6a000b6687fb5edc494c2d5fe05e8bb30bc40/soundpipe_data_master.patch
		patch -p1 -i soundpipe_data_master.patch
		rm soundpipe_data_master.patch
		rm modules/data/padsynth.lua
 	fi
    make
	popd
}

configure_Gwion() {
    ls "$PWD/Soundpipe/modules/data"
#	[ "$GW_FLOAT_TYPE" = "double" ] && $(SED) 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
	$(SED) "s/-lsoundpipe/Soundpipe\/libsoundpipe.a/" Makefile
}

prepare_directories() {
	mkdir -p "$GWION_DOC_DIR"
	mkdir -p "$GWION_API_DIR"
	mkdir -p "$GWION_TOK_DIR"
	mkdir -p "$GWION_TAG_DIR"
	mkdir -p "$GWION_PLUG_DIR"
}

[ "$TRAVIS_OS_NAME" = "osx" ] && brew_dependencies

install_bison
install_bats
install_soundpipe
configure_Gwion
prepare_directories

exit 0

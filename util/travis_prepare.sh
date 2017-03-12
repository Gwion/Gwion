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
check_variable "$GWION_ADD_DIR" GWION_PLUG_DIR
#check_variable "$GW_FLOAT_TYPE"  GW_FLOAT_TYPE

brew_dependencies() {
	brew install libsndfile # needed for soundpipe
	brew install valgrind   # needed for test
	brew install lua        # needed for importing soundpipe
}

install_bison() {
	wget "http://ftp.gnu.org/gnu/bison/$BISON_VERSION.tar.gz"
	tar -xzf "$BISON_VERSION.tar.gz"
	pushd "$BISON_VERSION"
    ./configure --prefix="$PWD/../bison"
    make install
    popd
}

install_soundpipe() {
	git clone -b "$SP_BRANCH" https://github.com/paulbatchelor/Soundpipe.git
#	git clone -b "$SP_BRANCH" https://github.com/fennecdjay/Soundpipe.git
	pushd Soundpipe
#	[ "$USE_DOUBLE" -eq 1 ] && export SP_FLOAT="double"
	[ "$USE_DOUBLE" -eq 0 ] && export SPFLOAT="float"
	[ -z "$USE_DOUBLE" ] && export SPFLOAT="float"
	[ "$USE_DOUBLE" -eq 0 ] && unset USE_DOUBLE
#    if [ "$SP_BRANCH" = "master" ]
#    then
#		wget https://gist.githubusercontent.com/fennecdjay/a5dbc54342bcf6f0c8d5f9a03355580b/raw/f7a6a000b6687fb5edc494c2d5fe05e8bb30bc40/soundpipe_data_master.patch
#		patch -p1 -i soundpipe_data_master.patch
#		rm soundpipe_data_master.patch
#		rm modules/data/padsynth.lua
# 	fi
    make
	popd
}

build_soundpipe() {
	[ "$USE_DOUBLE" -eq 0 ] && export SPFLOAT="float"
	[ -z "$USE_DOUBLE" ] && export SPFLOAT="float"
	[ "$USE_DOUBLE" -eq 0 ] && unset USE_DOUBLE
	make
}

check_soundpipe() {
	[ -d Soundpipe ] && {
		git clone -b "$SP_BRANCH" https://github.com/paulbatchelor/Soundpipe.git
		pushd Soundpipe
		build_soundpipe
		popd
		return 0
	}
	pushd Soundpipe
	git fetch
	[ $(git rev-parse HEAD) = $(git rev-parse @{u}) ] || build_soundpipe
	popd
	return 0
}

prepare_directories() {
	mkdir -p doc/{dat,dot,map,png,rst,search,tex}
	mkdir -p api
	mkdir -p tag
	mkdir -p tok
	mkdir -p add
}

[ "$#" = 1 ] && [ "$1" = "check_soundpipe" ] && check_soundpipe && return


[ "$TRAVIS_OS_NAME" = "osx" ] && brew_dependencies

install_bison
#install_soundpipe
check_soundpipe
prepare_directories
exit 0

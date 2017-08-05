#!/bin/bash
# needed variables: TRAVIS_OS_NAME SP_BRANCH GWION_PLUG_DIR

brew_dependencies() {
	brew install libsndfile # (still) needed for soundpipe
	brew install valgrind   # needed for test
}

build_soundpipe() {
	[ "$USE_DOUBLE" -eq 0 ] && export SPFLOAT="float"
	[ -z "$USE_DOUBLE" ] && export SPFLOAT="float"
	[ "$USE_DOUBLE" -eq 0 ] && unset USE_DOUBLE
	make h/soundpipe.h 
	unset USE_DOUBLE
	make
}

check_soundpipe() {
rm -rf Soundpipe
	[ -d Soundpipe ] || {
		git clone -b "$SP_BRANCH" https://github.com/paulbatchelor/Soundpipe.git
		pushd Soundpipe
		build_soundpipe
		popd
		return 0
	}
	pushd Soundpipe
	git fetch
	[ "$(git rev-parse HEAD)" = "$(git rev-parse '@{u}')" ] || build_soundpipe
	popd
	return 0
}

prepare_directories() {
	mkdir -p "$GWION_ADD_DIR"
}

[ "$TRAVIS_OS_NAME" = "osx" ] && brew_dependencies

check_soundpipe
prepare_directories
exit 0

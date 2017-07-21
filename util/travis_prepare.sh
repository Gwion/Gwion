#!/bin/bash

check_variable() {
	if [ -z "$1" ]
	then
		echo "unknown variable $2."
		exit 1
	fi
}

check_variable "$TRAVIS_OS_NAME" TRAVIS_OS_NAME
check_variable "$SP_BRANCH"      SP_BRANCH
check_variable "$GWION_ADD_DIR" GWION_PLUG_DIR

brew_dependencies() {
	brew install libsndfile # needed for soundpipe
	brew install valgrind   # needed for test
}

build_soundpipe() {
	[ "$USE_DOUBLE" -eq 0 ] && export SPFLOAT="float"
	[ -z "$USE_DOUBLE" ] && export SPFLOAT="float"
	[ "$USE_DOUBLE" -eq 0 ] && unset USE_DOUBLE
	make
}

check_soundpipe() {
#rm -rf Soundpipe # look at me!
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
#	[ "$(git rev-parse HEAD)" = "$(git rev-parse @{u})" ] || build_soundpipe
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

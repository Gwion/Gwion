#!/bin/bash

# needs $BISON_VERSION

SED() {
	if [ "$TRAVIS_OS_NAME" = "osx" ]
	then echo "sed -i ''";
	else echo "sed -i";
	fi
}
brew_dependencies() {
	brew install libsndfile # needed for soundpipe
	brew install valgrind   # needed for test
	brew install lua        # need for importing soundpipe
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
	[ "$GW_FLOAT_TYPE" = "double" ] && $(SED) 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
    if [ "$SP_BRANCH" = "master" ]
    then
		wget https://gist.githubusercontent.com/fennecdjay/688b657519faa21f0460457de36beafb/raw/4c9c9090b100a24f49353069b7b2262c50aa0704/data_fix.diff
		patch -p1 < data_fix.diff
 	fi
	rm data_fix.diff
    make
	popd
}

configure_Gwion() {
	[ "$GW_FLOAT_TYPE" = "double" ] && $(SED) 's/#USE_DOUBLE/USE_DOUBLE/' config.def.mk
	$(SED) "s/CFLAGS+=-DD_FUNC=alsa_driver/CFLAGS+=-DD_FUNC=dummy_driver/" config.def.mk
	$(SED) "s/ALSA_D/#ALSA_D/" config.def.mk
	$(SED) "s/-lsoundpipe/Soundpipe\/libsoundpipe.a/" Makefile
    if [ "$TRAVIS_OS_NAME" = "osx" ]
	then
#		$(SED) "s/YACC ?= bison/YACC=$PWD\/bison-3.0.4\/src\/bison/" ast/Makefile
		$(SED) "s/-lrt//" Makefile
		mkdir /Users/travis/build/fennecdjay/Gwion/bison-3.0.4/share/bison/
		cp -r bison-3.0.4/data bison-3.0.4/share/bison
	fi
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

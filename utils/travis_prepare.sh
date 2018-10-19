#!/bin/bash
# needed variables: TRAVIS_OS_NAME SP_BRANCH GWION_PLUG_DIR

brew_dependencies() {
	brew update
}

prepare_directories() {
	mkdir -p "$GWION_ADD_DIR"
}

[ "$TRAVIS_OS_NAME" = "osx" ] && brew_dependencies

prepare_directories
exit 0

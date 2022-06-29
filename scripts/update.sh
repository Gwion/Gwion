#!/bin/bash

update() {
  git pull
  git submodule update
  make clean-all
  make -j
  make -C plug clean
  make -C plug
  make -C plug local-install
}

reset() {
  pushd "$1"
  git reset --hard
  popd
}

echo "this will erase all your changes in the gwion codebase."
echo "type 'Enter' to proceed, anything + 'Enter' to abort"
read a
[ -z "$a" ] || exit 1

for a in . ast fmt libcmdapp plug util
do reset "$a"
done
update

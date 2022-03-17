#!/bin/bash
git pull
git submodule update
make clean-all
make -j
make -C plug
sudo make install
for a in $(cat plug/list.txt)
do cp plug/$a/*.so ~/gwplug
done

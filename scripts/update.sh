#!/bin/bash
git pull
git submodule update --recursive --init
make clean-all
make -j
make -C plug
sudo make install
mkdir -p ~/.gwplug
rm -f ~/.gwplug/*.so
for a in $(cat plug/list.txt)
do cp plug/$a/*.so ~/.gwplug
done

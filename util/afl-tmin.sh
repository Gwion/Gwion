#!/bin/bash

# RAMDISK
#
#mount -t tmpfs -o size=512m tmpfs $RAMDISK

tmin() {
  [ $# -lt 3 ] && {
	echo "usage: $0 <inputdir(optionnal)> <outputdir(optionnal)> <cmd(optionnal)>"
	exit 1
  }

  mem_limit=256

  cores=$(( $(lscpu | head -n 4 | tail -n -1 | cut -d ':' -f2 | xargs) -1 ))
  inputdir=$1
  outputdir=$2
  cmd=$3
  total=`ls $inputdir | wc -l`
  for k in $inputdir/*.gw
  do
    file=$(basename $k)
		afl-tmin -m $mem_limit -i $k -o $outputdir/$file -- "$cmd" & #put the command to run after the -- 
  done
  wait
}

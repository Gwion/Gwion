#!/bin/bash

# RAMDISK
#
#mount -t tmpfs -o size=512m tmpfs $RAMDISK

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

#for k in `seq 1 $cores $total`
for k in $inputdir/*.gw
do
#	for i in `seq 0 $(expr $cores - 1)`
#	do
file=$(basename $k)
#		file=`ls -Sr $inputdir/*.gw | sed $(expr $i + $k)"q;d"`
#		afl-tmin -m $mem_limit -i $inputdir/$file -o $outputdir/$file -- "$cmd" & #put the command to run after the -- 
		afl-tmin -m $mem_limit -i $k -o $outputdir/$file -- "$cmd" & #put the command to run after the -- 
#	done

#	wait
done
wait

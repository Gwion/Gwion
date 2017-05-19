#!/bin/sh
skip=0
IFS=''
while read line
do
	if [ "${line:0:13}" = "#ifdef DEBUG_" ]
	then skip=1
	elif [ "${line:0:6}" = "#endif" ]
	then [ $skip ] && skip=0
	elif [ $skip = 0 ]
	then echo "$line"
	fi
done < "$1"

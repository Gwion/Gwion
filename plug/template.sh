#!/bin/sh
#needs an argument
[ "$1" ] || {
	echo "usage: $0 <plugin name>"
	exit 1
}

mkdir $1
echo "\
include ../config.mk
NAME=$1
#CFLAGS+=-I<more include>
#LDFLAGS+=-I<more LIBS>
include ../config_post.mk
" > $1/Makefile

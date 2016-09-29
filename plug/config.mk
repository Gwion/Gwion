CC      = cc
SRC     =$(wildcard *.c)
OBJ     = $(SRC:.c=.o)
CFLAGS  =-I../../include
LDFLAGS =-shared -fPIC
PLUG_DIR=/usr/lib/Gwion/plug


INC      = -I../../include -I../../util/include -I../../ast/include
CC       ?= gcc

NAME := ${NAME}
SRC = ${NAME}.c
OBJ = $(SRC:.c=.o)
CFLAGS   = -std=c99 $(../../gwion -k 2>&1 | grep CFLAGS | sed 's/CFLAGS: //') ${INC} ${CICFLAGS} -Wall -Wextra -g -Og
LDFLAGS   = $(../../gwion -k 2>&1 | grep LDFLAGS)

# os specific
ifeq ($(shell uname), Darwin)
LDFLAGS += -bundle -undefined dynamic_lookup
else
ifeq ($(shell uname), Linux)
LDFLAGS += -shared
else
LDFLAGS += -shared -lpsapi -fPIC -Wl,--export-all -Wl,--enable-auto-import
LDLAGS += ../../libgwion.dll.a
endif
endif

all: ${NAME}.so


${NAME}.so: ${OBJ}
ifeq (${USE_LD}, 1)
	${LD} $^ -o ${NAME}.so ${LDFLAGS}
else
	${CC} $^ -o ${NAME}.so ${LDFLAGS}
endif

clean:
	rm -f ${OBJ} ${NAME}.so

.c.o:
	${CC} -fPIC ${CFLAGS} -c $< -o $(<:.c=.o) -g

install: ${NAME}.so
	install ${NAME}.so ${GWION_ADD_DIR}

uninstall:
	rm ${GWION_ADD_DIR}/${NAME}.so

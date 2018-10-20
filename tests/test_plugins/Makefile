INC      = -I../../include -I../../util/include
CC       ?= gcc

SRC = ${NAME}.c
OBJ = $(SRC:.c=.o)
CFLAGS   = $(../../gwion -C 2>&1 | grep CFLAGS) ${INC}
LDFLAGS   = $(../../gwion -C 2>&1 | grep LDFLAGS)

# os specific
ifeq ($(shell uname), Linux)
LDFLAGS += -shared
else
LDFLAGS += -bundle -undefined dynamic_lookup
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

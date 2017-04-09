INC      = ../../include
CC       ?= gcc

SRC = ${NAME}.c
OBJ = $(SRC:.c=.o)
CFLAGS   = -I${INC} -ftest-coverage -fprofile-arcs

LDFLAGS  = -fPIC --coverage

# os specific
ifeq ($(shell uname), Linux)
LDFLAGS += -shared
else
LDFLAGS += -bundle -undefined dynamic_lookup
endif

default: ${NAME}.so


${NAME}.so: ${OBJ}
ifeq (${USE_LD}, 1)
	${LD} $^ -o ${NAME}.so ${LDFLAGS}
else
	${CC} $^ -o ${NAME}.so ${LDFLAGS}
endif


clean:
	rm -f ${OBJ} ${NAME}.so

.c.o:
	${CC} -fPIC ${CFLAGS} -c $< -o $(<:.c=.o)

install: ${NAME}.so
	install ${NAME}.so ${GWION_ADD_DIR}

uninstall:
	rm ${GWION_ADD_DIR}/${NAME}.so

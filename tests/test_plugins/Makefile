INC      = ../../include
CC       = gcc

SRC = ${NAME}.c
OBJ = $(SRC:.c=.o)
CFLAGS   = -I${INC} -ftest-coverage -fprofile-arcs

LDFLAGS  = -shared -fPIC --coverage

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
	install ${NAME}.so ${TRAVIS_BUILD_DIR}/${GWION_ADD_DIR}

uninstall:
	rm ${TRAVIS_BUILD_DIR}/${GWION_ADD_DIR}/${NAME}.so

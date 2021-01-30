INC      = -I../../include -I../../util/include -I../../ast/include
CC       ?= gcc

NAME := ${NAME}
SRC = ${NAME}.c
OBJ = $(SRC:.c=.o)
CFLAGS += -std=c99 ${INC} -Wall -Wextra -g -Og

# os specific
ifeq ($(shell uname), Darwin)
LDFLAGS += -bundle -undefined dynamic_lookup
else
ifeq ($(shell uname), Linux)
LDFLAGS += -shared
else
CFLAGS += -DBUILD_ON_WINDOWS=1
LDFLAGS += -shared -lpsapi -fPIC -Wl,--export-all -Wl,--enable-auto-import
LDFLAGS += -L../../ -lgwion
LDLAGS += ../../libgwion.dll.a
LDLAGS += ../../libgwion.a
LDFLAGS += -L../../ast -lgwion-ast
LDLAGS += ../../ast/libgwion-ast.dll.a
LDLAGS += ../../ast/libgwion-ast.a
LDFLAGS += -L../../util -lgwion-util
LDLAGS += ../../util/libgwion-util.dll.a
LDLAGS += ../../util/libgwion-util.a
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

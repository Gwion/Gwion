default: ${OBJ}
	${CC} -shared -fPIC ${CFLAGS}  ${OBJ} -o ${NAME}.so
clean:
	rm -f ${OBJ} ${NAME}.so
.c.o:
	cc -fPIC ${CFLAGS} -c $< -o $(<:.c=.o)
install:
	install ${NAME}.so ${PLUG_DIR}

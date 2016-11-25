CC = cc
YACC = bison
YFLAGS = -o parser.c -dv gwion.y -x
LEX = flex
LFLAGS =  -o lexer.c gwion.l

OBJ = lexer.o parser.o absyn.o
DEF =-I../include


default:
	@${YACC} ${YFLAGS}
	@${LEX}  ${LFLAGS}
	@make -s objs

objs: ${OBJ}
.c.o:
	@${CC} ${DEF} ${CFLAGS} -c $< -o $(<:.c=.o) -g

clean:
	@rm -f *.o parser.* lexer.*

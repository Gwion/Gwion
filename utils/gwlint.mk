gwlint:
	$(CC) -g -o gwlint -DGWLINT -Iinclude utils/gwlint.c \
		src/util/map.c src/util/vector.c\
		src/util/symbol.c src/util/err_msg.c src/util/absyn.c\
		src/ast/lexer.c src/ast/parser.c src/parse/op_utils.c -lm

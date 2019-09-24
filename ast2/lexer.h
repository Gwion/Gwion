#ifndef __LEXER
#define __LEXER
struct Lexer_;
typedef struct Lexer_ Lexer;
Token lexer_consume(Lexer *lex);
#endif


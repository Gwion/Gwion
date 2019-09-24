#ifndef __Parser
#define __Parser
typedef struct Parser_ {
  Lexer *lex;
//  uint8_t* input;
  Token left;
  Token self;
} Parser;

static Token tdop(Parser* p, int rbp);
static Token empty_token;
#endif

#ifndef __TOKEN
#define __TOKEN

struct Parser_;
typedef struct Token_ {
  uint8_t type;
  char* value;
  size_t len;
} Token;

typedef Token (*SemanticCode) (struct Parser_*);

struct TokenDesc {
  int           lbp;
  SemanticCode nud;
  SemanticCode led;
};

#endif

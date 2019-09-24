#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "gwion_util.h"
#include "keyword.h"
#include "token.h"
#include "lexer.h"
#include "tdop.h"

#define ADV { ++lex->right; ++lex->pos; }
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

typedef struct Lexer_ {
  char * str;
  struct TrieNode *root;
  int pos;
  int line;
  char instring;
  int left, right;
  bool escape;
} Lexer;

static inline void nl(Lexer *lex, char ch) {
  if(ch == '\n') {
    ++lex->line;
    lex->pos = 1;
  }
}

static inline bool is_escape(Lexer *lex, char ch) {
  return ch == '\\' && !lex->escape;
}

// Returns 'true' if the character is an OPERATOR.
static bool isOperator(const char ch) {
  if(ch == '+' || ch == '-' || ch == '*' ||
     ch == '/' ||
		 ch == '<' || ch == '=' || ch == '?' ||
     ch == '>' || ch == ':' || ch == '>' ||
     ch == '$' || ch == '%' || ch == '~' ||
     ch == '^' || ch == '&' || ch == '!')
		return (true);
  if(ch == '\n')
    return true;
	return (false);
}

// Returns 'true' if the character is a DELIMITER.
static bool isDelimiter(char ch)
{
	if (ch == ' ' || ch == '\r' || ch == '\t' ||
      ch == '(' || ch == ')' || ch == '[' ||
      ch == ']' || ch == '{' || ch == '}' ||
      ch == '\\' ||
      ch == '.' || ch == '\n' || ch == ',' || ch == ';' ||
      isOperator(ch))
		return (true);
	return (false);
}

// Returns 'true' if the string is a VALID IDENTIFIER.
ANN static bool validIdentifier(const char* str) {
  const char c = *str;
	if (c == '0' || c == '1' || c == '2' ||
		c == '3' || c == '4' || c == '5' ||
		c == '6' || c == '7' || c == '8' ||
		c == '9' || isDelimiter(c) == true)
		return false;
	return true;
}

// Returns 'true' if the string is an INTEGER.
bool isInteger(char* str) {
	int i, len = strlen(str) -1;
//	if(!len)
//		return false;
	for(i = 0; i < len; i++) {
		if (str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9')
			return false;
	}
  if(len &&
    (str[len] == 'u' || str[len] == 'U' || str[len] == 'l' || str[len] == 'L'))
    return true;
	return isdigit(str[i]);
}

// Extracts the SUBSTRING.
char* subString(char* str, int left, int right) {
	int i;
	char* subStr = (char*)malloc(
				sizeof(char) * (right - left + 2));

	for (i = left; i <= right; i++)
		subStr[i - left] = str[i];
	subStr[right - left + 1] = '\0';
	return (subStr);
}

static inline char isstring(Lexer *lex, const char ch) {
  if(!lex->escape && (ch == '"' || ch == '\''))
    return ch;
  return 0;
}

struct TrieNode* drive(const Vector);
void free_node(struct TrieNode*);
long unsigned int search(struct TrieNode *const restrict root, const char *key);

// Parsing the input STRING.
//void parse(char* str) {
Token lexer_consume(Lexer *lex) {
  lex->left = lex->right;
	int len = strlen(lex->str);
	while (lex->right <= len && lex->left <= lex->right) {
    nl(lex, lex->str[lex->right]);
    lex->escape = is_escape(lex, lex->str[lex->right]);
    char isstr = isstring(lex, lex->str[lex->right]);
    if(isstr) {
//  if(isstr && isstr == instring) {
    if(lex->instring) {
			char* subStr = subString(lex->str, lex->left, lex->right);
			printf("'%s' IS A VALID %s\n", subStr,
             lex->instring == '"' ? "STRING" : "CHAR");
xfree(subStr);
      lex->instring = 0;
ADV;
if(lex->escape)
ADV;
return (Token){ .type='s', .value="s"}; // string or char
    } else
      lex->instring = isstr;
    ADV
    continue;
  } else if(lex->instring) {
    ADV
    continue;
  }
  if(isDelimiter(lex->str[lex->right]) == false)
    ADV;
  if(isDelimiter(lex->str[lex->right]) == true && lex->left == lex->right) {
    if(isOperator(lex->str[lex->right]) == true) {
      do ADV while (isOperator(lex->str[lex->right]) == true);
      char* subStr = subString(lex->str, lex->left, lex->right - 1);
      printf("'%s' IS AN OPERATOR\n", subStr);
      //ADV;
      xfree(subStr);
      return (Token){ .type=lex->str[lex->right-1], .value="s" }; // string or char
    } else {
      char* subStr = subString(lex->str, lex->left, lex->right);
      const char type = lex->str[lex->right];
      printf("'%s' IS A DELIMITER %c\n", subStr, lex->str[lex->right]);
ADV;
      xfree(subStr);
      return (Token){ .type=type, .value="s" }; // string or char
    }
    lex->left = lex->right;
  } else if ((isDelimiter(lex->str[lex->right]) == true && lex->left != lex->right)
    || (lex->right == len && lex->left != lex->right)) {
    char* subStr = subString(lex->str, lex->left, lex->right - 1);
      if(search(lex->root, subStr)) {
				printf("'%s' IS A KEYWORD\n", subStr);
xfree(subStr);
//return (Token){ .type='k', .value="s" }; // string or char
/*}*/

    } else if (isInteger(subStr) == true) {
      printf("'%s' IS AN INTEGER\n", subStr);
      xfree(subStr);
      return (Token){ .type='i', .value="s" }; // string or char
    } else if (validIdentifier(subStr) == true 
	&& isDelimiter(lex->str[lex->right - 1]) == false) {
	printf("'%s' IS A VALID IDENTIFIER\n", subStr);
        xfree(subStr);
	return (Token){ .type='$', .value="id" };
    } else if (validIdentifier(subStr) == false
	&& isDelimiter(lex->str[lex->right - 1]) == false) {
	printf("'%s' IS NOT A VALID IDENTIFIER\n", subStr);
        xfree(subStr);
        return (Token){};
      } else
        xfree(subStr);
      lex->left = lex->right;
    }
  }
  return lex->right < len ? (Token){ .type= 'c' } : (Token){};
}

int main(int argc NUSED, char **argv) {
  assert(argv[1]);
  Lexer lex = { .line=1, .pos=1, .str = argv[1] };
  struct Vector_ keys;
  vector_init(&keys);
  for(long unsigned int i = 0; i < ARRAY_SIZE(keywords); i++)
    vector_add(&keys, (vtype)keywords[i]);
  lex.root = drive(&keys);//init the trie
/*
  {
    const Token empty_token = {};
    Token t;
    do { t = lexer_consume(&lex); }
    while(memcmp(&t, &empty_token, sizeof(Token)));
  }
*/
  Parser p = { .lex=&lex };
  {
//    const Token empty_token = {};
    Token t;
    do { t = tdop(&p, 0); }
    while(memcmp(&t, &empty_token, sizeof(Token)));
  }
  printf("%i lines %i pos\n", lex.line, lex.pos);
  free_node(lex.root);
  vector_release(&keys);
  return (0);
}

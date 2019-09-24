#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "token.h"
#include "lexer.h"
#include "tdop.h"
#define T_VAR 1

static struct TokenDesc tokens[256];

Token tdop(Parser* p, int rbp);
//static Token empty_token;

static Token led_add(Parser* p){
  puts(__func__);
  const Token right = tdop(p, 100);
  return right;
}

static Token nud_minus(Parser* p){
  puts(__func__);
//  const Token right = tdop(p, 10);
  return p->self;
}

static Token nud_mul(Parser* p){
  puts(__func__);
  const Token right = tdop(p, 100);
  return right;
}

static Token led_mul(Parser* p){
  puts(__func__);
//  const Token right = tdop(p, 10);
//printf("%c %s\n", right.type, right.value);
//  return right;
}

static Token led_minus(Parser* p){
//exit(3);
  printf("is it binary minus?\n", __func__);
  const Token right = tdop(p, 100);
  return right;
}

static Token nud_paren(Parser* p){
puts(__func__);
	Token t = tdop(p, 0);
puts("here");
//	if(p->self.type != ')'){
//	if(p->left.type != ')'){
	if(t.type != ')'){
		printf(" missing ')' %c %s", t.type, t.value);
		printf(" missing ')' %c %s", p->left.type, p->left.value);
		exit(1);
	} else {
//  t = tdop(p, 0);
//		p->input++;
	}
	return t;
}

static struct TokenDesc tokens[256] = {
//	['?'] = { 1, NULL     , led_question },
	['('] = { 0, nud_paren, NULL         },
	[')'] = { 0, NULL, NULL },
//	['>'] = { 2, NULL     , led_implies  },
//	['v'] = { 3, NULL     , led_or       },
//	['^'] = { 4, NULL     , led_and      },
//	['~'] = { 0, nud_not  , NULL         },
//	['+'] = { 10, nud_add, led_add   },
//	['+'] = { 10, NULL, led_add   },
//	['-'] = { 10, nud_minus, led_minus   },
//	['*'] = { 20, nud_mul, led_mul   },
//	['i'] = { 0, NULL, NULL         },
};

static Token vars[256];

static Token nonud(Parser* p){
  printf("[%s] ...\n", __func__);
	if(tokens[p->self.type].led == NULL){
		Token t = vars[p->self.type];
//		if(t.value)
//		assert(t.value);
			return t;
//		else {
//			t = generate();
//			vars[p->self.type] = t;
//			return t;
//		}
	} else {
		printf(" '%c' has no argument.\n", p->self.type);
		exit(1);
	}
//  exit(3);
}

Token tdop(Parser* p, const int rbp){
printf("[%s] %i\n", __func__, rbp);
  p->self = lexer_consume(p->lex);
printf("start with %c\n", p->self.type);
  if(!memcmp(&p->self, &empty_token, sizeof(Token)))
    return empty_token;
	struct TokenDesc* d = tokens + p->self.type;
	if(d->nud)
		p->left = d->nud(p);
	else
		p->left = nonud(p);
	for(;;){
printf("consume %c\n", p->self.type);
    p->self = lexer_consume(p->lex);
    if(!memcmp(&p->self, &empty_token, sizeof(Token)))
			return empty_token;
		struct TokenDesc * d = tokens + p->self.type;

		if(rbp >= d->lbp)
			break;

//		p->input++;

		if(d->led)
			p->left = d->led(p);
	}
	return p->left;
}

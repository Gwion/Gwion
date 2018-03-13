IS          (u|U|l|L)*

%{
#include "absyn.h"
#include "scanner.h"
#include "parser.h"
#include "err_msg.h"
#include <setjmp.h>

#define YYEROR
static jmp_buf jmp;
#define YY_FATAL_ERROR(msg) gwion_fatal_error(msg, yyscanner)
static void yynoreturn gwion_fatal_error (const char* msg , yyscan_t yyscanner) {
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    (void)yyg;
    gw_err("%s\n", msg );
    longjmp(jmp, 1);
}
static char* strip_lit(char* str);
static char* alloc_str(char* str);
static long htol(char* str);
static int newline(void* data);
static int adjust(void* data);



%}

%option noyywrap
%option noyylineno noyymore
%option noyyget_extra
%option noyyget_text
%option noyyget_lineno noyyset_lineno
%option noyyget_in
%option noyyget_out noyyset_out
%option noyyget_lval noyyset_lval
%option noyyget_lloc noyyset_lloc
%option noyyget_debug noyyset_debug
%option bison-bridge
%option reentrant
%option header-file="include/lexer.h"
%option prefix="gwion_"
%option noinput
%option nounput
%option never-interactive batch
%option nobackup nodebug

%x comment
%%

"//"              { BEGIN(comment); }
<comment>.         { adjust(yyscanner); continue;}
<comment>"\n"      { newline(yyscanner); BEGIN(INITIAL); }

"\n"                                { newline(yyscanner); continue; }
[ \r\t]                             { adjust(yyscanner); continue; }
";"                                 { adjust(yyscanner); return SEMICOLON;}
","                                 { adjust(yyscanner); return COMMA;}
"@"                                 { adjust(yyscanner); return ATSYM;}
"$"                                 { adjust(yyscanner); return DOLLAR;}
"=>"                                { adjust(yyscanner); return CHUCK;}
"=<"                                { adjust(yyscanner); return UNCHUCK;}
"+=>"                               { adjust(yyscanner); return PLUSCHUCK;}
"-=>"                               { adjust(yyscanner); return MINUSCHUCK;}
"*=>"                               { adjust(yyscanner); return TIMESCHUCK;}
"/=>"                               { adjust(yyscanner); return DIVIDECHUCK;}
"%=>"                               { adjust(yyscanner); return MODULOCHUCK;}
"@=>"                               { adjust(yyscanner); return ATCHUCK;}
"<<=>"                              { adjust(yyscanner); return RSL;}
">>=>"                              { adjust(yyscanner); return RSR;}
"&=>"                               { adjust(yyscanner); return RSAND;}
"|=>"                               { adjust(yyscanner); return RSOR;}
"^=>"                               { adjust(yyscanner); return RSXOR;}
"++"                                { adjust(yyscanner); return PLUSPLUS;}
"--"                                { adjust(yyscanner); return MINUSMINUS;}
"fun"|"function"                    { adjust(yyscanner); return FUNCTION;}
"variadic"                          { adjust(yyscanner); return VARARG; }
"typedef"                           { adjust(yyscanner); return TYPEDEF;}
"<<<"                               { adjust(yyscanner); return L_HACK;}
">>>"                               { adjust(yyscanner); return R_HACK;}
"#("                                { adjust(yyscanner); return SHARPPAREN;}
"%("                                { adjust(yyscanner); return PERCENTPAREN;}
"@("                                { adjust(yyscanner); return ATPAREN;}
"template"                          { adjust(yyscanner); return TEMPLATE;}

"+"                                 { adjust(yyscanner); return PLUS; }
"-"                                 { adjust(yyscanner); return MINUS; }
"/"                                 { adjust(yyscanner); return DIVIDE; }
"%"                                 { adjust(yyscanner); return PERCENT; }
"~"                                 { adjust(yyscanner); return TILDA; }
"!"                                 { adjust(yyscanner); return EXCLAMATION; }
"*"                                 { adjust(yyscanner); return TIMES; }
"new"                               { adjust(yyscanner); return NEW; }
"spork"                             { adjust(yyscanner); return SPORK; }
"union"                             { adjust(yyscanner); return UNION; }

"=="                                { adjust(yyscanner); return EQ; }
"!="                                { adjust(yyscanner); return NEQ; }
"<"                                 { adjust(yyscanner); return LT; }
"<{"                                { adjust(yyscanner); return LTB; }
">"                                 { adjust(yyscanner); return GT; }
"}>"                                { adjust(yyscanner); return GTB; }
"<="                                { adjust(yyscanner); return LE; }
">="                                { adjust(yyscanner); return GE; }
"&&"                                { adjust(yyscanner); return AND; }
"||"                                { adjust(yyscanner); return OR; }
"&"                                 { adjust(yyscanner); return S_AND; }
"|"                                 { adjust(yyscanner); return S_OR; }
"^"                                 { adjust(yyscanner); return S_XOR; }
">>"                                { adjust(yyscanner); return SHIFT_RIGHT; }
"<<"                                { adjust(yyscanner); return SHIFT_LEFT; }
"="                                 { adjust(yyscanner); return ASSIGN; }
"("                                 { adjust(yyscanner); return LPAREN; }
")"                                 { adjust(yyscanner); return RPAREN; }
"["                                 { adjust(yyscanner); return LBRACK; }
"]"                                 { adjust(yyscanner); return RBRACK; }
"{"                                 { adjust(yyscanner); return LBRACE; }
"}"                                 { adjust(yyscanner); return RBRACE; }
"?"                                 { adjust(yyscanner); return QUESTION; }
":"                                 { adjust(yyscanner); return COLON; }
"::"                                { adjust(yyscanner); return COLONCOLON; }
"]=>"                               { adjust(yyscanner); return TRIG; }
"]=<"                               { adjust(yyscanner); return UNTRIG; }


"class"                             { adjust(yyscanner); return CLASS;}
"dtor"                              { adjust(yyscanner); return AST_DTOR;}
"operator"                          { adjust(yyscanner); return OPERATOR;}
"extends"                           { adjust(yyscanner); return EXTENDS;}
"public"                            { adjust(yyscanner); return PUBLIC;}
"private"                           { adjust(yyscanner); return PRIVATE;}
"static "                           { adjust(yyscanner); return STATIC;}
"."                                 { adjust(yyscanner); return DOT;}

"if"                                { adjust(yyscanner); return IF;}
"else"                              { adjust(yyscanner); return ELSE;}
"break"                             { adjust(yyscanner); return BREAK;}
"continue"                          { adjust(yyscanner); return CONTINUE;}
"return"                            { adjust(yyscanner); return RETURN;}
"while"                             { adjust(yyscanner); return WHILE;}
"do"                                { adjust(yyscanner); return DO;}
"until"                             { adjust(yyscanner); return UNTIL;}
"repeat"                            { adjust(yyscanner); return LOOP;}
"for"                               { adjust(yyscanner); return FOR;}
"goto"                              { adjust(yyscanner); return GOTO;}
"switch"                            { adjust(yyscanner); return SWITCH;}
"case"                              { adjust(yyscanner); return CASE;}
"enum"                              { adjust(yyscanner); return ENUM;}
"typeof"                            { adjust(yyscanner); return TYPEOF;}
"const"                             { adjust(yyscanner); return CONST;}
"auto_ptr"                          { adjust(yyscanner); return AUTO_PTR;}
"auto"                              { adjust(yyscanner); return AUTO;}


0[xX][0-9a-fA-F]+{IS}?              { adjust(yyscanner); yylval->ival = htol(yytext);                 return NUM;        }
0[cC][0-7]+{IS}?                    { adjust(yyscanner); yylval->ival = atoi(yytext);                 return NUM;        }
[0-9]+{IS}?                         { adjust(yyscanner); yylval->ival = atoi(yytext);                 return NUM;        }
([0-9]+"."[0-9]*)|([0-9]*"."[0-9]+) { adjust(yyscanner); yylval->fval = atof(yytext);                 return FLOAT;      }
[A-Za-z_][A-Za-z0-9_]*              { adjust(yyscanner); yylval->sval = alloc_str(yytext);            return ID;         }
\"(\\.|[^\\"])*\"                   { adjust(yyscanner); yylval->sval = alloc_str(strip_lit(yytext)); return STRING_LIT; }
'(\\.|[^\\'])'                      { adjust(yyscanner); yylval->sval = alloc_str(strip_lit(yytext)); return CHAR_LIT;   }

.                                   { gw_err("error: stray in program\n"); return 1; }

%% // LCOV_EXCL_LINE
// LCOV_EXCL_LINE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "err_msg.h"

char* strip_lit(char* str){
  str[strlen(str)-1] = '\0';
  return str+1;
}

char* alloc_str(char* str) {
  S_Symbol sym = insert_symbol(str);
  return s_name(sym);
}

long htol(char* str) {
  char * c = str;
  unsigned long n = 0;

  c += 2;
  while(*c) {
    n <<= 4;
    switch(*c) {
      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0':
        n += *c - '0';
        break;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        n += *c - 'a' + 10;
        break;
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        n += *c - 'A' + 10;
        break;
    }
    c++;
  }
  return n;
}

int newline(void* data) {
  Scanner* scan = *(Scanner**)data;
  scan->line++;
  scan->pos = 1;
  return 1;
}

int adjust(void* data) {
  Scanner* scan = *(Scanner**)data;
  scan->pos += gwion_get_leng(data);
  return 0;
}

static void err_line(Scanner* scan) {
  int n = 1;
  size_t len = 0;
  m_str line = NULL;
  fseek(scan->file, 0, SEEK_SET);
  while (getline(&line, &len, scan->file) != -1) {
    if(n == scan->line) {
      gw_err("%s", line);
      break;
    }
    n++;
  }
  free(line);
}

static void err_marker(Scanner* scan) {
  while(--scan->pos > 0)
    gw_err(" ");
  gw_err("\033[1;31m^\033[0m\n");
}

int gwion_error(Scanner* scan, char* s) {
  err_msg(PARSE_, 0, "in file '%s'.", scan->filename);
  err_msg(PARSE_, scan->line, "%s. at pos %i\n", s, scan->pos);
  err_line(scan);
  err_marker(scan);
  return 0;
}

int get_pos(Scanner* scan) {
  return scan->line;
}

Ast parse(const m_str filename, FILE* f) {
  Scanner scan;
  gwion_lex_init(&scan.scanner);
  gwion_set_extra(&scan, scan.scanner);
  scan.line = 1;
  scan.pos  = 1;
  scan.file = f;
  scan.filename = filename;
  gwion_set_in(scan.file, scan.scanner);
  if(setjmp(jmp)) {
    scan.ast = NULL;
  } else if(gwion_parse(&scan))
    scan.ast = NULL;
//  fclose(scan.file);
  gwion_lex_destroy(scan.scanner);
  return scan.ast;
}

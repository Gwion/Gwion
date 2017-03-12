IS          (u|U|l|L)*

%{

#include "absyn.h"
#include "parser.h"

static char* strip_lit( char* str )
{
  str[strlen(str)-1] = '\0';
  return str+1;
}

static char* alloc_str( char* str )
{
/*
  char* s = (char*)calloc(1,  strlen(str) + 1 );
  strcpy( s, str );
  return s;
*/
 S_Symbol sym = insert_symbol(str);
 return S_name(sym);
}

static long htol( char* str )
{
char * c = str;
  unsigned long n = 0;

  c += 2;
  while( *c )
  {
    n <<= 4; 
    switch( *c )
    {
      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0':
      n += *c - '0';
      break;

      case 'a': case 'b': case 'c': case 'd': case 'e': case 
      'f':
      n += *c - 'a' + 10;
      break;

      case 'A': case 'B': case 
      'C': case 'D': case 'E': 
      case 'F':
      n += *c - 'A' 
      + 10;
      break;
    }    
    c++;
  }
  return n;
}

static int newline(void*data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  arg->line++;
  arg->pos = 1;
  return 1;
}
char *minimal_get_text (yyscan_t yyscanner );
/*
static int doc_newline(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  arg->line++;
  arg->pos = 1;
  strcat(arg->doc_str, "\n");
  return 1;
}
 
static m_str doc_append(void* data, m_str str)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  sprintf(arg->doc_str, "%s%s", arg->doc_str, str);
  return arg->doc_str;
}

static m_str doc_set(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  return arg->doc_str;
}

static int doc_init(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  vector_append(arg->doc, (vtype)"");
  //memset(arg->doc_str, 0, sizeof(arg->doc_str));
  return 1;
}

//static int doc_adjust(void* data, char* str);
*/
static int adjust(void* data);
%}


%option noyywrap
%option noyylineno
%option bison-bridge
%option reentrant
%option header-file="eval/lexer.h"
%option prefix="minimal_"
%option noinput
%option nounput

%x comment
%x cdoc
%x COMMENT
%x CDOC
%% 

"//!"                               { BEGIN(cdoc); }
<cdoc>\n                            { newline(yyscanner); BEGIN(INITIAL); }
<cdoc>.*  		                      { adjust(yyscanner); yylval->sval = alloc_str(yytext); return DOC;}


"/*!"                               { BEGIN(CDOC); }
<CDOC>"!*/"                         { adjust(yyscanner); BEGIN(INITIAL); }
<CDOC>"\n"                          { newline(yyscanner);  strcat(yylval->sval, "\n\n"); }
<CDOC>.             		            { adjust(yyscanner); yylval->sval =  strndup(yytext, strlen(yytext) - 4); return DOC;}

"//"                                { BEGIN(comment); }
<comment>.                          { adjust(yyscanner); continue;}
<comment>"\n"                       { newline(yyscanner); BEGIN(INITIAL); }



"/*"                                { BEGIN(COMMENT); }
<COMMENT>.                          { adjust(yyscanner); continue;}
<COMMENT>\n                         { newline(yyscanner); continue;}
<COMMENT>"*/"                       { BEGIN(INITIAL); }
 

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
"<<=>"                               { adjust(yyscanner); return RSL;}
">>=>"                               { adjust(yyscanner); return RSR;}
"&=>"                               { adjust(yyscanner); return RSAND;}
"|=>"                               { adjust(yyscanner); return RSOR;}
"^=>"                               { adjust(yyscanner); return RSXOR;}
"&&=>"                               { adjust(yyscanner); return RAND;}
"||=>"                               { adjust(yyscanner); return ROR;}
"==>"                               { adjust(yyscanner); return REQ;}
"!=>"                               { adjust(yyscanner); return RNEQ;}
"<=>"                 					      { adjust(yyscanner); return RLT; }
">=>"                     					  { adjust(yyscanner); return RGT; }
"<==>"                    					  { adjust(yyscanner); return RLE; }
">==>"                    					  { adjust(yyscanner); return RGE; }
"++"                                { adjust(yyscanner); return PLUSPLUS;}
"--"                                { adjust(yyscanner); return MINUSMINUS;}
"function"                              { adjust(yyscanner); return FUNCTION;}
"variadic"                    			{ adjust(yyscanner); return VARARG; }
"typedef"                              { adjust(yyscanner); return FUNC_PTR;}
"protected"                         { adjust(yyscanner); return PROTECTED;}
"<<<"                               { adjust(yyscanner); return L_HACK;}
">>>"                               { adjust(yyscanner); return R_HACK;}
"#("                               { adjust(yyscanner); return SHARPPAREN;}
"%("                               { adjust(yyscanner); return PERCENTPAREN;}
"@("                               { adjust(yyscanner); return ATPAREN;}
"template"                               { adjust(yyscanner); return TEMPLATE;}

"+"                    					    { adjust(yyscanner); return PLUS; }
"-"                    					    { adjust(yyscanner); return MINUS; }
"/"                    					    { adjust(yyscanner); return DIVIDE; }
"%"                    					    { adjust(yyscanner); return PERCENT; }
"~"                    					    { adjust(yyscanner); return TILDA; }
"!"                    					    { adjust(yyscanner); return EXCLAMATION; }
"*"                    					    { adjust(yyscanner); return TIMES; }
"new"                    					  { adjust(yyscanner); return NEW; }
"typeof"                    				{ adjust(yyscanner); return TYPEOF; }
"sizeof"                    				{ adjust(yyscanner); return SIZEOF; }
"spork"                    				  { adjust(yyscanner); return SPORK; }
"union"                    				  { adjust(yyscanner); return UNION; }

"=="                    					  { adjust(yyscanner); return EQ; }
"!="                    					  { adjust(yyscanner); return NEQ; }
"<"                 					      { adjust(yyscanner); return LT; }
"<{"                 					      { adjust(yyscanner); return LTB; }
">"                     					  { adjust(yyscanner); return GT; }
"}>"                     					  { adjust(yyscanner); return GTB; }
"<="                    					  { adjust(yyscanner); return LE; }
">="                    					  { adjust(yyscanner); return GE; }
"&&"                    					  { adjust(yyscanner); return AND; }
"||"                    					  { adjust(yyscanner); return OR; }
"&"                     					  { adjust(yyscanner); return S_AND; }
"|"                     					  { adjust(yyscanner); return S_OR; }
"^"                     					  { adjust(yyscanner); return S_XOR; }
">>"                    					  { adjust(yyscanner); return SHIFT_RIGHT; }
"<<"                    					  { adjust(yyscanner); return SHIFT_LEFT; }
"="                     					  { adjust(yyscanner); return ASSIGN; }
"("                     					  { adjust(yyscanner); return LPAREN; }
")"                     					  { adjust(yyscanner); return RPAREN; }
"["                     					  { adjust(yyscanner); return LBRACK; }
"]"                     					  { adjust(yyscanner); return RBRACK; }
"{"                     					  { adjust(yyscanner); return LBRACE; }
"}"                     					  { adjust(yyscanner); return RBRACE; }
"?"                     					  { adjust(yyscanner); return QUESTION; }
":"                     					  { adjust(yyscanner); return COLON; }
"::"                     					  { adjust(yyscanner); return COLONCOLON; }
"]=>"                     					  { adjust(yyscanner); return TRIG; }
"]=<"                     					  { adjust(yyscanner); return UNTRIG; }


"class"                 					  { adjust(yyscanner); return CLASS;}
"dtor"                 					    { adjust(yyscanner); return AST_DTOR;}
"operator"                 					    { adjust(yyscanner); return OPERATOR;}
"interface"             					  { adjust(yyscanner); return INTERFACE;}
"extends"               					  { adjust(yyscanner); return EXTENDS;}
"implement"             					  { adjust(yyscanner); return IMPLEMENTS;}
"public"                					  { adjust(yyscanner); return PUBLIC;}
"static "               					  { adjust(yyscanner); return STATIC;}
"private"               					  { adjust(yyscanner); return PRIVATE;}
"pure"               					      { adjust(yyscanner); return ABSTRACT;}
"."                     					  { adjust(yyscanner); return DOT;}

"if"                					      { adjust(yyscanner); return IF;}
"else"                					    { adjust(yyscanner); return ELSE;}
"break"                					    { adjust(yyscanner); return BREAK;}
"continue"                				  { adjust(yyscanner); return CONTINUE;}
"return"                					  { adjust(yyscanner); return RETURN;}
"while"                 					  { adjust(yyscanner); return WHILE;}
"do"                 					      { adjust(yyscanner); return DO;}
"until"                 					  { adjust(yyscanner); return UNTIL;}
"repeat"                 					  { adjust(yyscanner); return LOOP;}
"for"                 					    { adjust(yyscanner); return FOR;}
"goto"                 					    { adjust(yyscanner); return GOTO;}
"switch"                 					  { adjust(yyscanner); return SWITCH;}
"case"                 					    { adjust(yyscanner); return CASE;}
"enum"                 					    { adjust(yyscanner); return ENUM;}


0[xX][0-9a-fA-F]+{IS}?  					  { adjust(yyscanner); yylval->ival = htol(yytext);                 return NUM;        }
0[cC][0-7]+{IS}?        					  { adjust(yyscanner); yylval->ival = atoi(yytext);                 return NUM;        }
[0-9]+{IS}?                         { adjust(yyscanner); yylval->ival = atoi(yytext);                 return NUM;        }
([0-9]+"."[0-9]*)|([0-9]*"."[0-9]+) { adjust(yyscanner); yylval->fval = atof(yytext);                 return FLOAT;      }
[A-Za-z_][A-Za-z0-9_]*  					  { adjust(yyscanner); yylval->sval = alloc_str(yytext);           return ID;         }
\"(\\.|[^\\"])*\"       						{ adjust(yyscanner); yylval->sval = alloc_str(strip_lit(yytext)); return STRING_LIT; }
'(\\.|[^\\'])'          						{ adjust(yyscanner); yylval->sval = alloc_str(strip_lit(yytext)); return CHAR_LIT;   }

. { printf("error: stray in program\n"); }
%%
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "err_msg.h"


static int adjust(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  arg->pos += minimal_get_leng(data);
  return 0;
}
/*
static int doc_adjust(void* data, char* str)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  arg->pos += minimal_get_leng(data);
  m_str c = (m_str)vector_back(arg->doc);
  if(!c)
    vector_append(arg->doc, (vtype)str);
  else
    strcat(c, str);
    printf("doc adjust\n%s\nend of doc_adjust\n", (m_str)vector_front(arg->doc));
  return 0;
}
*/
int minimal_error(MyArg* arg, char* s)
{
  m_uint i;
  m_str line = NULL;
  char sign[256];
  int n = 1;
  size_t len = 0;
  memset(sign, 0, 256);
  err_msg(PARSE_, arg->line, "in file '%s'", arg->filename);
  err_msg(PARSE_, arg->line, "%s at pos %i\n", s, arg->pos);

  fseek(arg->file, 0, SEEK_SET);
  while (getline(&line, &len, arg->file) != -1)
  {
    if(n == arg->line)
    {
      fprintf(stderr, "%s\n", line);
      break;
    }
    n++;
  }
  free(line);
  for(i = 0; i < arg->pos - 1; i++)
    strcat(sign, " ");
  strcat(sign, "^\n");
  fprintf(stderr, "%s", sign);
  return 0;
}


Map scan_map = NULL;

//Ast parse(const m_str filename)
Ast parse(m_str filename)
{
  Ast    ast  = NULL;
  FILE*  file = NULL;
  MyArg arg;
  minimal_lex_init(&arg.scanner);
  minimal_set_extra(&arg, arg.scanner);
//  minimal_lex_init_extra(&arg, arg.scanner);
  map_set(scan_map, (vtype)arg.scanner, (vtype)&arg);
  map_set(scan_map, (vtype)minimal_get_extra(arg.scanner), (vtype)&arg);
  arg.line = 1; arg.pos  = 0; arg.doc = new_Vector();
  char c[1025];
  memset(c, 0, 1025);
  strncat(c, filename, 1024);

  if(!(file = fopen(c, "r")))
  {
    err_msg(PARSE_, 0, "error while opening file '%s'.", filename);
    return NULL;
  }
//  arg.fd = fileno(file);
  arg.file = file;
  arg.filename = filename;
  minimal_set_in(file, arg.scanner);
  if(minimal_parse(&arg))
  {
    free_Ast(ast);
    free_Vector(arg.doc);
    minimal_lex_destroy(arg.scanner);
    return NULL;
  }
  filename = c;
  ast = arg.ast;
  if(vector_front(arg.doc))
  {
    ast->doc = malloc(sizeof(char));
    memset(ast->doc, 0, 1);
  }
  while(vector_front(arg.doc))
  {
    strcat(ast->doc, (m_str)vector_front(arg.doc));
    vector_remove(arg.doc, 0);
  }
  free_Vector(arg.doc);
  fclose(file);
  minimal_lex_destroy(arg.scanner);
  return ast;
}

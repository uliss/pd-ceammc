%option noyywrap
%option prefix="dict_parser_"
%option outfile="lex.dict_parser.c"

%{

#include "dict_parser_impl.h"
#include "dict_parser.tab.h"

#define YY_DECL int yylex(t_dict* dict)
//#define MSG(s) printf("%s ", s)
#define MSG(s)
%}

space      [ ]+
qstring    \"([^\"]|\\.)*\"
word       [^:"\[\] ]+

%s KEYVAL

%%

<INITIAL>\[      { BEGIN(KEYVAL);  return TOK_BEGIN; }
<KEYVAL>\]       { BEGIN(INITIAL); return TOK_END; }

:                {  return TOK_ASSOC; }
{word}           {  dict_lexer_push(dict, yytext); return TOK_WORD; }
{qstring}        {  dict_lexer_push(dict, yytext); return TOK_QSTR; }

{space}          {}
%%

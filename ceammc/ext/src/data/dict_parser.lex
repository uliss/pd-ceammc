%option noyywrap
%option prefix="dict_parser_"
%option outfile="lex.dict_parser.c"

%{

#include "dict_parser_impl.h"
#include "dict_parser.tab.h"
#include "m_pd.h"

#define YY_DECL int yylex(t_dict* dict)
//#define MSG(s) printf("%s ", s)
#define MSG(s)
%}

space      [ ]+
qstring    \"([^\"]|\\.)*\"
word       [^:"\[\] ]+

%s KEYVAL

%%

\[               { return TOK_PAIR_BEGIN; }
\]               { return TOK_PAIR_END; }
\(               { return TOK_DICT_BEGIN; }
\)               { return TOK_DICT_END; }

:                { return TOK_ASSOC; }
{word}           { dict_parser_lval.txt = yytext; return TOK_WORD; }
{qstring}        { dict_parser_lval.txt = yytext; return TOK_QSTR; }

{space}          {}
%%

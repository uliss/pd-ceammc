%{
#include <string.h>
#include <stdio.h>

#include "mlist_parser_impl.h"
#define YY_DECL int yylex(mlist_ast *tree)

#include "m_pd.h"
#include "lex.mlist_parser.h"

#include "mlist_parser.tab.h"

void yyerror(mlist_ast * tree, const char* s);
int yylex(mlist_ast* tree);

int mlist_parse_string(mlist_ast* tree, const char* s);

%}

%define api.prefix {mlist_parser_}
%define api.pure false
%parse-param {mlist_ast *tree}
%lex-param   {mlist_ast *tree}

%union {
    const char* txt;
}

%token <txt> TOK_OPEN_PAR TOK_CLOSE_PAR TOK_SPACE TOK_ERROR
%token <txt> TOK_ATOM

%right TOK_SPACE
%left TOK_OPEN_PAR

%start input

%%

input:  mlist

sexpr:    TOK_ATOM
        | TOK_ERROR { yyerror(tree, $1); YYERROR; }
        | mlist
        ;

expr:     sexpr
        | sexpr TOK_SPACE expr
        | sexpr expr
        ;

mlist:    TOK_OPEN_PAR TOK_CLOSE_PAR
        | TOK_OPEN_PAR expr TOK_CLOSE_PAR
        ;

%%

void yyerror(mlist_ast* tree, const char* s)
{
    pd_error(0, "[mlist] parse error: %s", s);
}

int mlist_parse_string(mlist_ast* tree, const char* s)
{
    if(strlen(s) == 0)
        return 0;

    YY_BUFFER_STATE b;
    b = mlist_parser__scan_string(s);

    int ok = yyparse(tree);

    mlist_parser__delete_buffer(b);
    return ok;
}


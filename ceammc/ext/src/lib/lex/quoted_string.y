%require "3.5"
%skeleton "lalr1.cc"
%output "quoted_string.parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc}
%define api.parser.class {QuotedAtomListParser}

%define parse.error verbose
%define parse.assert
%debug
%define api.value.type {t_interval}

%parse-param { ceammc::QuotedAtomListLexer& lexer }

%code requires {
    # include <stddef.h>
    # include <stdio.h>

    namespace ceammc {
        class QuotedAtomListLexer;


        struct t_interval {
            int start {0};
            int end {0};
            bool quoted {false};
            bool quoted_property {false};

            int length() const { return end - start + 1; }
        };

    }
}

%code {
    # undef yylex
    # define yylex lexer.lex

    # include "quoted_atomlist_lexer.h"
}

%token  DOUBLE_QUOTE
%token  DOUBLE_QUOTE_BEGIN
%token  DOUBLE_QUOTE_END
%token  QUOTED_ATOM
%token  QUOTED_PROPERTY
%token  SIMPLE_ATOM
%token  STRING_END 0 "end of string"

%left SIMPLE_ATOM

%start EXPR

%%

SIMPLE_ATOM_LIST
    : SIMPLE_ATOM                  { $$.start = lexer.idx(); $$.end = lexer.idx(); }
    | SIMPLE_ATOM_LIST SIMPLE_ATOM { $$.start = $1.start; $$.end = lexer.idx(); }
    ;

DQB
    : DOUBLE_QUOTE_BEGIN { $$.start = lexer.idx(); }
    | DOUBLE_QUOTE       { $$.start = lexer.idx(); $$.end = lexer.idx(); }
    ;

DQE
    : DOUBLE_QUOTE_END   { $$.end = lexer.idx(); }
    | DOUBLE_QUOTE       { $$.start = lexer.idx(); $$.end = lexer.idx(); }
    ;

DOUBLE_QUOTED_STRING
    : DQB DQE                  { $$.start = $1.start; $$.end = $2.end; }
    | DQB SIMPLE_ATOM_LIST DQE { $$.start = $1.start; $$.end = $3.end; }
    ;

ATOMLIST
    : QUOTED_ATOM          { $$.start = lexer.idx(); $$.end = lexer.idx(); $$.quoted = true; }
    | QUOTED_PROPERTY      { $$.start = lexer.idx(); $$.end = lexer.idx(); $$.quoted = true; $$.quoted_property = true; }
    | DOUBLE_QUOTED_STRING { $$ = $1; $$.quoted = 1; }
    | SIMPLE_ATOM_LIST     { $$ = $1; }
    ;

EXPR
    : %empty
    | EXPR ATOMLIST { lexer.pushRange($2); }
    ;

%%

void ceammc::QuotedAtomListParser::error(const std::string& err_message)
{
    std::cerr << err_message << std::endl;
}


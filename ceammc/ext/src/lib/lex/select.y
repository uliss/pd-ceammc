%require "3.5"
%skeleton "lalr1.cc"
%output "select.parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::select}
%define api.parser.class {SelectParser}
%define api.value.type variant
%define api.token.constructor

%define parse.error verbose
%define parse.assert
%debug
//%define api.value.type {t_interval}

%parse-param { ceammc::select::SelectLexer& lexer }

%code requires {
    # include <stddef.h>
    # include <stdio.h>

    # include "ceammc_atom.h"

    namespace ceammc {
    namespace select {
        class SelectLexer;
    }
    }
}

%code {
    # undef yylex
    # define yylex lexer.lex

    # include "select.lexer.h"
}

%token                SPACE
%token                OR
%token                RANGE
%token                EPSILON
%token  <std::string> FLOAT
%token  <std::string> SYMBOL
%token  <int>         OPEN_BRACKET
%token  <int>         CLOSE_BRACKET
%token  <int>         OPEN_PAR
%token  <int>         CLOSE_PAR
%token                OP_LESS
%token                OP_LESS_EQ
%token                OP_GREATER
%token                OP_GREATER_EQ

%type                 ATOM
%type                 ATOM0
%type   <int>         OPEN_B
%type   <int>         CLOSE_B

%start EXPRLIST

%%

OPEN_B
    : OPEN_BRACKET
    | OPEN_PAR
    ;

CLOSE_B
    : CLOSE_BRACKET
    | CLOSE_PAR
    ;

ATOM0
    : FLOAT  { lexer.pushLexerAtom({$1, true}); }
    | SYMBOL { lexer.pushLexerAtom({$1, false}); }
    ;

ATOM
    : ATOM0
    | ATOM ATOM0
    ;

SET
    : ATOM   { lexer.popLexerAtoms(); }
    | SET OR ATOM  { lexer.popLexerAtoms(); lexer.mergeMatch(); }
    ;

COMPARE
    : OP_LESS FLOAT {
        const auto a = std::strtod($2.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_LESS));
      }
    | OP_LESS_EQ FLOAT {
        const auto a = std::strtod($2.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_LESS_EQ));
      }
    | OP_GREATER_EQ FLOAT {
        const auto a = std::strtod($2.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_GREATER_EQ));
      }
    | OP_GREATER FLOAT {
        const auto a = std::strtod($2.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_GREATER));
      }
    ;

EXPR
    : FLOAT RANGE FLOAT {
        const auto a = std::strtod($1.c_str(), nullptr);
        const auto b = std::strtod($3.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CC));
      }
    | FLOAT EPSILON FLOAT {
        const auto a = std::strtod($1.c_str(), nullptr);
        const auto b = std::strtod($3.c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::epsilon(a, b));
      }
    | OPEN_B FLOAT RANGE FLOAT CLOSE_B {
        const auto a = std::strtod($2.c_str(), nullptr);
        const auto b = std::strtod($4.c_str(), nullptr);
        if($1 == '[' && $5 == ']')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CC));
        else if($1 == '[' && $5 == ')')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CO));
        else if($1 == '(' && $5 == ']')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_OC));
        else
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_OO));
      }
    | COMPARE
    | SET
    ;

EXPRLIST
    : %empty
    | EXPR
    | EXPRLIST SPACE EXPR
    ;

%%

void ceammc::select::SelectParser::error(const std::string& err_message)
{
    lexer.setErrorMsg(err_message);
}


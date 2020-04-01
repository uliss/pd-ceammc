%require "3.5"
%output "quoted_string.parser.c"

%defines
%define api.pure full
%define api.push-pull push

%define api.token.prefix {TOK_}
%define parse.error verbose
%define parse.assert
%debug
%define api.prefix {ceammc_quoted_string_}
%define api.value.type {t_interval}

%parse-param { t_param* p }

%code requires {
    # include <stddef.h>
    # include <stdio.h>

    typedef struct interval {
        int start;
        int end;
        int compressed;
    } t_interval;

    typedef struct param {
        size_t idx;
        t_interval** isp; // interval stack pointer
        size_t n;
        const size_t max_n;
    } t_param;

    void push_range(t_param* p, t_interval rng);
    void yyerror(t_param* param, const char *s);
}

%token  DOUBLE_QUOTE
%token  DOUBLE_QUOTE_BEGIN
%token  DOUBLE_QUOTE_END
%token  QUOTED_ATOM
%token  SIMPLE_ATOM
%token  STRING_END 0 "end of string"

%left SIMPLE_ATOM

%start EXPR

%%

SIMPLE_ATOM_LIST
    : SIMPLE_ATOM                  { $$.start = p->idx; $$.end = p->idx; }
    | SIMPLE_ATOM_LIST SIMPLE_ATOM { $$.start = $1.start; $$.end = p->idx; }
    ;

DQB
    : DOUBLE_QUOTE_BEGIN { $$.start = p->idx; }
    | DOUBLE_QUOTE       { $$.start = p->idx; $$.end = p->idx; }
    ;

DQE
    : DOUBLE_QUOTE_END   { $$.end = p->idx; }
    | DOUBLE_QUOTE       { $$.start = p->idx; $$.end = p->idx; }
    ;

DOUBLE_QUOTED_STRING
    : DQB DQE                  { $$.start = $1.start; $$.end = $2.end; }
    | DQB SIMPLE_ATOM_LIST DQE { $$.start = $1.start; $$.end = $3.end; }
    ;

ATOMLIST
    : QUOTED_ATOM          { $$.start = p->idx; $$.end = p->idx; $$.compressed = 1; }
    | DOUBLE_QUOTED_STRING { $$ = $1; $$.compressed = 1; }
    | SIMPLE_ATOM_LIST     { $$ = $1; }
    ;

EXPR
    : ATOMLIST      { push_range(p, $1); }
    | EXPR ATOMLIST { push_range(p, $2); }
    ;

%%

void yyerror(t_param* p, const char *s)
{
    fprintf (stderr, "%d: %s\n", (int)p->idx, s);
}

void push_range(t_param* p, t_interval rng)
{
    if (p->n >= (p->max_n)) {
        yyerror(p, "to many levels");
        return;
    }

    p->n++;
    (*(*(p->isp))) = rng;
    ++(*(p->isp));
}


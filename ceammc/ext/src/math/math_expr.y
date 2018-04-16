%{
#include <math.h>   /* For math functions: cos(), sin(), etc. */
#include <string.h>
#include <stdio.h>

#include "m_pd.h"
#include "math_expr_calc.h" /* Contains definition of 'symrec'        */
#include "lex.math_expr.h"
#include "math_expr_ast.h"

static double fn_plus(double d0, double d1) { return d0 + d1; }
static double fn_minus(double d0, double d1) { return d0 - d1; }
static double fn_mul(double d0, double d1) { return d0 * d1; }
static double fn_div(double d0, double d1) { return d0 / d1; }
static double fn_mod(double d0, double d1) { return (long)d0 % (long)d1; }
static double fn_pow(double d0, double d1) { return pow(d0, d1); }
static double fn_neg(double d0) { return -d0; }

static void print_error(ast* tree, const char* msg, int c) {
    char buf[100];

    switch(c) {
    case ERR_UNKNOWN_FUNC:
        snprintf(buf, 100, "unknown function \"%s\"", msg);
        break;
    default:
        sprintf(buf, "???");
        break;
    }

    math_expr_error(tree, buf);
}

%}

%define api.prefix {math_expr_}
%define api.pure false
%parse-param {ast *ast}

%union {
  double val;   /* for returning numbers                  */
  Node* node;
}

%token <val>  NUM     /* Simple double precision number */
%token <val>  REF
%token <val>  UFUNC BFUNC ERROR

%type  <node>  exp input      /* For nonterminal symbols        */

%right '='
%left  '-' '+'
%left  '*' '/' '%'
%left  NEG     /* unary minus    */
%right '^'     /* exponentiation */

/* Grammar follows */

%%

input : exp { node_add_cont(ast_root(ast), $1); }
;

exp : NUM                 { $$ = node_create_value($1);                               }
      | ERROR             { print_error(ast, math_expr_text, $1); YYERROR;            }
      | UFUNC '(' exp ')' { $$ = node_create_ufunc(ufnNameToPtr($1), $3);             }
      | BFUNC '(' exp ',' exp ')' { $$ = node_create_bfunc(bfnNameToPtr($1), $3, $5); }
      | REF               { $$ = node_create_ref(ast_ref(ast, $1));                   }
      | exp '+' exp       { $$ = node_create_bfunc(fn_plus, $1, $3);                  }
      | exp '-' exp       { $$ = node_create_bfunc(fn_minus, $1, $3);                 }
      | exp '*' exp       { $$ = node_create_bfunc(fn_mul, $1, $3);                   }
      | exp '/' exp       { $$ = node_create_bfunc(fn_div, $1, $3);                   }
      | exp '%' exp       { $$ = node_create_bfunc(fn_mod, $1, $3);                   }
      | '-' exp %prec NEG { $$ = node_create_ufunc(fn_neg, $2);                       }
      | exp '^' exp       { $$ = node_create_bfunc(fn_pow, $1, $3);                   }
      | '(' exp ')'       { $$ = node_create_cont($2);                                }
;

/* End of Grammar */

%%

void math_expr_error(ast* ast, const char* s)
{
    ast_invalidate(ast);
    pd_error(0, "[math.expr] parse error: %s", s);
}

int math_expr_parse_ast(ast* ast, const char* s)
{
    post("parse: %s", s);

    YY_BUFFER_STATE b;
    b = math_expr__scan_string(s);

    int ok = yyparse(ast);

    math_expr__delete_buffer(b);
    return ok;
}


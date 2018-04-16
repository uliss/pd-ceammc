/* multi-function calculator */
/* Time-stamp: <2000-10-23 14:52:06 ronaldo> mfcalc.y */

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

typedef struct Node Node;
%}

%define api.prefix {math_expr_}
%define api.pure false
%parse-param {ast *ast}

%union {
  double val;   /* for returning numbers                  */
  symrec *tptr; /* for returning symbol-table of 'symrec' */
  Node* node;
}

%token <val>  NUM        /* Simple double precision number */
%token <tptr> VAR        /* Variable and Function          */
%token <val>  REF
%token <val> UFUNC

%type  <node>  exp       /* For nonterminal symbols        */
%type  <node>  input


%right '='
%left  '-' '+'
%left  '*' '/' '%'
%left  NEG     /* unary minus    */
%right '^'     /* exponentiation */

/* Grammar follows */

%%

input : exp { node_add_cont(ast_root(ast), $1); }
;

exp : NUM                   { $$ = node_create_value($1);           }
//      | VAR               { $$ = $1->value.var;              }
//      | VAR '=' exp       { $$ = $3; $1->value.var = $3;     }
//      | FNCT '(' exp ')'  { $$ = (*($1->value.fnctptr))($3); }
      | UFUNC '(' exp ')' { $$ = node_create_ufunc(ufnNameToPtr($1), $3);       }
      | REF               { $$ = node_create_ref(ast_ref(ast, $1));   }
      | exp '+' exp       { $$ = node_create_bfunc(fn_plus, $1, $3);  }
      | exp '-' exp       { $$ = node_create_bfunc(fn_minus, $1, $3); }
      | exp '*' exp       { $$ = node_create_bfunc(fn_mul, $1, $3);   }
      | exp '/' exp       { $$ = node_create_bfunc(fn_div, $1, $3);   }
      | exp '%' exp       { $$ = node_create_bfunc(fn_mod, $1, $3);   }
      | '-' exp %prec NEG { $$ = node_create_ufunc(fn_neg, $2);       }
      | exp '^' exp       { $$ = node_create_bfunc(fn_pow, $1, $3);   }
      | '(' exp ')'       { $$ = node_create_cont($2);                }
;

/* End of Grammar */

%%

struct init {
  char fname[8];
  double (*fnct)(double);
};

struct init arith_fncts[] = {
    "sin" , &sin,
    "cos" , &cos,
    "atan", &atan,
    "ln"  , &log,
    "exp" , &exp,
    "sqrt", &sqrt,
    0     , 0
};

/* The symbol table: a chain of 'struct symrec' */
symrec * sym_table = (symrec *) 0;

symrec * math_expr_putsym(const char * sym_name, int sym_type) {
    symrec * ptr;
    ptr = (symrec *) calloc (1, sizeof (symrec));
    strncpy (ptr->name, sym_name, 7);
    ptr->type = sym_type;
    ptr->value.var = 0; /* set value to 0 even if fctn */
    ptr->next = (struct symrec *) sym_table;
    sym_table = ptr;
    return ptr;
}

symrec * math_expr_getsym(const char * sym_name) {
    symrec * ptr;
    for (ptr=sym_table; ptr != (symrec *) 0; ptr = (symrec *) ptr->next)
    if (strcmp (ptr->name, sym_name) == 0)
        return ptr;

    return (symrec *) 0;
}

symrec* math_expr_putvar(const char* var_name, double v)
{
    symrec* s = math_expr_putsym(var_name, VAR);
    s->value.var = v;
    return s;
}

symrec* math_expr_setvar(const char* var_name, double v)
{
    symrec* s = math_expr_getsym(var_name);
    if(!s)
        return (symrec *) 0;

    s->value.var = v;
    return s;
}

/* puts arithmetic functions in table */
void math_expr_init_table() {
    symrec * ptr;
    for (int i = 0; arith_fncts[i].fname[0] != 0; i++) {
        ptr = math_expr_putsym(arith_fncts[i].fname, VAR);
        ptr->value.fnctptr = arith_fncts[i].fnct;
    }
}

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


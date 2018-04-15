/* multi-function calculator */
/* Time-stamp: <2000-10-23 14:52:06 ronaldo> mfcalc.y */

%{
#include <math.h>   /* For math functions: cos(), sin(), etc. */
#include <string.h>
#include <stdio.h>

#include "m_pd.h"
#include "math_expr_calc.h" /* Contains definition of 'symrec'        */
#include "lex.math_expr.h"
%}

%define api.prefix {math_expr_}
%define api.pure false

%union {
  double val;   /* for returning numbers                  */
  symrec *tptr; /* for returning symbol-table of 'symrec' */
}

%token <val>  NUM        /* Simple double precision number */
%token <tptr> VAR FNCT   /* Variable and Function          */
%type  <val>  exp        /* For nonterminal symbols        */

%right '='
%left  '-' '+'
%left  '*' '/'
%left  NEG     /* unary minus    */
%right '^'     /* exponentiation */

/* Grammar follows */

%%

input : /* empty */
        | exp { post("%.10lf\n", $1); }
;

//line : '\n'
//       | exp '\n'   { post("%.10lf\n", $1); }
//       | error '\n' { yyerrok;              }
//;

exp : NUM                 { $$ = $1;                         }
      | VAR               { $$ = $1->value.var;              }
//      | VAR '=' exp       { $$ = $3; $1->value.var = $3;     }
      | FNCT '(' exp ')'  { $$ = (*($1->value.fnctptr))($3); }
      | exp '+' exp       { $$ = $1 + $3;                    }
      | exp '-' exp       { $$ = $1 - $3;                    }
      | exp '*' exp       { $$ = $1 * $3;                    }
      | exp '/' exp       { $$ = $1 / $3;                    }
      | '-' exp %prec NEG { $$ = -$2;                        }
      | exp '^' exp       { $$ = pow ($1, $3);               }
      | '(' exp ')'       { $$ = $2;                         }
;

/* End of Grammar */

%%

struct init {
  char * fname;
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

symrec * math_expr_putsym(char * sym_name, int sym_type) {
    symrec * ptr;
    ptr = (symrec *) malloc (sizeof (symrec));
    ptr->name = (char *) malloc (strlen (sym_name) + 1);
    strcpy (ptr->name, sym_name);
    ptr->type = sym_type;
    ptr->value.var = 0; /* set value to 0 even if fctn */
    ptr->next = (struct symrec *) sym_table;
    sym_table = ptr;
    return ptr;
}

symrec * math_expr_getsym (char * sym_name) {
    symrec * ptr;
    for (ptr=sym_table; ptr != (symrec *) 0; ptr = (symrec *) ptr->next)
    if (strcmp (ptr->name, sym_name) == 0)
        return ptr;
    return (symrec *) 0;
}

/* puts arithmetic functions in table */
void init_table (void) {
    symrec * ptr;
    for (int i = 0; arith_fncts[i].fname != 0; i++) {
        ptr = math_expr_putsym(arith_fncts[i].fname, FNCT);
        ptr->value.fnctptr = arith_fncts[i].fnct;
    }
}

void math_expr_error(const char* s)
{
    post("%s", s);
}

int math_expr_calc(const char* s, double* res)
{
    post("parse: %s", s);

    YY_BUFFER_STATE b;
    b = math_expr__scan_string(s);

    int ok = yyparse();

    math_expr__delete_buffer(b);
    return ok;
}


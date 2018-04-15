/* mfcalc.l */
/* Time-stamp: <2000-10-23 14:51:35 ronaldo> */
%option noyywrap
%option prefix="math_expr_"
%option outfile="lex.math_expr.c"

%{
# include "math_expr_calc.h"
# include "math_expr.tab.h"
%}

%%

[0-9]*\.?[0-9]+ {
  sscanf (math_expr_text, "%lf", &math_expr_lval.val);
  return NUM;
}

[a-zA-Z$][a-zA-Z_0-9]* {
  symrec *s;
  s = math_expr_getsym(math_expr_text);
  if (s == 0) {
    s = math_expr_putsym(math_expr_text, VAR);
  }
  math_expr_lval.tptr = s;
  return s->type;
}

[ \t]*

.|\n {
  return *math_expr_text;
}

%%

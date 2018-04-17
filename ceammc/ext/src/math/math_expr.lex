/* mfcalc.l */
/* Time-stamp: <2000-10-23 14:51:35 ronaldo> */
%option noyywrap
%option prefix="math_expr_"
%option outfile="lex.math_expr.c"

%{
#include <math.h>

#include "math_expr_calc.h"
#include "math_expr.tab.h"
#include "math_expr_ast.h"

typedef struct Node Node;
%}

%%

0b[0-1]+ { // binary 0x1101101
    math_expr_lval.val = strtol(math_expr_text + 2, NULL, 2);
    return NUM;
}

0x[0-9A-Fa-f]+ { // hex 0xBEEFA24
    math_expr_lval.val = strtol(math_expr_text, NULL, 16);
    return NUM;
}

[0-9]*\.?[0-9]+ { // double
    sscanf (math_expr_text, "%lf", &math_expr_lval.val);
    return NUM;
}

$f[0-9]* { // refs
    char n = math_expr_text[2];

    if(n == '\0')
        math_expr_lval.val = 0;
    else
        math_expr_lval.val = n - '0';

    return REF;
}

"==" { return EQ; }
"!=" { return NOT_EQ; }
"<=" { return LE; }
"<"  { return LT; }
">=" { return GE; }
">"  { return GT; }

$pi { // PI
    math_expr_lval.val = M_PI;
    return NUM;
}

$e { // E
    math_expr_lval.val = M_E;
    return NUM;
}

[a-z][a-z_0-9]* {
    if(strcmp(math_expr_text, "sin") == 0) {
        math_expr_lval.val = UFN_SIN;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "cos") == 0) {
        math_expr_lval.val = UFN_COS;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "tan") == 0) {
        math_expr_lval.val = UFN_TAN;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "sqrt") == 0) {
        math_expr_lval.val = UFN_SQRT;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "ln") == 0) {
        math_expr_lval.val = UFN_LN;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "log2") == 0) {
        math_expr_lval.val = UFN_LOG2;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "log10") == 0) {
        math_expr_lval.val = UFN_LOG10;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "exp") == 0) {
        math_expr_lval.val = UFN_EXP;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "atan") == 0) {
        math_expr_lval.val = UFN_ATAN;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "max") == 0) {
        math_expr_lval.val = BFN_MAX;
        return BFUNC;
    }
    else if(strcmp(math_expr_text, "min") == 0) {
        math_expr_lval.val = BFN_MIN;
        return BFUNC;
    }
    else if(strcmp(math_expr_text, "abs") == 0) {
        math_expr_lval.val = UFN_ABS;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "sign") == 0) {
        math_expr_lval.val = UFN_SIGN;
        return UFUNC;
    }
    else if(strcmp(math_expr_text, "fact") == 0) {
        math_expr_lval.val = UFN_FACTORIAL;
        return UFUNC;
    }

    math_expr_lval.val = ERR_UNKNOWN_FUNC;
    return ERROR;
}

[ \t]*

.|\n {
    return *math_expr_text;
}

%%

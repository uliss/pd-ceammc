/* mfcalc.l */
%option noyywrap
%option prefix="math_expr_"
%option outfile="lex.math_expr.c"

%{
    # include <math.h>

    # include "math_expr_calc.h"
    # include "math_expr.tab.h"
%}

%%

0b[0-1]+ { // binary 0x1101101
    math_expr_lval.val = strtol(math_expr_text + 2, NULL, 2);
    return T_NUM;
}

0x[0-9A-Fa-f]+ { // hex 0xBEEFA24
    math_expr_lval.val = strtol(math_expr_text, NULL, 16);
    return T_NUM;
}

[0-9]*\.?[0-9]+ { // double
    sscanf (math_expr_text, "%lf", &math_expr_lval.val);
    return T_NUM;
}

$f[0-9]* { // refs
    char n = math_expr_text[2];

    if(n == '\0')
        math_expr_lval.val = 0;
    else
        math_expr_lval.val = n - '0';

    return T_REF;
}

"==" { return T_EQ; }
"~=" { return T_APPROX_EQ; }
"!=" { return T_NOT_EQ; }
"<=" { return T_LE; }
"<"  { return T_LT; }
">=" { return T_GE; }
">"  { return T_GT; }

$pi { // PI
    math_expr_lval.val = M_PI;
    return T_NUM;
}

$e { // E
    math_expr_lval.val = M_E;
    return T_NUM;
}

[a-zA-Z][a-zA-Z0-9]*[\[] {
    math_expr_lval.txt = math_expr_text;
    return T_ARRAY_BEGIN;
}

"]" { return T_ARRAY_END; }

[a-z][a-z_0-9]* {
    if(strcmp(math_expr_text, "sin") == 0) {
        math_expr_lval.val = UFN_SIN;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "cos") == 0) {
        math_expr_lval.val = UFN_COS;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "tan") == 0) {
        math_expr_lval.val = UFN_TAN;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "sqrt") == 0) {
        math_expr_lval.val = UFN_SQRT;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "ln") == 0) {
        math_expr_lval.val = UFN_LN;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "log2") == 0) {
        math_expr_lval.val = UFN_LOG2;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "log10") == 0) {
        math_expr_lval.val = UFN_LOG10;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "exp") == 0) {
        math_expr_lval.val = UFN_EXP;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "atan") == 0) {
        math_expr_lval.val = UFN_ATAN;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "max") == 0) {
        math_expr_lval.val = BFN_MAX;
        return T_BFUNC;
    }
    else if(strcmp(math_expr_text, "min") == 0) {
        math_expr_lval.val = BFN_MIN;
        return T_BFUNC;
    }
    else if(strcmp(math_expr_text, "abs") == 0) {
        math_expr_lval.val = UFN_ABS;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "sign") == 0) {
        math_expr_lval.val = UFN_SIGN;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "fact") == 0) {
        math_expr_lval.val = UFN_FACTORIAL;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "round") == 0) {
        math_expr_lval.val = UFN_ROUND;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "ceil") == 0) {
        math_expr_lval.val = UFN_CEIL;
        return T_UFUNC;
    }
    else if(strcmp(math_expr_text, "floor") == 0) {
        math_expr_lval.val = UFN_FLOOR;
        return T_UFUNC;
    }

    math_expr_lval.val = ERR_UNKNOWN_FUNC;
    return T_ERROR;
}

[ \t]*

.|\n {
    return *math_expr_text;
}

%%

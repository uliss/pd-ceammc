%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int math_expr_lex(MathExprResult* res)

typedef struct MathExprResult MathExprResult;

    typedef struct MathExprResult {
        double v;
        double fval;
        double ival;
        int error;
    } MathExprResult;

#include "math_expr.tab.h"

%}

%%

[ \t]	        ; // ignore all whitespace
[0-9]+\.[0-9]+ 	{res->fval = atof(yytext); return T_FLOAT;}
[0-9]+		    {res->ival = atoi(yytext); return T_INT;}
"+"		        {return T_PLUS;}
"-"		        {return T_MINUS;}
"*"		        {return T_MULTIPLY;}
"/"		        {return T_DIVIDE;}
"^"		        {return T_POWER;}
"("		        {return T_LEFT;}
")"		        {return T_RIGHT;}

%%

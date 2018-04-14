%{

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "m_pd.h"

#include "math_expr.lex.yy.h"


typedef struct MathExprResult {
    double v;
    double fval;
    double ival;
    int error;
} MathExprResult;

//extern int math_expr_parse(MathExprResult* res);

void math_expr_error(MathExprResult* res, const char* s);
double math_expr_yyparse(const char* s);

%}

%union {
    MathExprResult* res;
}

// %define api.token.prefix {ME_}
%define api.pure full
%define api.prefix {math_expr_}
%parse-param {MathExprResult *result}

%token<res->ival> T_INT
%token<res->fval> T_FLOAT

%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_POWER T_LEFT T_RIGHT
%token T_NEWLINE
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_POWER

%type <res>  program
%type <res->ival> expression
%type <res->fval> mixed_expression
%type <res->fval> calculation

%start program

%%

program: calculation  { post("\tResult: %f\n", $1); result->v = $1; }
;

calculation: mixed_expression { $$ = $1; }
    | expression              { $$ = $1; }
;

mixed_expression: T_FLOAT                            { $$ = $1; }
      | mixed_expression T_PLUS mixed_expression     { $$ = $1 + $3; }
      | mixed_expression T_MINUS mixed_expression    { $$ = $1 - $3; }
      | mixed_expression T_MULTIPLY mixed_expression { $$ = $1 * $3; }
      | mixed_expression T_DIVIDE mixed_expression   { $$ = $1 / $3; }
      | mixed_expression T_POWER mixed_expression    { $$ = pow($1, $3); }
      | T_LEFT mixed_expression T_RIGHT              { $$ = $2; }
      | expression T_PLUS mixed_expression           { $$ = $1 + $3; }
      | expression T_MINUS mixed_expression          { $$ = $1 - $3; }
      | expression T_MULTIPLY mixed_expression       { $$ = $1 * $3; }
      | expression T_DIVIDE mixed_expression         { $$ = $1 / $3; }
      | expression T_POWER mixed_expression          { $$ = pow($1, $3); }
      | mixed_expression T_PLUS expression           { $$ = $1 + $3; }
      | mixed_expression T_MINUS expression          { $$ = $1 - $3; }
      | mixed_expression T_MULTIPLY expression       { $$ = $1 * $3; }
      | mixed_expression T_DIVIDE expression         { $$ = $1 / $3; }
      | mixed_expression T_POWER expression          { $$ = pow($1, $3); }
      | expression T_DIVIDE expression               { $$ = $1 / (float)$3; }
      | expression T_POWER expression                { $$ = pow($1, $3); }
;

expression: T_INT				            { $$ = $1; }
      | expression T_PLUS expression        { $$ = $1 + $3; }
      | expression T_MINUS expression       { $$ = $1 - $3; }
      | expression T_MULTIPLY expression    { $$ = $1 * $3; }
      | T_LEFT expression T_RIGHT           { $$ = $2; }
;

%%

void math_expr_error(MathExprResult* res, const char* s) {
    post("Parse error: %s", s);
    res->error = 1;
}

double math_expr_yyparse(const char* s)
{
    post("math_expr_yyparse: %s", s);

    YY_BUFFER_STATE buffer = math_expr_scan_string(s);

    MathExprResult res;
    res.v = 0;
    res.error = 0;
    math_expr_parse(&res);

    math_expr_delete_buffer(buffer);
    return res.v;
}

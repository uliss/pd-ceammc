/* math_expr.lex */
%option freespace
%option namespace=ceammc::math
%option lexer=MathExprLexer
%option header-file=math_expr.lexer.h
%option outfile=math_expr.lexer.cpp
%option bison-cc-namespace=ceammc::math
%option bison-cc-parser=MathExprParser
%option bison-cc
%option bison-complete
%option bison-locations
%option noyywrap
%option reentrant
%option fast
/*%option debug*/

%top {
    # include <cstdlib>

    # include "math_expr.location.hpp"
    # include "math_expr.parser.hpp"

    using token = ceammc::math::MathExprParser::token;
%}

space           [ \t]+
bin             0b[0-1]+
hex             0x[0-9A-Fa-f]+
double          [0-9]*\.?[0-9]+
ref             \$f[0-9]*
square_open     \[
square_close    \]
par_open        \(
par_close       \)
comma           ,
symbol          [a-zA-Z][^)(\]\[]+

%%

{space}  { }

{bin}    { // binary 0x1101101
            return MathExprParser::make_NUM(strtol(text() + 2, NULL, 2), location());
         }

{hex}    { // hex 0xBEEFA24
            return MathExprParser::make_NUM(strtol(text() + 2, NULL, 16), location());
         }

{double} { // double
            return MathExprParser::make_NUM(strtod(text(), NULL), location());
         }

{ref}    { // refs
            auto n = text()[2];
            return MathExprParser::make_REF(n ? (n - '0') : 0, location());
         }

"$pi"    { return MathExprParser::make_NUM(M_PI, location()); }
"$e"     { return MathExprParser::make_NUM(M_E, location()); }

{square_open}   { return MathExprParser::make_SQR_OPEN(location()); }
{square_close}  { return MathExprParser::make_SQR_CLOSE(location());}
{par_open}      { return MathExprParser::make_PAR_OPEN(location());}
{par_close}     { return MathExprParser::make_PAR_CLOSE(location()); }
{comma}         { return MathExprParser::make_COMMA(location()); }
"+"             { return MathExprParser::make_PLUS(location()); }
"-"             { return MathExprParser::make_MINUS(location()); }
"*"             { return MathExprParser::make_MUL(location()); }
"/"             { return MathExprParser::make_DIV(location()); }
"%"             { return MathExprParser::make_MOD(location()); }
"^"             { return MathExprParser::make_EXP(location()); }
"=="            { return MathExprParser::make_T_EQ(location()); }
"~="            { return MathExprParser::make_T_APPROX_EQ(location()); }
"!="            { return MathExprParser::make_T_NOT_EQ(location()); }
"<="            { return MathExprParser::make_T_LE(location()); }
"<"             { return MathExprParser::make_T_LT(location()); }
">="            { return MathExprParser::make_T_GE(location()); }
">"             { return MathExprParser::make_T_GT(location()); }

{symbol}/({par_open}|{square_open}) {
                  return MathExprParser::make_SYMBOL(text(), location()); }

<*> .           { return MathExprParser::make_LEXER_ERROR(location());}

%%

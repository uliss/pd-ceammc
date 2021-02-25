%skeleton "lalr1.cc"
%require "3.5"
%output "math_expr.parser.cpp"
%debug

%defines
%define api.namespace {ceammc::math}
%define api.parser.class {MathExprParser}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose
%locations
%define api.location.file "math_expr.location.hpp"

%code requires{
    # include "math_expr_ast.h"

    namespace ceammc {
    namespace math {
        class MathExprLexer;
    }
    }
}

%code {
    # include <cmath>   /* For math functions: cos(), sin(), etc. */
    # include <cstring>
    # include <cstdio>

    # include "m_pd.h"
    # include "math_expr_ast.h"
    # include "math_expr.lexer.h"

    static double fn_plus(double d0, double d1) { return d0 + d1; }
    static double fn_minus(double d0, double d1) { return d0 - d1; }
    static double fn_mul(double d0, double d1) { return d0 * d1; }
    static double fn_div(double d0, double d1) { return d0 / d1; }
    static double fn_mod(double d0, double d1) { return (long)d0 % (long)d1; }
    static double fn_pow(double d0, double d1) { return pow(d0, d1); }
    static double fn_neg(double d0) { return -d0; }

    static double fn_eq(double d0, double d1) { return d0 == d1; }
    static double fn_ne(double d0, double d1) { return d0 != d1; }
    static double fn_le(double d0, double d1) { return d0 <= d1; }
    static double fn_lt(double d0, double d1) { return d0 < d1; }
    static double fn_ge(double d0, double d1) { return d0 >= d1; }
    static double fn_gt(double d0, double d1) { return d0 > d1; }
    static double fn_approx_eq(double d0, double d1) {
        const double epsilon = 1.0e-08;
        double da0 = fabs(d0);
        double da1 = fabs(d1);

        double x = fabs(d0 - d1);
        if (x <= epsilon)
            return 1;

        double max = (da0 < da1) ? da1 : da0;
        return x <= (epsilon * max);
    }

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

}

%parse-param { ceammc::math::MathExprLexer& lexer  }
%parse-param { ceammc::math::Ast& tree}

%token                  SQR_OPEN SQR_CLOSE PAR_OPEN PAR_CLOSE COMMA PLUS MINUS MUL DIV MOD EXP
%token                  LEXER_ERROR
%token <double>         NUM REF
%token <std::string>    SYMBOL

%type  <Node>           exp input array_at unary_func binary_func group

%left  T_EQ T_NOT_EQ T_APPROX_EQ T_LE T_LT T_GT T_GE
%left  MINUS PLUS
%left  MUL DIV MOD
%right T_NEG     /* unary minus    */
%right EXP     /* exponentiation */

/* Grammar follows */

%%

input : exp { tree.root.add($1); }
;

array_at
    : SYMBOL SQR_OPEN exp SQR_CLOSE
        { $$ = Node::createArrayFunc(gensym($1.c_str()), $3); }

unary_func : SYMBOL PAR_OPEN exp PAR_CLOSE
    {
        UnaryFloatFunc fn;
        if (getUnaryFunction($1, fn))
            $$ = Node::createUnaryFunction(fn, $3);
        else {
            throw std::runtime_error(std::string("unknown unary function: " + $1));
        }
    }

binary_func : SYMBOL PAR_OPEN exp COMMA exp PAR_CLOSE
    {
        BinaryFloatFunc fn;
        if (getBinaryFunction($1, fn))
            $$ = Node::createBinaryFunction(fn, $3, $5);
        else
            throw std::runtime_error(std::string("unknown binary function: " + $1));
    }
group
    : PAR_OPEN exp PAR_CLOSE
        { $$ = Node::createGroup($2); }

exp
    : NUM                 { $$ = Node::createValue($1); }
    | REF                 { $$ = Node::createRef(tree.ref($1)); }
    | exp T_EQ exp        { $$ = Node::createBinaryFunction(fn_eq, $1, $3);        }
    | exp T_APPROX_EQ exp { $$ = Node::createBinaryFunction(fn_approx_eq, $1, $3); }
    | exp T_NOT_EQ exp    { $$ = Node::createBinaryFunction(fn_ne, $1, $3);        }
    | exp T_LT exp        { $$ = Node::createBinaryFunction(fn_lt, $1, $3);        }
    | exp T_LE exp        { $$ = Node::createBinaryFunction(fn_le, $1, $3);        }
    | exp T_GT exp        { $$ = Node::createBinaryFunction(fn_gt, $1, $3);        }
    | exp T_GE exp        { $$ = Node::createBinaryFunction(fn_ge, $1, $3);        }
    | exp PLUS exp        { $$ = Node::createBinaryFunction(fn_plus, $1, $3);      }
    | exp MINUS exp       { $$ = Node::createBinaryFunction(fn_minus, $1, $3);     }
    | exp MUL exp         { $$ = Node::createBinaryFunction(fn_mul, $1, $3);       }
    | exp DIV exp         { $$ = Node::createBinaryFunction(fn_div, $1, $3);       }
    | exp MOD exp         { $$ = Node::createBinaryFunction(fn_mod, $1, $3);       }
    | MINUS exp %prec T_NEG { $$ = Node::createUnaryFunction(fn_neg, $2);            }
    | exp EXP exp         { $$ = Node::createBinaryFunction(fn_pow, $1, $3);       }
    | group
    | array_at
    | unary_func
    | binary_func
    ;

/* End of Grammar */

%%

void ceammc::math::MathExprParser::error(const ceammc::math::location& loc, const std::string& str)
{

    pd_error(0, "[math.expr] parse error: %s", str.c_str());
}


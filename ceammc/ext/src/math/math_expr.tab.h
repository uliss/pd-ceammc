/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_MATH_EXPR_MATH_EXPR_TAB_H_INCLUDED
# define YY_MATH_EXPR_MATH_EXPR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef MATH_EXPR_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define MATH_EXPR_DEBUG 1
#  else
#   define MATH_EXPR_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define MATH_EXPR_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined MATH_EXPR_DEBUG */
#if MATH_EXPR_DEBUG
extern int math_expr_debug;
#endif

/* Token type.  */
#ifndef MATH_EXPR_TOKENTYPE
# define MATH_EXPR_TOKENTYPE
  enum math_expr_tokentype
  {
    NUM = 258,
    REF = 259,
    UFUNC = 260,
    BFUNC = 261,
    NEG = 262
  };
#endif

/* Value type.  */
#if ! defined MATH_EXPR_STYPE && ! defined MATH_EXPR_STYPE_IS_DECLARED

union MATH_EXPR_STYPE
{
#line 26 "math_expr.y" /* yacc.c:1915  */

  double val;   /* for returning numbers                  */
  Node* node;

#line 75 "math_expr.tab.h" /* yacc.c:1915  */
};

typedef union MATH_EXPR_STYPE MATH_EXPR_STYPE;
# define MATH_EXPR_STYPE_IS_TRIVIAL 1
# define MATH_EXPR_STYPE_IS_DECLARED 1
#endif


extern MATH_EXPR_STYPE math_expr_lval;

int math_expr_parse (ast *ast);

#endif /* !YY_MATH_EXPR_MATH_EXPR_TAB_H_INCLUDED  */

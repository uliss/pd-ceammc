/* A Bison parser, made by GNU Bison 3.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    T_NUM = 258,
    T_REF = 259,
    T_EQ = 260,
    T_NOT_EQ = 261,
    T_APPROX_EQ = 262,
    T_ARRAY_BEGIN = 263,
    T_ARRAY_END = 264,
    T_UFUNC = 265,
    T_BFUNC = 266,
    T_ERROR = 267,
    T_LE = 268,
    T_LT = 269,
    T_GT = 270,
    T_GE = 271,
    T_NEG = 272
  };
#endif

/* Value type.  */
#if ! defined MATH_EXPR_STYPE && ! defined MATH_EXPR_STYPE_IS_DECLARED
union MATH_EXPR_STYPE
{
#line 59 "math_expr.y"

  double val;   /* for returning numbers                  */
  const char* txt;
  Node* node;

#line 89 "math_expr.tab.h"

};
typedef union MATH_EXPR_STYPE MATH_EXPR_STYPE;
# define MATH_EXPR_STYPE_IS_TRIVIAL 1
# define MATH_EXPR_STYPE_IS_DECLARED 1
#endif


extern MATH_EXPR_STYPE math_expr_lval;

int math_expr_parse (Ast *tree);

#endif /* !YY_MATH_EXPR_MATH_EXPR_TAB_H_INCLUDED  */

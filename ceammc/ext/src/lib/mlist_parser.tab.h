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

#ifndef YY_MLIST_PARSER_MLIST_PARSER_TAB_H_INCLUDED
# define YY_MLIST_PARSER_MLIST_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef MLIST_PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define MLIST_PARSER_DEBUG 1
#  else
#   define MLIST_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define MLIST_PARSER_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined MLIST_PARSER_DEBUG */
#if MLIST_PARSER_DEBUG
extern int mlist_parser_debug;
#endif

/* Token type.  */
#ifndef MLIST_PARSER_TOKENTYPE
# define MLIST_PARSER_TOKENTYPE
  enum mlist_parser_tokentype
  {
    TOK_OPEN_PAR = 258,
    TOK_CLOSE_PAR = 259,
    TOK_SPACE = 260,
    TOK_ERROR = 261,
    TOK_ATOM = 262
  };
#endif

/* Value type.  */
#if ! defined MLIST_PARSER_STYPE && ! defined MLIST_PARSER_STYPE_IS_DECLARED

union MLIST_PARSER_STYPE
{
#line 25 "mlist_parser.y" /* yacc.c:1915  */

    const char* txt;

#line 74 "mlist_parser.tab.h" /* yacc.c:1915  */
};

typedef union MLIST_PARSER_STYPE MLIST_PARSER_STYPE;
# define MLIST_PARSER_STYPE_IS_TRIVIAL 1
# define MLIST_PARSER_STYPE_IS_DECLARED 1
#endif


extern MLIST_PARSER_STYPE mlist_parser_lval;

int mlist_parser_parse (mlist_ast *tree);

#endif /* !YY_MLIST_PARSER_MLIST_PARSER_TAB_H_INCLUDED  */

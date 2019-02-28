/* A Bison parser, made by GNU Bison 3.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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

#ifndef YY_DICT_PARSER_DICT_PARSER_TAB_H_INCLUDED
# define YY_DICT_PARSER_DICT_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef DICT_PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define DICT_PARSER_DEBUG 1
#  else
#   define DICT_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define DICT_PARSER_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined DICT_PARSER_DEBUG */
#if DICT_PARSER_DEBUG
extern int dict_parser_debug;
#endif
/* "%code requires" blocks.  */
#line 20 "dict_parser.y" /* yacc.c:1919  */

#include "m_pd.h"
enum ValueType {
    VTYPE_LIST = 0,
    VTYPE_DICT = 1
};

typedef struct dict_token {
    void* sym;
    enum ValueType type;
    int npairs;
} t_dict_token;

#line 66 "dict_parser.tab.h" /* yacc.c:1919  */

/* Token type.  */
#ifndef DICT_PARSER_TOKENTYPE
# define DICT_PARSER_TOKENTYPE
  enum dict_parser_tokentype
  {
    TOK_ASSOC = 258,
    TOK_WORD = 259,
    TOK_QSTR = 260,
    TOK_PAIR_BEGIN = 261,
    TOK_PAIR_END = 262,
    TOK_DICT_BEGIN = 263,
    TOK_DICT_END = 264
  };
#endif

/* Value type.  */
#if ! defined DICT_PARSER_STYPE && ! defined DICT_PARSER_STYPE_IS_DECLARED

union DICT_PARSER_STYPE
{
#line 40 "dict_parser.y" /* yacc.c:1919  */

    const char* txt;
    t_dict_token tok;

#line 93 "dict_parser.tab.h" /* yacc.c:1919  */
};

typedef union DICT_PARSER_STYPE DICT_PARSER_STYPE;
# define DICT_PARSER_STYPE_IS_TRIVIAL 1
# define DICT_PARSER_STYPE_IS_DECLARED 1
#endif


extern DICT_PARSER_STYPE dict_parser_lval;

int dict_parser_parse (t_dict *dict);

#endif /* !YY_DICT_PARSER_DICT_PARSER_TAB_H_INCLUDED  */

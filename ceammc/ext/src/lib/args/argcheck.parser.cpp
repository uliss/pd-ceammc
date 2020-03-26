// A Bison parser, made by GNU Bison 3.5.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "argcheck.parser.hpp"


// Unqualified %code blocks.
#line 41 "argcheck.yy"

    # include <memory>
    # include <string>
    # include <utility>

    // header file generated with reflex
    # include "argcheck.lexer.h"

    # include "arg_is_atom.h"
    # include "arg_is_bool.h"
    # include "arg_is_data.h"
    # include "arg_is_float.h"
    # include "arg_is_int.h"
    # include "arg_is_symbol.h"
    # include "arg_group_or.h"
    # include "../ceammc_args.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static void set_repeats(const ceammc::ArgCheckerNodePtr& n, const std::vector<int>& v) {
        switch(v.size()) {
        case 1:
            n->setInfinitePattern();
            n->setMinRepeats(v[0]);
        break;
        case 2:
            n->setMinRepeats(v[0]);
            n->setMaxRepeats(v[1]);
        break;
        default:
        break;
        }
    }

#line 81 "argcheck.parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 7 "argcheck.yy"
namespace ceammc {
#line 154 "argcheck.parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ArgCheckParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  ArgCheckParser::ArgCheckParser (ceammc::ArgCheckLexer& lexer_yyarg, ceammc::ArgCheckerNode& n_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      n (n_yyarg)
  {}

  ArgCheckParser::~ArgCheckParser ()
  {}

  ArgCheckParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  ArgCheckParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  ArgCheckParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  ArgCheckParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  ArgCheckParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  ArgCheckParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  ArgCheckParser::symbol_number_type
  ArgCheckParser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  ArgCheckParser::stack_symbol_type::stack_symbol_type ()
  {}

  ArgCheckParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 38: // ATOM_BOOL
      case 39: // ATOM_INT
      case 40: // ATOM_FLOAT
      case 41: // ATOM_SYMBOL
      case 42: // ATOM_DATA
      case 43: // ATOM_SINGLE
      case 44: // ATOM
      case 46: // GROUP_OR
        value.YY_MOVE_OR_COPY< ArgCheckPtr > (YY_MOVE (that.value));
        break;

      case 45: // ATOM_OR_SEQ
      case 47: // ATOM_SEQ
        value.YY_MOVE_OR_COPY< ArgCheckPtrList > (YY_MOVE (that.value));
        break;

      case 31: // FLOAT
      case 36: // NUMBER
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 32: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 33: // SYMBOL
      case 34: // STRING
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 37: // REPEAT
        value.YY_MOVE_OR_COPY< std::vector<int> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  ArgCheckParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 38: // ATOM_BOOL
      case 39: // ATOM_INT
      case 40: // ATOM_FLOAT
      case 41: // ATOM_SYMBOL
      case 42: // ATOM_DATA
      case 43: // ATOM_SINGLE
      case 44: // ATOM
      case 46: // GROUP_OR
        value.move< ArgCheckPtr > (YY_MOVE (that.value));
        break;

      case 45: // ATOM_OR_SEQ
      case 47: // ATOM_SEQ
        value.move< ArgCheckPtrList > (YY_MOVE (that.value));
        break;

      case 31: // FLOAT
      case 36: // NUMBER
        value.move< double > (YY_MOVE (that.value));
        break;

      case 32: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case 33: // SYMBOL
      case 34: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 37: // REPEAT
        value.move< std::vector<int> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  ArgCheckParser::stack_symbol_type&
  ArgCheckParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 38: // ATOM_BOOL
      case 39: // ATOM_INT
      case 40: // ATOM_FLOAT
      case 41: // ATOM_SYMBOL
      case 42: // ATOM_DATA
      case 43: // ATOM_SINGLE
      case 44: // ATOM
      case 46: // GROUP_OR
        value.copy< ArgCheckPtr > (that.value);
        break;

      case 45: // ATOM_OR_SEQ
      case 47: // ATOM_SEQ
        value.copy< ArgCheckPtrList > (that.value);
        break;

      case 31: // FLOAT
      case 36: // NUMBER
        value.copy< double > (that.value);
        break;

      case 32: // INTEGER
        value.copy< int > (that.value);
        break;

      case 33: // SYMBOL
      case 34: // STRING
        value.copy< std::string > (that.value);
        break;

      case 37: // REPEAT
        value.copy< std::vector<int> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  ArgCheckParser::stack_symbol_type&
  ArgCheckParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 38: // ATOM_BOOL
      case 39: // ATOM_INT
      case 40: // ATOM_FLOAT
      case 41: // ATOM_SYMBOL
      case 42: // ATOM_DATA
      case 43: // ATOM_SINGLE
      case 44: // ATOM
      case 46: // GROUP_OR
        value.move< ArgCheckPtr > (that.value);
        break;

      case 45: // ATOM_OR_SEQ
      case 47: // ATOM_SEQ
        value.move< ArgCheckPtrList > (that.value);
        break;

      case 31: // FLOAT
      case 36: // NUMBER
        value.move< double > (that.value);
        break;

      case 32: // INTEGER
        value.move< int > (that.value);
        break;

      case 33: // SYMBOL
      case 34: // STRING
        value.move< std::string > (that.value);
        break;

      case 37: // REPEAT
        value.move< std::vector<int> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  ArgCheckParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  ArgCheckParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  ArgCheckParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  ArgCheckParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  ArgCheckParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ArgCheckParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ArgCheckParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ArgCheckParser::debug_level_type
  ArgCheckParser::debug_level () const
  {
    return yydebug_;
  }

  void
  ArgCheckParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  ArgCheckParser::state_type
  ArgCheckParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  ArgCheckParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  ArgCheckParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  ArgCheckParser::operator() ()
  {
    return parse ();
  }

  int
  ArgCheckParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", static_cast<state_type> (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 38: // ATOM_BOOL
      case 39: // ATOM_INT
      case 40: // ATOM_FLOAT
      case 41: // ATOM_SYMBOL
      case 42: // ATOM_DATA
      case 43: // ATOM_SINGLE
      case 44: // ATOM
      case 46: // GROUP_OR
        yylhs.value.emplace< ArgCheckPtr > ();
        break;

      case 45: // ATOM_OR_SEQ
      case 47: // ATOM_SEQ
        yylhs.value.emplace< ArgCheckPtrList > ();
        break;

      case 31: // FLOAT
      case 36: // NUMBER
        yylhs.value.emplace< double > ();
        break;

      case 32: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case 33: // SYMBOL
      case 34: // STRING
        yylhs.value.emplace< std::string > ();
        break;

      case 37: // REPEAT
        yylhs.value.emplace< std::vector<int> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 131 "argcheck.yy"
              { yylhs.value.as < double > () = yystack_[0].value.as < int > (); }
#line 740 "argcheck.parser.cpp"
    break;

  case 3:
#line 132 "argcheck.yy"
            { yylhs.value.as < double > () = yystack_[0].value.as < double > (); }
#line 746 "argcheck.parser.cpp"
    break;

  case 4:
#line 136 "argcheck.yy"
           {
        yylhs.value.as < std::vector<int> > ().push_back(1);
        }
#line 754 "argcheck.parser.cpp"
    break;

  case 5:
#line 139 "argcheck.yy"
               {
        yylhs.value.as < std::vector<int> > ().push_back(0);
        }
#line 762 "argcheck.parser.cpp"
    break;

  case 6:
#line 142 "argcheck.yy"
               {
        yylhs.value.as < std::vector<int> > ().push_back(0);
        yylhs.value.as < std::vector<int> > ().push_back(1);
        }
#line 771 "argcheck.parser.cpp"
    break;

  case 7:
#line 146 "argcheck.yy"
                                      {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        }
#line 780 "argcheck.parser.cpp"
    break;

  case 8:
#line 150 "argcheck.yy"
                                                   {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[2].value.as < int > ());
        }
#line 788 "argcheck.parser.cpp"
    break;

  case 9:
#line 153 "argcheck.yy"
                                                           {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[3].value.as < int > ());
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        }
#line 797 "argcheck.parser.cpp"
    break;

  case 10:
#line 160 "argcheck.yy"
            { yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsBool); }
#line 803 "argcheck.parser.cpp"
    break;

  case 11:
#line 164 "argcheck.yy"
           {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsInt);
        }
#line 811 "argcheck.parser.cpp"
    break;

  case 12:
#line 167 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 821 "argcheck.parser.cpp"
    break;

  case 13:
#line 172 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_NOT_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 831 "argcheck.parser.cpp"
    break;

  case 14:
#line 177 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 841 "argcheck.parser.cpp"
    break;

  case 15:
#line 182 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 851 "argcheck.parser.cpp"
    break;

  case 16:
#line 187 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 861 "argcheck.parser.cpp"
    break;

  case 17:
#line 192 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 871 "argcheck.parser.cpp"
    break;

  case 18:
#line 197 "argcheck.yy"
                                        {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_RANGE, yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 881 "argcheck.parser.cpp"
    break;

  case 19:
#line 202 "argcheck.yy"
                                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_MOD, yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 891 "argcheck.parser.cpp"
    break;

  case 20:
#line 207 "argcheck.yy"
                        {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_POWER_OF_TWO, 0, 0);
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 901 "argcheck.parser.cpp"
    break;

  case 21:
#line 215 "argcheck.yy"
             {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsFloat);
        }
#line 909 "argcheck.parser.cpp"
    break;

  case 22:
#line 218 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 919 "argcheck.parser.cpp"
    break;

  case 23:
#line 223 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_NOT_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 929 "argcheck.parser.cpp"
    break;

  case 24:
#line 228 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 939 "argcheck.parser.cpp"
    break;

  case 25:
#line 233 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 949 "argcheck.parser.cpp"
    break;

  case 26:
#line 238 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 959 "argcheck.parser.cpp"
    break;

  case 27:
#line 243 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 969 "argcheck.parser.cpp"
    break;

  case 28:
#line 251 "argcheck.yy"
              {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsSymbol);
        }
#line 977 "argcheck.parser.cpp"
    break;

  case 29:
#line 254 "argcheck.yy"
                        {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_EQUAL, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 987 "argcheck.parser.cpp"
    break;

  case 30:
#line 259 "argcheck.yy"
                        {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_NOT_EQUAL, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 997 "argcheck.parser.cpp"
    break;

  case 31:
#line 264 "argcheck.yy"
                  {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_BEGINS_WITH, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1007 "argcheck.parser.cpp"
    break;

  case 32:
#line 269 "argcheck.yy"
                   {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_CONTAINS, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
    }
#line 1017 "argcheck.parser.cpp"
    break;

  case 33:
#line 274 "argcheck.yy"
                    {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_ENDS_WITH, yystack_[1].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1027 "argcheck.parser.cpp"
    break;

  case 34:
#line 279 "argcheck.yy"
             {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_MATCH, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1037 "argcheck.parser.cpp"
    break;

  case 35:
#line 287 "argcheck.yy"
            {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsData);
        }
#line 1045 "argcheck.parser.cpp"
    break;

  case 36:
#line 290 "argcheck.yy"
                      {
        auto p = new ArgIsData;
        p->setType(yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1055 "argcheck.parser.cpp"
    break;

  case 37:
#line 298 "argcheck.yy"
                  { yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsAtom); }
#line 1061 "argcheck.parser.cpp"
    break;

  case 38:
#line 299 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1067 "argcheck.parser.cpp"
    break;

  case 39:
#line 300 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1073 "argcheck.parser.cpp"
    break;

  case 40:
#line 301 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1079 "argcheck.parser.cpp"
    break;

  case 41:
#line 302 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1085 "argcheck.parser.cpp"
    break;

  case 42:
#line 303 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1091 "argcheck.parser.cpp"
    break;

  case 43:
#line 307 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1097 "argcheck.parser.cpp"
    break;

  case 44:
#line 308 "argcheck.yy"
                         { yylhs.value.as < ArgCheckPtr > () = yystack_[1].value.as < ArgCheckPtr > (); set_repeats(yylhs.value.as < ArgCheckPtr > (), yystack_[0].value.as < std::vector<int> > ()); }
#line 1103 "argcheck.parser.cpp"
    break;

  case 45:
#line 315 "argcheck.yy"
                                 {
        yylhs.value.as < ArgCheckPtrList > ().assign({yystack_[2].value.as < ArgCheckPtr > (), yystack_[0].value.as < ArgCheckPtr > ()});
        }
#line 1111 "argcheck.parser.cpp"
    break;

  case 46:
#line 318 "argcheck.yy"
                                 {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[2].value.as < ArgCheckPtr > ());
        yylhs.value.as < ArgCheckPtrList > ().insert(std::end(yylhs.value.as < ArgCheckPtrList > ()), std::begin(yystack_[0].value.as < ArgCheckPtrList > ()), std::end(yystack_[0].value.as < ArgCheckPtrList > ()));
        }
#line 1120 "argcheck.parser.cpp"
    break;

  case 47:
#line 329 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[0].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
        }
#line 1131 "argcheck.parser.cpp"
    break;

  case 48:
#line 336 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[1].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
        }
#line 1142 "argcheck.parser.cpp"
    break;

  case 49:
#line 343 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[2].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
            set_repeats(yylhs.value.as < ArgCheckPtr > (), yystack_[0].value.as < std::vector<int> > ());
        }
#line 1154 "argcheck.parser.cpp"
    break;

  case 50:
#line 353 "argcheck.yy"
           {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[0].value.as < ArgCheckPtr > ());
        }
#line 1162 "argcheck.parser.cpp"
    break;

  case 51:
#line 356 "argcheck.yy"
                          {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[2].value.as < ArgCheckPtr > ());
        yylhs.value.as < ArgCheckPtrList > ().insert(std::end(yylhs.value.as < ArgCheckPtrList > ()), std::begin(yystack_[0].value.as < ArgCheckPtrList > ()), std::end(yystack_[0].value.as < ArgCheckPtrList > ()));
        }
#line 1171 "argcheck.parser.cpp"
    break;

  case 52:
#line 363 "argcheck.yy"
               {
        for(auto& p: yystack_[0].value.as < ArgCheckPtrList > ())
            n.insertChild(p);
        }
#line 1180 "argcheck.parser.cpp"
    break;

  case 53:
#line 367 "argcheck.yy"
               {
        n.insertChild(yystack_[0].value.as < ArgCheckPtr > ());
        }
#line 1188 "argcheck.parser.cpp"
    break;

  case 56:
#line 378 "argcheck.yy"
             { }
#line 1194 "argcheck.parser.cpp"
    break;


#line 1198 "argcheck.parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = static_cast<state_type> (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  ArgCheckParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  ArgCheckParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char ArgCheckParser::yypact_ninf_ = -24;

  const signed char ArgCheckParser::yytable_ninf_ = -1;

  const signed char
  ArgCheckParser::yypact_[] =
  {
      -1,   -24,    59,   -24,     6,    36,     8,     7,   -16,   -12,
      -5,   -24,   -24,   -24,   -24,   -24,   -24,     5,     1,   -24,
     -24,   -24,    -1,   -24,    38,   -23,   -23,   -23,   -23,   -23,
     -23,     9,    14,    22,    23,    25,    28,    29,   -24,    32,
      49,    33,    34,    16,    46,   -24,   -24,   -24,    50,   -24,
       7,   -24,   -24,   -24,     7,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,    71,    52,   -24,   -24,    45,    27,    16,   -24,
      45,   -24,    53,   -24,   -24,   -24,    13,   -24,   -24,    77,
     -24
  };

  const signed char
  ArgCheckParser::yydefact_[] =
  {
      56,    10,    21,    37,    35,    11,    28,     0,     0,     0,
       0,    34,    39,    41,    40,    42,    38,    43,    50,    47,
      53,    52,    54,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     5,
       0,     4,     6,    44,     0,    55,     1,     3,     2,    22,
      24,    25,    26,    27,    23,    36,    12,    15,    14,    17,
      16,    13,     0,     0,    29,    30,    48,     0,    45,    46,
      43,    51,     0,    18,    49,     7,     0,    19,     8,     0,
       9
  };

  const signed char
  ArgCheckParser::yypgoto_[] =
  {
     -24,    51,    11,   -24,   -24,   -24,   -24,   -24,    -7,   -24,
      -6,   -24,    35,   -24,    66,   -24
  };

  const signed char
  ArgCheckParser::yydefgoto_[] =
  {
      -1,    59,    53,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24
  };

  const signed char
  ArgCheckParser::yytable_[] =
  {
      43,    44,     1,     2,     3,     4,     5,     6,    57,    58,
       1,     2,     3,     4,     5,     6,    48,    45,    31,     7,
      41,    46,    88,    49,    47,    42,     8,     9,    50,    51,
      52,    54,    10,    11,     8,     9,    85,    86,    56,    50,
      10,    11,    65,    78,    79,    89,    66,    80,    32,    33,
      34,    35,    36,    37,    67,    68,    48,    69,    38,    73,
      70,    71,    39,    49,    72,    76,    74,    75,    40,    51,
      52,    25,    26,    27,    28,    29,    30,    60,    61,    62,
      63,    64,    77,    82,    83,    87,    90,    84,    55,    81
  };

  const signed char
  ArgCheckParser::yycheck_[] =
  {
       7,     7,     3,     4,     5,     6,     7,     8,    31,    32,
       3,     4,     5,     6,     7,     8,    11,    33,    12,    20,
      12,    33,     9,    18,    29,    17,    27,    28,    23,    24,
      25,    30,    33,    34,    27,    28,     9,    10,     0,    23,
      33,    34,    33,    50,    50,    32,    32,    54,    12,    13,
      14,    15,    16,    17,    32,    32,    11,    32,    22,    10,
      32,    32,    26,    18,    32,    19,    33,    33,    32,    24,
      25,    12,    13,    14,    15,    16,    17,    26,    27,    28,
      29,    30,    32,    12,    32,    32,     9,    76,    22,    54
  };

  const signed char
  ArgCheckParser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,    20,    27,    28,
      33,    34,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    12,    13,    14,    15,    16,
      17,    12,    12,    13,    14,    15,    16,    17,    22,    26,
      32,    12,    17,    43,    45,    33,    33,    29,    11,    18,
      23,    24,    25,    37,    30,    49,     0,    31,    32,    36,
      36,    36,    36,    36,    36,    33,    32,    32,    32,    32,
      32,    32,    32,    10,    33,    33,    19,    32,    43,    45,
      43,    47,    12,    32,    37,     9,    10,    32,     9,    32,
       9
  };

  const signed char
  ArgCheckParser::yyr1_[] =
  {
       0,    35,    36,    36,    37,    37,    37,    37,    37,    37,
      38,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    40,    40,    40,    40,    40,    40,    40,    41,    41,
      41,    41,    41,    41,    41,    42,    42,    43,    43,    43,
      43,    43,    43,    44,    44,    45,    45,    46,    46,    46,
      47,    47,    48,    48,    49,    49,    50,    50
  };

  const signed char
  ArgCheckParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     3,     4,     5,
       1,     1,     3,     3,     3,     3,     3,     3,     4,     5,
       2,     1,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     2,     2,     2,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     3,     1,     3,     4,
       1,     3,     1,     1,     1,     2,     0,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const ArgCheckParser::yytname_[] =
  {
  "$end", "error", "$undefined", "TBOOL", "TFLOAT", "TATOM", "TDATA",
  "TINT", "TSYMBOL", "REPEAT_END", "REPEAT_RANGE", "REPEAT_START", "EQ",
  "GE", "GT", "LE", "LT", "NE", "ASTERISK", "GROUP_END", "GROUP_START",
  "LEXER_ERR", "POWER_OF_TWO", "OR", "PLUS", "QUESTION", "MODULUS", "CAPS",
  "TILDE", "DOLLAR", "SPACE", "FLOAT", "INTEGER", "SYMBOL", "STRING",
  "$accept", "NUMBER", "REPEAT", "ATOM_BOOL", "ATOM_INT", "ATOM_FLOAT",
  "ATOM_SYMBOL", "ATOM_DATA", "ATOM_SINGLE", "ATOM", "ATOM_OR_SEQ",
  "GROUP_OR", "ATOM_SEQ", "SEQ", "EXPR", "REGEXP", YY_NULLPTR
  };

#if YYDEBUG
  const short
  ArgCheckParser::yyrline_[] =
  {
       0,   131,   131,   132,   136,   139,   142,   146,   150,   153,
     160,   164,   167,   172,   177,   182,   187,   192,   197,   202,
     207,   215,   218,   223,   228,   233,   238,   243,   251,   254,
     259,   264,   269,   274,   279,   287,   290,   298,   299,   300,
     301,   302,   303,   307,   308,   315,   318,   328,   335,   342,
     353,   356,   363,   367,   373,   374,   378,   379
  };

  // Print the state stack on the debug stream.
  void
  ArgCheckParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  ArgCheckParser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 7 "argcheck.yy"
} // ceammc
#line 1639 "argcheck.parser.cpp"

#line 381 "argcheck.yy"


void ceammc::ArgCheckParser::error(const std::string& err_message)
{
    std::cerr << "Error: " << err_message << '\n';
    throw std::runtime_error(err_message);
}

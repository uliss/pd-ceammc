// A Bison parser, made by GNU Bison 3.7.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





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

#line 82 "argcheck.parser.cpp"


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
      yy_stack_print_ ();                \
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
#line 156 "argcheck.parser.cpp"

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
  | symbol kinds.  |
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

  ArgCheckParser::symbol_kind_type
  ArgCheckParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  ArgCheckParser::stack_symbol_type::stack_symbol_type ()
  {}

  ArgCheckParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_ATOM_BOOL: // ATOM_BOOL
      case symbol_kind::S_ATOM_INT: // ATOM_INT
      case symbol_kind::S_ATOM_FLOAT: // ATOM_FLOAT
      case symbol_kind::S_ATOM_SYMBOL: // ATOM_SYMBOL
      case symbol_kind::S_ATOM_DATA: // ATOM_DATA
      case symbol_kind::S_ATOM_SINGLE: // ATOM_SINGLE
      case symbol_kind::S_ATOM: // ATOM
      case symbol_kind::S_GROUP_OR: // GROUP_OR
        value.YY_MOVE_OR_COPY< ArgCheckPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ATOM_OR_SEQ: // ATOM_OR_SEQ
      case symbol_kind::S_ATOM_SEQ: // ATOM_SEQ
        value.YY_MOVE_OR_COPY< ArgCheckPtrList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_NUMBER: // NUMBER
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_STRING: // STRING
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REPEAT: // REPEAT
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
    switch (that.kind ())
    {
      case symbol_kind::S_ATOM_BOOL: // ATOM_BOOL
      case symbol_kind::S_ATOM_INT: // ATOM_INT
      case symbol_kind::S_ATOM_FLOAT: // ATOM_FLOAT
      case symbol_kind::S_ATOM_SYMBOL: // ATOM_SYMBOL
      case symbol_kind::S_ATOM_DATA: // ATOM_DATA
      case symbol_kind::S_ATOM_SINGLE: // ATOM_SINGLE
      case symbol_kind::S_ATOM: // ATOM
      case symbol_kind::S_GROUP_OR: // GROUP_OR
        value.move< ArgCheckPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ATOM_OR_SEQ: // ATOM_OR_SEQ
      case symbol_kind::S_ATOM_SEQ: // ATOM_SEQ
        value.move< ArgCheckPtrList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_STRING: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REPEAT: // REPEAT
        value.move< std::vector<int> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  ArgCheckParser::stack_symbol_type&
  ArgCheckParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_ATOM_BOOL: // ATOM_BOOL
      case symbol_kind::S_ATOM_INT: // ATOM_INT
      case symbol_kind::S_ATOM_FLOAT: // ATOM_FLOAT
      case symbol_kind::S_ATOM_SYMBOL: // ATOM_SYMBOL
      case symbol_kind::S_ATOM_DATA: // ATOM_DATA
      case symbol_kind::S_ATOM_SINGLE: // ATOM_SINGLE
      case symbol_kind::S_ATOM: // ATOM
      case symbol_kind::S_GROUP_OR: // GROUP_OR
        value.copy< ArgCheckPtr > (that.value);
        break;

      case symbol_kind::S_ATOM_OR_SEQ: // ATOM_OR_SEQ
      case symbol_kind::S_ATOM_SEQ: // ATOM_SEQ
        value.copy< ArgCheckPtrList > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_NUMBER: // NUMBER
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_STRING: // STRING
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_REPEAT: // REPEAT
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
    switch (that.kind ())
    {
      case symbol_kind::S_ATOM_BOOL: // ATOM_BOOL
      case symbol_kind::S_ATOM_INT: // ATOM_INT
      case symbol_kind::S_ATOM_FLOAT: // ATOM_FLOAT
      case symbol_kind::S_ATOM_SYMBOL: // ATOM_SYMBOL
      case symbol_kind::S_ATOM_DATA: // ATOM_DATA
      case symbol_kind::S_ATOM_SINGLE: // ATOM_SINGLE
      case symbol_kind::S_ATOM: // ATOM
      case symbol_kind::S_GROUP_OR: // GROUP_OR
        value.move< ArgCheckPtr > (that.value);
        break;

      case symbol_kind::S_ATOM_OR_SEQ: // ATOM_OR_SEQ
      case symbol_kind::S_ATOM_SEQ: // ATOM_SEQ
        value.move< ArgCheckPtrList > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_STRING: // STRING
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_REPEAT: // REPEAT
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
  ArgCheckParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YYUSE (yykind);
        yyo << ')';
      }
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
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
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
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
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

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
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
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
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
      case symbol_kind::S_ATOM_BOOL: // ATOM_BOOL
      case symbol_kind::S_ATOM_INT: // ATOM_INT
      case symbol_kind::S_ATOM_FLOAT: // ATOM_FLOAT
      case symbol_kind::S_ATOM_SYMBOL: // ATOM_SYMBOL
      case symbol_kind::S_ATOM_DATA: // ATOM_DATA
      case symbol_kind::S_ATOM_SINGLE: // ATOM_SINGLE
      case symbol_kind::S_ATOM: // ATOM
      case symbol_kind::S_GROUP_OR: // GROUP_OR
        yylhs.value.emplace< ArgCheckPtr > ();
        break;

      case symbol_kind::S_ATOM_OR_SEQ: // ATOM_OR_SEQ
      case symbol_kind::S_ATOM_SEQ: // ATOM_SEQ
        yylhs.value.emplace< ArgCheckPtrList > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_NUMBER: // NUMBER
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_STRING: // STRING
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_REPEAT: // REPEAT
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
  case 2: // NUMBER: INTEGER
#line 131 "argcheck.yy"
              { yylhs.value.as < double > () = yystack_[0].value.as < int > (); }
#line 709 "argcheck.parser.cpp"
    break;

  case 3: // NUMBER: FLOAT
#line 132 "argcheck.yy"
            { yylhs.value.as < double > () = yystack_[0].value.as < double > (); }
#line 715 "argcheck.parser.cpp"
    break;

  case 4: // REPEAT: PLUS
#line 136 "argcheck.yy"
           {
        yylhs.value.as < std::vector<int> > ().push_back(1);
        }
#line 723 "argcheck.parser.cpp"
    break;

  case 5: // REPEAT: ASTERISK
#line 139 "argcheck.yy"
               {
        yylhs.value.as < std::vector<int> > ().push_back(0);
        }
#line 731 "argcheck.parser.cpp"
    break;

  case 6: // REPEAT: QUESTION
#line 142 "argcheck.yy"
               {
        yylhs.value.as < std::vector<int> > ().push_back(0);
        yylhs.value.as < std::vector<int> > ().push_back(1);
        }
#line 740 "argcheck.parser.cpp"
    break;

  case 7: // REPEAT: REPEAT_START INTEGER REPEAT_END
#line 146 "argcheck.yy"
                                      {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        }
#line 749 "argcheck.parser.cpp"
    break;

  case 8: // REPEAT: REPEAT_START INTEGER REPEAT_RANGE REPEAT_END
#line 150 "argcheck.yy"
                                                   {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[2].value.as < int > ());
        }
#line 757 "argcheck.parser.cpp"
    break;

  case 9: // REPEAT: REPEAT_START INTEGER REPEAT_RANGE INTEGER REPEAT_END
#line 153 "argcheck.yy"
                                                           {
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[3].value.as < int > ());
        yylhs.value.as < std::vector<int> > ().push_back(yystack_[1].value.as < int > ());
        }
#line 766 "argcheck.parser.cpp"
    break;

  case 10: // ATOM_BOOL: TBOOL
#line 160 "argcheck.yy"
            { yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsBool); }
#line 772 "argcheck.parser.cpp"
    break;

  case 11: // ATOM_INT: TINT
#line 164 "argcheck.yy"
           {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsInt);
        }
#line 780 "argcheck.parser.cpp"
    break;

  case 12: // ATOM_INT: TINT EQ INTEGER
#line 167 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 790 "argcheck.parser.cpp"
    break;

  case 13: // ATOM_INT: TINT NE INTEGER
#line 172 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_NOT_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 800 "argcheck.parser.cpp"
    break;

  case 14: // ATOM_INT: TINT GT INTEGER
#line 177 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 810 "argcheck.parser.cpp"
    break;

  case 15: // ATOM_INT: TINT GE INTEGER
#line 182 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 820 "argcheck.parser.cpp"
    break;

  case 16: // ATOM_INT: TINT LT INTEGER
#line 187 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 830 "argcheck.parser.cpp"
    break;

  case 17: // ATOM_INT: TINT LE INTEGER
#line 192 "argcheck.yy"
                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS_EQUAL, yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 840 "argcheck.parser.cpp"
    break;

  case 18: // ATOM_INT: TINT INTEGER REPEAT_RANGE INTEGER
#line 197 "argcheck.yy"
                                        {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_RANGE, yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 850 "argcheck.parser.cpp"
    break;

  case 19: // ATOM_INT: TINT MODULUS INTEGER EQ INTEGER
#line 202 "argcheck.yy"
                                      {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_MOD, yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 860 "argcheck.parser.cpp"
    break;

  case 20: // ATOM_INT: TINT POWER_OF_TWO
#line 207 "argcheck.yy"
                        {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_POWER_OF_TWO, 0, 0);
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 870 "argcheck.parser.cpp"
    break;

  case 21: // ATOM_FLOAT: TFLOAT
#line 215 "argcheck.yy"
             {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsFloat);
        }
#line 878 "argcheck.parser.cpp"
    break;

  case 22: // ATOM_FLOAT: TFLOAT EQ NUMBER
#line 218 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 888 "argcheck.parser.cpp"
    break;

  case 23: // ATOM_FLOAT: TFLOAT NE NUMBER
#line 223 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_NOT_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 898 "argcheck.parser.cpp"
    break;

  case 24: // ATOM_FLOAT: TFLOAT GE NUMBER
#line 228 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 908 "argcheck.parser.cpp"
    break;

  case 25: // ATOM_FLOAT: TFLOAT GT NUMBER
#line 233 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 918 "argcheck.parser.cpp"
    break;

  case 26: // ATOM_FLOAT: TFLOAT LE NUMBER
#line 238 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS_EQUAL, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 928 "argcheck.parser.cpp"
    break;

  case 27: // ATOM_FLOAT: TFLOAT LT NUMBER
#line 243 "argcheck.yy"
                       {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS, yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 938 "argcheck.parser.cpp"
    break;

  case 28: // ATOM_FLOAT: TFLOAT NUMBER REPEAT_RANGE NUMBER
#line 248 "argcheck.yy"
                                        {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_RANGE, yystack_[2].value.as < double > (), yystack_[0].value.as < double > ());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 948 "argcheck.parser.cpp"
    break;

  case 29: // ATOM_SYMBOL: TSYMBOL
#line 256 "argcheck.yy"
              {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsSymbol);
        }
#line 956 "argcheck.parser.cpp"
    break;

  case 30: // ATOM_SYMBOL: TSYMBOL EQ STRING
#line 259 "argcheck.yy"
                        {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_EQUAL, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 966 "argcheck.parser.cpp"
    break;

  case 31: // ATOM_SYMBOL: TSYMBOL NE STRING
#line 264 "argcheck.yy"
                        {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_NOT_EQUAL, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 976 "argcheck.parser.cpp"
    break;

  case 32: // ATOM_SYMBOL: CAPS STRING
#line 269 "argcheck.yy"
                  {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_BEGINS_WITH, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 986 "argcheck.parser.cpp"
    break;

  case 33: // ATOM_SYMBOL: TILDE STRING
#line 274 "argcheck.yy"
                   {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_CONTAINS, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
    }
#line 996 "argcheck.parser.cpp"
    break;

  case 34: // ATOM_SYMBOL: STRING DOLLAR
#line 279 "argcheck.yy"
                    {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_ENDS_WITH, yystack_[1].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1006 "argcheck.parser.cpp"
    break;

  case 35: // ATOM_SYMBOL: STRING
#line 284 "argcheck.yy"
             {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_MATCH, yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1016 "argcheck.parser.cpp"
    break;

  case 36: // ATOM_DATA: TDATA
#line 292 "argcheck.yy"
            {
        yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsData);
        }
#line 1024 "argcheck.parser.cpp"
    break;

  case 37: // ATOM_DATA: TDATA EQ SYMBOL
#line 295 "argcheck.yy"
                      {
        auto p = new ArgIsData;
        p->setType(yystack_[0].value.as < std::string > ().c_str());
        yylhs.value.as < ArgCheckPtr > ().reset(p);
        }
#line 1034 "argcheck.parser.cpp"
    break;

  case 38: // ATOM_SINGLE: TATOM
#line 303 "argcheck.yy"
                  { yylhs.value.as < ArgCheckPtr > ().reset(new ArgIsAtom); }
#line 1040 "argcheck.parser.cpp"
    break;

  case 39: // ATOM_SINGLE: ATOM_DATA
#line 304 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1046 "argcheck.parser.cpp"
    break;

  case 40: // ATOM_SINGLE: ATOM_BOOL
#line 305 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1052 "argcheck.parser.cpp"
    break;

  case 41: // ATOM_SINGLE: ATOM_FLOAT
#line 306 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1058 "argcheck.parser.cpp"
    break;

  case 42: // ATOM_SINGLE: ATOM_INT
#line 307 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1064 "argcheck.parser.cpp"
    break;

  case 43: // ATOM_SINGLE: ATOM_SYMBOL
#line 308 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1070 "argcheck.parser.cpp"
    break;

  case 44: // ATOM: ATOM_SINGLE
#line 312 "argcheck.yy"
      { yylhs.value.as < ArgCheckPtr > () = yystack_[0].value.as < ArgCheckPtr > (); }
#line 1076 "argcheck.parser.cpp"
    break;

  case 45: // ATOM: ATOM_SINGLE REPEAT
#line 313 "argcheck.yy"
                         { yylhs.value.as < ArgCheckPtr > () = yystack_[1].value.as < ArgCheckPtr > (); set_repeats(yylhs.value.as < ArgCheckPtr > (), yystack_[0].value.as < std::vector<int> > ()); }
#line 1082 "argcheck.parser.cpp"
    break;

  case 46: // ATOM_OR_SEQ: ATOM_SINGLE OR ATOM_SINGLE
#line 320 "argcheck.yy"
                                 {
        yylhs.value.as < ArgCheckPtrList > ().assign({yystack_[2].value.as < ArgCheckPtr > (), yystack_[0].value.as < ArgCheckPtr > ()});
        }
#line 1090 "argcheck.parser.cpp"
    break;

  case 47: // ATOM_OR_SEQ: ATOM_SINGLE OR ATOM_OR_SEQ
#line 323 "argcheck.yy"
                                 {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[2].value.as < ArgCheckPtr > ());
        yylhs.value.as < ArgCheckPtrList > ().insert(std::end(yylhs.value.as < ArgCheckPtrList > ()), std::begin(yystack_[0].value.as < ArgCheckPtrList > ()), std::end(yystack_[0].value.as < ArgCheckPtrList > ()));
        }
#line 1099 "argcheck.parser.cpp"
    break;

  case 48: // GROUP_OR: ATOM_OR_SEQ
#line 334 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[0].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
        }
#line 1110 "argcheck.parser.cpp"
    break;

  case 49: // GROUP_OR: GROUP_START ATOM_OR_SEQ GROUP_END
#line 341 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[1].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
        }
#line 1121 "argcheck.parser.cpp"
    break;

  case 50: // GROUP_OR: GROUP_START ATOM_OR_SEQ GROUP_END REPEAT
#line 348 "argcheck.yy"
        {
            yylhs.value.as < ArgCheckPtr > ().reset(new ArgGroupOr);
            for(auto& p: yystack_[2].value.as < ArgCheckPtrList > ()) {
                yylhs.value.as < ArgCheckPtr > ()->insertChild(p);
            }
            set_repeats(yylhs.value.as < ArgCheckPtr > (), yystack_[0].value.as < std::vector<int> > ());
        }
#line 1133 "argcheck.parser.cpp"
    break;

  case 51: // ATOM_SEQ: ATOM
#line 358 "argcheck.yy"
           {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[0].value.as < ArgCheckPtr > ());
        }
#line 1141 "argcheck.parser.cpp"
    break;

  case 52: // ATOM_SEQ: ATOM SPACE ATOM_SEQ
#line 361 "argcheck.yy"
                          {
        yylhs.value.as < ArgCheckPtrList > ().push_back(yystack_[2].value.as < ArgCheckPtr > ());
        yylhs.value.as < ArgCheckPtrList > ().insert(std::end(yylhs.value.as < ArgCheckPtrList > ()), std::begin(yystack_[0].value.as < ArgCheckPtrList > ()), std::end(yystack_[0].value.as < ArgCheckPtrList > ()));
        }
#line 1150 "argcheck.parser.cpp"
    break;

  case 53: // SEQ: ATOM_SEQ
#line 368 "argcheck.yy"
               {
        for(auto& p: yystack_[0].value.as < ArgCheckPtrList > ())
            n.insertChild(p);
        }
#line 1159 "argcheck.parser.cpp"
    break;

  case 54: // SEQ: GROUP_OR
#line 372 "argcheck.yy"
               {
        n.insertChild(yystack_[0].value.as < ArgCheckPtr > ());
        }
#line 1167 "argcheck.parser.cpp"
    break;

  case 57: // REGEXP: %empty
#line 383 "argcheck.yy"
             { }
#line 1173 "argcheck.parser.cpp"
    break;


#line 1177 "argcheck.parser.cpp"

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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
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
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
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
    YY_STACK_PRINT ();
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

  std::string
  ArgCheckParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // ArgCheckParser::context.
  ArgCheckParser::context::context (const ArgCheckParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  ArgCheckParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
  ArgCheckParser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
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

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  ArgCheckParser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

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
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char ArgCheckParser::yypact_ninf_ = -23;

  const signed char ArgCheckParser::yytable_ninf_ = -1;

  const signed char
  ArgCheckParser::yypact_[] =
  {
       5,   -23,    59,   -23,     2,    38,    14,    13,    -4,     3,
      15,   -23,   -23,   -23,   -23,   -23,    11,    12,   -23,   -23,
     -23,     5,   -23,    48,    -8,    -8,    -8,    -8,    -8,    -8,
     -23,   -23,    46,    24,    26,    27,    29,    30,    31,    33,
     -23,    34,    57,    35,    43,    45,    61,   -23,   -23,   -23,
      49,   -23,    13,   -23,   -23,   -23,    13,   -23,   -23,   -23,
     -23,   -23,   -23,   -23,   -23,    -8,   -23,   -23,   -23,   -23,
     -23,   -23,   -23,    66,    50,   -23,   -23,    68,    18,    45,
     -23,    68,   -23,   -23,    51,   -23,   -23,   -23,     6,   -23,
     -23,    75,   -23
  };

  const signed char
  ArgCheckParser::yydefact_[] =
  {
      57,    10,    21,    38,    36,    11,    29,     0,     0,     0,
      35,    40,    42,    41,    43,    39,    44,    51,    48,    54,
      53,    55,    58,     0,     0,     0,     0,     0,     0,     0,
       3,     2,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     5,     0,     4,     6,    45,     0,    56,     1,    22,
      24,    25,    26,    27,    23,     0,    37,    12,    15,    14,
      17,    16,    13,     0,     0,    30,    31,    49,     0,    46,
      47,    44,    52,    28,     0,    18,    50,     7,     0,    19,
       8,     0,     9
  };

  const signed char
  ArgCheckParser::yypgoto_[] =
  {
     -23,   -22,     8,   -23,   -23,   -23,   -23,   -23,    -7,   -23,
      -6,   -23,    32,   -23,    73,   -23
  };

  const signed char
  ArgCheckParser::yydefgoto_[] =
  {
      -1,    32,    55,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23
  };

  const signed char
  ArgCheckParser::yytable_[] =
  {
      45,    46,    59,    60,    61,    62,    63,    64,     1,     2,
       3,     4,     5,     6,    33,    90,     1,     2,     3,     4,
       5,     6,    50,    30,    31,     7,    43,    87,    88,    51,
      47,    44,     8,     9,    52,    53,    54,    48,    91,    10,
       8,     9,    56,    83,    49,    79,    80,    10,    58,    81,
      34,    35,    36,    37,    38,    39,    65,    66,    67,    68,
      40,    69,    70,    71,    41,    72,    73,    74,    52,    75,
      42,    24,    25,    26,    27,    28,    29,    76,    84,    50,
      77,    78,    85,    89,    92,    86,    51,     0,    82,     0,
      30,    31,    53,    54,    57
  };

  const signed char
  ArgCheckParser::yycheck_[] =
  {
       7,     7,    24,    25,    26,    27,    28,    29,     3,     4,
       5,     6,     7,     8,    12,     9,     3,     4,     5,     6,
       7,     8,    11,    31,    32,    20,    12,     9,    10,    18,
      34,    17,    27,    28,    23,    24,    25,    34,    32,    34,
      27,    28,    30,    65,    29,    52,    52,    34,     0,    56,
      12,    13,    14,    15,    16,    17,    10,    33,    32,    32,
      22,    32,    32,    32,    26,    32,    32,    10,    23,    34,
      32,    12,    13,    14,    15,    16,    17,    34,    12,    11,
      19,    32,    32,    32,     9,    77,    18,    -1,    56,    -1,
      31,    32,    24,    25,    21
  };

  const signed char
  ArgCheckParser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,    20,    27,    28,
      34,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    12,    13,    14,    15,    16,    17,
      31,    32,    36,    12,    12,    13,    14,    15,    16,    17,
      22,    26,    32,    12,    17,    43,    45,    34,    34,    29,
      11,    18,    23,    24,    25,    37,    30,    49,     0,    36,
      36,    36,    36,    36,    36,    10,    33,    32,    32,    32,
      32,    32,    32,    32,    10,    34,    34,    19,    32,    43,
      45,    43,    47,    36,    12,    32,    37,     9,    10,    32,
       9,    32,     9
  };

  const signed char
  ArgCheckParser::yyr1_[] =
  {
       0,    35,    36,    36,    37,    37,    37,    37,    37,    37,
      38,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    40,    40,    40,    40,    40,    40,    40,    40,    41,
      41,    41,    41,    41,    41,    41,    42,    42,    43,    43,
      43,    43,    43,    43,    44,    44,    45,    45,    46,    46,
      46,    47,    47,    48,    48,    49,    49,    50,    50
  };

  const signed char
  ArgCheckParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     3,     4,     5,
       1,     1,     3,     3,     3,     3,     3,     3,     4,     5,
       2,     1,     3,     3,     3,     3,     3,     3,     4,     1,
       3,     3,     2,     2,     2,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     3,     3,     1,     3,
       4,     1,     3,     1,     1,     1,     2,     0,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const ArgCheckParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "TBOOL", "TFLOAT",
  "TATOM", "TDATA", "TINT", "TSYMBOL", "REPEAT_END", "REPEAT_RANGE",
  "REPEAT_START", "EQ", "GE", "GT", "LE", "LT", "NE", "ASTERISK",
  "GROUP_END", "GROUP_START", "LEXER_ERR", "POWER_OF_TWO", "OR", "PLUS",
  "QUESTION", "MODULUS", "CAPS", "TILDE", "DOLLAR", "SPACE", "FLOAT",
  "INTEGER", "SYMBOL", "STRING", "$accept", "NUMBER", "REPEAT",
  "ATOM_BOOL", "ATOM_INT", "ATOM_FLOAT", "ATOM_SYMBOL", "ATOM_DATA",
  "ATOM_SINGLE", "ATOM", "ATOM_OR_SEQ", "GROUP_OR", "ATOM_SEQ", "SEQ",
  "EXPR", "REGEXP", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  ArgCheckParser::yyrline_[] =
  {
       0,   131,   131,   132,   136,   139,   142,   146,   150,   153,
     160,   164,   167,   172,   177,   182,   187,   192,   197,   202,
     207,   215,   218,   223,   228,   233,   238,   243,   248,   256,
     259,   264,   269,   274,   279,   284,   292,   295,   303,   304,
     305,   306,   307,   308,   312,   313,   320,   323,   333,   340,
     347,   358,   361,   368,   372,   378,   379,   383,   384
  };

  void
  ArgCheckParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  ArgCheckParser::yy_reduce_print_ (int yyrule) const
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
#line 1699 "argcheck.parser.cpp"

#line 386 "argcheck.yy"


void ceammc::ArgCheckParser::error(const std::string& err_message)
{
    std::cerr << "Error: " << err_message << '\n';
    throw std::runtime_error(err_message);
}

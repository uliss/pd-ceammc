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





#include "score.parser.hpp"


// Unqualified %code blocks.
#line 92 "score.y"

    # undef yylex
    # define yylex lexer.lex

    # include "score.lexer.h"

#line 53 "score.parser.cpp"


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

#line 7 "score.y"
namespace ceammc { namespace score {
#line 127 "score.parser.cpp"

  /// Build a parser object.
  ScoreParser::ScoreParser (ceammc::score::ScoreLexer& lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg)
  {}

  ScoreParser::~ScoreParser ()
  {}

  ScoreParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  ScoreParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  ScoreParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  ScoreParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  ScoreParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  ScoreParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  ScoreParser::symbol_kind_type
  ScoreParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  ScoreParser::stack_symbol_type::stack_symbol_type ()
  {}

  ScoreParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BAR_CONTENT: // BAR_CONTENT
      case symbol_kind::S_BAR: // BAR
        value.YY_MOVE_OR_COPY< Bar > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_EXPR: // EXPR
        value.YY_MOVE_OR_COPY< BarList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_KEY: // KEY
        value.YY_MOVE_OR_COPY< KeyId > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPT: // OPT
        value.YY_MOVE_OR_COPY< KeyOption > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPT_LIST: // OPT_LIST
        value.YY_MOVE_OR_COPY< Options > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RATIO: // RATIO
        value.YY_MOVE_OR_COPY< Signature > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REPEAT: // REPEAT
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BAR_CHECK: // BAR_CHECK
      case symbol_kind::S_BEAT_DIVISION: // BEAT_DIVISION
      case symbol_kind::S_BAR_POS: // BAR_POS
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  ScoreParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BAR_CONTENT: // BAR_CONTENT
      case symbol_kind::S_BAR: // BAR
        value.move< Bar > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_EXPR: // EXPR
        value.move< BarList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_KEY: // KEY
        value.move< KeyId > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPT: // OPT
        value.move< KeyOption > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_OPT_LIST: // OPT_LIST
        value.move< Options > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RATIO: // RATIO
        value.move< Signature > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_REPEAT: // REPEAT
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BAR_CHECK: // BAR_CHECK
      case symbol_kind::S_BEAT_DIVISION: // BEAT_DIVISION
      case symbol_kind::S_BAR_POS: // BAR_POS
        value.move< int > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  ScoreParser::stack_symbol_type&
  ScoreParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BAR_CONTENT: // BAR_CONTENT
      case symbol_kind::S_BAR: // BAR
        value.copy< Bar > (that.value);
        break;

      case symbol_kind::S_EXPR: // EXPR
        value.copy< BarList > (that.value);
        break;

      case symbol_kind::S_KEY: // KEY
        value.copy< KeyId > (that.value);
        break;

      case symbol_kind::S_OPT: // OPT
        value.copy< KeyOption > (that.value);
        break;

      case symbol_kind::S_OPT_LIST: // OPT_LIST
        value.copy< Options > (that.value);
        break;

      case symbol_kind::S_RATIO: // RATIO
        value.copy< Signature > (that.value);
        break;

      case symbol_kind::S_REPEAT: // REPEAT
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BAR_CHECK: // BAR_CHECK
      case symbol_kind::S_BEAT_DIVISION: // BEAT_DIVISION
      case symbol_kind::S_BAR_POS: // BAR_POS
        value.copy< int > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  ScoreParser::stack_symbol_type&
  ScoreParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BAR_CONTENT: // BAR_CONTENT
      case symbol_kind::S_BAR: // BAR
        value.move< Bar > (that.value);
        break;

      case symbol_kind::S_EXPR: // EXPR
        value.move< BarList > (that.value);
        break;

      case symbol_kind::S_KEY: // KEY
        value.move< KeyId > (that.value);
        break;

      case symbol_kind::S_OPT: // OPT
        value.move< KeyOption > (that.value);
        break;

      case symbol_kind::S_OPT_LIST: // OPT_LIST
        value.move< Options > (that.value);
        break;

      case symbol_kind::S_RATIO: // RATIO
        value.move< Signature > (that.value);
        break;

      case symbol_kind::S_REPEAT: // REPEAT
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BAR_CHECK: // BAR_CHECK
      case symbol_kind::S_BEAT_DIVISION: // BEAT_DIVISION
      case symbol_kind::S_BAR_POS: // BAR_POS
        value.move< int > (that.value);
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
  ScoreParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  ScoreParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  ScoreParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  ScoreParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  ScoreParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ScoreParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ScoreParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ScoreParser::debug_level_type
  ScoreParser::debug_level () const
  {
    return yydebug_;
  }

  void
  ScoreParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  ScoreParser::state_type
  ScoreParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  ScoreParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  ScoreParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  ScoreParser::operator() ()
  {
    return parse ();
  }

  int
  ScoreParser::parse ()
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
      case symbol_kind::S_BAR_CONTENT: // BAR_CONTENT
      case symbol_kind::S_BAR: // BAR
        yylhs.value.emplace< Bar > ();
        break;

      case symbol_kind::S_EXPR: // EXPR
        yylhs.value.emplace< BarList > ();
        break;

      case symbol_kind::S_KEY: // KEY
        yylhs.value.emplace< KeyId > ();
        break;

      case symbol_kind::S_OPT: // OPT
        yylhs.value.emplace< KeyOption > ();
        break;

      case symbol_kind::S_OPT_LIST: // OPT_LIST
        yylhs.value.emplace< Options > ();
        break;

      case symbol_kind::S_RATIO: // RATIO
        yylhs.value.emplace< Signature > ();
        break;

      case symbol_kind::S_REPEAT: // REPEAT
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BAR_CHECK: // BAR_CHECK
      case symbol_kind::S_BEAT_DIVISION: // BEAT_DIVISION
      case symbol_kind::S_BAR_POS: // BAR_POS
        yylhs.value.emplace< int > ();
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
  case 2: // BAR_POS: %empty
#line 122 "score.y"
             { yylhs.value.as < int > () = 0; }
#line 679 "score.parser.cpp"
    break;

  case 3: // BAR_POS: AT
#line 123 "score.y"
             { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 685 "score.parser.cpp"
    break;

  case 4: // OPT: FERMATA
#line 127 "score.y"
                              { yylhs.value.as < KeyOption > () = { KeyId('F', 0), Option() }; }
#line 691 "score.parser.cpp"
    break;

  case 5: // OPT: BAR_CHECK
#line 128 "score.y"
                              { yylhs.value.as < KeyOption > () = { KeyId('B', 0), Option(Option::NONE, yystack_[0].value.as < int > (), 0) }; }
#line 697 "score.parser.cpp"
    break;

  case 6: // OPT: BEAT_DIVISION
#line 129 "score.y"
                              { yylhs.value.as < KeyOption > () = { KeyId('D', 0), Option(Option::NONE, yystack_[0].value.as < int > (), 0) }; }
#line 703 "score.parser.cpp"
    break;

  case 7: // OPT: KEY INT BAR_POS
#line 130 "score.y"
                              { yylhs.value.as < KeyOption > () = { yystack_[2].value.as < KeyId > (), Option(Option::SET, yystack_[1].value.as < int > (), yystack_[0].value.as < int > ()) }; }
#line 709 "score.parser.cpp"
    break;

  case 8: // OPT: KEY INT CHANGE BAR_POS
#line 131 "score.y"
                              { yylhs.value.as < KeyOption > () = { yystack_[3].value.as < KeyId > (), Option(Option::CHANGE_BEGIN, yystack_[2].value.as < int > (), yystack_[0].value.as < int > ()) }; }
#line 715 "score.parser.cpp"
    break;

  case 9: // OPT: KEY CHANGE INT BAR_POS
#line 132 "score.y"
                              { yylhs.value.as < KeyOption > () = { yystack_[3].value.as < KeyId > (), Option(Option::CHANGE_END, yystack_[1].value.as < int > (), yystack_[0].value.as < int > ()) }; }
#line 721 "score.parser.cpp"
    break;

  case 10: // OPT_LIST: %empty
#line 136 "score.y"
                         {  }
#line 727 "score.parser.cpp"
    break;

  case 11: // OPT_LIST: OPT_LIST SPACE OPT
#line 137 "score.y"
                         {
        yylhs.value.as < Options > () = yystack_[2].value.as < Options > ();

        const auto& key_id = yystack_[0].value.as < KeyOption > ().first;
        const auto& opt = yystack_[0].value.as < KeyOption > ().second;

        switch(key_id.first) {
            case 'a':
                yylhs.value.as < Options > ().amp[key_id.second] = opt;
                break;
            case 'c':
                yylhs.value.as < Options > ().cue[key_id.second] = opt;
                break;
            case 'm':
                yylhs.value.as < Options > ().mark[key_id.second] = opt;
                break;
            case 'p':
                yylhs.value.as < Options > ().pan[key_id.second] = opt;
                break;
            case 'v':
                yylhs.value.as < Options > ().vol[key_id.second] = opt;
                break;
            case 't':
                yylhs.value.as < Options > ().tempo = opt;
                break;
            case 'B': // barcheck
                yylhs.value.as < Options > ().bar_check = opt.value;
                break;
            case 'F': // fermata
                yylhs.value.as < Options > ().fermata = true;
                break;
            case 'D': // beat division
                yylhs.value.as < Options > ().beat_division = opt.value;
                break;
            default:
                std::cerr << "unknown option: " << key_id.first << std::endl;
                break;
        }
      }
#line 771 "score.parser.cpp"
    break;

  case 12: // BAR_CONTENT: RATIO OPT_LIST
#line 179 "score.y"
                     { yylhs.value.as < Bar > ().sig = yystack_[1].value.as < Signature > (); yylhs.value.as < Bar > ().opts = yystack_[0].value.as < Options > (); }
#line 777 "score.parser.cpp"
    break;

  case 13: // BAR: PIPE BAR_CONTENT PIPE
#line 183 "score.y"
                            { yylhs.value.as < Bar > () = yystack_[1].value.as < Bar > (); }
#line 783 "score.parser.cpp"
    break;

  case 14: // EXPR: BAR
#line 187 "score.y"
                  { yylhs.value.as < BarList > ().push_back(yystack_[0].value.as < Bar > ()); }
#line 789 "score.parser.cpp"
    break;

  case 15: // EXPR: REPEAT BAR
#line 188 "score.y"
                  {
        yylhs.value.as < BarList > ().push_back(yystack_[0].value.as < Bar > ());
        const Bar b{ yystack_[0].value.as < Bar > ().sig, {} };
        for(int i = 1; i < yystack_[1].value.as < int > (); i++) {
            yylhs.value.as < BarList > ().push_back(b);
        }
      }
#line 801 "score.parser.cpp"
    break;

  case 16: // EXPRLIST: %empty
#line 198 "score.y"
             { }
#line 807 "score.parser.cpp"
    break;

  case 17: // EXPRLIST: EXPR
#line 199 "score.y"
           { lexer.bars.reserve(yystack_[0].value.as < BarList > ().size()); for(auto& b: yystack_[0].value.as < BarList > ()) lexer.bars.push_back(b); }
#line 813 "score.parser.cpp"
    break;

  case 18: // EXPRLIST: EXPRLIST SPACE EXPR
#line 200 "score.y"
                          {
        lexer.bars.reserve(yystack_[0].value.as < BarList > ().size());

        for(auto& b: yystack_[0].value.as < BarList > ())
            lexer.bars.push_back(b);
      }
#line 824 "score.parser.cpp"
    break;


#line 828 "score.parser.cpp"

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
  ScoreParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ScoreParser::yytnamerr_ (const char *yystr)
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
  ScoreParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // ScoreParser::context.
  ScoreParser::context::context (const ScoreParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  ScoreParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  ScoreParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  ScoreParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char ScoreParser::yypact_ninf_ = -22;

  const signed char ScoreParser::yytable_ninf_ = -1;

  const signed char
  ScoreParser::yypact_[] =
  {
       4,     1,    11,   -22,   -22,     9,   -22,    12,   -22,   -22,
       4,    14,   -22,   -22,    -6,   -22,   -22,   -22,    -2,   -22,
      10,     5,    13,    13,   -22,   -22,   -22,   -22
  };

  const signed char
  ScoreParser::yydefact_[] =
  {
      16,     0,     0,    14,    17,     0,    10,     0,    15,     1,
       0,    12,    13,    18,     0,     4,     5,     6,     0,    11,
       0,     2,     2,     2,     3,     7,     9,     8
  };

  const signed char
  ScoreParser::yypgoto_[] =
  {
     -22,   -21,   -22,   -22,   -22,    16,    15,   -22
  };

  const signed char
  ScoreParser::yydefgoto_[] =
  {
      -1,    25,    19,    11,     7,     3,     4,     5
  };

  const signed char
  ScoreParser::yytable_[] =
  {
      15,    26,    27,    20,    16,    17,    18,    21,     1,     9,
      23,     2,    10,    24,     6,     1,    12,    14,     8,    22,
       0,    24,     0,     0,     0,    13
  };

  const signed char
  ScoreParser::yycheck_[] =
  {
       6,    22,    23,     5,    10,    11,    12,     9,     4,     0,
       5,     7,     3,     8,    13,     4,     4,     3,     2,     9,
      -1,     8,    -1,    -1,    -1,    10
  };

  const signed char
  ScoreParser::yystos_[] =
  {
       0,     4,     7,    19,    20,    21,    13,    18,    19,     0,
       3,    17,     4,    20,     3,     6,    10,    11,    12,    16,
       5,     9,     9,     5,     8,    15,    15,    15
  };

  const signed char
  ScoreParser::yyr1_[] =
  {
       0,    14,    15,    15,    16,    16,    16,    16,    16,    16,
      17,    17,    18,    19,    20,    20,    21,    21,    21
  };

  const signed char
  ScoreParser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     1,     1,     3,     4,     4,
       0,     3,     2,     3,     1,     2,     0,     1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const ScoreParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "SPACE", "PIPE",
  "CHANGE", "FERMATA", "REPEAT", "AT", "INT", "BAR_CHECK", "BEAT_DIVISION",
  "KEY", "RATIO", "$accept", "BAR_POS", "OPT", "OPT_LIST", "BAR_CONTENT",
  "BAR", "EXPR", "EXPRLIST", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  ScoreParser::yyrline_[] =
  {
       0,   122,   122,   123,   127,   128,   129,   130,   131,   132,
     136,   137,   179,   183,   187,   188,   198,   199,   200
  };

  void
  ScoreParser::yy_stack_print_ () const
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
  ScoreParser::yy_reduce_print_ (int yyrule) const
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


#line 7 "score.y"
} } // ceammc::score
#line 1296 "score.parser.cpp"

#line 208 "score.y"


void ceammc::score::ScoreParser::error(const std::string& err_message)
{
    std::cerr << err_message << "\n";
//    lexer.setErrorMsg(err_message);
}


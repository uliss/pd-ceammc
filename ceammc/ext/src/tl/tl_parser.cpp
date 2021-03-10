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

// "%code top" blocks.
#line 34 "tl_parser.y"

    # undef yylex
    # define yylex lexer.lex

    # include "tl_lexer.h"

#line 46 "tl_parser.cpp"




#include "tl_parser.hpp"


// Unqualified %code blocks.
#line 43 "tl_parser.y"

    # include "tl_cmd_driver.h"

#line 59 "tl_parser.cpp"


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

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


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

#line 7 "tl_parser.y"
namespace ceammc { namespace tl {
#line 152 "tl_parser.cpp"

  /// Build a parser object.
  TlParser::TlParser (ceammc::tl::TlLexer& lexer_yyarg, ceammc::tl::TlCmdDriver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      driver (driver_yyarg)
  {}

  TlParser::~TlParser ()
  {}

  TlParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  TlParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  TlParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  TlParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  TlParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  TlParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  TlParser::symbol_kind_type
  TlParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  TlParser::stack_symbol_type::stack_symbol_type ()
  {}

  TlParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT: // "float number"
      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
      case symbol_kind::S_18_time_in_msec_: // "time in msec"
      case symbol_kind::S_time: // time
      case symbol_kind::S_ms_time: // ms_time
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // "integer number"
        value.YY_MOVE_OR_COPY< long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_WORD: // "word"
      case symbol_kind::S_UNIT: // "unit"
      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  TlParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT: // "float number"
      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
      case symbol_kind::S_18_time_in_msec_: // "time in msec"
      case symbol_kind::S_time: // time
      case symbol_kind::S_ms_time: // ms_time
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // "integer number"
        value.move< long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_WORD: // "word"
      case symbol_kind::S_UNIT: // "unit"
      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  TlParser::stack_symbol_type&
  TlParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT: // "float number"
      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
      case symbol_kind::S_18_time_in_msec_: // "time in msec"
      case symbol_kind::S_time: // time
      case symbol_kind::S_ms_time: // ms_time
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INT: // "integer number"
        value.copy< long > (that.value);
        break;

      case symbol_kind::S_WORD: // "word"
      case symbol_kind::S_UNIT: // "unit"
      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  TlParser::stack_symbol_type&
  TlParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT: // "float number"
      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
      case symbol_kind::S_18_time_in_msec_: // "time in msec"
      case symbol_kind::S_time: // time
      case symbol_kind::S_ms_time: // ms_time
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INT: // "integer number"
        value.move< long > (that.value);
        break;

      case symbol_kind::S_WORD: // "word"
      case symbol_kind::S_UNIT: // "unit"
      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  TlParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  TlParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_INT: // "integer number"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < long > (); }
#line 376 "tl_parser.cpp"
        break;

      case symbol_kind::S_FLOAT: // "float number"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < double > (); }
#line 382 "tl_parser.cpp"
        break;

      case symbol_kind::S_WORD: // "word"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 388 "tl_parser.cpp"
        break;

      case symbol_kind::S_UNIT: // "unit"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 394 "tl_parser.cpp"
        break;

      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < std::string > (); }
#line 400 "tl_parser.cpp"
        break;

      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < double > (); }
#line 406 "tl_parser.cpp"
        break;

      case symbol_kind::S_18_time_in_msec_: // "time in msec"
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < double > (); }
#line 412 "tl_parser.cpp"
        break;

      case symbol_kind::S_time: // time
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < double > (); }
#line 418 "tl_parser.cpp"
        break;

      case symbol_kind::S_ms_time: // ms_time
#line 66 "tl_parser.y"
                 { yyoutput << yysym.value.template as < double > (); }
#line 424 "tl_parser.cpp"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  TlParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  TlParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  TlParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  TlParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  TlParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  TlParser::debug_level_type
  TlParser::debug_level () const
  {
    return yydebug_;
  }

  void
  TlParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  TlParser::state_type
  TlParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  TlParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  TlParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  TlParser::operator() ()
  {
    return parse ();
  }

  int
  TlParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

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
      case symbol_kind::S_FLOAT: // "float number"
      case symbol_kind::S_17_time_in_any_format_: // "time in any format"
      case symbol_kind::S_18_time_in_msec_: // "time in msec"
      case symbol_kind::S_time: // time
      case symbol_kind::S_ms_time: // ms_time
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INT: // "integer number"
        yylhs.value.emplace< long > ();
        break;

      case symbol_kind::S_WORD: // "word"
      case symbol_kind::S_UNIT: // "unit"
      case symbol_kind::S_CLOCK_TIME: // "clock time in format 00:00:00.000"
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 7: // add_command: "add action" time
#line 78 "tl_parser.y"
                                { driver.addAbsEvent("", yystack_[0].value.as < double > ()); }
#line 695 "tl_parser.cpp"
    break;

  case 8: // add_command: "add action" "word" time
#line 79 "tl_parser.y"
                                { driver.addAbsEvent(yystack_[1].value.as < std::string > (), yystack_[0].value.as < double > ()); }
#line 701 "tl_parser.cpp"
    break;

  case 9: // add_command: "add action" "word" time BEFORE "word"
#line 80 "tl_parser.y"
                                { driver.addRelEvent(yystack_[3].value.as < std::string > (), -yystack_[2].value.as < double > (), yystack_[0].value.as < std::string > ()); }
#line 707 "tl_parser.cpp"
    break;

  case 10: // add_command: "add action" "word" time AFTER "word"
#line 81 "tl_parser.y"
                                { driver.addRelEvent(yystack_[3].value.as < std::string > (), yystack_[2].value.as < double > (), yystack_[0].value.as < std::string > ()); }
#line 713 "tl_parser.cpp"
    break;

  case 11: // remove_command: "remove action" "word"
#line 84 "tl_parser.y"
                                { driver.removeEvent(yystack_[0].value.as < std::string > ()); }
#line 719 "tl_parser.cpp"
    break;

  case 12: // remove_command: "remove action" "integer number"
#line 85 "tl_parser.y"
                                { driver.removeEventAtPos(yystack_[0].value.as < long > ()); }
#line 725 "tl_parser.cpp"
    break;

  case 13: // remove_command_at: "remove at specified time ms" time
#line 88 "tl_parser.y"
                                { driver.removeEventAtTime(yystack_[0].value.as < double > ()); }
#line 731 "tl_parser.cpp"
    break;

  case 14: // clear_command: "clear action"
#line 90 "tl_parser.y"
                                { driver.clearAll(); }
#line 737 "tl_parser.cpp"
    break;

  case 15: // move_command: "move to named event" "word"
#line 93 "tl_parser.y"
                                { driver.moveToEvent(yystack_[0].value.as < std::string > ()); }
#line 743 "tl_parser.cpp"
    break;

  case 16: // move_command: "move to named event" "integer number"
#line 94 "tl_parser.y"
                                { driver.moveToEvent(yystack_[0].value.as < long > ()); }
#line 749 "tl_parser.cpp"
    break;

  case 17: // move_command: "move to time" time
#line 95 "tl_parser.y"
                                { driver.moveToTime(yystack_[0].value.as < double > ()); }
#line 755 "tl_parser.cpp"
    break;

  case 18: // time: "clock time in format 00:00:00.000"
#line 98 "tl_parser.y"
                                { yylhs.value.as < double > () = driver.parseClock(yystack_[0].value.as < std::string > ()); }
#line 761 "tl_parser.cpp"
    break;

  case 19: // time: ms_time
#line 99 "tl_parser.y"
                                { yylhs.value.as < double > () = double(yystack_[0].value.as < double > ()); }
#line 767 "tl_parser.cpp"
    break;

  case 20: // ms_time: "integer number" "unit"
#line 102 "tl_parser.y"
                                { yylhs.value.as < double > () = driver.parseTime(yystack_[1].value.as < long > (), yystack_[0].value.as < std::string > ()); }
#line 773 "tl_parser.cpp"
    break;

  case 21: // ms_time: "float number" "unit"
#line 103 "tl_parser.y"
                                { yylhs.value.as < double > () = driver.parseTime(yystack_[1].value.as < double > (), yystack_[0].value.as < std::string > ()); }
#line 779 "tl_parser.cpp"
    break;


#line 783 "tl_parser.cpp"

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
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
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

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

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
  TlParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  TlParser::yytnamerr_ (const char *yystr)
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
  TlParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // TlParser::context.
  TlParser::context::context (const TlParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  TlParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  TlParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  TlParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char TlParser::yypact_ninf_ = -9;

  const signed char TlParser::yytable_ninf_ = -1;

  const signed char
  TlParser::yypact_[] =
  {
      12,    -8,    -7,    -9,    13,    -1,    -1,     4,    -9,    -9,
      -9,    -9,    -9,    -4,     7,    -1,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,     2,    10,
      11,    -9,    -9
  };

  const signed char
  TlParser::yydefact_[] =
  {
       0,     0,     0,    14,     0,     0,     0,     0,     2,     3,
       4,     6,     5,     0,     0,     0,    18,     7,    19,    12,
      11,    16,    15,    17,    13,     1,    20,    21,     8,     0,
       0,    10,     9
  };

  const signed char
  TlParser::yypgoto_[] =
  {
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -5,    -9
  };

  const signed char
  TlParser::yydefgoto_[] =
  {
      -1,     7,     8,     9,    10,    11,    12,    17,    18
  };

  const signed char
  TlParser::yytable_[] =
  {
      23,    24,    13,    19,    25,    14,    15,    20,    16,    13,
      28,    26,    14,    29,    30,    16,     1,     2,     3,     4,
       5,     6,    27,    21,    31,    32,     0,    22
  };

  const signed char
  TlParser::yycheck_[] =
  {
       5,     6,    10,    10,     0,    13,    14,    14,    16,    10,
      15,    15,    13,    11,    12,    16,     4,     5,     6,     7,
       8,     9,    15,    10,    14,    14,    -1,    14
  };

  const signed char
  TlParser::yystos_[] =
  {
       0,     4,     5,     6,     7,     8,     9,    20,    21,    22,
      23,    24,    25,    10,    13,    14,    16,    26,    27,    10,
      14,    10,    14,    26,    26,     0,    15,    15,    26,    11,
      12,    14,    14
  };

  const signed char
  TlParser::yyr1_[] =
  {
       0,    19,    20,    20,    20,    20,    20,    21,    21,    21,
      21,    22,    22,    23,    24,    25,    25,    25,    26,    26,
      27,    27
  };

  const signed char
  TlParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     2,     3,     5,
       5,     2,     2,     2,     1,     2,     2,     2,     1,     1,
       2,     2
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const TlParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\" \"",
  "\"add action\"", "\"remove action\"", "\"clear action\"",
  "\"move to named event\"", "\"move to time\"",
  "\"remove at specified time ms\"", "\"integer number\"", "AFTER",
  "BEFORE", "\"float number\"", "\"word\"", "\"unit\"",
  "\"clock time in format 00:00:00.000\"", "\"time in any format\"",
  "\"time in msec\"", "$accept", "command", "add_command",
  "remove_command", "remove_command_at", "clear_command", "move_command",
  "time", "ms_time", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  TlParser::yyrline_[] =
  {
       0,    71,    71,    72,    73,    74,    75,    78,    79,    80,
      81,    84,    85,    88,    90,    93,    94,    95,    98,    99,
     102,   103
  };

  void
  TlParser::yy_stack_print_ () const
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
  TlParser::yy_reduce_print_ (int yyrule) const
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


#line 7 "tl_parser.y"
} } // ceammc::tl
#line 1266 "tl_parser.cpp"

#line 105 "tl_parser.y"


void ceammc::tl::TlParser::error(const ceammc::tl::TlParser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

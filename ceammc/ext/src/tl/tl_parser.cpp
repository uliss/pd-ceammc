// A Bison parser, made by GNU Bison 3.0.5.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

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


// First part of user declarations.

#line 37 "tl_parser.cpp" // lalr1.cc:406

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "tl_parser.hpp"

// User implementation prologue.

#line 51 "tl_parser.cpp" // lalr1.cc:414
// Unqualified %code blocks.
#line 36 "tl_parser.y" // lalr1.cc:415

#include "tl_cmd_driver.h"

#line 57 "tl_parser.cpp" // lalr1.cc:415


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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


namespace yy {
#line 143 "tl_parser.cpp" // lalr1.cc:481

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  TlCmdParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
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
  TlCmdParser::TlCmdParser (ceammc::tl::TlCmdDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  TlCmdParser::~TlCmdParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  TlCmdParser::by_state::by_state ()
    : state (empty_state)
  {}

  TlCmdParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  void
  TlCmdParser::by_state::clear ()
  {
    state = empty_state;
  }

  void
  TlCmdParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  TlCmdParser::by_state::by_state (state_type s)
    : state (s)
  {}

  TlCmdParser::symbol_number_type
  TlCmdParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  TlCmdParser::stack_symbol_type::stack_symbol_type ()
  {}

  TlCmdParser::stack_symbol_type::stack_symbol_type (const stack_symbol_type& that)
    : super_type (that.state, that.location)
  {
    switch (that.type_get ())
    {
      case 13: // "float number"
      case 17: // "time in any format"
      case 18: // "time in msec"
      case 26: // time
      case 27: // ms_time
        value.copy< double > (that.value);
        break;

      case 10: // "integer number"
        value.copy< long > (that.value);
        break;

      case 14: // "word"
      case 15: // "unit"
      case 16: // "clock time in format 00:00:00.000"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

  }

  TlCmdParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    switch (that.type_get ())
    {
      case 13: // "float number"
      case 17: // "time in any format"
      case 18: // "time in msec"
      case 26: // time
      case 27: // ms_time
        value.move< double > (that.value);
        break;

      case 10: // "integer number"
        value.move< long > (that.value);
        break;

      case 14: // "word"
      case 15: // "unit"
      case 16: // "clock time in format 00:00:00.000"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  TlCmdParser::stack_symbol_type&
  TlCmdParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 13: // "float number"
      case 17: // "time in any format"
      case 18: // "time in msec"
      case 26: // time
      case 27: // ms_time
        value.copy< double > (that.value);
        break;

      case 10: // "integer number"
        value.copy< long > (that.value);
        break;

      case 14: // "word"
      case 15: // "unit"
      case 16: // "clock time in format 00:00:00.000"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  void
  TlCmdParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  TlCmdParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 10: // "integer number"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< long > (); }
#line 364 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 13: // "float number"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< double > (); }
#line 371 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 14: // "word"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< std::string > (); }
#line 378 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 15: // "unit"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< std::string > (); }
#line 385 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 16: // "clock time in format 00:00:00.000"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< std::string > (); }
#line 392 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 17: // "time in any format"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< double > (); }
#line 399 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 18: // "time in msec"

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< double > (); }
#line 406 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 26: // time

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< double > (); }
#line 413 "tl_parser.cpp" // lalr1.cc:635
        break;

      case 27: // ms_time

#line 60 "tl_parser.y" // lalr1.cc:635
        { yyoutput << yysym.value.template as< double > (); }
#line 420 "tl_parser.cpp" // lalr1.cc:635
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  TlCmdParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  void
  TlCmdParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  void
  TlCmdParser::yypop_ (unsigned n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  TlCmdParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  TlCmdParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  TlCmdParser::debug_level_type
  TlCmdParser::debug_level () const
  {
    return yydebug_;
  }

  void
  TlCmdParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  TlCmdParser::state_type
  TlCmdParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  TlCmdParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  TlCmdParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  TlCmdParser::parse ()
  {
    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
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
  | yyreduce -- Do a reduction.  |
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
      case 13: // "float number"
      case 17: // "time in any format"
      case 18: // "time in msec"
      case 26: // time
      case 27: // ms_time
        yylhs.value.build< double > ();
        break;

      case 10: // "integer number"
        yylhs.value.build< long > ();
        break;

      case 14: // "word"
      case 15: // "unit"
      case 16: // "clock time in format 00:00:00.000"
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 7:
#line 72 "tl_parser.y" // lalr1.cc:856
    { driver.addAbsEvent("", yystack_[0].value.as< double > ()); }
#line 656 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 8:
#line 73 "tl_parser.y" // lalr1.cc:856
    { driver.addAbsEvent(yystack_[1].value.as< std::string > (), yystack_[0].value.as< double > ()); }
#line 662 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 9:
#line 74 "tl_parser.y" // lalr1.cc:856
    { driver.addRelEvent(yystack_[3].value.as< std::string > (), -yystack_[2].value.as< double > (), yystack_[0].value.as< std::string > ()); }
#line 668 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 10:
#line 75 "tl_parser.y" // lalr1.cc:856
    { driver.addRelEvent(yystack_[3].value.as< std::string > (), yystack_[2].value.as< double > (), yystack_[0].value.as< std::string > ()); }
#line 674 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 11:
#line 78 "tl_parser.y" // lalr1.cc:856
    { driver.removeEvent(yystack_[0].value.as< std::string > ()); }
#line 680 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 12:
#line 79 "tl_parser.y" // lalr1.cc:856
    { driver.removeEventAtPos(yystack_[0].value.as< long > ()); }
#line 686 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 13:
#line 82 "tl_parser.y" // lalr1.cc:856
    { driver.removeEventAtTime(yystack_[0].value.as< double > ()); }
#line 692 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 14:
#line 84 "tl_parser.y" // lalr1.cc:856
    { driver.clearAll(); }
#line 698 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 15:
#line 87 "tl_parser.y" // lalr1.cc:856
    { driver.moveToEvent(yystack_[0].value.as< std::string > ()); }
#line 704 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 16:
#line 88 "tl_parser.y" // lalr1.cc:856
    { driver.moveToEvent(yystack_[0].value.as< long > ()); }
#line 710 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 17:
#line 89 "tl_parser.y" // lalr1.cc:856
    { driver.moveToTime(yystack_[0].value.as< double > ()); }
#line 716 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 18:
#line 92 "tl_parser.y" // lalr1.cc:856
    { yylhs.value.as< double > () = driver.parseClock(yystack_[0].value.as< std::string > ()); }
#line 722 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 19:
#line 93 "tl_parser.y" // lalr1.cc:856
    { yylhs.value.as< double > () = double(yystack_[0].value.as< double > ()); }
#line 728 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 20:
#line 96 "tl_parser.y" // lalr1.cc:856
    { yylhs.value.as< double > () = driver.parseTime(yystack_[1].value.as< long > (), yystack_[0].value.as< std::string > ()); }
#line 734 "tl_parser.cpp" // lalr1.cc:856
    break;

  case 21:
#line 97 "tl_parser.y" // lalr1.cc:856
    { yylhs.value.as< double > () = driver.parseTime(yystack_[1].value.as< double > (), yystack_[0].value.as< std::string > ()); }
#line 740 "tl_parser.cpp" // lalr1.cc:856
    break;


#line 744 "tl_parser.cpp" // lalr1.cc:856
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
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
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

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
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  TlCmdParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  TlCmdParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
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
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
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
    size_t yyi = 0;
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


  const signed char TlCmdParser::yypact_ninf_ = -9;

  const signed char TlCmdParser::yytable_ninf_ = -1;

  const signed char
  TlCmdParser::yypact_[] =
  {
      12,    -8,    -7,    -9,    13,    -1,    -1,     4,    -9,    -9,
      -9,    -9,    -9,    -4,     7,    -1,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,     2,    10,
      11,    -9,    -9
  };

  const unsigned char
  TlCmdParser::yydefact_[] =
  {
       0,     0,     0,    14,     0,     0,     0,     0,     2,     3,
       4,     6,     5,     0,     0,     0,    18,     7,    19,    12,
      11,    16,    15,    17,    13,     1,    20,    21,     8,     0,
       0,    10,     9
  };

  const signed char
  TlCmdParser::yypgoto_[] =
  {
      -9,    -9,    -9,    -9,    -9,    -9,    -9,    -5,    -9
  };

  const signed char
  TlCmdParser::yydefgoto_[] =
  {
      -1,     7,     8,     9,    10,    11,    12,    17,    18
  };

  const unsigned char
  TlCmdParser::yytable_[] =
  {
      23,    24,    13,    19,    25,    14,    15,    20,    16,    13,
      28,    26,    14,    29,    30,    16,     1,     2,     3,     4,
       5,     6,    27,    21,    31,    32,     0,    22
  };

  const signed char
  TlCmdParser::yycheck_[] =
  {
       5,     6,    10,    10,     0,    13,    14,    14,    16,    10,
      15,    15,    13,    11,    12,    16,     4,     5,     6,     7,
       8,     9,    15,    10,    14,    14,    -1,    14
  };

  const unsigned char
  TlCmdParser::yystos_[] =
  {
       0,     4,     5,     6,     7,     8,     9,    20,    21,    22,
      23,    24,    25,    10,    13,    14,    16,    26,    27,    10,
      14,    10,    14,    26,    26,     0,    15,    15,    26,    11,
      12,    14,    14
  };

  const unsigned char
  TlCmdParser::yyr1_[] =
  {
       0,    19,    20,    20,    20,    20,    20,    21,    21,    21,
      21,    22,    22,    23,    24,    25,    25,    25,    26,    26,
      27,    27
  };

  const unsigned char
  TlCmdParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     2,     3,     5,
       5,     2,     2,     2,     1,     2,     2,     2,     1,     1,
       2,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const TlCmdParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\" \"", "\"add action\"",
  "\"remove action\"", "\"clear action\"", "\"move to named event\"",
  "\"move to time\"", "\"remove at specified time ms\"",
  "\"integer number\"", "AFTER", "BEFORE", "\"float number\"", "\"word\"",
  "\"unit\"", "\"clock time in format 00:00:00.000\"",
  "\"time in any format\"", "\"time in msec\"", "$accept", "command",
  "add_command", "remove_command", "remove_command_at", "clear_command",
  "move_command", "time", "ms_time", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  TlCmdParser::yyrline_[] =
  {
       0,    65,    65,    66,    67,    68,    69,    72,    73,    74,
      75,    78,    79,    82,    84,    87,    88,    89,    92,    93,
      96,    97
  };

  // Print the state stack on the debug stream.
  void
  TlCmdParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  TlCmdParser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
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



} // yy
#line 1131 "tl_parser.cpp" // lalr1.cc:1163
#line 99 "tl_parser.y" // lalr1.cc:1164


void yy::TlCmdParser::error(const location_type& l, const std::string& m)
{
    driver.error(l, m);
}

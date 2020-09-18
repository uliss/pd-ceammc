// A Bison parser, made by GNU Bison 3.6.4.

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





#include "grainprops.parser.hpp"


// Unqualified %code blocks.
#line 29 "grain_properties.yy"

    # include <string>
    # include "grainprops.lexer.h"
    # include "grain.h"
    # include "grain_random.h"
    # include "ceammc_log.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static inline float frand(float a, float b) { return ceammc::GrainRandom::urandf(a, b); }
    static inline float frand_closed(float a, float b) { return ceammc::GrainRandom::urandf_closed(a, b); }

#line 60 "grainprops.parser.cpp"


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

#line 7 "grain_properties.yy"
namespace ceammc {
#line 134 "grainprops.parser.cpp"

  /// Build a parser object.
  GrainPropertiesParser::GrainPropertiesParser (ceammc::GrainPropertiesLexer& lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg)
  {}

  GrainPropertiesParser::~GrainPropertiesParser ()
  {}

  GrainPropertiesParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  GrainPropertiesParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  GrainPropertiesParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  GrainPropertiesParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  GrainPropertiesParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  GrainPropertiesParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  GrainPropertiesParser::symbol_kind_type
  GrainPropertiesParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  GrainPropertiesParser::stack_symbol_type::stack_symbol_type ()
  {}

  GrainPropertiesParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case 37: // TIME
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 24: // FLOAT
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case 20: // OVERFLOW_CLIP
      case 21: // OVERFLOW_WRAP
      case 22: // OVERFLOW_FOLD
      case 23: // PAN_MODE
      case 25: // INTEGER
      case 30: // OVERFLOW_MODE
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 26: // STRING
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

  GrainPropertiesParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case 37: // TIME
        value.move< double > (YY_MOVE (that.value));
        break;

      case 24: // FLOAT
        value.move< float > (YY_MOVE (that.value));
        break;

      case 20: // OVERFLOW_CLIP
      case 21: // OVERFLOW_WRAP
      case 22: // OVERFLOW_FOLD
      case 23: // PAN_MODE
      case 25: // INTEGER
      case 30: // OVERFLOW_MODE
        value.move< int > (YY_MOVE (that.value));
        break;

      case 26: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  GrainPropertiesParser::stack_symbol_type&
  GrainPropertiesParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case 37: // TIME
        value.copy< double > (that.value);
        break;

      case 24: // FLOAT
        value.copy< float > (that.value);
        break;

      case 20: // OVERFLOW_CLIP
      case 21: // OVERFLOW_WRAP
      case 22: // OVERFLOW_FOLD
      case 23: // PAN_MODE
      case 25: // INTEGER
      case 30: // OVERFLOW_MODE
        value.copy< int > (that.value);
        break;

      case 26: // STRING
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  GrainPropertiesParser::stack_symbol_type&
  GrainPropertiesParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case 37: // TIME
        value.move< double > (that.value);
        break;

      case 24: // FLOAT
        value.move< float > (that.value);
        break;

      case 20: // OVERFLOW_CLIP
      case 21: // OVERFLOW_WRAP
      case 22: // OVERFLOW_FOLD
      case 23: // PAN_MODE
      case 25: // INTEGER
      case 30: // OVERFLOW_MODE
        value.move< int > (that.value);
        break;

      case 26: // STRING
        value.move< std::string > (that.value);
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
  GrainPropertiesParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  GrainPropertiesParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  GrainPropertiesParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  GrainPropertiesParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  GrainPropertiesParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  GrainPropertiesParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  GrainPropertiesParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  GrainPropertiesParser::debug_level_type
  GrainPropertiesParser::debug_level () const
  {
    return yydebug_;
  }

  void
  GrainPropertiesParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  GrainPropertiesParser::state_type
  GrainPropertiesParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  GrainPropertiesParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  GrainPropertiesParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  GrainPropertiesParser::operator() ()
  {
    return parse ();
  }

  int
  GrainPropertiesParser::parse ()
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
      case 37: // TIME
        yylhs.value.emplace< double > ();
        break;

      case 24: // FLOAT
        yylhs.value.emplace< float > ();
        break;

      case 20: // OVERFLOW_CLIP
      case 21: // OVERFLOW_WRAP
      case 22: // OVERFLOW_FOLD
      case 23: // PAN_MODE
      case 25: // INTEGER
      case 30: // OVERFLOW_MODE
        yylhs.value.emplace< int > ();
        break;

      case 26: // STRING
        yylhs.value.emplace< std::string > ();
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
  case 4:
#line 82 "grain_properties.yy"
                                        { lexer.grain()->setSpeed(yystack_[0].value.as < float > ()); }
#line 621 "grainprops.parser.cpp"
    break;

  case 5:
#line 83 "grain_properties.yy"
                                        { lexer.grain()->setSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 627 "grainprops.parser.cpp"
    break;

  case 6:
#line 84 "grain_properties.yy"
                                        { lexer.grain()->setSpeedExpr(yystack_[0].value.as < std::string > ()); }
#line 633 "grainprops.parser.cpp"
    break;

  case 7:
#line 85 "grain_properties.yy"
                                        { lexer.grain()->setSpeed(yystack_[0].value.as < float > ()); }
#line 639 "grainprops.parser.cpp"
    break;

  case 8:
#line 86 "grain_properties.yy"
                                        { lexer.grain()->setSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 645 "grainprops.parser.cpp"
    break;

  case 10:
#line 88 "grain_properties.yy"
                                        { lexer.grain()->addSpeed(yystack_[0].value.as < float > ()); }
#line 651 "grainprops.parser.cpp"
    break;

  case 11:
#line 89 "grain_properties.yy"
                                        { lexer.grain()->addSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 657 "grainprops.parser.cpp"
    break;

  case 19:
#line 97 "grain_properties.yy"
                                        {
                                          auto d = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([d](Grain* g){ return g->speed() + d; });
                                        }
#line 666 "grainprops.parser.cpp"
    break;

  case 20:
#line 102 "grain_properties.yy"
                                        {
                                          auto a = yystack_[1].value.as < float > ();
                                          auto b = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([a,b](Grain* g){ return g->speed() + frand(a, b); });
                                        }
#line 676 "grainprops.parser.cpp"
    break;

  case 22:
#line 109 "grain_properties.yy"
                                        {
                                          auto v = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([v](Grain*){ return v; });
                                        }
#line 685 "grainprops.parser.cpp"
    break;

  case 23:
#line 114 "grain_properties.yy"
                                        {
                                          auto a = yystack_[1].value.as < float > ();
                                          auto b = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([a,b](Grain*){ return frand(a, b); });
                                        }
#line 695 "grainprops.parser.cpp"
    break;

  case 25:
#line 120 "grain_properties.yy"
                                        { lexer.grain()->setSpeedRange(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ()); }
#line 701 "grainprops.parser.cpp"
    break;

  case 26:
#line 124 "grain_properties.yy"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 707 "grainprops.parser.cpp"
    break;

  case 27:
#line 125 "grain_properties.yy"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 713 "grainprops.parser.cpp"
    break;

  case 28:
#line 126 "grain_properties.yy"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 719 "grainprops.parser.cpp"
    break;

  case 29:
#line 130 "grain_properties.yy"
                                      { lexer.grain()->setPan(yystack_[0].value.as < float > ()); }
#line 725 "grainprops.parser.cpp"
    break;

  case 30:
#line 132 "grain_properties.yy"
                                      { lexer.grain()->setPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 731 "grainprops.parser.cpp"
    break;

  case 32:
#line 134 "grain_properties.yy"
                                      { lexer.grain()->setPan(yystack_[0].value.as < float > ()); }
#line 737 "grainprops.parser.cpp"
    break;

  case 33:
#line 135 "grain_properties.yy"
                                      { lexer.grain()->setPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 743 "grainprops.parser.cpp"
    break;

  case 35:
#line 137 "grain_properties.yy"
                                      { lexer.grain()->addPan(yystack_[0].value.as < float > ()); }
#line 749 "grainprops.parser.cpp"
    break;

  case 36:
#line 138 "grain_properties.yy"
                                      { lexer.grain()->addPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 755 "grainprops.parser.cpp"
    break;

  case 44:
#line 146 "grain_properties.yy"
                                             {
                                              auto v = yystack_[0].value.as < float > ();
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([v,g](){ return g->pan() + v; });
                                             }
#line 765 "grainprops.parser.cpp"
    break;

  case 45:
#line 151 "grain_properties.yy"
                                             {
                                              auto a = yystack_[1].value.as < float > ();
                                              auto b = yystack_[0].value.as < float > ();
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([a,b,g](){ return g->pan() + frand_closed(a, b); });
                                             }
#line 776 "grainprops.parser.cpp"
    break;

  case 47:
#line 158 "grain_properties.yy"
                                             { auto v = yystack_[0].value.as < float > (); lexer.grain()->setPanDone([v](){ return v; }); }
#line 782 "grainprops.parser.cpp"
    break;

  case 48:
#line 159 "grain_properties.yy"
                                             {
                                               auto a = yystack_[1].value.as < float > ();
                                               auto b = yystack_[0].value.as < float > ();
                                               lexer.grain()->setPanDone([a,b](){ return frand_closed(a, b); });
                                             }
#line 792 "grainprops.parser.cpp"
    break;

  case 50:
#line 165 "grain_properties.yy"
                                             { lexer.grain()->setPanOverflow(static_cast<Grain::PanOverflow>(yystack_[0].value.as < int > ())); }
#line 798 "grainprops.parser.cpp"
    break;

  case 51:
#line 166 "grain_properties.yy"
                                             { lexer.grain()->setPanMode(static_cast<Grain::PanMode>(yystack_[0].value.as < int > ())); }
#line 804 "grainprops.parser.cpp"
    break;

  case 52:
#line 171 "grain_properties.yy"
                               { lexer.grain()->array_pos_samp = yystack_[0].value.as < double > (); }
#line 810 "grainprops.parser.cpp"
    break;

  case 53:
#line 172 "grain_properties.yy"
                               { lexer.grain()->array_pos_samp = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 816 "grainprops.parser.cpp"
    break;

  case 54:
#line 176 "grain_properties.yy"
                                   { lexer.grain()->length_samp = yystack_[0].value.as < double > (); }
#line 822 "grainprops.parser.cpp"
    break;

  case 55:
#line 177 "grain_properties.yy"
                                   { lexer.grain()->length_samp = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 828 "grainprops.parser.cpp"
    break;

  case 56:
#line 181 "grain_properties.yy"
                                   { lexer.grain()->play_pos = yystack_[0].value.as < double > (); }
#line 834 "grainprops.parser.cpp"
    break;

  case 57:
#line 182 "grain_properties.yy"
                                   { lexer.grain()->play_pos = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 840 "grainprops.parser.cpp"
    break;

  case 66:
#line 200 "grain_properties.yy"
            { yylhs.value.as < double > () = yystack_[0].value.as < float > (); }
#line 846 "grainprops.parser.cpp"
    break;

  case 67:
#line 201 "grain_properties.yy"
                 { yylhs.value.as < double > () = sys_getsr() * 0.001 * yystack_[1].value.as < float > (); }
#line 852 "grainprops.parser.cpp"
    break;

  case 68:
#line 202 "grain_properties.yy"
                 { yylhs.value.as < double > () = sys_getsr() * yystack_[1].value.as < float > (); }
#line 858 "grainprops.parser.cpp"
    break;


#line 862 "grainprops.parser.cpp"

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
  GrainPropertiesParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  GrainPropertiesParser::yytnamerr_ (const char *yystr)
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
  GrainPropertiesParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // GrainPropertiesParser::context.
  GrainPropertiesParser::context::context (const GrainPropertiesParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  GrainPropertiesParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  GrainPropertiesParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  GrainPropertiesParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char GrainPropertiesParser::yypact_ninf_ = -12;

  const signed char GrainPropertiesParser::yytable_ninf_ = -1;

  const signed char
  GrainPropertiesParser::yypact_[] =
  {
      76,    -7,    54,     7,     1,     5,    40,   -12,   -12,   -12,
     -12,   -12,   -12,   -12,    76,    11,   -11,    65,    35,     0,
      17,    77,    33,     4,   -12,    45,   -12,    13,    79,    19,
      27,    81,    50,    37,   -12,    64,    78,   -12,    64,   -12,
      64,   -12,   -12,   -12,    73,   -12,   -12,   -12,   -12,    29,
      30,    74,    75,   -12,    80,    82,   -12,    31,    41,   -12,
     -12,   -12,    83,    43,    48,    85,    84,   -12,    87,    86,
     -12,    51,    53,   -12,    89,    64,   -12,   -12,    64,    64,
     -12,    90,    91,   -12,    92,    93,   -12,    94,   -12,    96,
     -12,    97,    98,   -12,    99,   100,   -12,   -12,   101,   102,
     -12,   103,   104,   -12,   105,   -12,   107,   -12,   108,   109,
     -12,   110,   111,   -12,   -12,   -12,   -12,   -12,   112,   -12,
     114,   -12,   -12,   -12,   115,   -12,   116,   -12,   117,   -12,
     118,   -12,   -12,   -12,   119,   -12,   120,   -12,   -12,   -12,
     -12,   -12,   -12,   -12,   -12,   -12
  };

  const signed char
  GrainPropertiesParser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,    60,    59,    58,
      61,    62,    63,    64,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     4,     0,    66,    52,     0,    54,
       0,    56,    65,     1,     0,    26,    27,    28,    50,     0,
       0,     0,     0,    35,     0,     0,    32,     0,     0,    31,
      51,     3,     0,     0,     0,     0,     0,    10,     0,     0,
       7,     0,     0,     6,     0,     0,    67,    68,     0,     0,
      30,     0,     0,    44,     0,     0,    47,     0,    37,     0,
      34,     0,     0,    38,     0,     0,    41,     5,     0,     0,
      19,     0,     0,    22,     0,    12,     0,     9,     0,     0,
      13,     0,     0,    16,    25,    53,    55,    57,     0,    46,
       0,    49,    36,    33,     0,    40,     0,    43,     0,    21,
       0,    24,    11,     8,     0,    15,     0,    18,    45,    48,
      39,    42,    20,    23,    14,    17
  };

  const signed char
  GrainPropertiesParser::yypgoto_[] =
  {
     -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,    88,   -12,
      -5,   -12
  };

  const signed char
  GrainPropertiesParser::yydefgoto_[] =
  {
      -1,     7,     8,    48,     9,    10,    11,    12,    13,    14,
      37,    15
  };

  const unsigned char
  GrainPropertiesParser::yytable_[] =
  {
      39,    41,    16,    17,    18,    19,    20,    21,    22,    51,
      35,    43,    23,    44,    38,    52,    27,    24,    28,    29,
      30,    31,    32,    33,    53,    36,    54,    60,    65,    36,
      75,    34,    55,    78,    66,    79,    68,    62,    81,    84,
      91,    56,    69,    67,    82,    85,    92,    49,    50,    40,
      94,    70,    98,    83,    86,    93,    95,   101,    99,    59,
     108,    74,   111,   102,    36,    96,   109,   100,   112,    25,
     115,    61,   103,   116,   117,   110,    73,   113,    26,     1,
       2,     3,     4,     5,     6,    45,    46,    47,    36,    57,
      58,    63,    64,    71,    72,    76,    77,    80,    87,     0,
       0,    88,    42,     0,    89,     0,     0,    97,    90,   104,
     105,   106,   107,   114,   118,     0,   120,   119,   122,   121,
     123,   124,     0,   126,   125,   128,   127,   130,   129,   132,
     131,   133,   134,     0,   136,   135,   138,   137,   139,   140,
     141,   142,   143,   144,   145
  };

  const signed char
  GrainPropertiesParser::yycheck_[] =
  {
       5,     6,     9,    10,    11,    12,    13,    14,    15,     9,
       9,     0,    19,    24,     9,    15,     9,    24,    11,    12,
      13,    14,    15,    16,    24,    24,     9,    23,     9,    24,
      35,    24,    15,    38,    15,    40,     9,    24,     9,     9,
       9,    24,    15,    24,    15,    15,    15,    12,    13,     9,
       9,    24,     9,    24,    24,    24,    15,     9,    15,    26,
       9,    24,     9,    15,    24,    24,    15,    24,    15,    15,
      75,    26,    24,    78,    79,    24,    26,    24,    24,     3,
       4,     5,     6,     7,     8,    20,    21,    22,    24,    12,
      13,    12,    13,    12,    13,    17,    18,    24,    24,    -1,
      -1,    26,    14,    -1,    24,    -1,    -1,    24,    26,    24,
      26,    24,    26,    24,    24,    -1,    24,    26,    24,    26,
      24,    24,    -1,    24,    26,    24,    26,    24,    26,    24,
      26,    24,    24,    -1,    24,    26,    24,    26,    24,    24,
      24,    24,    24,    24,    24
  };

  const signed char
  GrainPropertiesParser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,    28,    29,    31,
      32,    33,    34,    35,    36,    38,     9,    10,    11,    12,
      13,    14,    15,    19,    24,    15,    24,     9,    11,    12,
      13,    14,    15,    16,    24,     9,    24,    37,     9,    37,
       9,    37,    35,     0,    24,    20,    21,    22,    30,    12,
      13,     9,    15,    24,     9,    15,    24,    12,    13,    26,
      23,    26,    24,    12,    13,     9,    15,    24,     9,    15,
      24,    12,    13,    26,    24,    37,    17,    18,    37,    37,
      24,     9,    15,    24,     9,    15,    24,    24,    26,    24,
      26,     9,    15,    24,     9,    15,    24,    24,     9,    15,
      24,     9,    15,    24,    24,    26,    24,    26,     9,    15,
      24,     9,    15,    24,    24,    37,    37,    37,    24,    26,
      24,    26,    24,    24,    24,    26,    24,    26,    24,    26,
      24,    26,    24,    24,    24,    26,    24,    26,    24,    24,
      24,    24,    24,    24,    24,    24
  };

  const signed char
  GrainPropertiesParser::yyr1_[] =
  {
       0,    27,    28,    28,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    30,    30,    30,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    31,    31,
      31,    31,    32,    32,    33,    33,    34,    34,    35,    35,
      35,    35,    35,    35,    36,    36,    37,    37,    37,    38
  };

  const signed char
  GrainPropertiesParser::yyr2_[] =
  {
       0,     2,     2,     3,     2,     4,     3,     3,     5,     4,
       3,     5,     4,     4,     6,     5,     4,     6,     5,     4,
       6,     5,     4,     6,     5,     4,     1,     1,     1,     2,
       4,     3,     3,     5,     4,     3,     5,     4,     4,     6,
       5,     4,     6,     5,     4,     6,     5,     4,     6,     5,
       3,     3,     2,     4,     2,     4,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     2,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const GrainPropertiesParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "PROP_PAN", "PROP_AMP",
  "PROP_SPEED", "PROP_AT", "PROP_LENGTH", "PROP_WHEN", "RANDOM",
  "TOVERFLOW", "ONDONE", "ADD", "SET", "MOTION", "EXPR", "RANGE", "MSEC",
  "SEC", "MODE", "OVERFLOW_CLIP", "OVERFLOW_WRAP", "OVERFLOW_FOLD",
  "PAN_MODE", "FLOAT", "INTEGER", "STRING", "$accept", "AMP", "SPEED",
  "OVERFLOW_MODE", "PAN", "AT", "LENGTH", "WHEN", "PROP", "PROPS", "TIME",
  "GRAIN", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  GrainPropertiesParser::yyrline_[] =
  {
       0,    77,    77,    78,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
     101,   107,   108,   113,   119,   120,   124,   125,   126,   130,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   151,   157,   158,   159,   164,
     165,   166,   171,   172,   176,   177,   181,   182,   186,   187,
     188,   189,   190,   191,   195,   196,   200,   201,   202,   206
  };

  void
  GrainPropertiesParser::yy_stack_print_ () const
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
  GrainPropertiesParser::yy_reduce_print_ (int yyrule) const
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


#line 7 "grain_properties.yy"
} // ceammc
#line 1410 "grainprops.parser.cpp"

#line 208 "grain_properties.yy"


void ceammc::GrainPropertiesParser::error(const std::string& err_message)
{
    LIB_ERR << "grain property parse error: " << err_message << '\n';
}

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
#line 30 "grain_properties.yy"

    # include <string>
    # include "grainprops.lexer.h"
    # include "grain_random.h"
    # include "ceammc_log.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static inline float frand(float a, float b) { return ceammc::GrainRandom::urandf(a, b); }
    static inline float frand_closed(float a, float b) { return ceammc::GrainRandom::urandf_closed(a, b); }

#line 59 "grainprops.parser.cpp"


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
#line 133 "grainprops.parser.cpp"

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
      case 33: // ENUM_PAN_MODE
        value.YY_MOVE_OR_COPY< Grain::PanMode > (YY_MOVE (that.value));
        break;

      case 32: // ENUM_OVERFLOW_MODE
        value.YY_MOVE_OR_COPY< Grain::PanOverflow > (YY_MOVE (that.value));
        break;

      case 34: // ENUM_INTERP_MODE
        value.YY_MOVE_OR_COPY< Grain::PlayInterp > (YY_MOVE (that.value));
        break;

      case 43: // TIME
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 29: // FLOAT
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case 30: // STRING
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
      case 33: // ENUM_PAN_MODE
        value.move< Grain::PanMode > (YY_MOVE (that.value));
        break;

      case 32: // ENUM_OVERFLOW_MODE
        value.move< Grain::PanOverflow > (YY_MOVE (that.value));
        break;

      case 34: // ENUM_INTERP_MODE
        value.move< Grain::PlayInterp > (YY_MOVE (that.value));
        break;

      case 43: // TIME
        value.move< double > (YY_MOVE (that.value));
        break;

      case 29: // FLOAT
        value.move< float > (YY_MOVE (that.value));
        break;

      case 30: // STRING
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
      case 33: // ENUM_PAN_MODE
        value.copy< Grain::PanMode > (that.value);
        break;

      case 32: // ENUM_OVERFLOW_MODE
        value.copy< Grain::PanOverflow > (that.value);
        break;

      case 34: // ENUM_INTERP_MODE
        value.copy< Grain::PlayInterp > (that.value);
        break;

      case 43: // TIME
        value.copy< double > (that.value);
        break;

      case 29: // FLOAT
        value.copy< float > (that.value);
        break;

      case 30: // STRING
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
      case 33: // ENUM_PAN_MODE
        value.move< Grain::PanMode > (that.value);
        break;

      case 32: // ENUM_OVERFLOW_MODE
        value.move< Grain::PanOverflow > (that.value);
        break;

      case 34: // ENUM_INTERP_MODE
        value.move< Grain::PlayInterp > (that.value);
        break;

      case 43: // TIME
        value.move< double > (that.value);
        break;

      case 29: // FLOAT
        value.move< float > (that.value);
        break;

      case 30: // STRING
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
      case 33: // ENUM_PAN_MODE
        yylhs.value.emplace< Grain::PanMode > ();
        break;

      case 32: // ENUM_OVERFLOW_MODE
        yylhs.value.emplace< Grain::PanOverflow > ();
        break;

      case 34: // ENUM_INTERP_MODE
        yylhs.value.emplace< Grain::PlayInterp > ();
        break;

      case 43: // TIME
        yylhs.value.emplace< double > ();
        break;

      case 29: // FLOAT
        yylhs.value.emplace< float > ();
        break;

      case 30: // STRING
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
  case 2:
#line 84 "grain_properties.yy"
             { yylhs.value.as < Grain::PanOverflow > () = Grain::PAN_OVERFLOW_CLIP; }
#line 635 "grainprops.parser.cpp"
    break;

  case 3:
#line 85 "grain_properties.yy"
             { yylhs.value.as < Grain::PanOverflow > () = Grain::PAN_OVERFLOW_WRAP; }
#line 641 "grainprops.parser.cpp"
    break;

  case 4:
#line 86 "grain_properties.yy"
             { yylhs.value.as < Grain::PanOverflow > () = Grain::PAN_OVERFLOW_FOLD; }
#line 647 "grainprops.parser.cpp"
    break;

  case 5:
#line 90 "grain_properties.yy"
               { yylhs.value.as < Grain::PanMode > () = Grain::PAN_MODE_NONE; }
#line 653 "grainprops.parser.cpp"
    break;

  case 6:
#line 91 "grain_properties.yy"
               { yylhs.value.as < Grain::PanMode > () = Grain::PAN_MODE_LINEAR; }
#line 659 "grainprops.parser.cpp"
    break;

  case 7:
#line 92 "grain_properties.yy"
               { yylhs.value.as < Grain::PanMode > () = Grain::PAN_MODE_SQRT; }
#line 665 "grainprops.parser.cpp"
    break;

  case 8:
#line 96 "grain_properties.yy"
               { yylhs.value.as < Grain::PlayInterp > () = Grain::INTERP_NO; }
#line 671 "grainprops.parser.cpp"
    break;

  case 9:
#line 97 "grain_properties.yy"
               { yylhs.value.as < Grain::PlayInterp > () = Grain::INTERP_LINEAR; }
#line 677 "grainprops.parser.cpp"
    break;

  case 10:
#line 98 "grain_properties.yy"
               { yylhs.value.as < Grain::PlayInterp > () = Grain::INTERP_CUBIC; }
#line 683 "grainprops.parser.cpp"
    break;

  case 13:
#line 107 "grain_properties.yy"
                                            { lexer.grain()->setSpeed(yystack_[0].value.as < float > ()); }
#line 689 "grainprops.parser.cpp"
    break;

  case 14:
#line 108 "grain_properties.yy"
                                            { lexer.grain()->setSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 695 "grainprops.parser.cpp"
    break;

  case 15:
#line 109 "grain_properties.yy"
                                            { lexer.grain()->setSpeedExpr(yystack_[0].value.as < std::string > ()); }
#line 701 "grainprops.parser.cpp"
    break;

  case 16:
#line 110 "grain_properties.yy"
                                            { lexer.grain()->setSpeed(yystack_[0].value.as < float > ()); }
#line 707 "grainprops.parser.cpp"
    break;

  case 17:
#line 111 "grain_properties.yy"
                                            { lexer.grain()->setSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 713 "grainprops.parser.cpp"
    break;

  case 18:
#line 112 "grain_properties.yy"
                                            { lexer.grain()->setSpeedExpr(yystack_[0].value.as < std::string > ()); }
#line 719 "grainprops.parser.cpp"
    break;

  case 19:
#line 113 "grain_properties.yy"
                                            { lexer.grain()->addSpeed(yystack_[0].value.as < float > ()); }
#line 725 "grainprops.parser.cpp"
    break;

  case 20:
#line 114 "grain_properties.yy"
                                            { lexer.grain()->addSpeed(frand(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 731 "grainprops.parser.cpp"
    break;

  case 28:
#line 122 "grain_properties.yy"
                                        {
                                          auto d = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([d](Grain* g){ return g->speed() + d; });
                                        }
#line 740 "grainprops.parser.cpp"
    break;

  case 29:
#line 127 "grain_properties.yy"
                                        {
                                          auto a = yystack_[1].value.as < float > ();
                                          auto b = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([a,b](Grain* g){ return g->speed() + frand(a, b); });
                                        }
#line 750 "grainprops.parser.cpp"
    break;

  case 31:
#line 134 "grain_properties.yy"
                                        {
                                          auto v = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([v](Grain*){ return v; });
                                        }
#line 759 "grainprops.parser.cpp"
    break;

  case 32:
#line 139 "grain_properties.yy"
                                        {
                                          auto a = yystack_[1].value.as < float > ();
                                          auto b = yystack_[0].value.as < float > ();
                                          lexer.grain()->setSpeedDone([a,b](Grain*){ return frand(a, b); });
                                        }
#line 769 "grainprops.parser.cpp"
    break;

  case 34:
#line 145 "grain_properties.yy"
                                        { lexer.grain()->setSpeedRange(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ()); }
#line 775 "grainprops.parser.cpp"
    break;

  case 35:
#line 149 "grain_properties.yy"
                                          { lexer.grain()->setPan(yystack_[0].value.as < float > ()); }
#line 781 "grainprops.parser.cpp"
    break;

  case 36:
#line 150 "grain_properties.yy"
                                          { lexer.grain()->setPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 787 "grainprops.parser.cpp"
    break;

  case 38:
#line 152 "grain_properties.yy"
                                          { lexer.grain()->setPan(yystack_[0].value.as < float > ()); }
#line 793 "grainprops.parser.cpp"
    break;

  case 39:
#line 153 "grain_properties.yy"
                                          { lexer.grain()->setPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 799 "grainprops.parser.cpp"
    break;

  case 41:
#line 155 "grain_properties.yy"
                                          { lexer.grain()->addPan(yystack_[0].value.as < float > ()); }
#line 805 "grainprops.parser.cpp"
    break;

  case 42:
#line 156 "grain_properties.yy"
                                          { lexer.grain()->addPan(frand_closed(yystack_[1].value.as < float > (), yystack_[0].value.as < float > ())); }
#line 811 "grainprops.parser.cpp"
    break;

  case 50:
#line 164 "grain_properties.yy"
                                            {
                                              auto v = yystack_[0].value.as < float > ();
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([v,g](){ return g->pan() + v; });
                                            }
#line 821 "grainprops.parser.cpp"
    break;

  case 51:
#line 170 "grain_properties.yy"
                                            {
                                              auto a = yystack_[1].value.as < float > ();
                                              auto b = yystack_[0].value.as < float > ();
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([a,b,g](){ return g->pan() + frand_closed(a, b); });
                                            }
#line 832 "grainprops.parser.cpp"
    break;

  case 53:
#line 177 "grain_properties.yy"
                                            { auto v = yystack_[0].value.as < float > (); lexer.grain()->setPanDone([v](){ return v; }); }
#line 838 "grainprops.parser.cpp"
    break;

  case 54:
#line 179 "grain_properties.yy"
                                            {
                                              auto a = yystack_[1].value.as < float > ();
                                              auto b = yystack_[0].value.as < float > ();
                                              lexer.grain()->setPanDone([a,b](){ return frand_closed(a, b); });
                                            }
#line 848 "grainprops.parser.cpp"
    break;

  case 56:
#line 185 "grain_properties.yy"
                                             { lexer.grain()->setPanOverflow(yystack_[0].value.as < Grain::PanOverflow > ()); }
#line 854 "grainprops.parser.cpp"
    break;

  case 57:
#line 186 "grain_properties.yy"
                                             { lexer.grain()->setPanMode(yystack_[0].value.as < Grain::PanMode > ()); }
#line 860 "grainprops.parser.cpp"
    break;

  case 58:
#line 191 "grain_properties.yy"
                               { lexer.grain()->array_pos_samp = yystack_[0].value.as < double > (); }
#line 866 "grainprops.parser.cpp"
    break;

  case 59:
#line 192 "grain_properties.yy"
                                 { lexer.grain()->array_pos_samp = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 872 "grainprops.parser.cpp"
    break;

  case 60:
#line 196 "grain_properties.yy"
                                   { lexer.grain()->length_samp = yystack_[0].value.as < double > (); }
#line 878 "grainprops.parser.cpp"
    break;

  case 61:
#line 197 "grain_properties.yy"
                                     { lexer.grain()->length_samp = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 884 "grainprops.parser.cpp"
    break;

  case 62:
#line 201 "grain_properties.yy"
                                   { lexer.grain()->play_pos = yystack_[0].value.as < double > (); }
#line 890 "grainprops.parser.cpp"
    break;

  case 63:
#line 202 "grain_properties.yy"
                                     { lexer.grain()->play_pos = frand_closed(yystack_[1].value.as < double > (), yystack_[0].value.as < double > ()); }
#line 896 "grainprops.parser.cpp"
    break;

  case 70:
#line 212 "grain_properties.yy"
                                        { lexer.grain()->setPlayInterpolation(yystack_[0].value.as < Grain::PlayInterp > ()); }
#line 902 "grainprops.parser.cpp"
    break;

  case 73:
#line 221 "grain_properties.yy"
            { yylhs.value.as < double > () = yystack_[0].value.as < float > (); }
#line 908 "grainprops.parser.cpp"
    break;

  case 74:
#line 222 "grain_properties.yy"
                   { yylhs.value.as < double > () = sys_getsr() * 0.001 * yystack_[1].value.as < float > (); }
#line 914 "grainprops.parser.cpp"
    break;

  case 75:
#line 223 "grain_properties.yy"
                   { yylhs.value.as < double > () = sys_getsr() * yystack_[1].value.as < float > (); }
#line 920 "grainprops.parser.cpp"
    break;


#line 924 "grainprops.parser.cpp"

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


  const signed char GrainPropertiesParser::yypact_ninf_ = -25;

  const signed char GrainPropertiesParser::yytable_ninf_ = -1;

  const signed char
  GrainPropertiesParser::yypact_[] =
  {
      94,    50,    81,   -10,    82,    -9,     6,    83,   -25,   -25,
     -25,   -25,   -25,   -25,   -25,    94,    11,   -24,   -25,     2,
      14,   -25,   -25,   -25,   -25,   -25,     2,   -25,     5,    21,
      69,    -1,    55,    10,    45,    13,   -25,    24,    47,    61,
      62,    49,    56,    27,   -25,     2,   -25,   -25,   -25,   -25,
       2,   -25,   -25,     2,    60,    63,   -25,   -25,   -25,   -25,
     -25,   -25,    31,    32,    35,    39,   -25,   -25,   -25,   -25,
      64,    65,    78,   -25,    79,    84,   -25,   -25,    44,    46,
      53,    57,    85,    86,    87,    89,   -25,     2,   -25,   -25,
     -25,    90,    91,    93,   -25,    95,    97,   -25,    98,   100,
     -25,   101,   103,   -25,   -25,   -25,   104,   -25,   105,   106,
     108,   -25,   109,   111,   -25,   112,   114,   -25,   115,   117,
     -25,   -25,   -25,   -25,   118,   -25,   -25,   -25,   119,   -25,
     120,   -25,   121,   -25,   122,   -25,   -25,   -25,   123,   -25,
     124,   -25,   125,   -25,   126,   -25,   -25,   -25,   -25,   -25,
     -25,   -25,   -25,   -25
  };

  const signed char
  GrainPropertiesParser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     0,     0,    66,    65,
      64,    67,    68,    69,    71,    76,     0,     0,    11,     0,
      73,    58,    10,     9,     8,    70,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,     0,     0,     0,    13,     0,    62,    72,     1,    12,
       0,    74,    75,     0,     0,     0,    41,    37,     6,     5,
       7,    57,     0,     0,     0,     0,     2,     4,     3,    56,
       0,     0,     0,    38,     0,     0,    19,    15,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,    59,    61,
      43,     0,     0,     0,    44,     0,     0,    47,     0,     0,
      50,     0,     0,    53,    36,    40,     0,    21,     0,     0,
       0,    22,     0,     0,    25,     0,     0,    28,     0,     0,
      31,    14,    34,    18,     0,    63,    42,    46,     0,    49,
       0,    52,     0,    55,     0,    39,    20,    24,     0,    27,
       0,    30,     0,    33,     0,    17,    45,    48,    51,    54,
      23,    26,    29,    32
  };

  const signed char
  GrainPropertiesParser::yypgoto_[] =
  {
     -25,   -25,   -25,   -25,   -25,   -25,   -25,   -25,   -25,   -25,
      28,   -25,    -4,   -25
  };

  const signed char
  GrainPropertiesParser::yydefgoto_[] =
  {
      -1,    69,    61,    25,     8,     9,    10,    11,    12,    13,
      14,    15,    21,    16
  };

  const unsigned char
  GrainPropertiesParser::yytable_[] =
  {
      27,    28,    22,    46,    29,    23,    49,    30,    31,    62,
      24,    48,    32,    33,    34,    50,    37,    35,    54,    38,
      36,    66,    53,    39,    67,    63,    71,    40,    55,    41,
      42,    20,    43,    51,    56,    44,    72,    74,    68,    52,
      84,    87,    73,    47,    92,    95,    88,    75,    98,    89,
      85,    57,   101,    76,    93,    96,    86,   109,    99,   112,
      94,    97,   102,    17,   100,    64,   115,   110,   103,   113,
     118,    78,    80,   111,    70,   114,   116,    77,    82,    18,
     119,    65,   117,   125,    58,    83,   120,    79,    81,    59,
      90,     0,    91,   104,     0,   105,    60,     1,     2,     3,
       4,     5,     6,     7,    19,    26,    45,   106,     0,   107,
      20,    20,    20,   108,   121,   122,     0,   123,   124,   126,
       0,   127,   128,     0,     0,   129,   130,     0,   131,   132,
       0,   133,   134,   135,   136,     0,   137,   138,     0,   139,
     140,     0,   141,   142,     0,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153
  };

  const signed char
  GrainPropertiesParser::yycheck_[] =
  {
       4,    10,    12,     7,    13,    15,    30,    16,    17,    10,
      20,     0,    21,    22,    23,    19,    10,    26,    13,    13,
      29,    11,    26,    17,    14,    26,    13,    21,    23,    23,
      24,    29,    26,    19,    29,    29,    23,    13,    28,    25,
      13,    45,    29,    15,    13,    13,    50,    23,    13,    53,
      23,    30,    13,    29,    23,    23,    29,    13,    23,    13,
      29,    29,    23,    13,    29,    10,    13,    23,    29,    23,
      13,    10,    10,    29,    29,    29,    23,    30,    29,    29,
      23,    26,    29,    87,    15,    29,    29,    26,    26,    20,
      30,    -1,    29,    29,    -1,    30,    27,     3,     4,     5,
       6,     7,     8,     9,    23,    23,    23,    29,    -1,    30,
      29,    29,    29,    29,    29,    29,    -1,    30,    29,    29,
      -1,    30,    29,    -1,    -1,    30,    29,    -1,    30,    29,
      -1,    30,    29,    29,    29,    -1,    30,    29,    -1,    30,
      29,    -1,    30,    29,    -1,    30,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29
  };

  const signed char
  GrainPropertiesParser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,     9,    35,    36,
      37,    38,    39,    40,    41,    42,    44,    13,    29,    23,
      29,    43,    12,    15,    20,    34,    23,    43,    10,    13,
      16,    17,    21,    22,    23,    26,    29,    10,    13,    17,
      21,    23,    24,    26,    29,    23,    43,    41,     0,    30,
      43,    19,    25,    43,    13,    23,    29,    30,    15,    20,
      27,    33,    10,    26,    10,    26,    11,    14,    28,    32,
      29,    13,    23,    29,    13,    23,    29,    30,    10,    26,
      10,    26,    29,    29,    13,    23,    29,    43,    43,    43,
      30,    29,    13,    23,    29,    13,    23,    29,    13,    23,
      29,    13,    23,    29,    29,    30,    29,    30,    29,    13,
      23,    29,    13,    23,    29,    13,    23,    29,    13,    23,
      29,    29,    29,    30,    29,    43,    29,    30,    29,    30,
      29,    30,    29,    30,    29,    29,    29,    30,    29,    30,
      29,    30,    29,    30,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29
  };

  const signed char
  GrainPropertiesParser::yyr1_[] =
  {
       0,    31,    32,    32,    32,    33,    33,    33,    34,    34,
      34,    35,    35,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    38,    38,
      39,    39,    40,    40,    41,    41,    41,    41,    41,    41,
      41,    42,    42,    43,    43,    43,    44
  };

  const signed char
  GrainPropertiesParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     2,     4,     3,     3,     5,     4,     3,
       5,     4,     4,     6,     5,     4,     6,     5,     4,     6,
       5,     4,     6,     5,     4,     2,     4,     3,     3,     5,
       4,     3,     5,     4,     4,     6,     5,     4,     6,     5,
       4,     6,     5,     4,     6,     5,     3,     3,     2,     4,
       2,     4,     2,     4,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     2,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const GrainPropertiesParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "PROP_AMP", "PROP_AT",
  "PROP_INTERP", "PROP_LENGTH", "PROP_PAN", "PROP_SPEED", "PROP_WHEN",
  "S_ADD", "S_CLIP", "S_CUBIC", "S_EXPR", "S_FOLD", "S_LINEAR", "S_MODE",
  "S_MOTION", "S_MSEC2", "S_MSEC", "S_NONE", "S_ONDONE", "S_OVERFLOW",
  "S_RANDOM", "S_RANGE", "S_SEC", "S_SET", "S_SQRT", "S_WRAP", "FLOAT",
  "STRING", "$accept", "ENUM_OVERFLOW_MODE", "ENUM_PAN_MODE",
  "ENUM_INTERP_MODE", "AMP", "SPEED", "PAN", "AT", "LENGTH", "WHEN",
  "PROP", "PROPS", "TIME", "GRAIN", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  GrainPropertiesParser::yyrline_[] =
  {
       0,    84,    84,    85,    86,    90,    91,    92,    96,    97,
      98,   102,   103,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   126,
     132,   133,   138,   144,   145,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   169,   176,   177,   178,   184,   185,   186,   191,   192,
     196,   197,   201,   202,   206,   207,   208,   209,   210,   211,
     212,   216,   217,   221,   222,   223,   227
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
#line 1481 "grainprops.parser.cpp"

#line 229 "grain_properties.yy"


void ceammc::GrainPropertiesParser::error(const std::string& err_message)
{
    LIB_ERR << "grain property parse error: " << err_message << '\n';
}

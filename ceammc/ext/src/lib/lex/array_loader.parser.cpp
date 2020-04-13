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

// "%code top" blocks.
#line 35 "array_loader.y"

    # undef yylex
    # define yylex lexer.lex

    # include "array_loader.lexer.h"
    # include "array_loader.h"
    # include "ceammc_convert.h"

    using OPT = ceammc::ArrayLoader::OptionType;

    static StringList generateRange(uint from, uint to) {
        StringList res;
        res.reserve(to - from + 1);
        for(uint i = from; i <= to; i++)
            res.emplace_back(std::to_string(i));

        return res;
    }

#line 58 "array_loader.parser.cpp"




#include "array_loader.parser.hpp"




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

#line 7 "array_loader.y"
namespace ceammc {
#line 157 "array_loader.parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ArrayLoaderParser::yytnamerr_ (const char *yystr)
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
  ArrayLoaderParser::ArrayLoaderParser (ceammc::ArrayLoaderLexer& lexer_yyarg, ceammc::ArrayLoader& loader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      loader (loader_yyarg)
  {}

  ArrayLoaderParser::~ArrayLoaderParser ()
  {}

  ArrayLoaderParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  ArrayLoaderParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  ArrayLoaderParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  ArrayLoaderParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  ArrayLoaderParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  ArrayLoaderParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  ArrayLoaderParser::symbol_number_type
  ArrayLoaderParser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  ArrayLoaderParser::stack_symbol_type::stack_symbol_type ()
  {}

  ArrayLoaderParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 39: // array_pattern
      case 40: // var_list
        value.YY_MOVE_OR_COPY< StringList > (YY_MOVE (that.value));
        break;

      case 21: // FLOAT
      case 38: // number
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 22: // INT
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 36: // time
      case 37: // smpte
        value.YY_MOVE_OR_COPY< std::size_t > (YY_MOVE (that.value));
        break;

      case 4: // QUOTED_STRING
      case 5: // SYMBOL
      case 11: // OPTION
      case 20: // SMPTE
      case 41: // var
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 23: // UINT
        value.YY_MOVE_OR_COPY< uint > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  ArrayLoaderParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 39: // array_pattern
      case 40: // var_list
        value.move< StringList > (YY_MOVE (that.value));
        break;

      case 21: // FLOAT
      case 38: // number
        value.move< double > (YY_MOVE (that.value));
        break;

      case 22: // INT
        value.move< int > (YY_MOVE (that.value));
        break;

      case 36: // time
      case 37: // smpte
        value.move< std::size_t > (YY_MOVE (that.value));
        break;

      case 4: // QUOTED_STRING
      case 5: // SYMBOL
      case 11: // OPTION
      case 20: // SMPTE
      case 41: // var
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 23: // UINT
        value.move< uint > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  ArrayLoaderParser::stack_symbol_type&
  ArrayLoaderParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 39: // array_pattern
      case 40: // var_list
        value.copy< StringList > (that.value);
        break;

      case 21: // FLOAT
      case 38: // number
        value.copy< double > (that.value);
        break;

      case 22: // INT
        value.copy< int > (that.value);
        break;

      case 36: // time
      case 37: // smpte
        value.copy< std::size_t > (that.value);
        break;

      case 4: // QUOTED_STRING
      case 5: // SYMBOL
      case 11: // OPTION
      case 20: // SMPTE
      case 41: // var
        value.copy< std::string > (that.value);
        break;

      case 23: // UINT
        value.copy< uint > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  ArrayLoaderParser::stack_symbol_type&
  ArrayLoaderParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 39: // array_pattern
      case 40: // var_list
        value.move< StringList > (that.value);
        break;

      case 21: // FLOAT
      case 38: // number
        value.move< double > (that.value);
        break;

      case 22: // INT
        value.move< int > (that.value);
        break;

      case 36: // time
      case 37: // smpte
        value.move< std::size_t > (that.value);
        break;

      case 4: // QUOTED_STRING
      case 5: // SYMBOL
      case 11: // OPTION
      case 20: // SMPTE
      case 41: // var
        value.move< std::string > (that.value);
        break;

      case 23: // UINT
        value.move< uint > (that.value);
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
  ArrayLoaderParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  ArrayLoaderParser::yy_print_ (std::ostream& yyo,
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
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  ArrayLoaderParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  ArrayLoaderParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  ArrayLoaderParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ArrayLoaderParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ArrayLoaderParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ArrayLoaderParser::debug_level_type
  ArrayLoaderParser::debug_level () const
  {
    return yydebug_;
  }

  void
  ArrayLoaderParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  ArrayLoaderParser::state_type
  ArrayLoaderParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  ArrayLoaderParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  ArrayLoaderParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  ArrayLoaderParser::operator() ()
  {
    return parse ();
  }

  int
  ArrayLoaderParser::parse ()
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
      case 39: // array_pattern
      case 40: // var_list
        yylhs.value.emplace< StringList > ();
        break;

      case 21: // FLOAT
      case 38: // number
        yylhs.value.emplace< double > ();
        break;

      case 22: // INT
        yylhs.value.emplace< int > ();
        break;

      case 36: // time
      case 37: // smpte
        yylhs.value.emplace< std::size_t > ();
        break;

      case 4: // QUOTED_STRING
      case 5: // SYMBOL
      case 11: // OPTION
      case 20: // SMPTE
      case 41: // var
        yylhs.value.emplace< std::string > ();
        break;

      case 23: // UINT
        yylhs.value.emplace< uint > ();
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
  case 2:
#line 84 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(0, yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > (), 0); }
#line 740 "array_loader.parser.cpp"
    break;

  case 3:
#line 85 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(yystack_[4].value.as < uint > (), yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > (), 0); }
#line 746 "array_loader.parser.cpp"
    break;

  case 4:
#line 86 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(yystack_[6].value.as < uint > (), yystack_[4].value.as < uint > (), yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > ()); }
#line 752 "array_loader.parser.cpp"
    break;

  case 5:
#line 90 "array_loader.y"
      { yylhs.value.as < double > () = yystack_[0].value.as < double > (); }
#line 758 "array_loader.parser.cpp"
    break;

  case 6:
#line 91 "array_loader.y"
           { yylhs.value.as < double > () = yystack_[0].value.as < int > (); }
#line 764 "array_loader.parser.cpp"
    break;

  case 7:
#line 95 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[0].value.as < double > (); }
#line 770 "array_loader.parser.cpp"
    break;

  case 8:
#line 96 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = loader.sec2samples(yystack_[1].value.as < double > ()); }
#line 776 "array_loader.parser.cpp"
    break;

  case 9:
#line 97 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = loader.ms2samples(yystack_[1].value.as < double > ()); }
#line 782 "array_loader.parser.cpp"
    break;

  case 10:
#line 98 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[1].value.as < double > (); }
#line 788 "array_loader.parser.cpp"
    break;

  case 11:
#line 99 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[0].value.as < std::size_t > (); }
#line 794 "array_loader.parser.cpp"
    break;

  case 12:
#line 103 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_RESIZE); }
#line 800 "array_loader.parser.cpp"
    break;

  case 13:
#line 104 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_NORMALIZE); }
#line 806 "array_loader.parser.cpp"
    break;

  case 14:
#line 105 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_VERBOSE); }
#line 812 "array_loader.parser.cpp"
    break;

  case 15:
#line 106 "array_loader.y"
                       { if(!loader.setTimeOption(OPT::OPT_BEGIN, yystack_[0].value.as < std::size_t > ()))      error(yystack_[0].location, "invalid @begin value");}
#line 818 "array_loader.parser.cpp"
    break;

  case 16:
#line 107 "array_loader.y"
                       { if(!loader.setTimeOption(OPT::OPT_END, yystack_[0].value.as < std::size_t > ()))        error(yystack_[0].location, "invalid @end value");}
#line 824 "array_loader.parser.cpp"
    break;

  case 17:
#line 108 "array_loader.y"
                       { if(!loader.setTimeOption(OPT::OPT_LENGTH, yystack_[0].value.as < std::size_t > ()))     error(yystack_[0].location, "invalid @length value");}
#line 830 "array_loader.parser.cpp"
    break;

  case 18:
#line 109 "array_loader.y"
                       { if(!loader.setGain(yystack_[0].value.as < double > ()))                            error(yystack_[0].location, "invalid @gain value");}
#line 836 "array_loader.parser.cpp"
    break;

  case 19:
#line 110 "array_loader.y"
                       { if(!loader.setGain(ceammc::convert::dbfs2amp(yystack_[1].value.as < double > ()))) error(yystack_[1].location, "invalid @gain value");}
#line 842 "array_loader.parser.cpp"
    break;

  case 20:
#line 111 "array_loader.y"
                       {
            if(!loader.setResampleRatio(yystack_[0].value.as < double > ()/loader.sampleRate()))
                error(yystack_[0].location, "invalid @resample value");
        }
#line 851 "array_loader.parser.cpp"
    break;

  case 26:
#line 129 "array_loader.y"
      { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 857 "array_loader.parser.cpp"
    break;

  case 27:
#line 130 "array_loader.y"
             { yylhs.value.as < std::string > () = std::to_string(yystack_[0].value.as < uint > ()); }
#line 863 "array_loader.parser.cpp"
    break;

  case 28:
#line 134 "array_loader.y"
                              { yylhs.value.as < StringList > ().push_back(yystack_[2].value.as < std::string > ()); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 869 "array_loader.parser.cpp"
    break;

  case 29:
#line 135 "array_loader.y"
                              { std::copy(yystack_[2].value.as < StringList > ().begin(), yystack_[2].value.as < StringList > ().end(), std::back_inserter(yylhs.value.as < StringList > ())); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 875 "array_loader.parser.cpp"
    break;

  case 30:
#line 139 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = generateRange(0, 9); }
#line 881 "array_loader.parser.cpp"
    break;

  case 31:
#line 140 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = yystack_[1].value.as < StringList > (); }
#line 887 "array_loader.parser.cpp"
    break;

  case 32:
#line 141 "array_loader.y"
                                                        {

        if(yystack_[3].value.as < uint > () > yystack_[1].value.as < uint > ())
            error(yystack_[3].location, "invalid range values");
        else
            yylhs.value.as < StringList > () = generateRange(yystack_[3].value.as < uint > (), yystack_[1].value.as < uint > ());
                                                        }
#line 899 "array_loader.parser.cpp"
    break;

  case 33:
#line 151 "array_loader.y"
                                    { loader.addArray(yystack_[0].value.as < std::string > ()); }
#line 905 "array_loader.parser.cpp"
    break;

  case 34:
#line 152 "array_loader.y"
                                    {
        for(auto& s: yystack_[0].value.as < StringList > ())
            loader.addArray(yystack_[1].value.as < std::string > () + s);
                                    }
#line 914 "array_loader.parser.cpp"
    break;

  case 35:
#line 156 "array_loader.y"
                             {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(s + yystack_[0].value.as < std::string > ());
                                    }
#line 923 "array_loader.parser.cpp"
    break;

  case 36:
#line 160 "array_loader.y"
                                    {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(yystack_[2].value.as < std::string > () + s + yystack_[0].value.as < std::string > ());
                                    }
#line 932 "array_loader.parser.cpp"
    break;


#line 936 "array_loader.parser.cpp"

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

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

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
  ArrayLoaderParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  ArrayLoaderParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const signed char ArrayLoaderParser::yypact_ninf_ = -31;

  const signed char ArrayLoaderParser::yytable_ninf_ = -1;

  const signed char
  ArrayLoaderParser::yypact_[] =
  {
      15,    25,   -31,     4,   -31,   -31,    12,   -31,    28,    -3,
      17,   -31,    32,    34,   -31,   -31,    31,    18,    33,   -31,
      12,    -6,   -31,    19,   -31,    -2,    -2,   -31,    -7,   -31,
      10,    10,    -7,    -7,   -31,   -31,   -31,    36,   -31,   -31,
     -31,   -31,   -31,    13,   -31,   -31,    -1,    20,    14,   -31,
     -31,   -31,    22,   -31,   -31,   -31,   -31,    10,    21,   -31,
      27,    23,    29,   -31
  };

  const signed char
  ArrayLoaderParser::yydefact_[] =
  {
       0,     0,    40,     0,    24,    25,     0,     1,    33,     0,
       0,    37,    22,    34,    26,    30,    27,     0,     0,    35,
       0,    39,    36,     0,    31,     0,     0,    38,     0,    12,
       0,     0,     0,     0,    14,    13,    23,     0,    27,    29,
      28,     5,     6,     0,    17,    11,     7,    18,    20,    15,
      16,    32,     0,     8,     9,    10,    19,     0,     2,    21,
       0,     3,     0,     4
  };

  const signed char
  ArrayLoaderParser::yypgoto_[] =
  {
     -31,     1,   -31,   -30,    40,   -31,    11,   -31,   -31,   -31,
      35,   -31,   -31,   -31
  };

  const signed char
  ArrayLoaderParser::yydefgoto_[] =
  {
      -1,    44,    45,    46,    10,    17,    18,    36,    21,     6,
      11,    12,     2,     3
  };

  const signed char
  ArrayLoaderParser::yytable_[] =
  {
      47,    48,    14,    14,     7,    15,    28,    29,    30,    31,
      32,    33,    34,    35,    41,    42,    43,     8,     1,     9,
      16,    38,    19,    53,    54,    55,    24,    59,    25,     4,
       5,    41,    42,    49,    50,     9,    39,    40,    20,    22,
      23,    52,    37,    26,    51,    58,    57,    56,    13,    60,
      61,     0,    63,    62,     0,    27
  };

  const signed char
  ArrayLoaderParser::yycheck_[] =
  {
      30,    31,     5,     5,     0,     8,    12,    13,    14,    15,
      16,    17,    18,    19,    21,    22,    23,     5,     3,     7,
      23,    23,     5,    24,    25,    26,     8,    57,    10,     4,
       5,    21,    22,    32,    33,     7,    25,    26,     6,     5,
       9,    28,    23,    10,     8,    23,    32,    27,     8,    28,
      23,    -1,    23,    30,    -1,    20
  };

  const signed char
  ArrayLoaderParser::yystos_[] =
  {
       0,     3,    47,    48,     4,     5,    44,     0,     5,     7,
      39,    45,    46,    39,     5,     8,    23,    40,    41,     5,
       6,    43,     5,     9,     8,    10,    10,    45,    12,    13,
      14,    15,    16,    17,    18,    19,    42,    23,    23,    41,
      41,    21,    22,    23,    36,    37,    38,    38,    38,    36,
      36,     8,    28,    24,    25,    26,    27,    32,    23,    38,
      28,    23,    30,    23
  };

  const signed char
  ArrayLoaderParser::yyr1_[] =
  {
       0,    35,    37,    37,    37,    38,    38,    36,    36,    36,
      36,    36,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    43,    43,    44,    44,    41,    41,    40,    40,
      39,    39,    39,    45,    45,    45,    45,    46,    46,    47,
      48
  };

  const signed char
  ArrayLoaderParser::yyr2_[] =
  {
       0,     2,     3,     5,     7,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       2,     4,     0,     2,     1,     1,     1,     1,     3,     3,
       2,     3,     5,     1,     2,     2,     3,     1,     3,     4,
       1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const ArrayLoaderParser::yytname_[] =
  {
  "\"end of string\"", "error", "$undefined", "ACT_LOAD", "QUOTED_STRING",
  "SYMBOL", "ARRAY_DELIM", "PATTERN_BEGIN", "PATTERN_END", "RANGE_DELIM",
  "VAR_DELIM", "OPTION", "LENGTH", "RESIZE", "GAIN", "RESAMPLE", "BEGIN",
  "END", "VERBOSE", "NORMALIZE", "SMPTE", "FLOAT", "INT", "UINT", "SEC",
  "MSEC", "SAMPLES", "DB", "COLON", "\"colon\"", "DOT", "\"dot\"", "FRAC",
  "\"fraction\"", "LEXER_ERROR", "$accept", "time", "smpte", "number",
  "array_pattern", "var_list", "var", "opt", "options", "filename",
  "array_name", "array_list", "load", "EXPR", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  ArrayLoaderParser::yyrline_[] =
  {
       0,    84,    84,    85,    86,    90,    91,    95,    96,    97,
      98,    99,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   115,   119,   120,   124,   125,   129,   130,   134,   135,
     139,   140,   141,   151,   152,   156,   160,   167,   168,   172,
     176
  };

  // Print the state stack on the debug stream.
  void
  ArrayLoaderParser::yystack_print_ ()
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
  ArrayLoaderParser::yy_reduce_print_ (int yyrule)
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


#line 7 "array_loader.y"
} // ceammc
#line 1363 "array_loader.parser.cpp"

#line 179 "array_loader.y"


void ceammc::ArrayLoaderParser::error(const location& loc, const std::string& err_message)
{
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << lexer.matcher().line() << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;
}


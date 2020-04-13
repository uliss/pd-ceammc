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
#line 36 "array_loader.y"

    # undef yylex
    # define yylex lexer.lex

    # include "array_loader.lexer.h"
    # include "array_loader.h"
    # include "ceammc_convert.h"

    using OPT = ceammc::ArrayLoader::OptionType;

    static StringList generateStringRange(uint from, uint to) {
        StringList res;
        res.reserve(to - from + 1);
        for(uint i = from; i <= to; i++)
            res.emplace_back(std::to_string(i));

        return res;
    }

    static ChannelList generateIntRange(int from, int to) {
        ChannelList res;
        res.reserve(std::abs(from - to) + 1);
        if(from < to) {
            for(int i = from; i <= to; i++)
                res.push_back(i);
        } else {
            for(int i = from; i >= to; i--)
                res.push_back(i);
        }

        return res;
    }

#line 72 "array_loader.parser.cpp"




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
#line 171 "array_loader.parser.cpp"


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
      case 42: // channel_list
        value.YY_MOVE_OR_COPY< ChannelList > (YY_MOVE (that.value));
        break;

      case 39: // array_pattern
      case 40: // var_list
        value.YY_MOVE_OR_COPY< StringList > (YY_MOVE (that.value));
        break;

      case 19: // FLOAT
      case 38: // number
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 20: // INT
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 36: // time
      case 37: // smpte
        value.YY_MOVE_OR_COPY< std::size_t > (YY_MOVE (that.value));
        break;

      case 3: // SYMBOL
      case 18: // SMPTE
      case 41: // var
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 21: // UINT
      case 22: // RANGE_BEGIN
      case 23: // RANGE_END
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
      case 42: // channel_list
        value.move< ChannelList > (YY_MOVE (that.value));
        break;

      case 39: // array_pattern
      case 40: // var_list
        value.move< StringList > (YY_MOVE (that.value));
        break;

      case 19: // FLOAT
      case 38: // number
        value.move< double > (YY_MOVE (that.value));
        break;

      case 20: // INT
        value.move< int > (YY_MOVE (that.value));
        break;

      case 36: // time
      case 37: // smpte
        value.move< std::size_t > (YY_MOVE (that.value));
        break;

      case 3: // SYMBOL
      case 18: // SMPTE
      case 41: // var
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 21: // UINT
      case 22: // RANGE_BEGIN
      case 23: // RANGE_END
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
      case 42: // channel_list
        value.copy< ChannelList > (that.value);
        break;

      case 39: // array_pattern
      case 40: // var_list
        value.copy< StringList > (that.value);
        break;

      case 19: // FLOAT
      case 38: // number
        value.copy< double > (that.value);
        break;

      case 20: // INT
        value.copy< int > (that.value);
        break;

      case 36: // time
      case 37: // smpte
        value.copy< std::size_t > (that.value);
        break;

      case 3: // SYMBOL
      case 18: // SMPTE
      case 41: // var
        value.copy< std::string > (that.value);
        break;

      case 21: // UINT
      case 22: // RANGE_BEGIN
      case 23: // RANGE_END
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
      case 42: // channel_list
        value.move< ChannelList > (that.value);
        break;

      case 39: // array_pattern
      case 40: // var_list
        value.move< StringList > (that.value);
        break;

      case 19: // FLOAT
      case 38: // number
        value.move< double > (that.value);
        break;

      case 20: // INT
        value.move< int > (that.value);
        break;

      case 36: // time
      case 37: // smpte
        value.move< std::size_t > (that.value);
        break;

      case 3: // SYMBOL
      case 18: // SMPTE
      case 41: // var
        value.move< std::string > (that.value);
        break;

      case 21: // UINT
      case 22: // RANGE_BEGIN
      case 23: // RANGE_END
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
      case 42: // channel_list
        yylhs.value.emplace< ChannelList > ();
        break;

      case 39: // array_pattern
      case 40: // var_list
        yylhs.value.emplace< StringList > ();
        break;

      case 19: // FLOAT
      case 38: // number
        yylhs.value.emplace< double > ();
        break;

      case 20: // INT
        yylhs.value.emplace< int > ();
        break;

      case 36: // time
      case 37: // smpte
        yylhs.value.emplace< std::size_t > ();
        break;

      case 3: // SYMBOL
      case 18: // SMPTE
      case 41: // var
        yylhs.value.emplace< std::string > ();
        break;

      case 21: // UINT
      case 22: // RANGE_BEGIN
      case 23: // RANGE_END
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
#line 98 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(0, yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > (), 0); }
#line 774 "array_loader.parser.cpp"
    break;

  case 3:
#line 99 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(0, yystack_[4].value.as < uint > (), yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > ()); }
#line 780 "array_loader.parser.cpp"
    break;

  case 4:
#line 100 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(yystack_[4].value.as < uint > (), yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > (), 0); }
#line 786 "array_loader.parser.cpp"
    break;

  case 5:
#line 101 "array_loader.y"
                                          { yylhs.value.as < std::size_t > () = loader.smpte2samples(yystack_[6].value.as < uint > (), yystack_[4].value.as < uint > (), yystack_[2].value.as < uint > (), yystack_[0].value.as < uint > ()); }
#line 792 "array_loader.parser.cpp"
    break;

  case 6:
#line 105 "array_loader.y"
      { yylhs.value.as < double > () = yystack_[0].value.as < double > (); }
#line 798 "array_loader.parser.cpp"
    break;

  case 7:
#line 106 "array_loader.y"
           { yylhs.value.as < double > () = yystack_[0].value.as < int > (); }
#line 804 "array_loader.parser.cpp"
    break;

  case 8:
#line 110 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[0].value.as < double > (); }
#line 810 "array_loader.parser.cpp"
    break;

  case 9:
#line 111 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = loader.sec2samples(yystack_[1].value.as < double > ()); }
#line 816 "array_loader.parser.cpp"
    break;

  case 10:
#line 112 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = loader.ms2samples(yystack_[1].value.as < double > ()); }
#line 822 "array_loader.parser.cpp"
    break;

  case 11:
#line 113 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[1].value.as < double > (); }
#line 828 "array_loader.parser.cpp"
    break;

  case 12:
#line 114 "array_loader.y"
                      { yylhs.value.as < std::size_t > () = yystack_[0].value.as < std::size_t > (); }
#line 834 "array_loader.parser.cpp"
    break;

  case 13:
#line 118 "array_loader.y"
                            { yylhs.value.as < ChannelList > () = {yystack_[0].value.as < int > ()}; }
#line 840 "array_loader.parser.cpp"
    break;

  case 14:
#line 119 "array_loader.y"
                            { yylhs.value.as < ChannelList > () = generateIntRange(yystack_[1].value.as < uint > (), yystack_[0].value.as < uint > ()); }
#line 846 "array_loader.parser.cpp"
    break;

  case 15:
#line 120 "array_loader.y"
                            { std::copy(yystack_[1].value.as < ChannelList > ().begin(), yystack_[1].value.as < ChannelList > ().end(), std::back_inserter(yylhs.value.as < ChannelList > ())); yylhs.value.as < ChannelList > ().push_back(yystack_[0].value.as < int > ()); }
#line 852 "array_loader.parser.cpp"
    break;

  case 16:
#line 124 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_RESIZE); }
#line 858 "array_loader.parser.cpp"
    break;

  case 17:
#line 125 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_NORMALIZE); }
#line 864 "array_loader.parser.cpp"
    break;

  case 18:
#line 126 "array_loader.y"
                       { loader.setFlagOption(OPT::OPT_VERBOSE); }
#line 870 "array_loader.parser.cpp"
    break;

  case 19:
#line 127 "array_loader.y"
                       { if(!loader.setSampleOption(OPT::OPT_BEGIN, yystack_[0].value.as < std::size_t > ()))      error(yystack_[0].location, "invalid @begin value");}
#line 876 "array_loader.parser.cpp"
    break;

  case 20:
#line 128 "array_loader.y"
                       { if(!loader.setSampleOption(OPT::OPT_END, yystack_[0].value.as < std::size_t > ()))        error(yystack_[0].location, "invalid @end value");}
#line 882 "array_loader.parser.cpp"
    break;

  case 21:
#line 129 "array_loader.y"
                       { if(!loader.setSampleOption(OPT::OPT_LENGTH, yystack_[0].value.as < std::size_t > ()))     error(yystack_[0].location, "invalid @length value");}
#line 888 "array_loader.parser.cpp"
    break;

  case 22:
#line 130 "array_loader.y"
                       { if(!loader.setGain(yystack_[0].value.as < double > ()))                            error(yystack_[0].location, "invalid @gain value");}
#line 894 "array_loader.parser.cpp"
    break;

  case 23:
#line 131 "array_loader.y"
                       { if(!loader.setGain(ceammc::convert::dbfs2amp(yystack_[1].value.as < double > ()))) error(yystack_[1].location, "invalid @gain value");}
#line 900 "array_loader.parser.cpp"
    break;

  case 24:
#line 132 "array_loader.y"
                                  { if(!loader.setResampleRatio(loader.destSamplerate(), loader.srcSampleRate()))
                                        error(yystack_[0].location, "invalid ratio"); }
#line 907 "array_loader.parser.cpp"
    break;

  case 25:
#line 134 "array_loader.y"
                                  { if(!loader.setResampleRatio(yystack_[0].value.as < double > (), loader.srcSampleRate()))
                                        error(yystack_[0].location, "invalid ratio"); }
#line 914 "array_loader.parser.cpp"
    break;

  case 26:
#line 136 "array_loader.y"
                                  { if(!loader.setResampleRatio(yystack_[2].value.as < int > (), yystack_[0].value.as < int > ()))
                                        error(yystack_[2].location, "invalid ratio"); }
#line 921 "array_loader.parser.cpp"
    break;

  case 27:
#line 138 "array_loader.y"
                                  { for(auto& c: yystack_[0].value.as < ChannelList > ()) loader.addChannel(c); }
#line 927 "array_loader.parser.cpp"
    break;

  case 30:
#line 147 "array_loader.y"
      { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 933 "array_loader.parser.cpp"
    break;

  case 31:
#line 148 "array_loader.y"
             { yylhs.value.as < std::string > () = std::to_string(yystack_[0].value.as < uint > ()); }
#line 939 "array_loader.parser.cpp"
    break;

  case 32:
#line 152 "array_loader.y"
                              { yylhs.value.as < StringList > ().push_back(yystack_[2].value.as < std::string > ()); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 945 "array_loader.parser.cpp"
    break;

  case 33:
#line 153 "array_loader.y"
                              { std::copy(yystack_[2].value.as < StringList > ().begin(), yystack_[2].value.as < StringList > ().end(), std::back_inserter(yylhs.value.as < StringList > ())); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 951 "array_loader.parser.cpp"
    break;

  case 34:
#line 157 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = generateStringRange(0, 9); }
#line 957 "array_loader.parser.cpp"
    break;

  case 35:
#line 158 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = yystack_[1].value.as < StringList > (); }
#line 963 "array_loader.parser.cpp"
    break;

  case 36:
#line 159 "array_loader.y"
                                                        {

        if(yystack_[3].value.as < uint > () > yystack_[1].value.as < uint > ())
            error(yystack_[3].location, "invalid range values");
        else
            yylhs.value.as < StringList > () = generateStringRange(yystack_[3].value.as < uint > (), yystack_[1].value.as < uint > ());
                                                        }
#line 975 "array_loader.parser.cpp"
    break;

  case 37:
#line 169 "array_loader.y"
                                    { loader.addArray(yystack_[0].value.as < std::string > ()); }
#line 981 "array_loader.parser.cpp"
    break;

  case 38:
#line 170 "array_loader.y"
                                    {
        for(auto& s: yystack_[0].value.as < StringList > ())
            loader.addArray(yystack_[1].value.as < std::string > () + s);
                                    }
#line 990 "array_loader.parser.cpp"
    break;

  case 39:
#line 174 "array_loader.y"
                             {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(s + yystack_[0].value.as < std::string > ());
                                    }
#line 999 "array_loader.parser.cpp"
    break;

  case 40:
#line 178 "array_loader.y"
                                    {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(yystack_[2].value.as < std::string > () + s + yystack_[0].value.as < std::string > ());
                                    }
#line 1008 "array_loader.parser.cpp"
    break;


#line 1012 "array_loader.parser.cpp"

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


  const signed char ArrayLoaderParser::yypact_ninf_ = -12;

  const signed char ArrayLoaderParser::yytable_ninf_ = -1;

  const signed char
  ArrayLoaderParser::yypact_[] =
  {
      20,    34,    -3,    -1,   -12,    36,    41,    39,   -12,   -12,
      37,    18,    35,   -12,    20,    -5,   -12,   -12,    24,   -12,
      -2,    -2,   -12,    -6,   -12,    12,    14,    -6,    -6,   -12,
     -12,     7,   -12,    40,   -12,   -12,   -12,   -12,   -12,    19,
     -12,   -12,    -4,    21,    22,   -12,   -12,   -12,   -12,    26,
      30,   -12,    31,   -12,   -12,   -12,   -12,    33,   -12,   -12,
       0,   -12,    38,    42,    25,   -12,    43,   -12
  };

  const signed char
  ArrayLoaderParser::yydefact_[] =
  {
       0,    37,     0,     0,    41,    28,     0,    38,    30,    34,
      31,     0,     0,    39,     0,    43,     1,    40,     0,    35,
       0,     0,    42,     0,    16,     0,    24,     0,     0,    18,
      17,     0,    29,     0,    31,    33,    32,     6,     7,     0,
      21,    12,     8,    22,     7,    25,    19,    20,    13,     0,
      27,    36,     0,     9,    10,    11,    23,     0,    14,    15,
       2,    26,     0,     0,     4,     3,     0,     5
  };

  const signed char
  ArrayLoaderParser::yypgoto_[] =
  {
     -12,   -11,   -12,    10,    50,   -12,    17,   -12,   -12,   -12,
      44,   -12,   -12
  };

  const signed char
  ArrayLoaderParser::yydefgoto_[] =
  {
      -1,    40,    41,    42,     3,    11,    12,    50,    32,    15,
       4,     5,     6
  };

  const signed char
  ArrayLoaderParser::yytable_[] =
  {
       8,     8,    13,     9,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    37,    38,    39,    46,    47,    10,    34,
      53,    54,    55,     1,    19,     2,    20,    48,    62,    49,
      63,    37,    38,    37,    44,    43,    45,    35,    36,     2,
      14,    16,    17,    21,    18,    33,    51,    52,    56,    58,
      59,     7,    60,    61,    57,    66,     0,     0,    22,    64,
       0,     0,     0,    65,    67
  };

  const signed char
  ArrayLoaderParser::yycheck_[] =
  {
       3,     3,     3,     6,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    19,    20,    21,    27,    28,    21,    21,
      24,    25,    26,     3,     6,     5,     8,    20,    28,    22,
      30,    19,    20,    19,    20,    25,    26,    20,    21,     5,
       4,     0,     3,     8,     7,    21,     6,    28,    27,    23,
      20,     1,    21,    20,    32,    30,    -1,    -1,    14,    21,
      -1,    -1,    -1,    21,    21
  };

  const signed char
  ArrayLoaderParser::yystos_[] =
  {
       0,     3,     5,    39,    45,    46,    47,    39,     3,     6,
      21,    40,    41,     3,     4,    44,     0,     3,     7,     6,
       8,     8,    45,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    43,    21,    21,    41,    41,    19,    20,    21,
      36,    37,    38,    38,    20,    38,    36,    36,    20,    22,
      42,     6,    28,    24,    25,    26,    27,    32,    23,    20,
      21,    20,    28,    30,    21,    21,    30,    21
  };

  const signed char
  ArrayLoaderParser::yyr1_[] =
  {
       0,    35,    37,    37,    37,    37,    38,    38,    36,    36,
      36,    36,    36,    42,    42,    42,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    43,    44,    44,
      41,    41,    40,    40,    39,    39,    39,    45,    45,    45,
      45,    46,    46,    47
  };

  const signed char
  ArrayLoaderParser::yyr2_[] =
  {
       0,     2,     3,     5,     5,     7,     1,     1,     1,     2,
       2,     2,     1,     1,     2,     2,     1,     1,     1,     2,
       2,     2,     2,     3,     1,     2,     4,     2,     0,     2,
       1,     1,     3,     3,     2,     3,     5,     1,     2,     2,
       3,     1,     3,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const ArrayLoaderParser::yytname_[] =
  {
  "\"end of string\"", "error", "$undefined", "SYMBOL", "ARRAY_DELIM",
  "PATTERN_BEGIN", "PATTERN_END", "RANGE_DELIM", "VAR_DELIM", "LENGTH",
  "RESIZE", "GAIN", "RESAMPLE", "BEGIN", "END", "VERBOSE", "NORMALIZE",
  "CHANNELS", "SMPTE", "FLOAT", "INT", "UINT", "RANGE_BEGIN", "RANGE_END",
  "SEC", "MSEC", "SAMPLES", "DB", "COLON", "\"colon\"", "DOT", "\"dot\"",
  "FRAC", "\"fraction\"", "LEXER_ERROR", "$accept", "time", "smpte",
  "number", "array_pattern", "var_list", "var", "channel_list", "opt",
  "options", "array_name", "array_list", "expr", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  ArrayLoaderParser::yyrline_[] =
  {
       0,    98,    98,    99,   100,   101,   105,   106,   110,   111,
     112,   113,   114,   118,   119,   120,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   134,   136,   138,   142,   143,
     147,   148,   152,   153,   157,   158,   159,   169,   170,   174,
     178,   185,   186,   190
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
#line 1441 "array_loader.parser.cpp"

#line 193 "array_loader.y"


void ceammc::ArrayLoaderParser::error(const location& loc, const std::string& err_message)
{
    auto line = lexer.matcher().line();
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << line << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;
}


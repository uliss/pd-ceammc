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
#line 39 "array_loader.y"

    # undef yylex
    # define yylex lexer.lex

    # include "array_loader.lexer.h"
    # include "array_loader.h"
    # include "ceammc_convert.h"

    using OPT = ceammc::ArrayLoader::OptionType;
    using OFFSET = ceammc::ArrayLoader::OffsetType;

    static StringList generateStringRange(int from, int to) {
        StringList res;
        res.reserve(to - from + 1);
        for(int i = from; i <= to; i++)
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

#line 74 "array_loader.parser.cpp"




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

#line 7 "array_loader.y"
namespace ceammc { namespace al {
#line 174 "array_loader.parser.cpp"

  /// Build a parser object.
  ArrayLoaderParser::ArrayLoaderParser (ceammc::al::ArrayLoaderLexer& lexer_yyarg, ceammc::ArrayLoader& loader_yyarg)
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
  | symbol kinds.  |
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

  ArrayLoaderParser::symbol_kind_type
  ArrayLoaderParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  ArrayLoaderParser::stack_symbol_type::stack_symbol_type ()
  {}

  ArrayLoaderParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_channel_list: // channel_list
        value.YY_MOVE_OR_COPY< ChannelList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_array_pattern: // array_pattern
        value.YY_MOVE_OR_COPY< StringList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_number: // number
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // INT
      case symbol_kind::S_smpte: // smpte
      case symbol_kind::S_time: // time
        value.YY_MOVE_OR_COPY< int32_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_SMPTE: // SMPTE
      case symbol_kind::S_var: // var
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UINT: // UINT
      case symbol_kind::S_RANGE_BEGIN: // RANGE_BEGIN
      case symbol_kind::S_RANGE_END: // RANGE_END
        value.YY_MOVE_OR_COPY< uint32_t > (YY_MOVE (that.value));
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
    switch (that.kind ())
    {
      case symbol_kind::S_channel_list: // channel_list
        value.move< ChannelList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_array_pattern: // array_pattern
        value.move< StringList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_number: // number
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT: // INT
      case symbol_kind::S_smpte: // smpte
      case symbol_kind::S_time: // time
        value.move< int32_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_SMPTE: // SMPTE
      case symbol_kind::S_var: // var
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UINT: // UINT
      case symbol_kind::S_RANGE_BEGIN: // RANGE_BEGIN
      case symbol_kind::S_RANGE_END: // RANGE_END
        value.move< uint32_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  ArrayLoaderParser::stack_symbol_type&
  ArrayLoaderParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_channel_list: // channel_list
        value.copy< ChannelList > (that.value);
        break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_array_pattern: // array_pattern
        value.copy< StringList > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_number: // number
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INT: // INT
      case symbol_kind::S_smpte: // smpte
      case symbol_kind::S_time: // time
        value.copy< int32_t > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_SMPTE: // SMPTE
      case symbol_kind::S_var: // var
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_UINT: // UINT
      case symbol_kind::S_RANGE_BEGIN: // RANGE_BEGIN
      case symbol_kind::S_RANGE_END: // RANGE_END
        value.copy< uint32_t > (that.value);
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
    switch (that.kind ())
    {
      case symbol_kind::S_channel_list: // channel_list
        value.move< ChannelList > (that.value);
        break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_array_pattern: // array_pattern
        value.move< StringList > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_number: // number
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INT: // INT
      case symbol_kind::S_smpte: // smpte
      case symbol_kind::S_time: // time
        value.move< int32_t > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_SMPTE: // SMPTE
      case symbol_kind::S_var: // var
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_UINT: // UINT
      case symbol_kind::S_RANGE_BEGIN: // RANGE_BEGIN
      case symbol_kind::S_RANGE_END: // RANGE_END
        value.move< uint32_t > (that.value);
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
  ArrayLoaderParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
        YYUSE (yykind);
        yyo << ')';
      }
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
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
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
      case symbol_kind::S_channel_list: // channel_list
        yylhs.value.emplace< ChannelList > ();
        break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_array_pattern: // array_pattern
        yylhs.value.emplace< StringList > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_number: // number
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INT: // INT
      case symbol_kind::S_smpte: // smpte
      case symbol_kind::S_time: // time
        yylhs.value.emplace< int32_t > ();
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
      case symbol_kind::S_SMPTE: // SMPTE
      case symbol_kind::S_var: // var
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_UINT: // UINT
      case symbol_kind::S_RANGE_BEGIN: // RANGE_BEGIN
      case symbol_kind::S_RANGE_END: // RANGE_END
        yylhs.value.emplace< uint32_t > ();
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
  case 2: // smpte: UINT COLON UINT
#line 103 "array_loader.y"
                                          { yylhs.value.as < int32_t > () = loader.smpte2samples(0, yystack_[2].value.as < uint32_t > (), yystack_[0].value.as < uint32_t > (), 0); }
#line 729 "array_loader.parser.cpp"
    break;

  case 3: // smpte: UINT COLON UINT DOT UINT
#line 104 "array_loader.y"
                                          { yylhs.value.as < int32_t > () = loader.smpte2samples(0, yystack_[4].value.as < uint32_t > (), yystack_[2].value.as < uint32_t > (), yystack_[0].value.as < uint32_t > ()); }
#line 735 "array_loader.parser.cpp"
    break;

  case 4: // smpte: UINT COLON UINT COLON UINT
#line 105 "array_loader.y"
                                          { yylhs.value.as < int32_t > () = loader.smpte2samples(yystack_[4].value.as < uint32_t > (), yystack_[2].value.as < uint32_t > (), yystack_[0].value.as < uint32_t > (), 0); }
#line 741 "array_loader.parser.cpp"
    break;

  case 5: // smpte: UINT COLON UINT COLON UINT DOT UINT
#line 106 "array_loader.y"
                                          { yylhs.value.as < int32_t > () = loader.smpte2samples(yystack_[6].value.as < uint32_t > (), yystack_[4].value.as < uint32_t > (), yystack_[2].value.as < uint32_t > (), yystack_[0].value.as < uint32_t > ()); }
#line 747 "array_loader.parser.cpp"
    break;

  case 6: // number: FLOAT
#line 110 "array_loader.y"
      { yylhs.value.as < double > () = yystack_[0].value.as < double > (); }
#line 753 "array_loader.parser.cpp"
    break;

  case 7: // number: INT
#line 111 "array_loader.y"
           { yylhs.value.as < double > () = yystack_[0].value.as < int32_t > (); }
#line 759 "array_loader.parser.cpp"
    break;

  case 8: // time: number
#line 115 "array_loader.y"
                      { yylhs.value.as < int32_t > () = yystack_[0].value.as < double > (); }
#line 765 "array_loader.parser.cpp"
    break;

  case 9: // time: number SEC
#line 116 "array_loader.y"
                      { yylhs.value.as < int32_t > () = loader.sec2samples(yystack_[1].value.as < double > ()); }
#line 771 "array_loader.parser.cpp"
    break;

  case 10: // time: number MSEC
#line 117 "array_loader.y"
                      { yylhs.value.as < int32_t > () = loader.ms2samples(yystack_[1].value.as < double > ()); }
#line 777 "array_loader.parser.cpp"
    break;

  case 11: // time: number SAMPLES
#line 118 "array_loader.y"
                      { yylhs.value.as < int32_t > () = yystack_[1].value.as < double > (); }
#line 783 "array_loader.parser.cpp"
    break;

  case 12: // time: smpte
#line 119 "array_loader.y"
                      { yylhs.value.as < int32_t > () = yystack_[0].value.as < int32_t > (); }
#line 789 "array_loader.parser.cpp"
    break;

  case 13: // channel_list: INT
#line 123 "array_loader.y"
                            { yylhs.value.as < ChannelList > () = {yystack_[0].value.as < int32_t > ()}; }
#line 795 "array_loader.parser.cpp"
    break;

  case 14: // channel_list: RANGE_BEGIN RANGE_END
#line 124 "array_loader.y"
                            { yylhs.value.as < ChannelList > () = generateIntRange(yystack_[1].value.as < uint32_t > (), yystack_[0].value.as < uint32_t > ()); }
#line 801 "array_loader.parser.cpp"
    break;

  case 15: // channel_list: channel_list INT
#line 125 "array_loader.y"
                            { std::copy(yystack_[1].value.as < ChannelList > ().begin(), yystack_[1].value.as < ChannelList > ().end(), std::back_inserter(yylhs.value.as < ChannelList > ())); yylhs.value.as < ChannelList > ().push_back(yystack_[0].value.as < int32_t > ()); }
#line 807 "array_loader.parser.cpp"
    break;

  case 16: // opt: RESIZE
#line 129 "array_loader.y"
                                  { loader.setFlagOption(OPT::OPT_RESIZE); }
#line 813 "array_loader.parser.cpp"
    break;

  case 17: // opt: NORMALIZE
#line 130 "array_loader.y"
                                  { loader.setFlagOption(OPT::OPT_NORMALIZE); }
#line 819 "array_loader.parser.cpp"
    break;

  case 18: // opt: BEGIN time
#line 131 "array_loader.y"
                                  { if(!loader.setBeginOption(yystack_[0].value.as < int32_t > (), OFFSET::OFF_BEGIN))
                                        { error(yystack_[0].location, "invalid @begin value"); return 1; }}
#line 826 "array_loader.parser.cpp"
    break;

  case 19: // opt: BEGIN OFFSET_END time
#line 133 "array_loader.y"
                                  { if(!loader.setBeginOption(yystack_[0].value.as < int32_t > (), OFFSET::OFF_END))
                                        { error(yystack_[1].location, "invalid @begin value"); return 1; }}
#line 833 "array_loader.parser.cpp"
    break;

  case 20: // opt: END time
#line 135 "array_loader.y"
                                  { if(!loader.setEndOption(yystack_[0].value.as < int32_t > (), OFFSET::OFF_BEGIN))
                                        { error(yystack_[0].location, "invalid @end value"); return 1; }}
#line 840 "array_loader.parser.cpp"
    break;

  case 21: // opt: END OFFSET_END time
#line 137 "array_loader.y"
                                  { if(!loader.setEndOption(yystack_[0].value.as < int32_t > (), OFFSET::OFF_END))
                                        { error(yystack_[1].location, "invalid @end value"); return 1; }}
#line 847 "array_loader.parser.cpp"
    break;

  case 22: // opt: LENGTH time
#line 139 "array_loader.y"
                                  { if(!loader.setLengthOption(yystack_[0].value.as < int32_t > ()))
                                        { error(yystack_[0].location, "invalid @length value"); return 1; }}
#line 854 "array_loader.parser.cpp"
    break;

  case 23: // opt: GAIN number
#line 141 "array_loader.y"
                                  { if(!loader.setGain(yystack_[0].value.as < double > ()))
                                        { error(yystack_[0].location, "invalid @gain value"); return 1; }}
#line 861 "array_loader.parser.cpp"
    break;

  case 24: // opt: GAIN number DB
#line 143 "array_loader.y"
                                  { if(!loader.setGain(ceammc::convert::dbfs2amp(yystack_[1].value.as < double > ())))
                                        { error(yystack_[1].location, "invalid @gain value"); return 1; }}
#line 868 "array_loader.parser.cpp"
    break;

  case 25: // opt: RESAMPLE
#line 145 "array_loader.y"
                                  { if(!loader.setResampleRatio(loader.destSamplerate(), loader.srcSampleRate()))
                                        { error(yystack_[0].location, "invalid ratio"); return 1; }}
#line 875 "array_loader.parser.cpp"
    break;

  case 26: // opt: RESAMPLE number
#line 147 "array_loader.y"
                                  { if(!loader.setResampleRatio(yystack_[0].value.as < double > () * loader.srcSampleRate(), loader.srcSampleRate()))
                                        { error(yystack_[0].location, "invalid ratio"); return 1; }}
#line 882 "array_loader.parser.cpp"
    break;

  case 27: // opt: RESAMPLE INT FRAC INT
#line 149 "array_loader.y"
                                  { if(!loader.setResampleRatio(yystack_[2].value.as < int32_t > (), yystack_[0].value.as < int32_t > ()))
                                        { error(yystack_[2].location, "invalid ratio"); return 1; }}
#line 889 "array_loader.parser.cpp"
    break;

  case 28: // opt: CHANNELS channel_list
#line 151 "array_loader.y"
                                  { for(auto& c: yystack_[0].value.as < ChannelList > ())
                                        loader.addChannel(c); }
#line 896 "array_loader.parser.cpp"
    break;

  case 29: // opt: ARRAY_OFFSET INT
#line 153 "array_loader.y"
                                  { if(!loader.setArrayOffset(yystack_[0].value.as < int32_t > (), OFFSET::OFF_BEGIN))
                                        { error(yystack_[0].location, "non-negative value expected"); return 1; }}
#line 903 "array_loader.parser.cpp"
    break;

  case 30: // opt: ARRAY_OFFSET OFFSET_END
#line 155 "array_loader.y"
                                  { loader.setArrayOffset(0, OFFSET::OFF_END); }
#line 909 "array_loader.parser.cpp"
    break;

  case 31: // opt: ARRAY_OFFSET OFFSET_END INT
#line 156 "array_loader.y"
                                  { loader.setArrayOffset(yystack_[0].value.as < int32_t > (), OFFSET::OFF_END); }
#line 915 "array_loader.parser.cpp"
    break;

  case 34: // var: SYMBOL
#line 165 "array_loader.y"
      { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 921 "array_loader.parser.cpp"
    break;

  case 35: // var: UINT
#line 166 "array_loader.y"
             { yylhs.value.as < std::string > () = std::to_string(yystack_[0].value.as < uint32_t > ()); }
#line 927 "array_loader.parser.cpp"
    break;

  case 36: // var_list: var VAR_DELIM var
#line 170 "array_loader.y"
                              { yylhs.value.as < StringList > ().push_back(yystack_[2].value.as < std::string > ()); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 933 "array_loader.parser.cpp"
    break;

  case 37: // var_list: var_list VAR_DELIM var
#line 171 "array_loader.y"
                              { std::copy(yystack_[2].value.as < StringList > ().begin(), yystack_[2].value.as < StringList > ().end(), std::back_inserter(yylhs.value.as < StringList > ())); yylhs.value.as < StringList > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 939 "array_loader.parser.cpp"
    break;

  case 38: // array_pattern: PATTERN_BEGIN PATTERN_END
#line 175 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = generateStringRange(0, 9); }
#line 945 "array_loader.parser.cpp"
    break;

  case 39: // array_pattern: PATTERN_BEGIN var_list PATTERN_END
#line 176 "array_loader.y"
                                                        { yylhs.value.as < StringList > () = yystack_[1].value.as < StringList > (); }
#line 951 "array_loader.parser.cpp"
    break;

  case 40: // array_pattern: PATTERN_BEGIN UINT RANGE_DELIM UINT PATTERN_END
#line 177 "array_loader.y"
                                                        {

        if(yystack_[3].value.as < uint32_t > () > yystack_[1].value.as < uint32_t > ())
            error(yystack_[3].location, "invalid range values");
        else
            yylhs.value.as < StringList > () = generateStringRange(yystack_[3].value.as < uint32_t > (), yystack_[1].value.as < uint32_t > ());
                                                        }
#line 963 "array_loader.parser.cpp"
    break;

  case 41: // array_name: SYMBOL
#line 187 "array_loader.y"
                                    { loader.addArray(yystack_[0].value.as < std::string > ()); }
#line 969 "array_loader.parser.cpp"
    break;

  case 42: // array_name: SYMBOL array_pattern
#line 188 "array_loader.y"
                                    {
        for(auto& s: yystack_[0].value.as < StringList > ())
            loader.addArray(yystack_[1].value.as < std::string > () + s);
                                    }
#line 978 "array_loader.parser.cpp"
    break;

  case 43: // array_name: array_pattern SYMBOL
#line 192 "array_loader.y"
                             {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(s + yystack_[0].value.as < std::string > ());
                                    }
#line 987 "array_loader.parser.cpp"
    break;

  case 44: // array_name: SYMBOL array_pattern SYMBOL
#line 196 "array_loader.y"
                                    {
        for(auto& s: yystack_[1].value.as < StringList > ())
            loader.addArray(yystack_[2].value.as < std::string > () + s + yystack_[0].value.as < std::string > ());
                                    }
#line 996 "array_loader.parser.cpp"
    break;


#line 1000 "array_loader.parser.cpp"

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
  ArrayLoaderParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

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

  std::string
  ArrayLoaderParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // ArrayLoaderParser::context.
  ArrayLoaderParser::context::context (const ArrayLoaderParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  ArrayLoaderParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  ArrayLoaderParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  ArrayLoaderParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char ArrayLoaderParser::yypact_ninf_ = -28;

  const signed char ArrayLoaderParser::yytable_ninf_ = -1;

  const signed char
  ArrayLoaderParser::yypact_[] =
  {
      34,    15,     0,    22,   -28,    42,    54,    52,   -28,   -28,
      23,    48,    32,   -28,    34,    -2,   -28,   -28,    35,     1,
     -28,     1,   -28,    -4,   -28,    28,    30,     6,    11,   -28,
      20,   -16,   -28,    53,   -28,   -28,   -28,   -28,   -28,    29,
     -28,     9,   -28,    33,    31,   -28,    -4,   -28,    -4,   -28,
     -28,    36,    41,    44,   -28,   -28,    45,   -28,   -28,   -28,
     -28,    47,   -28,   -28,   -28,   -28,   -28,    13,   -28,    49,
      50,    38,   -28,    51,   -28
  };

  const signed char
  ArrayLoaderParser::yydefact_[] =
  {
       0,    41,     0,     0,    45,    32,     0,    42,    34,    38,
      35,     0,     0,    43,     0,    47,     1,    44,     0,     0,
      39,     0,    46,     0,    16,     0,    25,     0,     0,    17,
       0,     0,    33,     0,    35,    36,    37,     6,     7,     0,
      12,     8,    22,    23,     7,    26,     0,    18,     0,    20,
      13,     0,    28,    30,    29,    40,     0,     9,    10,    11,
      24,     0,    19,    21,    14,    15,    31,     2,    27,     0,
       0,     4,     3,     0,     5
  };

  const signed char
  ArrayLoaderParser::yypgoto_[] =
  {
     -28,   -28,    27,   -27,   -28,   -28,   -28,    26,   -28,    62,
      56,   -28,   -28
  };

  const signed char
  ArrayLoaderParser::yydefgoto_[] =
  {
      -1,    40,    41,    42,    52,    32,    15,    11,    12,     3,
       4,     5,     6
  };

  const signed char
  ArrayLoaderParser::yytable_[] =
  {
      47,    49,    53,     8,     8,    54,     9,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    37,    38,    39,    62,
       2,    63,    10,    34,    46,    13,    37,    38,    39,    48,
      18,    37,    38,    39,    57,    58,    59,     1,    20,     2,
      21,    50,    69,    51,    70,    35,    14,    36,    37,    38,
      37,    44,    43,    45,    16,    17,    19,    33,    56,    55,
      64,    60,    65,     7,    61,    66,     0,    67,    68,    73,
      22,    71,    72,    74
  };

  const signed char
  ArrayLoaderParser::yycheck_[] =
  {
      27,    28,    18,     3,     3,    21,     6,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    20,    21,    22,    46,
       5,    48,    22,    22,    18,     3,    20,    21,    22,    18,
       7,    20,    21,    22,    25,    26,    27,     3,     6,     5,
       8,    21,    29,    23,    31,    19,     4,    21,    20,    21,
      20,    21,    25,    26,     0,     3,     8,    22,    29,     6,
      24,    28,    21,     1,    33,    21,    -1,    22,    21,    31,
      14,    22,    22,    22
  };

  const signed char
  ArrayLoaderParser::yystos_[] =
  {
       0,     3,     5,    45,    46,    47,    48,    45,     3,     6,
      22,    43,    44,     3,     4,    42,     0,     3,     7,     8,
       6,     8,    46,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    41,    22,    22,    43,    43,    20,    21,    22,
      37,    38,    39,    38,    21,    38,    18,    39,    18,    39,
      21,    23,    40,    18,    21,     6,    29,    25,    26,    27,
      28,    33,    39,    39,    24,    21,    21,    22,    21,    29,
      31,    22,    22,    31,    22
  };

  const signed char
  ArrayLoaderParser::yyr1_[] =
  {
       0,    36,    37,    37,    37,    37,    38,    38,    39,    39,
      39,    39,    39,    40,    40,    40,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    42,    42,    43,    43,    44,    44,    45,    45,
      45,    46,    46,    46,    46,    47,    47,    48
  };

  const signed char
  ArrayLoaderParser::yyr2_[] =
  {
       0,     2,     3,     5,     5,     7,     1,     1,     1,     2,
       2,     2,     1,     1,     2,     2,     1,     1,     2,     3,
       2,     3,     2,     2,     3,     1,     2,     4,     2,     2,
       2,     3,     0,     2,     1,     1,     3,     3,     2,     3,
       5,     1,     2,     2,     3,     1,     3,     2
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const ArrayLoaderParser::yytname_[] =
  {
  "\"end of string\"", "error", "\"invalid token\"", "SYMBOL",
  "ARRAY_DELIM", "PATTERN_BEGIN", "PATTERN_END", "RANGE_DELIM",
  "VAR_DELIM", "LENGTH", "RESIZE", "GAIN", "RESAMPLE", "BEGIN", "END",
  "NORMALIZE", "CHANNELS", "ARRAY_OFFSET", "OFFSET_END", "SMPTE", "FLOAT",
  "INT", "UINT", "RANGE_BEGIN", "RANGE_END", "SEC", "MSEC", "SAMPLES",
  "DB", "COLON", "\"colon\"", "DOT", "\"dot\"", "FRAC", "\"fraction\"",
  "LEXER_ERROR", "$accept", "smpte", "number", "time", "channel_list",
  "opt", "options", "var", "var_list", "array_pattern", "array_name",
  "array_list", "expr", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  ArrayLoaderParser::yyrline_[] =
  {
       0,   103,   103,   104,   105,   106,   110,   111,   115,   116,
     117,   118,   119,   123,   124,   125,   129,   130,   131,   133,
     135,   137,   139,   141,   143,   145,   147,   149,   151,   153,
     155,   156,   160,   161,   165,   166,   170,   171,   175,   176,
     177,   187,   188,   192,   196,   203,   204,   208
  };

  void
  ArrayLoaderParser::yy_stack_print_ () const
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
  ArrayLoaderParser::yy_reduce_print_ (int yyrule) const
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
} } // ceammc::al
#line 1513 "array_loader.parser.cpp"

#line 211 "array_loader.y"


void ceammc::al::ArrayLoaderParser::error(const location& loc, const std::string& err_message)
{
    auto line = lexer.matcher().line();
    lexer.out() << err_message << ':' << std::endl;
    lexer.out() << lexer.indent()
                << line << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;
}


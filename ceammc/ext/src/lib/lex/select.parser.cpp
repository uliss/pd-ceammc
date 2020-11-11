// A Bison parser, made by GNU Bison 3.7.3.

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





#include "select.parser.hpp"


// Unqualified %code blocks.
#line 30 "select.y"

    # undef yylex
    # define yylex lexer.lex

    # include "select.lexer.h"

#line 53 "select.parser.cpp"


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

#line 7 "select.y"
namespace ceammc {
#line 127 "select.parser.cpp"

  /// Build a parser object.
  SelectParser::SelectParser (ceammc::SelectLexer& lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg)
  {}

  SelectParser::~SelectParser ()
  {}

  SelectParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  SelectParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  SelectParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  SelectParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  SelectParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  SelectParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  SelectParser::symbol_kind_type
  SelectParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  SelectParser::stack_symbol_type::stack_symbol_type ()
  {}

  SelectParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_OPEN_BRACKET: // OPEN_BRACKET
      case symbol_kind::S_CLOSE_BRACKET: // CLOSE_BRACKET
      case symbol_kind::S_OPEN_PAR: // OPEN_PAR
      case symbol_kind::S_CLOSE_PAR: // CLOSE_PAR
      case symbol_kind::S_OPEN_B: // OPEN_B
      case symbol_kind::S_CLOSE_B: // CLOSE_B
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_SYMBOL: // SYMBOL
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

  SelectParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_OPEN_BRACKET: // OPEN_BRACKET
      case symbol_kind::S_CLOSE_BRACKET: // CLOSE_BRACKET
      case symbol_kind::S_OPEN_PAR: // OPEN_PAR
      case symbol_kind::S_CLOSE_PAR: // CLOSE_PAR
      case symbol_kind::S_OPEN_B: // OPEN_B
      case symbol_kind::S_CLOSE_B: // CLOSE_B
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_SYMBOL: // SYMBOL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  SelectParser::stack_symbol_type&
  SelectParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_OPEN_BRACKET: // OPEN_BRACKET
      case symbol_kind::S_CLOSE_BRACKET: // CLOSE_BRACKET
      case symbol_kind::S_OPEN_PAR: // OPEN_PAR
      case symbol_kind::S_CLOSE_PAR: // CLOSE_PAR
      case symbol_kind::S_OPEN_B: // OPEN_B
      case symbol_kind::S_CLOSE_B: // CLOSE_B
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_SYMBOL: // SYMBOL
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  SelectParser::stack_symbol_type&
  SelectParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_OPEN_BRACKET: // OPEN_BRACKET
      case symbol_kind::S_CLOSE_BRACKET: // CLOSE_BRACKET
      case symbol_kind::S_OPEN_PAR: // OPEN_PAR
      case symbol_kind::S_CLOSE_PAR: // CLOSE_PAR
      case symbol_kind::S_OPEN_B: // OPEN_B
      case symbol_kind::S_CLOSE_B: // CLOSE_B
        value.move< int > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_SYMBOL: // SYMBOL
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
  SelectParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  SelectParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  SelectParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  SelectParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  SelectParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SelectParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SelectParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SelectParser::debug_level_type
  SelectParser::debug_level () const
  {
    return yydebug_;
  }

  void
  SelectParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  SelectParser::state_type
  SelectParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  SelectParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  SelectParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SelectParser::operator() ()
  {
    return parse ();
  }

  int
  SelectParser::parse ()
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
      case symbol_kind::S_OPEN_BRACKET: // OPEN_BRACKET
      case symbol_kind::S_CLOSE_BRACKET: // CLOSE_BRACKET
      case symbol_kind::S_OPEN_PAR: // OPEN_PAR
      case symbol_kind::S_CLOSE_PAR: // CLOSE_PAR
      case symbol_kind::S_OPEN_B: // OPEN_B
      case symbol_kind::S_CLOSE_B: // CLOSE_B
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_SYMBOL: // SYMBOL
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
  case 2: // OPEN_B: OPEN_BRACKET
#line 62 "select.y"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 579 "select.parser.cpp"
    break;

  case 3: // OPEN_B: OPEN_PAR
#line 63 "select.y"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 585 "select.parser.cpp"
    break;

  case 4: // CLOSE_B: CLOSE_BRACKET
#line 67 "select.y"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 591 "select.parser.cpp"
    break;

  case 5: // CLOSE_B: CLOSE_PAR
#line 68 "select.y"
      { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 597 "select.parser.cpp"
    break;

  case 6: // ATOM0: FLOAT
#line 72 "select.y"
             { lexer.pushLexerAtom({yystack_[0].value.as < std::string > (), true}); }
#line 603 "select.parser.cpp"
    break;

  case 7: // ATOM0: SYMBOL
#line 73 "select.y"
             { lexer.pushLexerAtom({yystack_[0].value.as < std::string > (), false}); }
#line 609 "select.parser.cpp"
    break;

  case 10: // SET: ATOM
#line 82 "select.y"
             { lexer.popLexerAtoms(); }
#line 615 "select.parser.cpp"
    break;

  case 11: // SET: SET OR ATOM
#line 83 "select.y"
                   { lexer.popLexerAtoms(); lexer.mergeMatch(); }
#line 621 "select.parser.cpp"
    break;

  case 12: // COMPARE: OP_LESS FLOAT
#line 87 "select.y"
                    {
        const auto a = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_LESS));
      }
#line 630 "select.parser.cpp"
    break;

  case 13: // COMPARE: OP_LESS_EQ FLOAT
#line 91 "select.y"
                       {
        const auto a = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_LESS_EQ));
      }
#line 639 "select.parser.cpp"
    break;

  case 14: // COMPARE: OP_GREATER_EQ FLOAT
#line 95 "select.y"
                          {
        const auto a = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_GREATER_EQ));
      }
#line 648 "select.parser.cpp"
    break;

  case 15: // COMPARE: OP_GREATER FLOAT
#line 99 "select.y"
                       {
        const auto a = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::compare(a, SelectLexer::MATCH_GREATER));
      }
#line 657 "select.parser.cpp"
    break;

  case 16: // EXPR: FLOAT RANGE FLOAT
#line 106 "select.y"
                        {
        const auto a = std::strtod(yystack_[2].value.as < std::string > ().c_str(), nullptr);
        const auto b = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CC));
      }
#line 667 "select.parser.cpp"
    break;

  case 17: // EXPR: FLOAT EPSILON FLOAT
#line 111 "select.y"
                          {
        const auto a = std::strtod(yystack_[2].value.as < std::string > ().c_str(), nullptr);
        const auto b = std::strtod(yystack_[0].value.as < std::string > ().c_str(), nullptr);
        lexer.pushMatch(SelectLexer::MatchData::epsilon(a, b));
      }
#line 677 "select.parser.cpp"
    break;

  case 18: // EXPR: OPEN_B FLOAT RANGE FLOAT CLOSE_B
#line 116 "select.y"
                                       {
        const auto a = std::strtod(yystack_[3].value.as < std::string > ().c_str(), nullptr);
        const auto b = std::strtod(yystack_[1].value.as < std::string > ().c_str(), nullptr);
        if(yystack_[4].value.as < int > () == '[' && yystack_[0].value.as < int > () == ']')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CC));
        else if(yystack_[4].value.as < int > () == '[' && yystack_[0].value.as < int > () == ')')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_CO));
        else if(yystack_[4].value.as < int > () == '(' && yystack_[0].value.as < int > () == ']')
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_OC));
        else
            lexer.pushMatch(SelectLexer::MatchData::range(a, b, SelectLexer::MATCH_RANGE_OO));
      }
#line 694 "select.parser.cpp"
    break;


#line 698 "select.parser.cpp"

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
  SelectParser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  SelectParser::yytnamerr_ (const char *yystr)
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
  SelectParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // SelectParser::context.
  SelectParser::context::context (const SelectParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  SelectParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  SelectParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  SelectParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char SelectParser::yypact_ninf_ = -12;

  const signed char SelectParser::yytable_ninf_ = -1;

  const signed char
  SelectParser::yypact_[] =
  {
      -6,    12,   -12,   -12,   -12,    -3,    -1,     5,     9,    16,
     -12,    14,    15,   -12,   -12,    11,    17,    18,   -12,   -12,
     -12,   -12,    21,   -12,   -12,    14,   -12,    -6,   -12,   -12,
      20,    14,   -12,     3,   -12,   -12,   -12
  };

  const signed char
  SelectParser::yydefact_[] =
  {
      21,     6,     7,     2,     3,     0,     0,     0,     0,     0,
       8,    10,    20,    19,    22,     0,     0,     0,    12,    13,
      15,    14,     0,     6,     9,     0,     1,     0,    16,    17,
       0,    11,    23,     0,     4,     5,    18
  };

  const signed char
  SelectParser::yypgoto_[] =
  {
     -12,   -12,   -12,   -11,     4,   -12,   -12,     1,   -12
  };

  const signed char
  SelectParser::yydefgoto_[] =
  {
      -1,     9,    36,    10,    11,    12,    13,    14,    15
  };

  const signed char
  SelectParser::yytable_[] =
  {
      24,     1,     2,     3,    18,     4,    19,     5,     6,     7,
       8,    26,    20,    34,    27,    35,    21,    16,    17,    25,
      24,    23,     2,    22,    28,    29,    30,    33,    32,    31
  };

  const signed char
  SelectParser::yycheck_[] =
  {
      11,     7,     8,     9,     7,    11,     7,    13,    14,    15,
      16,     0,     7,    10,     3,    12,     7,     5,     6,     4,
      31,     7,     8,     7,     7,     7,     5,     7,    27,    25
  };

  const signed char
  SelectParser::yystos_[] =
  {
       0,     7,     8,     9,    11,    13,    14,    15,    16,    18,
      20,    21,    22,    23,    24,    25,     5,     6,     7,     7,
       7,     7,     7,     7,    20,     4,     0,     3,     7,     7,
       5,    21,    24,     7,    10,    12,    19
  };

  const signed char
  SelectParser::yyr1_[] =
  {
       0,    17,    18,    18,    19,    19,    20,    20,    21,    21,
      22,    22,    23,    23,    23,    23,    24,    24,    24,    24,
      24,    25,    25,    25
  };

  const signed char
  SelectParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     3,     2,     2,     2,     2,     3,     3,     5,     1,
       1,     0,     1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const SelectParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "SPACE", "OR", "RANGE",
  "EPSILON", "FLOAT", "SYMBOL", "OPEN_BRACKET", "CLOSE_BRACKET",
  "OPEN_PAR", "CLOSE_PAR", "OP_LESS", "OP_LESS_EQ", "OP_GREATER",
  "OP_GREATER_EQ", "$accept", "OPEN_B", "CLOSE_B", "ATOM0", "ATOM", "SET",
  "COMPARE", "EXPR", "EXPRLIST", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  SelectParser::yyrline_[] =
  {
       0,    62,    62,    63,    67,    68,    72,    73,    77,    78,
      82,    83,    87,    91,    95,    99,   106,   111,   116,   128,
     129,   133,   134,   135
  };

  void
  SelectParser::yy_stack_print_ () const
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
  SelectParser::yy_reduce_print_ (int yyrule) const
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


#line 7 "select.y"
} // ceammc
#line 1173 "select.parser.cpp"

#line 138 "select.y"


void ceammc::SelectParser::error(const std::string& err_message)
{
    std::cerr << err_message << std::endl;
}


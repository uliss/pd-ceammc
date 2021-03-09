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





#include "math_expr.parser.hpp"


// Unqualified %code blocks.
#line 25 "math_expr.y"

    # include <cmath>   /* For math functions: cos(), sin(), etc. */
    # include <cstring>
    # include <cstdio>

    # include "m_pd.h"
    # include "math_expr_ast.h"
    # include "math_expr.lexer.h"

    static double fn_plus(double d0, double d1) { return d0 + d1; }
    static double fn_minus(double d0, double d1) { return d0 - d1; }
    static double fn_mul(double d0, double d1) { return d0 * d1; }
    static double fn_div(double d0, double d1) { return d0 / d1; }
    static double fn_mod(double d0, double d1) { return (long)d0 % (long)d1; }
    static double fn_pow(double d0, double d1) { return pow(d0, d1); }
    static double fn_neg(double d0) { return -d0; }

    static double fn_eq(double d0, double d1) { return d0 == d1; }
    static double fn_ne(double d0, double d1) { return d0 != d1; }
    static double fn_le(double d0, double d1) { return d0 <= d1; }
    static double fn_lt(double d0, double d1) { return d0 < d1; }
    static double fn_ge(double d0, double d1) { return d0 >= d1; }
    static double fn_gt(double d0, double d1) { return d0 > d1; }
    static double fn_approx_eq(double d0, double d1) {
        const double epsilon = 1.0e-08;
        double da0 = fabs(d0);
        double da1 = fabs(d1);

        double x = fabs(d0 - d1);
        if (x <= epsilon)
            return 1;

        double max = (da0 < da1) ? da1 : da0;
        return x <= (epsilon * max);
    }

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()


#line 87 "math_expr.parser.cpp"


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

#line 7 "math_expr.y"
namespace ceammc { namespace math {
#line 180 "math_expr.parser.cpp"

  /// Build a parser object.
  MathExprParser::MathExprParser (ceammc::math::MathExprLexer& lexer_yyarg, ceammc::math::Ast& tree_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      tree (tree_yyarg)
  {}

  MathExprParser::~MathExprParser ()
  {}

  MathExprParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  MathExprParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  MathExprParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  MathExprParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  MathExprParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  MathExprParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  MathExprParser::symbol_kind_type
  MathExprParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  MathExprParser::stack_symbol_type::stack_symbol_type ()
  {}

  MathExprParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_input: // input
      case symbol_kind::S_array_at: // array_at
      case symbol_kind::S_unary_func: // unary_func
      case symbol_kind::S_binary_func: // binary_func
      case symbol_kind::S_group: // group
      case symbol_kind::S_exp: // exp
        value.YY_MOVE_OR_COPY< Node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_REF: // REF
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

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

  MathExprParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_input: // input
      case symbol_kind::S_array_at: // array_at
      case symbol_kind::S_unary_func: // unary_func
      case symbol_kind::S_binary_func: // binary_func
      case symbol_kind::S_group: // group
      case symbol_kind::S_exp: // exp
        value.move< Node > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_REF: // REF
        value.move< double > (YY_MOVE (that.value));
        break;

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
  MathExprParser::stack_symbol_type&
  MathExprParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_input: // input
      case symbol_kind::S_array_at: // array_at
      case symbol_kind::S_unary_func: // unary_func
      case symbol_kind::S_binary_func: // binary_func
      case symbol_kind::S_group: // group
      case symbol_kind::S_exp: // exp
        value.copy< Node > (that.value);
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_REF: // REF
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  MathExprParser::stack_symbol_type&
  MathExprParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_input: // input
      case symbol_kind::S_array_at: // array_at
      case symbol_kind::S_unary_func: // unary_func
      case symbol_kind::S_binary_func: // binary_func
      case symbol_kind::S_group: // group
      case symbol_kind::S_exp: // exp
        value.move< Node > (that.value);
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_REF: // REF
        value.move< double > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
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
  MathExprParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  MathExprParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  MathExprParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  MathExprParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  MathExprParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  MathExprParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  MathExprParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  MathExprParser::debug_level_type
  MathExprParser::debug_level () const
  {
    return yydebug_;
  }

  void
  MathExprParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  MathExprParser::state_type
  MathExprParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  MathExprParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  MathExprParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  MathExprParser::operator() ()
  {
    return parse ();
  }

  int
  MathExprParser::parse ()
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
      case symbol_kind::S_input: // input
      case symbol_kind::S_array_at: // array_at
      case symbol_kind::S_unary_func: // unary_func
      case symbol_kind::S_binary_func: // binary_func
      case symbol_kind::S_group: // group
      case symbol_kind::S_exp: // exp
        yylhs.value.emplace< Node > ();
        break;

      case symbol_kind::S_NUM: // NUM
      case symbol_kind::S_REF: // REF
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
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
  case 2: // input: exp
#line 86 "math_expr.y"
            { tree.root.add(yystack_[0].value.as < Node > ()); }
#line 665 "math_expr.parser.cpp"
    break;

  case 3: // array_at: SYMBOL SQR_OPEN exp SQR_CLOSE
#line 91 "math_expr.y"
        { yylhs.value.as < Node > () = Node::createArrayFunc(gensym(yystack_[3].value.as < std::string > ().c_str()), yystack_[1].value.as < Node > ()); }
#line 671 "math_expr.parser.cpp"
    break;

  case 4: // unary_func: SYMBOL PAR_OPEN exp PAR_CLOSE
#line 94 "math_expr.y"
    {
        UnaryFloatFunc fn;
        if (getUnaryFunction(yystack_[3].value.as < std::string > (), fn))
            yylhs.value.as < Node > () = Node::createUnaryFunction(fn, yystack_[1].value.as < Node > ());
        else {
            throw std::runtime_error(std::string("unknown unary function: " + yystack_[3].value.as < std::string > ()));
        }
    }
#line 684 "math_expr.parser.cpp"
    break;

  case 5: // binary_func: SYMBOL PAR_OPEN exp COMMA exp PAR_CLOSE
#line 104 "math_expr.y"
    {
        BinaryFloatFunc fn;
        if (getBinaryFunction(yystack_[5].value.as < std::string > (), fn))
            yylhs.value.as < Node > () = Node::createBinaryFunction(fn, yystack_[3].value.as < Node > (), yystack_[1].value.as < Node > ());
        else
            throw std::runtime_error(std::string("unknown binary function: " + yystack_[5].value.as < std::string > ()));
    }
#line 696 "math_expr.parser.cpp"
    break;

  case 6: // group: PAR_OPEN exp PAR_CLOSE
#line 113 "math_expr.y"
        { yylhs.value.as < Node > () = Node::createGroup(yystack_[1].value.as < Node > ()); }
#line 702 "math_expr.parser.cpp"
    break;

  case 7: // exp: NUM
#line 116 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createValue(yystack_[0].value.as < double > ()); }
#line 708 "math_expr.parser.cpp"
    break;

  case 8: // exp: REF
#line 117 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createRef(tree.ref(yystack_[0].value.as < double > ())); }
#line 714 "math_expr.parser.cpp"
    break;

  case 9: // exp: exp T_EQ exp
#line 118 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_eq, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 720 "math_expr.parser.cpp"
    break;

  case 10: // exp: exp T_APPROX_EQ exp
#line 119 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_approx_eq, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ()); }
#line 726 "math_expr.parser.cpp"
    break;

  case 11: // exp: exp T_NOT_EQ exp
#line 120 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_ne, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 732 "math_expr.parser.cpp"
    break;

  case 12: // exp: exp T_LT exp
#line 121 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_lt, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 738 "math_expr.parser.cpp"
    break;

  case 13: // exp: exp T_LE exp
#line 122 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_le, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 744 "math_expr.parser.cpp"
    break;

  case 14: // exp: exp T_GT exp
#line 123 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_gt, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 750 "math_expr.parser.cpp"
    break;

  case 15: // exp: exp T_GE exp
#line 124 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_ge, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());        }
#line 756 "math_expr.parser.cpp"
    break;

  case 16: // exp: exp PLUS exp
#line 125 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_plus, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());      }
#line 762 "math_expr.parser.cpp"
    break;

  case 17: // exp: exp MINUS exp
#line 126 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_minus, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());     }
#line 768 "math_expr.parser.cpp"
    break;

  case 18: // exp: exp MUL exp
#line 127 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_mul, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());       }
#line 774 "math_expr.parser.cpp"
    break;

  case 19: // exp: exp DIV exp
#line 128 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_div, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());       }
#line 780 "math_expr.parser.cpp"
    break;

  case 20: // exp: exp MOD exp
#line 129 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_mod, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());       }
#line 786 "math_expr.parser.cpp"
    break;

  case 21: // exp: MINUS exp
#line 130 "math_expr.y"
                            { yylhs.value.as < Node > () = Node::createUnaryFunction(fn_neg, yystack_[0].value.as < Node > ());            }
#line 792 "math_expr.parser.cpp"
    break;

  case 22: // exp: exp EXP exp
#line 131 "math_expr.y"
                          { yylhs.value.as < Node > () = Node::createBinaryFunction(fn_pow, yystack_[2].value.as < Node > (), yystack_[0].value.as < Node > ());       }
#line 798 "math_expr.parser.cpp"
    break;

  case 23: // exp: group
#line 132 "math_expr.y"
      { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 804 "math_expr.parser.cpp"
    break;

  case 24: // exp: array_at
#line 133 "math_expr.y"
      { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 810 "math_expr.parser.cpp"
    break;

  case 25: // exp: unary_func
#line 134 "math_expr.y"
      { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 816 "math_expr.parser.cpp"
    break;

  case 26: // exp: binary_func
#line 135 "math_expr.y"
      { yylhs.value.as < Node > () = yystack_[0].value.as < Node > (); }
#line 822 "math_expr.parser.cpp"
    break;


#line 826 "math_expr.parser.cpp"

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
  MathExprParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  MathExprParser::yytnamerr_ (const char *yystr)
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
  MathExprParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // MathExprParser::context.
  MathExprParser::context::context (const MathExprParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  MathExprParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  MathExprParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  MathExprParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char MathExprParser::yypact_ninf_ = -7;

  const signed char MathExprParser::yytable_ninf_ = -1;

  const signed char
  MathExprParser::yypact_[] =
  {
     124,   124,   124,    -7,    -7,    26,    12,    -7,    -7,    -7,
      -7,   104,    68,     2,   124,   124,    -7,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
      -7,    30,    49,    -2,    -2,     2,     2,     2,     2,    -6,
      -6,    -6,    -6,    -6,    -6,    -6,    -7,    -7,   124,    87,
      -7
  };

  const signed char
  MathExprParser::yydefact_[] =
  {
       0,     0,     0,     7,     8,     0,     0,    24,    25,    26,
      23,     2,     0,    21,     0,     0,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,     0,    16,    17,    18,    19,    20,    22,     9,
      11,    10,    13,    12,    14,    15,     3,     4,     0,     0,
       5
  };

  const signed char
  MathExprParser::yypgoto_[] =
  {
      -7,    -7,    -7,    -7,    -7,    -7,    -1
  };

  const signed char
  MathExprParser::yydefgoto_[] =
  {
      -1,     6,     7,     8,     9,    10,    11
  };

  const signed char
  MathExprParser::yytable_[] =
  {
      12,    13,    17,    18,    19,    20,    21,    22,    19,    20,
      21,    22,    16,    31,    32,    22,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    14,
       0,    15,     0,     0,    46,     0,     0,     0,    17,    18,
      19,    20,    21,    22,     0,     0,     0,    49,    23,    24,
      25,    26,    27,    28,    29,    47,    48,    17,    18,    19,
      20,    21,    22,     0,     0,     0,     0,    23,    24,    25,
      26,    27,    28,    29,    30,     0,    17,    18,    19,    20,
      21,    22,     0,     0,     0,     0,    23,    24,    25,    26,
      27,    28,    29,    50,     0,    17,    18,    19,    20,    21,
      22,     0,     0,     0,     0,    23,    24,    25,    26,    27,
      28,    29,    17,    18,    19,    20,    21,    22,     0,     0,
       0,     0,    23,    24,    25,    26,    27,    28,    29,     1,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     3,
       4,     5
  };

  const signed char
  MathExprParser::yycheck_[] =
  {
       1,     2,     8,     9,    10,    11,    12,    13,    10,    11,
      12,    13,     0,    14,    15,    13,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     3,
      -1,     5,    -1,    -1,     4,    -1,    -1,    -1,     8,     9,
      10,    11,    12,    13,    -1,    -1,    -1,    48,    18,    19,
      20,    21,    22,    23,    24,     6,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,     6,    -1,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,     6,    -1,     8,     9,    10,    11,    12,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,     5,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    -1,    -1,    15,
      16,    17
  };

  const signed char
  MathExprParser::yystos_[] =
  {
       0,     5,     9,    15,    16,    17,    27,    28,    29,    30,
      31,    32,    32,    32,     3,     5,     0,     8,     9,    10,
      11,    12,    13,    18,    19,    20,    21,    22,    23,    24,
       6,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    32,    32,    32,    32,     4,     6,     7,    32,
       6
  };

  const signed char
  MathExprParser::yyr1_[] =
  {
       0,    26,    27,    28,    29,    30,    31,    32,    32,    32,
      32,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      32,    32,    32,    32,    32,    32,    32
  };

  const signed char
  MathExprParser::yyr2_[] =
  {
       0,     2,     1,     4,     4,     6,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     1,     1,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const MathExprParser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "SQR_OPEN",
  "SQR_CLOSE", "PAR_OPEN", "PAR_CLOSE", "COMMA", "PLUS", "MINUS", "MUL",
  "DIV", "MOD", "EXP", "LEXER_ERROR", "NUM", "REF", "SYMBOL", "T_EQ",
  "T_NOT_EQ", "T_APPROX_EQ", "T_LE", "T_LT", "T_GT", "T_GE", "T_NEG",
  "$accept", "input", "array_at", "unary_func", "binary_func", "group",
  "exp", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  MathExprParser::yyrline_[] =
  {
       0,    86,    86,    90,    93,   103,   112,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135
  };

  void
  MathExprParser::yy_stack_print_ () const
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
  MathExprParser::yy_reduce_print_ (int yyrule) const
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


#line 7 "math_expr.y"
} } // ceammc::math
#line 1336 "math_expr.parser.cpp"

#line 140 "math_expr.y"


void ceammc::math::MathExprParser::error(const ceammc::math::location& loc, const std::string& str)
{

    pd_error(0, "[math.expr] parse error: %s", str.c_str());
}


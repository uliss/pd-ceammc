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





#include "data_string.parser.hpp"


// Unqualified %code blocks.
#line 37 "data_string.y"

    # include <memory>
    # include <string>
    # include <utility>

    // header file generated with reflex
    # include "data_string.lexer.h"

    # include "ceammc_atomlist.h"
    # include "ceammc_datastorage.h"
    # include "ceammc_function.h"
    # include "ceammc_datatypes.h"
    # include "ceammc_log.h"
    # include "ceammc_format.h"
    # include "ceammc_string.h"
    # include "datatype_dict.h"
    # include "datatype_mlist.h"
    # include "datatype_string.h"
    # include "fmt/format.h"

    static ceammc::Atom createDataDict(const ceammc::Dict& v) {
        using namespace ceammc;

        auto dict = new DataTypeDict;

        if(v.empty())
            return dict;

        for(auto& kv: v) {
            auto key = gensym(kv.first.c_str());
            auto& val = kv.second;

            if(val.isA<void>())
                dict->insert(key, AtomList());
            else if(val.isA<Atom>())
                dict->insert(key, val.asT<Atom>());
            else
                dict->insert(key, val);
        }

        return dict;
    }

    // create t_symbol* Atom with unescaped string
    static ceammc::Atom createSimpleString(const std::string& str) {
        return gensym(ceammc::string::pd_string_unescape(str).c_str());
    }

    // create Atom with DataTypeString
    static inline ceammc::Atom createDataString(const std::string& str) {
        return new ceammc::DataTypeString(ceammc::string::pd_string_unescape(str).c_str());
    }

    static inline ceammc::Atom createDataList(const ceammc::AtomList& l) {
        return new ceammc::DataTypeMList(l);
    }

    static inline ceammc::AtomList callFunction(const std::string& name, const ceammc::AtomList& args) {
        return ceammc::BuiltinFunctionMap::instance().call(gensym(name.c_str()), args);
    }

    static inline ceammc::Atom createFromList(const std::string& name, const ceammc::AtomList& args) {
        using namespace ceammc;

        auto fn = DataStorage::instance().fromListFunction(name);
        if(!fn) {
            LIB_ERR << fmt::format("datatype {} not found", name);
            return Atom();
        }

        return fn(args);
    }

    static inline ceammc::Atom createFromDict(const std::string& name, const ceammc::Dict& v) {
        using namespace ceammc;

        auto fn = DataStorage::instance().fromDictFunction(name);
        if(fn == nullptr) {
            LIB_ERR << fmt::format("can't create type {} from dict", name);
            return Atom();
        }

        return fn(v);
    }

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

#line 135 "data_string.parser.cpp"


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

#line 8 "data_string.y"
namespace ceammc { namespace ds {
#line 228 "data_string.parser.cpp"

  /// Build a parser object.
  DataStringParser::DataStringParser (ceammc::ds::DataStringLexer& lexer_yyarg, ceammc::AtomList& result_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      result (result_yyarg)
  {}

  DataStringParser::~DataStringParser ()
  {}

  DataStringParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  DataStringParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  DataStringParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  DataStringParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  DataStringParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  DataStringParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  DataStringParser::symbol_kind_type
  DataStringParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  DataStringParser::stack_symbol_type::stack_symbol_type ()
  {}

  DataStringParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.YY_MOVE_OR_COPY< ceammc::Atom > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_expr: // expr
        value.YY_MOVE_OR_COPY< ceammc::AtomList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair_list: // pair_list
        value.YY_MOVE_OR_COPY< ceammc::Dict > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair: // pair
        value.YY_MOVE_OR_COPY< ceammc::DictEntry > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        value.YY_MOVE_OR_COPY< const char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_DATA_TYPE: // DATA_TYPE
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_STRING: // STRING
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

  DataStringParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.move< ceammc::Atom > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_expr: // expr
        value.move< ceammc::AtomList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair_list: // pair_list
        value.move< ceammc::Dict > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair: // pair
        value.move< ceammc::DictEntry > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        value.move< const char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_DATA_TYPE: // DATA_TYPE
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_STRING: // STRING
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  DataStringParser::stack_symbol_type&
  DataStringParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.copy< ceammc::Atom > (that.value);
        break;

      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_expr: // expr
        value.copy< ceammc::AtomList > (that.value);
        break;

      case symbol_kind::S_pair_list: // pair_list
        value.copy< ceammc::Dict > (that.value);
        break;

      case symbol_kind::S_pair: // pair
        value.copy< ceammc::DictEntry > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        value.copy< const char* > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_DATA_TYPE: // DATA_TYPE
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_STRING: // STRING
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  DataStringParser::stack_symbol_type&
  DataStringParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.move< ceammc::Atom > (that.value);
        break;

      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_expr: // expr
        value.move< ceammc::AtomList > (that.value);
        break;

      case symbol_kind::S_pair_list: // pair_list
        value.move< ceammc::Dict > (that.value);
        break;

      case symbol_kind::S_pair: // pair
        value.move< ceammc::DictEntry > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        value.move< const char* > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (that.value);
        break;

      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_DATA_TYPE: // DATA_TYPE
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_STRING: // STRING
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
  DataStringParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  DataStringParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  DataStringParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  DataStringParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  DataStringParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  DataStringParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  DataStringParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  DataStringParser::debug_level_type
  DataStringParser::debug_level () const
  {
    return yydebug_;
  }

  void
  DataStringParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  DataStringParser::state_type
  DataStringParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  DataStringParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  DataStringParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  DataStringParser::operator() ()
  {
    return parse ();
  }

  int
  DataStringParser::parse ()
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
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        yylhs.value.emplace< ceammc::Atom > ();
        break;

      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_expr: // expr
        yylhs.value.emplace< ceammc::AtomList > ();
        break;

      case symbol_kind::S_pair_list: // pair_list
        yylhs.value.emplace< ceammc::Dict > ();
        break;

      case symbol_kind::S_pair: // pair
        yylhs.value.emplace< ceammc::DictEntry > ();
        break;

      case symbol_kind::S_SYMBOL: // SYMBOL
        yylhs.value.emplace< const char* > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_DATA_TYPE: // DATA_TYPE
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_STRING: // STRING
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
  case 2: // atom: NULL
#line 153 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(); }
#line 793 "data_string.parser.cpp"
    break;

  case 3: // atom: COMMA
#line 154 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(gensym(",")); }
#line 799 "data_string.parser.cpp"
    break;

  case 4: // atom: FLOAT
#line 155 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(yystack_[0].value.as < double > ()); }
#line 805 "data_string.parser.cpp"
    break;

  case 5: // atom: SYMBOL
#line 156 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(gensym(yystack_[0].value.as < const char* > ())); }
#line 811 "data_string.parser.cpp"
    break;

  case 6: // atom: STRING
#line 157 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = createSimpleString(yystack_[0].value.as < std::string > ()); }
#line 817 "data_string.parser.cpp"
    break;

  case 7: // atom: DATA_TYPE_STRING STRING
#line 158 "data_string.y"
                              { yylhs.value.as < ceammc::Atom > () = createDataString(yystack_[0].value.as < std::string > ()); }
#line 823 "data_string.parser.cpp"
    break;

  case 8: // atom: data
#line 159 "data_string.y"
      { yylhs.value.as < ceammc::Atom > () = yystack_[0].value.as < ceammc::Atom > (); }
#line 829 "data_string.parser.cpp"
    break;

  case 9: // atom_list: %empty
#line 163 "data_string.y"
                       { yylhs.value.as < ceammc::AtomList > () = ceammc::AtomList(); }
#line 835 "data_string.parser.cpp"
    break;

  case 10: // atom_list: atom_list atom
#line 164 "data_string.y"
                       { yylhs.value.as < ceammc::AtomList > ().append(yystack_[1].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::Atom > ()); }
#line 841 "data_string.parser.cpp"
    break;

  case 11: // atom_list: atom_list function_call
#line 165 "data_string.y"
                              { yylhs.value.as < ceammc::AtomList > ().append(yystack_[1].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 847 "data_string.parser.cpp"
    break;

  case 12: // pair: KEY atom_list
#line 169 "data_string.y"
                      { yylhs.value.as < ceammc::DictEntry > () = { yystack_[1].value.as < std::string > (), yystack_[0].value.as < ceammc::AtomList > () }; }
#line 853 "data_string.parser.cpp"
    break;

  case 13: // pair_list: %empty
#line 173 "data_string.y"
                     { yylhs.value.as < ceammc::Dict > () = Dict(); }
#line 859 "data_string.parser.cpp"
    break;

  case 14: // pair_list: pair_list pair
#line 174 "data_string.y"
                     { yylhs.value.as < ceammc::Dict > ().insert(yylhs.value.as < ceammc::Dict > ().end(), yystack_[1].value.as < ceammc::Dict > ().begin(), yystack_[1].value.as < ceammc::Dict > ().end()); yylhs.value.as < ceammc::Dict > ().push_back(yystack_[0].value.as < ceammc::DictEntry > ()); }
#line 865 "data_string.parser.cpp"
    break;

  case 15: // function_call: FUNC_CALL OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET
#line 178 "data_string.y"
                                                                 { yylhs.value.as < ceammc::AtomList > () = callFunction(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ceammc::AtomList > ()); }
#line 871 "data_string.parser.cpp"
    break;

  case 16: // data: OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET
#line 182 "data_string.y"
                                                                 { yylhs.value.as < ceammc::Atom > () = createDataDict(yystack_[1].value.as < ceammc::Dict > ()); }
#line 877 "data_string.parser.cpp"
    break;

  case 17: // data: DATA_TYPE OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET
#line 183 "data_string.y"
                                                                 { yylhs.value.as < ceammc::Atom > () = createFromDict(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ceammc::Dict > ()); }
#line 883 "data_string.parser.cpp"
    break;

  case 18: // data: OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET
#line 184 "data_string.y"
                                                                 { yylhs.value.as < ceammc::Atom > () = createDataList(yystack_[1].value.as < ceammc::AtomList > ()); }
#line 889 "data_string.parser.cpp"
    break;

  case 19: // data: DATA_TYPE OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET
#line 185 "data_string.y"
                                                                 { yylhs.value.as < ceammc::Atom > () = createFromList(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ceammc::AtomList > ()); }
#line 895 "data_string.parser.cpp"
    break;

  case 20: // expr: atom_list
#line 189 "data_string.y"
                { result.append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 901 "data_string.parser.cpp"
    break;


#line 905 "data_string.parser.cpp"

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
  DataStringParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  DataStringParser::yytnamerr_ (const char *yystr)
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
  DataStringParser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // DataStringParser::context.
  DataStringParser::context::context (const DataStringParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  DataStringParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
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
  DataStringParser::yy_syntax_error_arguments_ (const context& yyctx,
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
  DataStringParser::yysyntax_error_ (const context& yyctx) const
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


  const signed char DataStringParser::yypact_ninf_ = -11;

  const signed char DataStringParser::yytable_ninf_ = -1;

  const signed char
  DataStringParser::yypact_[] =
  {
     -11,    58,     4,   -11,   -11,   -11,   -11,   -11,   -11,    -2,
     -10,     2,   -11,   -11,   -11,   -11,   -11,    -5,    16,   -11,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,    -4,    30,    44,
      58,   -11,   -11,   -11
  };

  const signed char
  DataStringParser::yydefact_[] =
  {
       9,    20,     0,     2,     3,    13,     9,     4,     5,     0,
       0,     0,     6,    10,    11,     8,     1,     0,     0,    13,
       9,     7,     9,    16,     9,    14,    18,     0,     0,     0,
      12,    17,    19,    15
  };

  const signed char
  DataStringParser::yypgoto_[] =
  {
     -11,   -11,    -6,   -11,    -9,   -11,   -11,   -11
  };

  const signed char
  DataStringParser::yydefgoto_[] =
  {
      -1,    13,     1,    25,    17,    14,    15,     2
  };

  const signed char
  DataStringParser::yytable_[] =
  {
      18,    23,    31,    19,    16,    20,    21,    24,    24,    22,
      27,     0,     0,     0,    28,     0,    29,     0,    30,     3,
       4,     5,     0,     6,    26,     7,     0,     8,     0,     9,
      10,    11,    12,     3,     4,     5,     0,     6,    32,     7,
       0,     8,     0,     9,    10,    11,    12,     3,     4,     5,
       0,     6,    33,     7,     0,     8,     0,     9,    10,    11,
      12,     3,     4,     5,     0,     6,     0,     7,     0,     8,
       0,     9,    10,    11,    12
  };

  const signed char
  DataStringParser::yycheck_[] =
  {
       6,     6,     6,     5,     0,     7,    16,    12,    12,     7,
      19,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,     3,
       4,     5,    -1,     7,     8,     9,    -1,    11,    -1,    13,
      14,    15,    16,     3,     4,     5,    -1,     7,     8,     9,
      -1,    11,    -1,    13,    14,    15,    16,     3,     4,     5,
      -1,     7,     8,     9,    -1,    11,    -1,    13,    14,    15,
      16,     3,     4,     5,    -1,     7,    -1,     9,    -1,    11,
      -1,    13,    14,    15,    16
  };

  const signed char
  DataStringParser::yystos_[] =
  {
       0,    19,    24,     3,     4,     5,     7,     9,    11,    13,
      14,    15,    16,    18,    22,    23,     0,    21,    19,     5,
       7,    16,     7,     6,    12,    20,     8,    21,    19,    19,
      19,     6,     8,     8
  };

  const signed char
  DataStringParser::yyr1_[] =
  {
       0,    17,    18,    18,    18,    18,    18,    18,    18,    19,
      19,    19,    20,    21,    21,    22,    23,    23,    23,    23,
      24
  };

  const signed char
  DataStringParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     2,     1,     0,
       2,     2,     2,     0,     2,     4,     3,     4,     3,     4,
       1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const DataStringParser::yytname_[] =
  {
  "\"end of string\"", "error", "\"invalid token\"", "NULL", "COMMA",
  "OPEN_DICT_BRACKET", "CLOSE_DICT_BRACKET", "OPEN_LIST_BRACKET",
  "CLOSE_LIST_BRACKET", "FLOAT", "LEXER_ERROR", "SYMBOL", "KEY",
  "DATA_TYPE", "DATA_TYPE_STRING", "FUNC_CALL", "STRING", "$accept",
  "atom", "atom_list", "pair", "pair_list", "function_call", "data",
  "expr", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  DataStringParser::yyrline_[] =
  {
       0,   153,   153,   154,   155,   156,   157,   158,   159,   163,
     164,   165,   169,   173,   174,   178,   182,   183,   184,   185,
     189
  };

  void
  DataStringParser::yy_stack_print_ () const
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
  DataStringParser::yy_reduce_print_ (int yyrule) const
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


#line 8 "data_string.y"
} } // ceammc::ds
#line 1395 "data_string.parser.cpp"

#line 192 "data_string.y"


void ceammc::ds::DataStringParser::error(const location& loc, const std::string& err_message)
{
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << lexer.matcher().line() << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;

}

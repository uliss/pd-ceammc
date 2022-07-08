// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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





#include "data_string_parser2.h"


// Unqualified %code blocks.
#line 43 "data_string2.y"

    # include <memory>
    # include <string>
    # include <utility>

    # include "lexer_data_string.h"

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

    static inline ceammc::AtomList callFunction(const char* name, const ceammc::AtomList& args) {
        return ceammc::BuiltinFunctionMap::instance().call(gensym(name), args);
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

#line 134 "data_string_parser2.cpp"


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
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 14 "data_string2.y"
namespace ceammc { namespace parser {
#line 227 "data_string_parser2.cpp"

  /// Build a parser object.
  DataStringParser2::DataStringParser2 (ceammc::parser::DataStringLexer& lexer_yyarg, ceammc::AtomList& result_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      result (result_yyarg)
  {}

  DataStringParser2::~DataStringParser2 ()
  {}

  DataStringParser2::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  DataStringParser2::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  DataStringParser2::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  DataStringParser2::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  DataStringParser2::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  DataStringParser2::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  DataStringParser2::symbol_kind_type
  DataStringParser2::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  DataStringParser2::stack_symbol_type::stack_symbol_type ()
  {}

  DataStringParser2::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.YY_MOVE_OR_COPY< ceammc::Atom > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_atom_list_nz: // atom_list_nz
      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_property: // property
      case symbol_kind::S_property_list: // property_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_list_chunk: // list_chunk
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
      case symbol_kind::S_DICT_KEY: // DICT_KEY
      case symbol_kind::S_PROPERTY: // PROPERTY
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_DATA_CALL_LIST: // DATA_CALL_LIST
      case symbol_kind::S_DATA_CALL_DICT: // DATA_CALL_DICT
        value.YY_MOVE_OR_COPY< const char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

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

  DataStringParser2::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.move< ceammc::Atom > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_atom_list_nz: // atom_list_nz
      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_property: // property
      case symbol_kind::S_property_list: // property_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_list_chunk: // list_chunk
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
      case symbol_kind::S_DICT_KEY: // DICT_KEY
      case symbol_kind::S_PROPERTY: // PROPERTY
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_DATA_CALL_LIST: // DATA_CALL_LIST
      case symbol_kind::S_DATA_CALL_DICT: // DATA_CALL_DICT
        value.move< const char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (YY_MOVE (that.value));
        break;

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
  DataStringParser2::stack_symbol_type&
  DataStringParser2::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.copy< ceammc::Atom > (that.value);
        break;

      case symbol_kind::S_atom_list_nz: // atom_list_nz
      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_property: // property
      case symbol_kind::S_property_list: // property_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_list_chunk: // list_chunk
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
      case symbol_kind::S_DICT_KEY: // DICT_KEY
      case symbol_kind::S_PROPERTY: // PROPERTY
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_DATA_CALL_LIST: // DATA_CALL_LIST
      case symbol_kind::S_DATA_CALL_DICT: // DATA_CALL_DICT
        value.copy< const char* > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  DataStringParser2::stack_symbol_type&
  DataStringParser2::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_atom: // atom
      case symbol_kind::S_data: // data
        value.move< ceammc::Atom > (that.value);
        break;

      case symbol_kind::S_atom_list_nz: // atom_list_nz
      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_property: // property
      case symbol_kind::S_property_list: // property_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_list_chunk: // list_chunk
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
      case symbol_kind::S_DICT_KEY: // DICT_KEY
      case symbol_kind::S_PROPERTY: // PROPERTY
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_DATA_CALL_LIST: // DATA_CALL_LIST
      case symbol_kind::S_DATA_CALL_DICT: // DATA_CALL_DICT
        value.move< const char* > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< double > (that.value);
        break;

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
  DataStringParser2::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  DataStringParser2::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  DataStringParser2::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  DataStringParser2::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  DataStringParser2::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  DataStringParser2::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  DataStringParser2::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  DataStringParser2::debug_level_type
  DataStringParser2::debug_level () const
  {
    return yydebug_;
  }

  void
  DataStringParser2::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  DataStringParser2::state_type
  DataStringParser2::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  DataStringParser2::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  DataStringParser2::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  DataStringParser2::operator() ()
  {
    return parse ();
  }

  int
  DataStringParser2::parse ()
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

      case symbol_kind::S_atom_list_nz: // atom_list_nz
      case symbol_kind::S_atom_list: // atom_list
      case symbol_kind::S_property: // property
      case symbol_kind::S_property_list: // property_list
      case symbol_kind::S_function_call: // function_call
      case symbol_kind::S_list_chunk: // list_chunk
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
      case symbol_kind::S_DICT_KEY: // DICT_KEY
      case symbol_kind::S_PROPERTY: // PROPERTY
      case symbol_kind::S_FUNC_CALL: // FUNC_CALL
      case symbol_kind::S_DATA_CALL_LIST: // DATA_CALL_LIST
      case symbol_kind::S_DATA_CALL_DICT: // DATA_CALL_DICT
        yylhs.value.emplace< const char* > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        yylhs.value.emplace< double > ();
        break;

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
#line 162 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(); }
#line 822 "data_string_parser2.cpp"
    break;

  case 3: // atom: COMMA
#line 163 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(gensym(",")); }
#line 828 "data_string_parser2.cpp"
    break;

  case 4: // atom: FLOAT
#line 164 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(yystack_[0].value.as < double > ()); }
#line 834 "data_string_parser2.cpp"
    break;

  case 5: // atom: SYMBOL
#line 165 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = ceammc::Atom(gensym(yystack_[0].value.as < const char* > ())); }
#line 840 "data_string_parser2.cpp"
    break;

  case 6: // atom: STRING
#line 166 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = createSimpleString(yystack_[0].value.as < std::string > ()); }
#line 846 "data_string_parser2.cpp"
    break;

  case 7: // atom: DATA_TYPE_STRING STRING
#line 167 "data_string2.y"
                              { yylhs.value.as < ceammc::Atom > () = createDataString(yystack_[0].value.as < std::string > ()); }
#line 852 "data_string_parser2.cpp"
    break;

  case 8: // atom: data
#line 168 "data_string2.y"
      { yylhs.value.as < ceammc::Atom > () = yystack_[0].value.as < ceammc::Atom > (); }
#line 858 "data_string_parser2.cpp"
    break;

  case 9: // atom_list_nz: atom
#line 172 "data_string2.y"
                                      { yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::Atom > ()); }
#line 864 "data_string_parser2.cpp"
    break;

  case 10: // atom_list_nz: function_call
#line 173 "data_string2.y"
      { yylhs.value.as < ceammc::AtomList > () = yystack_[0].value.as < ceammc::AtomList > (); }
#line 870 "data_string_parser2.cpp"
    break;

  case 11: // atom_list_nz: atom_list_nz SPACE atom_list_nz
#line 174 "data_string2.y"
                                      { yylhs.value.as < ceammc::AtomList > ().append(yystack_[2].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 876 "data_string_parser2.cpp"
    break;

  case 12: // atom_list: %empty
#line 178 "data_string2.y"
                   { }
#line 882 "data_string_parser2.cpp"
    break;

  case 13: // atom_list: atom_list_nz
#line 179 "data_string2.y"
      { yylhs.value.as < ceammc::AtomList > () = yystack_[0].value.as < ceammc::AtomList > (); }
#line 888 "data_string_parser2.cpp"
    break;

  case 14: // property: PROPERTY
#line 183 "data_string2.y"
                                       { yylhs.value.as < ceammc::AtomList > ().append(gensym(yystack_[0].value.as < const char* > ())); }
#line 894 "data_string_parser2.cpp"
    break;

  case 15: // property: property SPACE atom_list_nz
#line 184 "data_string2.y"
                                       { yylhs.value.as < ceammc::AtomList > ().append(yystack_[2].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 900 "data_string_parser2.cpp"
    break;

  case 16: // property_list: property
#line 188 "data_string2.y"
                                       { yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 906 "data_string_parser2.cpp"
    break;

  case 17: // property_list: property SPACE property
#line 189 "data_string2.y"
                                       { yylhs.value.as < ceammc::AtomList > ().append(yystack_[2].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 912 "data_string_parser2.cpp"
    break;

  case 18: // pair: DICT_KEY atom_list
#line 193 "data_string2.y"
                           { yylhs.value.as < ceammc::DictEntry > () = { yystack_[1].value.as < const char* > (), yystack_[0].value.as < ceammc::AtomList > () }; }
#line 918 "data_string_parser2.cpp"
    break;

  case 19: // pair_list: %empty
#line 197 "data_string2.y"
                     { yylhs.value.as < ceammc::Dict > () = Dict(); }
#line 924 "data_string_parser2.cpp"
    break;

  case 20: // pair_list: pair_list pair
#line 198 "data_string2.y"
                     { yylhs.value.as < ceammc::Dict > ().insert(yylhs.value.as < ceammc::Dict > ().end(), yystack_[1].value.as < ceammc::Dict > ().begin(), yystack_[1].value.as < ceammc::Dict > ().end()); yylhs.value.as < ceammc::Dict > ().push_back(yystack_[0].value.as < ceammc::DictEntry > ()); }
#line 930 "data_string_parser2.cpp"
    break;

  case 21: // function_call: FUNC_CALL atom_list LIST_CLOSE
#line 202 "data_string2.y"
                                           { yylhs.value.as < ceammc::AtomList > () = callFunction(yystack_[2].value.as < const char* > (), yystack_[1].value.as < ceammc::AtomList > ()); }
#line 936 "data_string_parser2.cpp"
    break;

  case 22: // data: DICT_OPEN pair_list DICT_CLOSE
#line 206 "data_string2.y"
                                                     { yylhs.value.as < ceammc::Atom > () = createDataDict(yystack_[1].value.as < ceammc::Dict > ()); }
#line 942 "data_string_parser2.cpp"
    break;

  case 23: // data: DATA_CALL_DICT pair_list DICT_CLOSE
#line 207 "data_string2.y"
                                                     { yylhs.value.as < ceammc::Atom > () = createFromDict(yystack_[2].value.as < const char* > (), yystack_[1].value.as < ceammc::Dict > ()); }
#line 948 "data_string_parser2.cpp"
    break;

  case 24: // data: LIST_OPEN atom_list LIST_CLOSE
#line 208 "data_string2.y"
                                                     { yylhs.value.as < ceammc::Atom > () = createDataList(yystack_[1].value.as < ceammc::AtomList > ()); }
#line 954 "data_string_parser2.cpp"
    break;

  case 25: // data: DATA_CALL_LIST atom_list LIST_CLOSE
#line 209 "data_string2.y"
                                                     { yylhs.value.as < ceammc::Atom > () = createFromList(yystack_[2].value.as < const char* > (), yystack_[1].value.as < ceammc::AtomList > ()); }
#line 960 "data_string_parser2.cpp"
    break;

  case 26: // list_chunk: atom_list
#line 213 "data_string2.y"
                    { yylhs.value.as < ceammc::AtomList > () = yystack_[0].value.as < ceammc::AtomList > (); }
#line 966 "data_string_parser2.cpp"
    break;

  case 27: // list_chunk: property_list
#line 214 "data_string2.y"
                    { yylhs.value.as < ceammc::AtomList > () = yystack_[0].value.as < ceammc::AtomList > (); }
#line 972 "data_string_parser2.cpp"
    break;

  case 28: // list_chunk: atom_list_nz SPACE property_list
#line 215 "data_string2.y"
                                       { yylhs.value.as < ceammc::AtomList > ().append(yystack_[2].value.as < ceammc::AtomList > ()); yylhs.value.as < ceammc::AtomList > ().append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 978 "data_string_parser2.cpp"
    break;

  case 29: // expr: list_chunk
#line 218 "data_string2.y"
                 { result.append(yystack_[0].value.as < ceammc::AtomList > ()); }
#line 984 "data_string_parser2.cpp"
    break;


#line 988 "data_string_parser2.cpp"

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
  DataStringParser2::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  DataStringParser2::yytnamerr_ (const char *yystr)
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
  DataStringParser2::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // DataStringParser2::context.
  DataStringParser2::context::context (const DataStringParser2& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  DataStringParser2::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
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
  DataStringParser2::yy_syntax_error_arguments_ (const context& yyctx,
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
  DataStringParser2::yysyntax_error_ (const context& yyctx) const
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


  const signed char DataStringParser2::yypact_ninf_ = -17;

  const signed char DataStringParser2::yytable_ninf_ = -1;

  const signed char
  DataStringParser2::yypact_[] =
  {
       5,   -17,   -17,   -17,    33,   -17,   -17,   -17,    33,    33,
     -17,   -17,   -16,   -17,    -1,   -17,     1,   -17,   -17,   -17,
     -17,    12,    -3,    10,     8,    15,    16,    20,   -17,     5,
       5,   -17,   -17,    33,   -17,    33,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,    22,   -17,    33
  };

  const signed char
  DataStringParser2::yydefact_[] =
  {
      12,     2,     3,    19,    12,     4,     5,    14,    12,    12,
      19,     6,     0,     9,    13,    26,    16,    27,    10,     8,
      29,     0,     0,    13,     0,     0,     0,     0,     7,     0,
       0,     1,    22,    12,    20,     0,    24,    21,    25,    23,
      11,    28,    15,    17,    18,     0
  };

  const signed char
  DataStringParser2::yypgoto_[] =
  {
     -17,   -17,     0,    -2,     3,    11,   -17,    18,   -17,   -17,
     -17,   -17
  };

  const signed char
  DataStringParser2::yydefgoto_[] =
  {
       0,    13,    23,    15,    16,    17,    34,    22,    18,    19,
      20,    21
  };

  const signed char
  DataStringParser2::yytable_[] =
  {
      14,    28,    24,    29,    32,    30,    25,    26,     1,    33,
       2,     3,    31,     4,    35,     5,     6,    36,     7,     8,
       9,    10,    11,    12,    37,    38,    45,    39,    27,    40,
      42,    44,    33,    43,     0,    40,     1,     0,     2,     3,
      41,     4,     0,     5,     6,    42,     0,     8,     9,    10,
      11,    12
  };

  const signed char
  DataStringParser2::yycheck_[] =
  {
       0,    17,     4,     4,     7,     4,     8,     9,     3,    12,
       5,     6,     0,     8,     4,    10,    11,     9,    13,    14,
      15,    16,    17,    18,     9,     9,     4,     7,    10,    29,
      30,    33,    12,    30,    -1,    35,     3,    -1,     5,     6,
      29,     8,    -1,    10,    11,    45,    -1,    14,    15,    16,
      17,    18
  };

  const signed char
  DataStringParser2::yystos_[] =
  {
       0,     3,     5,     6,     8,    10,    11,    13,    14,    15,
      16,    17,    18,    21,    22,    23,    24,    25,    28,    29,
      30,    31,    27,    22,    23,    23,    23,    27,    17,     4,
       4,     0,     7,    12,    26,     4,     9,     9,     9,     7,
      22,    25,    22,    24,    23,     4
  };

  const signed char
  DataStringParser2::yyr1_[] =
  {
       0,    20,    21,    21,    21,    21,    21,    21,    21,    22,
      22,    22,    23,    23,    24,    24,    25,    25,    26,    27,
      27,    28,    29,    29,    29,    29,    30,    30,    30,    31
  };

  const signed char
  DataStringParser2::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     3,     0,     1,     1,     3,     1,     3,     2,     0,
       2,     3,     3,     3,     3,     3,     1,     1,     3,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const DataStringParser2::yytname_[] =
  {
  "\"EOF\"", "error", "\"invalid token\"", "NULL", "SPACE", "COMMA",
  "DICT_OPEN", "DICT_CLOSE", "LIST_OPEN", "LIST_CLOSE", "FLOAT", "SYMBOL",
  "DICT_KEY", "PROPERTY", "FUNC_CALL", "DATA_CALL_LIST", "DATA_CALL_DICT",
  "STRING", "DATA_TYPE_STRING", "LEXER_ERROR", "$accept", "atom",
  "atom_list_nz", "atom_list", "property", "property_list", "pair",
  "pair_list", "function_call", "data", "list_chunk", "expr", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  DataStringParser2::yyrline_[] =
  {
       0,   162,   162,   163,   164,   165,   166,   167,   168,   172,
     173,   174,   178,   179,   183,   184,   188,   189,   193,   197,
     198,   202,   206,   207,   208,   209,   213,   214,   215,   218
  };

  void
  DataStringParser2::yy_stack_print_ () const
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
  DataStringParser2::yy_reduce_print_ (int yyrule) const
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


#line 14 "data_string2.y"
} } // ceammc::parser
#line 1482 "data_string_parser2.cpp"

#line 221 "data_string2.y"


namespace ceammc {
namespace parser {
    void DataStringParser2::error(const location& loc, const std::string& err_message)
    {
        lexer.out() << err_message << std::endl;
        lexer.out() << lexer.indent()
                    << lexer.getSource() << std::endl;

//        lexer.out() << loc.begin.column << "\n";
        lexer.out() << lexer.indent(loc.begin.column-1)
                    << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;

    }
}
}



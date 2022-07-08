%skeleton "lalr1.cc"
%require "3.5"
%language "c++"
%output "data_string_parser2.cpp"
%header "data_string_parser2.h"
%debug
//%verbose

//%define api.pure full
//%define api.push-pull push

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::parser}
%define api.parser.class {DataStringParser2}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose
%locations
%define api.location.file "data_string_location2.h"

%code requires{
    # include <memory>
    # include <vector>

    # include "ceammc_atomlist.h"
    # include "ceammc_datastorage.h"

    namespace ceammc {
    namespace parser {
        class DataStringLexer;
    }
    }

# ifndef YY_NULLPTR
#   define YY_NULLPTR nullptr
# endif
}

%parse-param { ceammc::parser::DataStringLexer& lexer }
%parse-param { ceammc::AtomList& result }

%code {
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
}

%token                        NULL
%token                        SPACE
%token                        COMMA
%token                        DICT_OPEN
%token                        DICT_CLOSE
%token                        LIST_OPEN
%token                        LIST_CLOSE
%token <double>               FLOAT
%token <const char*>          SYMBOL
%token <const char*>          DICT_KEY
%token <const char*>          PROPERTY
%token <const char*>          FUNC_CALL
%token <const char*>          DATA_CALL_LIST
%token <const char*>          DATA_CALL_DICT
%token <std::string>          STRING
%token                        DATA_TYPE_STRING
%token                        LEXER_ERROR
%token                        END 0 "EOF"

%nterm <ceammc::Atom>         atom data
%nterm <ceammc::AtomList>     atom_list function_call expr property list_chunk property_list atom_list_nz
%nterm <ceammc::DictEntry>    pair
%nterm <ceammc::Dict>         pair_list

%left  SPACE

%start expr

%%

atom
    : NULL                    { $$ = ceammc::Atom(); }
    | COMMA                   { $$ = ceammc::Atom(gensym(",")); }
    | FLOAT                   { $$ = ceammc::Atom($1); }
    | SYMBOL                  { $$ = ceammc::Atom(gensym($1)); }
    | STRING                  { $$ = createSimpleString($1); }
    | DATA_TYPE_STRING STRING { $$ = createDataString($2); }
    | data
    ;

atom_list_nz
    : atom                            { $$.append($1); }
    | function_call
    | atom_list_nz SPACE atom_list_nz { $$.append($1); $$.append($3); }
    ;

atom_list
    : %empty       { }
    | atom_list_nz
    ;

property
    : PROPERTY                         { $$.append(gensym($1)); }
    | property SPACE atom_list_nz      { $$.append($1); $$.append($3); }
    ;

property_list
    : property                         { $$.append($1); };
    | property SPACE property          { $$.append($1); $$.append($3); }
    ;

pair
    : DICT_KEY atom_list   { $$ = { $1, $2 }; }
    ;

pair_list
    : %empty         { $$ = Dict(); }
    | pair_list pair { $$.insert($$.end(), $1.begin(), $1.end()); $$.push_back($2); }
    ;

function_call
    : FUNC_CALL atom_list LIST_CLOSE       { $$ = callFunction($1, $2); }


data
    : DICT_OPEN pair_list DICT_CLOSE                 { $$ = createDataDict($2); }
    | DATA_CALL_DICT pair_list DICT_CLOSE            { $$ = createFromDict($1, $2); }
    | LIST_OPEN atom_list LIST_CLOSE                 { $$ = createDataList($2); }
    | DATA_CALL_LIST atom_list LIST_CLOSE            { $$ = createFromList($1, $2); }
    ;

list_chunk
    : atom_list     { $$ = $1; }
    | property_list { $$ = $1; }
    | atom_list_nz SPACE property_list { $$.append($1); $$.append($3); }
    ;

expr: list_chunk { result.append($1); }
    ;

%%

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



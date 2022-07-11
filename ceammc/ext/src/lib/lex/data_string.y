%skeleton "lalr1.cc"
%require "3.5"
%output "data_string.parser.cpp"
%debug

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::ds}
%define api.parser.class {DataStringParser}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose
%locations
%define api.location.file "data_string.location.hpp"

%code requires{
    # include <memory>
    # include <vector>
    # include <utility>

    # include "ceammc_atomlist.h"
    # include "ceammc_datastorage.h"

    namespace ceammc {
    namespace ds {
        class DataStringLexer;
    }
    }

    using DictPair = std::pair<t_symbol*, ceammc::AtomList>;
    using PairList = std::vector<DictPair>;

# ifndef YY_NULLPTR
#   define YY_NULLPTR nullptr
# endif
}

%parse-param { ceammc::ds::DataStringLexer& lexer }
%parse-param { ceammc::AtomList& result }

%code {
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

    using namespace ceammc;

    static ceammc::Atom createDataDict(const PairList& v) {

        DictAtom dict;

        for (auto& kv: v)
            dict->insert(kv.first, kv.second.view());

        return dict;
    }

    // create t_symbol* Atom with unescaped string
    static Atom createSimpleString(const std::string& str) {
        return gensym(string::pd_string_unescape(str).c_str());
    }

    // create Atom with DataTypeString
    static inline Atom createDataString(const std::string& str) {
        return new DataTypeString(string::pd_string_unescape(str).c_str());
    }

    static inline Atom createDataList(const AtomList& l) {
        return new DataTypeMList(l);
    }

    static inline ceammc::AtomList callFunction(const std::string& name, const AtomList& args) {
        return BuiltinFunctionMap::instance().call(gensym(name.c_str()), args);
    }

    static inline Atom createFromList(const char* name, const AtomList& args) {
        auto fn = DataStorage::instance().fromListFunction(name);
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}' not found", name);
            return Atom();
        }

        return fn(args);
    }

    static inline Atom createFromDict(const char* name, const PairList& plist) {
        auto fn = DataStorage::instance().fromDictFunction(name);
        if(fn == nullptr) {
            LIB_ERR << fmt::format("can't create type '{}' from dict", name);
            return Atom();
        }

        DictAtom dict;
        for (auto& kv: plist)
            dict->insert(kv.first, kv.second.view());

        return fn(dict);
    }

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()
}

%token                        NULL
%token                        COMMA
%token                        OPEN_DICT_BRACKET
%token                        CLOSE_DICT_BRACKET
%token                        OPEN_LIST_BRACKET
%token                        CLOSE_LIST_BRACKET
%token <double>               FLOAT
%token                        LEXER_ERROR
%token <const char*>          SYMBOL
%token <std::string>          KEY
%token <std::string>          DATA_TYPE
%token                        DATA_TYPE_STRING
%token <std::string>          FUNC_CALL
%token <std::string>          STRING
%token                        END 0 "end of string"

%nterm <ceammc::Atom>         atom data
%nterm <ceammc::AtomList>     atom_list function_call expr
%nterm <DictPair>             pair
%nterm <PairList>             pair_list


%start expr

%%

atom
    : NULL                    { $$ = Atom(); }
    | COMMA                   { $$ = Atom(gensym(",")); }
    | FLOAT                   { $$ = Atom($1); }
    | SYMBOL                  { $$ = Atom(gensym($1)); }
    | STRING                  { $$ = createSimpleString($1); }
    | DATA_TYPE_STRING STRING { $$ = createDataString($2); }
    | data
    ;

atom_list
    : %empty                  { $$ = AtomList(); }
    | atom_list atom          { $$.append($1); $$.append($2); }
    | atom_list function_call { $$.append($1); $$.append($2); }
    ;

pair
    : KEY atom_list   { $$ = { gensym($1.c_str()), $2 }; }
    ;

pair_list
    : %empty         { $$ = PairList(); }
    | pair_list pair { $$.insert($$.end(), $1.begin(), $1.end()); $$.push_back($2); }
    ;

function_call
    : FUNC_CALL OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET   { $$ = callFunction($1, $3); }
    ;

data
    : OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET             { $$ = createDataDict($2); }
    | DATA_TYPE OPEN_DICT_BRACKET pair_list CLOSE_DICT_BRACKET   { $$ = createFromDict($1.c_str(), $3); }
    | OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET             { $$ = createDataList($2); }
    | DATA_TYPE OPEN_LIST_BRACKET atom_list CLOSE_LIST_BRACKET   { $$ = createFromList($1.c_str(), $3); }
    ;

expr
    : atom_list { result.append($1); }
    ;

%%

void ceammc::ds::DataStringParser::error(const location& loc, const std::string& err_message)
{
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << lexer.matcher().line() << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;

}

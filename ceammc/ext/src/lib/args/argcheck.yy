%skeleton "lalr1.cc"
%require "3.5"
%output "argcheck.parser.cpp"
%debug

%defines
%define api.namespace {ceammc}
%define api.parser.class {ArgCheckParser}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose

%code requires{
    # include <memory>
    # include <vector>

    namespace ceammc {
        class ArgCheckLexer;
        class ArgCheckerNode;
        class ArgGroupOr;

        class ArgIsAtom;
        class ArgIsBool;
        class ArgIsData;
        class ArgIsFloat;
        class ArgIsInt;
        class ArgIsSymbol;
    }

    using ArgCheckPtr = std::shared_ptr<ceammc::ArgCheckerNode>;
    using ArgCheckPtrList = std::vector<ArgCheckPtr>;

# ifndef YY_NULLPTR
#   define YY_NULLPTR nullptr
# endif
}

%parse-param { ceammc::ArgCheckLexer& lexer  }
%parse-param { ceammc::ArgCheckerNode& n  }

%code {
    # include <memory>
    # include <string>
    # include <utility>

    // header file generated with reflex
    # include "argcheck.lexer.h"

    # include "arg_is_atom.h"
    # include "arg_is_bool.h"
    # include "arg_is_data.h"
    # include "arg_is_float.h"
    # include "arg_is_int.h"
    # include "arg_is_symbol.h"
    # include "arg_group_or.h"
    # include "../ceammc_args.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static void set_repeats(const ceammc::ArgCheckerNodePtr& n, const std::vector<int>& v) {
        switch(v.size()) {
        case 1:
            n->setInfinitePattern();
            n->setMinRepeats(v[0]);
        break;
        case 2:
            n->setMinRepeats(v[0]);
            n->setMaxRepeats(v[1]);
        break;
        default:
        break;
        }
    }
}

%token                   TBOOL
%token                   TFLOAT
%token                   TATOM
%token                   TDATA
%token                   TINT
%token                   TSYMBOL
%token                   REPEAT_END
%token                   REPEAT_RANGE
%token                   REPEAT_START
%token                   EQ
%token                   GE
%token                   GT
%token                   LE
%token                   LT
%token                   NE
%token                   ASTERISK
%token                   GROUP_END
%token                   GROUP_START
%token                   LEXER_ERR
%token                   POWER_OF_TWO
%token                   OR
%token                   PLUS
%token                   QUESTION
%token                   MODULUS
%token                   CAPS
%token                   TILDE
%token                   DOLLAR
%token                   SPACE

%token <double>          FLOAT
%token <int>             INTEGER
%token <std::string>     SYMBOL
%token <std::string>     STRING


%type <double>           NUMBER
%type <std::vector<int>> REPEAT

%type <ArgCheckPtr>      ATOM
%type <ArgCheckPtr>      ATOM_SINGLE
%type <ArgCheckPtr>      ATOM_BOOL
%type <ArgCheckPtr>      ATOM_SYMBOL
%type <ArgCheckPtr>      ATOM_FLOAT
%type <ArgCheckPtr>      ATOM_DATA
%type <ArgCheckPtr>      ATOM_INT
%type <ArgCheckPtr>      GROUP_OR
%type <ArgCheckPtrList>  ATOM_OR_SEQ
%type <ArgCheckPtrList>  ATOM_SEQ

%start REGEXP

%%

NUMBER
    : INTEGER { $$ = $1; }
    | FLOAT { $$ = $1; }
    ;

REPEAT
    : PLUS {
        $$.push_back(1);
        }
    | ASTERISK {
        $$.push_back(0);
        }
    | QUESTION {
        $$.push_back(0);
        $$.push_back(1);
        }
    | REPEAT_START INTEGER REPEAT_END {
        $$.push_back($2);
        $$.push_back($2);
        }
    | REPEAT_START INTEGER REPEAT_RANGE REPEAT_END {
        $$.push_back($2);
        }
    | REPEAT_START INTEGER REPEAT_RANGE INTEGER REPEAT_END {
        $$.push_back($2);
        $$.push_back($4);
        }
    ;

ATOM_BOOL
    : TBOOL { $$.reset(new ArgIsBool); }
    ;

ATOM_INT
    : TINT {
        $$.reset(new ArgIsInt);
        }
    | TINT EQ INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_EQUAL, $3);
        $$.reset(p);
        }
    | TINT NE INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_NOT_EQUAL, $3);
        $$.reset(p);
        }
    | TINT GT INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER, $3);
        $$.reset(p);
        }
    | TINT GE INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_GREATER_EQUAL, $3);
        $$.reset(p);
        }
    | TINT LT INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS, $3);
        $$.reset(p);
        }
    | TINT LE INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_LESS_EQUAL, $3);
        $$.reset(p);
        }
    | TINT INTEGER REPEAT_RANGE INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_RANGE, $2, $4);
        $$.reset(p);
        }
    | TINT MODULUS INTEGER EQ INTEGER {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_MOD, $3, $5);
        $$.reset(p);
        }
    | TINT POWER_OF_TWO {
        auto p = new ArgIsInt;
        p->setCheck(ArgIsInt::INT_POWER_OF_TWO, 0, 0);
        $$.reset(p);
        }
    ;

ATOM_FLOAT
    : TFLOAT {
        $$.reset(new ArgIsFloat);
        }
    | TFLOAT EQ NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_EQUAL, $3);
        $$.reset(p);
        }
    | TFLOAT NE NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_NOT_EQUAL, $3);
        $$.reset(p);
        }
    | TFLOAT GE NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER_EQUAL, $3);
        $$.reset(p);
        }
    | TFLOAT GT NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_GREATER, $3);
        $$.reset(p);
        }
    | TFLOAT LE NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS_EQUAL, $3);
        $$.reset(p);
        }
    | TFLOAT LT NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_LESS, $3);
        $$.reset(p);
        }
    | TFLOAT NUMBER REPEAT_RANGE NUMBER {
        auto p = new ArgIsFloat;
        p->setCheck(ArgIsFloat::FLOAT_RANGE, $2, $4);
        $$.reset(p);
        }
    ;

ATOM_SYMBOL
    : TSYMBOL {
        $$.reset(new ArgIsSymbol);
        }
    | TSYMBOL EQ STRING {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_EQUAL, $3.c_str());
        $$.reset(p);
        }
    | TSYMBOL NE STRING {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_NOT_EQUAL, $3.c_str());
        $$.reset(p);
        }
    | CAPS STRING {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_BEGINS_WITH, $2.c_str());
        $$.reset(p);
        }
    | TILDE STRING {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_CONTAINS, $2.c_str());
        $$.reset(p);
    }
    | STRING DOLLAR {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_ENDS_WITH, $1.c_str());
        $$.reset(p);
        }
    | STRING {
        auto p = new ArgIsSymbol;
        p->setCheck(ArgIsSymbol::SYM_MATCH, $1.c_str());
        $$.reset(p);
        }
    ;

ATOM_DATA
    : TDATA {
        $$.reset(new ArgIsData);
        }
    | TDATA EQ SYMBOL {
        auto p = new ArgIsData;
        p->setType($3.c_str());
        $$.reset(p);
        }
    ;

ATOM_SINGLE
    : TATOM       { $$.reset(new ArgIsAtom); }
    | ATOM_DATA
    | ATOM_BOOL
    | ATOM_FLOAT
    | ATOM_INT
    | ATOM_SYMBOL
    ;

ATOM
    : ATOM_SINGLE
    | ATOM_SINGLE REPEAT { $$ = $1; set_repeats($$, $2); }
    ;

// a|b
// a|b|c

ATOM_OR_SEQ
    : ATOM_SINGLE OR ATOM_SINGLE {
        $$.assign({$1, $3});
        }
    | ATOM_SINGLE OR ATOM_OR_SEQ {
        $$.push_back($1);
        $$.insert(std::end($$), std::begin($3), std::end($3));
        }
    ;


// a|b or (a|b)

GROUP_OR
    : ATOM_OR_SEQ
        {
            $$.reset(new ArgGroupOr);
            for(auto& p: $1) {
                $$->insertChild(p);
            }
        }
    | GROUP_START ATOM_OR_SEQ GROUP_END
        {
            $$.reset(new ArgGroupOr);
            for(auto& p: $2) {
                $$->insertChild(p);
            }
        }
    | GROUP_START ATOM_OR_SEQ GROUP_END REPEAT
        {
            $$.reset(new ArgGroupOr);
            for(auto& p: $2) {
                $$->insertChild(p);
            }
            set_repeats($$, $4);
        }
    ;

ATOM_SEQ
    : ATOM {
        $$.push_back($1);
        }
    | ATOM SPACE ATOM_SEQ {
        $$.push_back($1);
        $$.insert(std::end($$), std::begin($3), std::end($3));
        }
    ;

SEQ
    : ATOM_SEQ {
        for(auto& p: $1)
            n.insertChild(p);
        }
    | GROUP_OR {
        n.insertChild($1);
        }
    ;

EXPR
    : SEQ
    | SEQ EXPR
    ;

REGEXP
    : %empty { }
    | EXPR
    ;
%%

void ceammc::ArgCheckParser::error(const std::string& err_message)
{
    std::cerr << "Error: " << err_message << '\n';
    throw std::runtime_error(err_message);
}

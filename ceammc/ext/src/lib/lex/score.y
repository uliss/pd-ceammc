%require "3.5"
%skeleton "lalr1.cc"
%output "score.parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::score}
%define api.parser.class {ScoreParser}
%define api.value.type variant
%define api.token.constructor

%define parse.error verbose
%define parse.assert
%debug
//%define api.value.type {t_interval}

%parse-param { ceammc::score::ScoreLexer& lexer }

%code requires {
    # include <array>
    # include <cstdint>
    # include <iostream>
    # include <memory>
    # include <vector>

    namespace ceammc {
        namespace score {
            class ScoreLexer;
        }

        namespace scp {
            using OptVal = uint32_t;
            using OptPos = uint16_t;

            struct Option {
                enum Mode : uint8_t { NONE, SET, CHANGE_BEGIN, CHANGE_END };

            public:
                Option() {}
                Option(Mode m, OptVal v, OptPos p) : value(v), pos(p), mode(m) {}

                OptVal value = {0};
                OptPos pos = {0};
                Mode mode = {NONE};

                void dump(const char* name, int i) {
                    printf("%s[%d]: value=%d pos=%d\n", name, i, value, pos);
                }
            };

            using NOptions = std::array<Option, 10>;
            using Signature = std::pair<uint16_t, uint16_t>;

            struct Options {
                NOptions amp, cue, mark, pan, vol;
                Option tempo;
                uint16_t bar_check = {0};
                uint8_t beat_division = {1};
                bool fermata = {false};

                void dump() {
                    int i = 0;
                    for(auto& a: pan) {
//                        if(a.mode != Option::NONE)
                            a.dump("pan", i++);
                    }
                }
            };

            struct Bar {
                Signature sig;
                Options opts;
                void dump() {
                    printf("|%d/%d|\n", sig.first, sig.second);
                    opts.dump();
                }
            };
        }
    }

    using OptVal = ceammc::scp::OptVal;
    using OptPos = ceammc::scp::OptPos;
    using Signature = ceammc::scp::Signature;
    using Option = ceammc::scp::Option;
    using KeyId = std::pair<char,char>;
    using KeyOption = std::pair<KeyId, Option>;
    using Options = ceammc::scp::Options;
    using Bar = ceammc::scp::Bar;
    using BarList = std::vector<Bar>;
}

%code {
    # undef yylex
    # define yylex lexer.lex

    # include "score.lexer.h"
}

%token                        SPACE
%token                        PIPE
%token                        CHANGE
%token                        FERMATA
%token <int>                  REPEAT
%token <int>                  AT
%token <int>                  INT
%token <int>                  BAR_CHECK
%token <int>                  BEAT_DIVISION
%token <KeyId>                KEY
%token <Signature>            RATIO

%type <int>                   BAR_POS
%type <KeyOption>             OPT
%type <Options>               OPT_LIST
%type <Bar>                   BAR_CONTENT BAR
%type <BarList>               EXPR

%start EXPRLIST

%%

BAR_POS
    : %empty { $$ = 0; }
    | AT     { $$ = $1; }
    ;

OPT
    : FERMATA                 { $$ = { KeyId('F', 0), Option() }; }
    | BAR_CHECK               { $$ = { KeyId('B', 0), Option(Option::NONE, $1, 0) }; }
    | BEAT_DIVISION           { $$ = { KeyId('D', 0), Option(Option::NONE, $1, 0) }; }
    | KEY INT BAR_POS         { $$ = { $1, Option(Option::SET, $2, $3) }; }
    | KEY INT CHANGE BAR_POS  { $$ = { $1, Option(Option::CHANGE_BEGIN, $2, $4) }; }
    | KEY CHANGE INT BAR_POS  { $$ = { $1, Option(Option::CHANGE_END, $3, $4) }; }
    ;

OPT_LIST
    : %empty             {  }
    | OPT_LIST SPACE OPT {
        $$ = $1;

        const auto& key_id = $3.first;
        const auto& opt = $3.second;

        switch(key_id.first) {
            case 'a':
                $$.amp[key_id.second] = opt;
                break;
            case 'c':
                $$.cue[key_id.second] = opt;
                break;
            case 'm':
                $$.mark[key_id.second] = opt;
                break;
            case 'p':
                $$.pan[key_id.second] = opt;
                break;
            case 'v':
                $$.vol[key_id.second] = opt;
                break;
            case 't':
                $$.tempo = opt;
                break;
            case 'B': // barcheck
                $$.bar_check = opt.value;
                break;
            case 'F': // fermata
                $$.fermata = true;
                break;
            case 'D': // beat division
                $$.beat_division = opt.value;
                break;
            default:
                std::cerr << "unknown option: " << key_id.first << std::endl;
                break;
        }
      }
    ;

BAR_CONTENT
    : RATIO OPT_LIST { $$.sig = $1; $$.opts = $2; }
    ;

BAR
    : PIPE BAR_CONTENT PIPE { $$ = $2; }
    ;

EXPR
    : BAR         { $$.push_back($1); }
    | REPEAT BAR  {
        $$.push_back($2);
        const Bar b{ $2.sig, {} };
        for(int i = 1; i < $1; i++) {
            $$.push_back(b);
        }
      }
    ;

EXPRLIST
    : %empty { }
    | EXPR { lexer.bars.reserve($1.size()); for(auto& b: $1) lexer.bars.push_back(b); }
    | EXPRLIST SPACE EXPR {
        lexer.bars.reserve($3.size());

        for(auto& b: $3)
            lexer.bars.push_back(b);
      }
    ;

%%

void ceammc::score::ScoreParser::error(const std::string& err_message)
{
    std::cerr << err_message << "\n";
//    lexer.setErrorMsg(err_message);
}


%skeleton "lalr1.cc"
%require "3.5"
%output "grainprops.parser.cpp"
%debug

%defines
%define api.namespace {ceammc}
%define api.parser.class {GrainPropertiesParser}
%define api.value.type variant
%define api.token.constructor
%define parse.error verbose

%code requires{
    # include <random>
    # include <limits>

    namespace ceammc {
        class GrainPropertiesLexer;
        class Grain;
    }

# ifndef YY_NULLPTR
#   define YY_NULLPTR nullptr
# endif
}

%parse-param { ceammc::GrainPropertiesLexer& lexer }

%code {
    # include <string>
    # include "grainprops.lexer.h"
    # include "grain.h"
    # include "grain_random.h"
    # include "ceammc_log.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static inline float frand(float a, float b) { return ceammc::GrainRandom::urandf(a, b); }
    static inline float frand_closed(float a, float b) { return ceammc::GrainRandom::urandf_closed(a, b); }
}

%token			         PROP_PAN
%token                   PROP_AMP
%token                   PROP_SPEED
%token                   PROP_AT
%token                   PROP_LENGTH
%token                   PROP_WHEN
%token 			         RANDOM
%token 			         TOVERFLOW
%token 			         ONDONE
%token         	         ADD
%token         	         SET
%token         	         MOTION
%token         	         EXPR
%token                   RANGE
%token                   MSEC
%token                   SEC
%token                   MODE
%token <int>             OVERFLOW_CLIP
%token <int>             OVERFLOW_WRAP
%token <int>             OVERFLOW_FOLD
%token <int>             PAN_MODE

%token <float>           FLOAT
%token <int>             INTEGER
%token <std::string>     STRING

%type <double>           TIME
%type <int>              OVERFLOW_MODE

%start GRAIN

%%

AMP
    : PROP_AMP FLOAT
    | PROP_AMP EXPR STRING
    ;

SPEED
    : PROP_SPEED FLOAT                  { lexer.grain()->setSpeed($2); }
    | PROP_SPEED RANDOM FLOAT FLOAT     { lexer.grain()->setSpeed(frand($3, $4)); }
    | PROP_SPEED EXPR STRING
    | PROP_SPEED SET FLOAT              { lexer.grain()->setSpeed($3); }
    | PROP_SPEED SET RANDOM FLOAT FLOAT { lexer.grain()->setSpeed(frand($4, $5)); }
    | PROP_SPEED SET EXPR STRING
    | PROP_SPEED ADD FLOAT              { lexer.grain()->addSpeed($3); }
    | PROP_SPEED ADD RANDOM FLOAT FLOAT { lexer.grain()->addSpeed(frand($4, $5)); }
    | PROP_SPEED ADD EXPR STRING
    | PROP_SPEED MOTION ADD FLOAT
    | PROP_SPEED MOTION ADD RANDOM FLOAT FLOAT
    | PROP_SPEED MOTION ADD EXPR STRING
    | PROP_SPEED MOTION SET FLOAT
    | PROP_SPEED MOTION SET RANDOM FLOAT FLOAT
    | PROP_SPEED MOTION SET EXPR STRING
    | PROP_SPEED ONDONE ADD FLOAT       {
                                          auto d = $4;
                                          lexer.grain()->setSpeedDone([d](Grain* g){ return g->speed() + d; });
                                        }
    | PROP_SPEED ONDONE ADD RANDOM FLOAT FLOAT
                                        {
                                          auto a = $5;
                                          auto b = $6;
                                          lexer.grain()->setSpeedDone([a,b](Grain* g){ return g->speed() + frand(a, b); });
                                        }
    | PROP_SPEED ONDONE ADD EXPR STRING
    | PROP_SPEED ONDONE SET FLOAT
                                        {
                                          auto v = $4;
                                          lexer.grain()->setSpeedDone([v](Grain*){ return v; });
                                        }
    | PROP_SPEED ONDONE SET RANDOM FLOAT FLOAT
                                        {
                                          auto a = $5;
                                          auto b = $6;
                                          lexer.grain()->setSpeedDone([a,b](Grain*){ return frand(a, b); });
                                        }
    | PROP_SPEED ONDONE SET EXPR STRING
    | PROP_SPEED RANGE FLOAT FLOAT      { lexer.grain()->setSpeedRange($3, $4); }
    ;

OVERFLOW_MODE
    : OVERFLOW_CLIP
    | OVERFLOW_WRAP
    | OVERFLOW_FOLD
    ;

PAN
    : PROP_PAN FLOAT                  { lexer.grain()->setPan($2); }
//    | PROP_PAN RANDOM                { lexer.grain()->setPan(frand_closed(-1, 1)); }
    | PROP_PAN RANDOM FLOAT FLOAT     { lexer.grain()->setPan(frand_closed($3, $4)); }
    | PROP_PAN EXPR STRING
    | PROP_PAN SET FLOAT              { lexer.grain()->setPan($3); }
    | PROP_PAN SET RANDOM FLOAT FLOAT { lexer.grain()->setPan(frand_closed($4, $5)); }
    | PROP_PAN SET EXPR STRING
    | PROP_PAN ADD FLOAT              { lexer.grain()->addPan($3); }
    | PROP_PAN ADD RANDOM FLOAT FLOAT { lexer.grain()->addPan(frand_closed($4, $5)); }
    | PROP_PAN ADD EXPR STRING
    | PROP_PAN MOTION ADD FLOAT
    | PROP_PAN MOTION ADD RANDOM FLOAT FLOAT
    | PROP_PAN MOTION ADD EXPR STRING
    | PROP_PAN MOTION SET FLOAT
    | PROP_PAN MOTION SET RANDOM FLOAT FLOAT
    | PROP_PAN MOTION SET EXPR STRING
    | PROP_PAN ONDONE ADD FLOAT              {
                                              auto v = $4;
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([v,g](){ return g->pan() + v; });
                                             }
    | PROP_PAN ONDONE ADD RANDOM FLOAT FLOAT {
                                              auto a = $5;
                                              auto b = $6;
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([a,b,g](){ return g->pan() + frand_closed(a, b); });
                                             }
    | PROP_PAN ONDONE ADD EXPR STRING
    | PROP_PAN ONDONE SET FLOAT              { auto v = $4; lexer.grain()->setPanDone([v](){ return v; }); }
    | PROP_PAN ONDONE SET RANDOM FLOAT FLOAT {
                                               auto a = $5;
                                               auto b = $6;
                                               lexer.grain()->setPanDone([a,b](){ return frand_closed(a, b); });
                                             }
    | PROP_PAN ONDONE SET EXPR STRING
    | PROP_PAN TOVERFLOW OVERFLOW_MODE       { lexer.grain()->setPanOverflow(static_cast<Grain::PanOverflow>($3)); }
    | PROP_PAN MODE PAN_MODE                 { lexer.grain()->setPanMode(static_cast<Grain::PanMode>($3)); }
    ;


AT
    : PROP_AT TIME             { lexer.grain()->array_pos_samp = $2; }
    | PROP_AT RANDOM TIME TIME { lexer.grain()->array_pos_samp = frand_closed($3, $4); }
    ;

LENGTH
    : PROP_LENGTH TIME             { lexer.grain()->length_samp = $2; }
    | PROP_LENGTH RANDOM TIME TIME { lexer.grain()->length_samp = frand_closed($3, $4); }
    ;

WHEN
    : PROP_WHEN TIME               { lexer.grain()->play_pos = $2; }
    | PROP_WHEN RANDOM TIME TIME   { lexer.grain()->play_pos = frand_closed($3, $4); }
    ;

PROP
    : PAN
    | SPEED
    | AMP
    | AT
    | LENGTH
    | WHEN
    ;

PROPS
    : PROP
    | PROPS PROP
    ;

TIME
    : FLOAT { $$ = $1; }
    | FLOAT MSEC { $$ = sys_getsr() * 0.001 * $1; }
    | FLOAT SEC  { $$ = sys_getsr() * $1; }
    ;

GRAIN
    : PROPS
    ;
%%

void ceammc::GrainPropertiesParser::error(const std::string& err_message)
{
    LIB_ERR << "grain property parse error: " << err_message << '\n';
}

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
    # include "grain.h"

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
    # include "grain_random.h"
    # include "ceammc_log.h"
    # include "muParser.h"

    # undef yylex
    # define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global yylex()

    static inline float frand(float a, float b) { return ceammc::GrainRandom::urandf(a, b); }
    static inline float frand_closed(float a, float b) { return ceammc::GrainRandom::urandf_closed(a, b); }
    static inline size_t ulrand(size_t a, size_t b) { return ceammc::GrainRandom::urandul(a, b); }
}

%token                   PROP_AMP
%token                   PROP_AT
%token                   PROP_INTERP
%token                   PROP_LENGTH
%token                   PROP_PAN
%token                   PROP_SPEED
%token                   PROP_WHEN

%token                   S_ADD
%token                   S_CLIP
%token                   S_CUBIC
%token                   S_EXPR
%token                   S_FOLD
%token                   S_LINEAR
%token                   S_MODE
%token                   S_MOTION
%token                   S_MSEC2
%token                   S_MSEC
%token                   S_NONE
%token                   S_ONDONE
%token                   S_OVERFLOW
%token                   S_RANDOM
%token                   S_RANGE
%token                   S_SEC
%token                   S_SET
%token                   S_SQRT
%token                   S_WRAP

%token <float>           FLOAT
%token <std::string>     STRING

%type <double>              TIME
%type <Grain::PanOverflow>  ENUM_OVERFLOW_MODE
%type <Grain::PanMode>      ENUM_PAN_MODE
%type <Grain::PlayInterp>   ENUM_INTERP_MODE

%start GRAIN

%%

ENUM_OVERFLOW_MODE
    : S_CLIP { $$ = Grain::PAN_OVERFLOW_CLIP; }
    | S_WRAP { $$ = Grain::PAN_OVERFLOW_WRAP; }
    | S_FOLD { $$ = Grain::PAN_OVERFLOW_FOLD; }
    ;

ENUM_PAN_MODE
    : S_NONE   { $$ = Grain::PAN_MODE_NONE; }
    | S_LINEAR { $$ = Grain::PAN_MODE_LINEAR; }
    | S_SQRT   { $$ = Grain::PAN_MODE_SQRT; }
    ;

ENUM_INTERP_MODE
    : S_NONE   { $$ = Grain::INTERP_NO; }
    | S_LINEAR { $$ = Grain::INTERP_LINEAR; }
    | S_CUBIC  { $$ = Grain::INTERP_CUBIC; }
    ;

AMP
    : PROP_AMP FLOAT                        { lexer.grain()->setAmplitude($2); }
    | PROP_AMP S_RANDOM FLOAT FLOAT         { lexer.grain()->setAmplitude(frand($3, $4)); }
    | PROP_AMP S_EXPR STRING
    | PROP_AMP S_SET FLOAT                  { lexer.grain()->setAmplitude($3); }
    | PROP_AMP S_SET S_RANDOM FLOAT FLOAT   { lexer.grain()->setAmplitude(frand($4, $5)); }
    | PROP_AMP S_SET S_EXPR STRING
    | PROP_AMP S_ONDONE S_ADD FLOAT
                                            {
                                              auto d = $4;
                                              lexer.grain()->setAmplitudeDone([d](Grain* g){ return g->amplitude() + d; });
                                            }
    | PROP_AMP S_ONDONE S_ADD S_EXPR STRING
                                            {
                                              try {
                                                mu::Parser p;
                                                p.SetExpr($5);
                                                lexer.grain()->setAmplitudeDone([p](Grain* g) mutable -> float {
                                                    float delta = 0;
                                                    try {
                                                        g->initParserVars(p);
                                                        delta = p.Eval();
                                                    } catch(mu::ParserError& err) {
                                                        LIB_ERR << "invalid expression: " << err.GetMsg();
                                                    }

                                                    return g->amplitude() + delta;
                                                });
                                              } catch(mu::ParserError& err) {
                                                LIB_ERR << "invalid expression: " << $5;
                                              }
                                            }
    | PROP_AMP S_ONDONE S_SET FLOAT
                                            {
                                              auto d = $4;
                                              lexer.grain()->setAmplitudeDone([d](Grain* g){ return d; });
                                            }
    | PROP_AMP S_ONDONE S_SET S_EXPR STRING
                                            {
                                              try {
                                                mu::Parser p;
                                                p.SetExpr($5);
                                                lexer.grain()->setAmplitudeDone([p](Grain* g) mutable -> float {
                                                    try {
                                                        g->initParserVars(p);
                                                        return p.Eval();
                                                    } catch(mu::ParserError& err) {
                                                        LIB_ERR << "invalid expression: " << err.GetMsg();
                                                        return 0.;
                                                    }
                                                });
                                              } catch(mu::ParserError& err) {
                                                LIB_ERR << "invalid expression: " << $5;
                                              }
                                            }
    | PROP_AMP S_ONDONE S_SET S_RANDOM FLOAT FLOAT
                                            {
                                              auto a = $5;
                                              auto b = $6;
                                              lexer.grain()->setAmplitudeDone([a,b](Grain* g){ return frand_closed(a, b); });
                                            }
    | PROP_AMP S_RANGE FLOAT FLOAT          { lexer.grain()->setAmplitudeRange($3, $4); }
    ;

SPEED
    : PROP_SPEED FLOAT                      { lexer.grain()->setSpeed($2); }
    | PROP_SPEED S_RANDOM FLOAT FLOAT       { lexer.grain()->setSpeed(frand($3, $4)); }
    | PROP_SPEED S_EXPR STRING              { lexer.grain()->setSpeedExpr($3); }
    | PROP_SPEED S_SET FLOAT                { lexer.grain()->setSpeed($3); }
    | PROP_SPEED S_SET S_RANDOM FLOAT FLOAT { lexer.grain()->setSpeed(frand($4, $5)); }
    | PROP_SPEED S_SET S_EXPR STRING        { lexer.grain()->setSpeedExpr($4); }
    | PROP_SPEED S_ADD FLOAT                { lexer.grain()->addSpeed($3); }
    | PROP_SPEED S_ADD S_RANDOM FLOAT FLOAT { lexer.grain()->addSpeed(frand($4, $5)); }
    | PROP_SPEED S_ADD S_EXPR STRING
    | PROP_SPEED S_MOTION S_ADD FLOAT
    | PROP_SPEED S_MOTION S_ADD S_RANDOM FLOAT FLOAT
    | PROP_SPEED S_MOTION S_ADD S_EXPR STRING
    | PROP_SPEED S_MOTION S_SET FLOAT
    | PROP_SPEED S_MOTION S_SET S_RANDOM FLOAT FLOAT
    | PROP_SPEED S_MOTION S_SET S_EXPR STRING
    | PROP_SPEED S_ONDONE S_ADD FLOAT   {
                                          auto d = $4;
                                          lexer.grain()->setSpeedDone([d](Grain* g){ return g->speed() + d; });
                                        }
    | PROP_SPEED S_ONDONE S_ADD S_RANDOM FLOAT FLOAT
                                        {
                                          auto a = $5;
                                          auto b = $6;
                                          lexer.grain()->setSpeedDone([a,b](Grain* g){ return g->speed() + frand(a, b); });
                                        }
    | PROP_SPEED S_ONDONE S_ADD S_EXPR STRING
    | PROP_SPEED S_ONDONE S_SET FLOAT
                                        {
                                          auto v = $4;
                                          lexer.grain()->setSpeedDone([v](Grain*){ return v; });
                                        }
    | PROP_SPEED S_ONDONE S_SET S_RANDOM FLOAT FLOAT
                                        {
                                          auto a = $5;
                                          auto b = $6;
                                          lexer.grain()->setSpeedDone([a,b](Grain*){ return frand(a, b); });
                                        }
    | PROP_SPEED S_ONDONE S_SET S_EXPR STRING
    | PROP_SPEED S_RANGE FLOAT FLOAT    { lexer.grain()->setSpeedRange($3, $4); }
    ;

PAN
    : PROP_PAN FLOAT                      { lexer.grain()->setPan($2); }
    | PROP_PAN S_RANDOM FLOAT FLOAT       { lexer.grain()->setPan(frand_closed($3, $4)); }
    | PROP_PAN S_EXPR STRING
    | PROP_PAN S_SET FLOAT                { lexer.grain()->setPan($3); }
    | PROP_PAN S_SET S_RANDOM FLOAT FLOAT { lexer.grain()->setPan(frand_closed($4, $5)); }
    | PROP_PAN S_SET S_EXPR STRING
    | PROP_PAN S_ADD FLOAT                { lexer.grain()->addPan($3); }
    | PROP_PAN S_ADD S_RANDOM FLOAT FLOAT { lexer.grain()->addPan(frand_closed($4, $5)); }
    | PROP_PAN S_ADD S_EXPR STRING
    | PROP_PAN S_MOTION S_ADD FLOAT
    | PROP_PAN S_MOTION S_ADD S_RANDOM FLOAT FLOAT
    | PROP_PAN S_MOTION S_ADD S_EXPR STRING
    | PROP_PAN S_MOTION S_SET FLOAT
    | PROP_PAN S_MOTION S_SET S_RANDOM FLOAT FLOAT
    | PROP_PAN S_MOTION S_SET S_EXPR STRING
    | PROP_PAN S_ONDONE S_ADD FLOAT         {
                                              auto v = $4;
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([v,g](){ return g->pan() + v; });
                                            }
    | PROP_PAN S_ONDONE S_ADD S_RANDOM FLOAT FLOAT
                                            {
                                              auto a = $5;
                                              auto b = $6;
                                              auto g = lexer.grain();
                                              lexer.grain()->setPanDone([a,b,g](){ return g->pan() + frand_closed(a, b); });
                                            }
    | PROP_PAN S_ONDONE S_ADD S_EXPR STRING
    | PROP_PAN S_ONDONE S_SET FLOAT         { auto v = $4; lexer.grain()->setPanDone([v](){ return v; }); }
    | PROP_PAN S_ONDONE S_SET S_RANDOM FLOAT FLOAT
                                            {
                                              auto a = $5;
                                              auto b = $6;
                                              lexer.grain()->setPanDone([a,b](){ return frand_closed(a, b); });
                                            }
    | PROP_PAN S_ONDONE S_SET S_EXPR STRING
    | PROP_PAN S_OVERFLOW ENUM_OVERFLOW_MODE { lexer.grain()->setPanOverflow($3); }
    | PROP_PAN S_MODE ENUM_PAN_MODE          { lexer.grain()->setPanMode($3); }
    ;


AT
    : PROP_AT TIME               { lexer.grain()->setArrayPosInSamples($2); }
    | PROP_AT S_RANDOM           {
                                    const auto asize = lexer.arraySize();
                                    if (asize < 1)
                                        LIB_ERR << "empty array";
                                    else
                                        lexer.grain()->setArrayPosInSamples(ulrand(0, asize - 1));
                                 }
    | PROP_AT S_RANDOM TIME TIME { lexer.grain()->setArrayPosInSamples(ulrand($3, $4)); }
    ;

LENGTH
    : PROP_LENGTH TIME               { lexer.grain()->setLengthInSamples($2); }
    | PROP_LENGTH S_RANDOM TIME TIME { lexer.grain()->setLengthInSamples(ulrand($3, $4)); }
    ;

WHEN
    : PROP_WHEN TIME                 { lexer.grain()->setStartInSamples($2); }
    | PROP_WHEN S_RANDOM TIME TIME   { lexer.grain()->setStartInSamples(ulrand($3, $4)); }
    ;

PROP
    : PAN
    | SPEED
    | AMP
    | AT
    | LENGTH
    | WHEN
    | PROP_INTERP ENUM_INTERP_MODE   { lexer.grain()->setPlayInterpolation($2); }
    ;

PROPS
    : PROP
    | PROPS PROP
    ;

TIME
    : FLOAT { $$ = $1; }
    | FLOAT S_MSEC { $$ = sys_getsr() * 0.001 * $1; }
    | FLOAT S_SEC  { $$ = sys_getsr() * $1; }
    ;

GRAIN
    : PROPS
    ;
%%

void ceammc::GrainPropertiesParser::error(const std::string& err_message)
{
    LIB_ERR << "grain property parse error: " << err_message << '\n';
}

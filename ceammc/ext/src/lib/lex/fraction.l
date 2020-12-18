%top{
    # include "ceammc_units.h"
    # include <cstdlib>
%}

%class{


    public:
        units::FractionValue value;
%}

%option freespace
%option namespace=ceammc::fraction
%option lexer=FractionLexer
%option header-file=fraction.lexer.h
%option outfile=fraction.lexer.cpp
%option noyywrap
%option reentrant
%option fast

space       [ \t\n]+
udec        0 | [1-9] [0-9]*
frac        \. \d+

int         [-+]? {udec}
uint        {udec}
float       {int} {frac}?
ufloat      {udec} {frac}?
percent     {float} %
fratio      {float} [*]
iratio      {uint} [/] {uint}


%%

{space}         { }
{percent}       { auto v = strtod(text(), nullptr); value = units::FractionValue(v, units::FractionValue::PERCENT); return 1; }
{fratio}        { auto v = strtod(text(), nullptr); value = units::FractionValue(v, units::FractionValue::PHASE); return 1; }
{iratio}        {
                    char* end;
                    auto num = strtol(text(), &end, 10);
                    auto den = strtol(end+1, nullptr, 10);
                    value = units::FractionValue::ratio(num, den);
                    return 1;
                }
<*> .           { return -1; }

%%



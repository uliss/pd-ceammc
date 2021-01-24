%top {
    # include <memory>
    # include <cctype>
    # include "lex/score.parser.hpp"

    using token = ceammc::score::ScoreParser::token;
%}

%class {
    public:
        BarList bars;


%}

%option freespace
%option namespace=ceammc::score
%option lexer=ScoreLexer
%option header-file=score.lexer.h
%option outfile=score.lexer.cpp
/*%option graphs-file*/
%option debug
/*%option perf-report*/
%option bison-cc-namespace=ceammc::score
%option bison-cc-parser="ScoreParser"
%option bison-cc
%option bison-complete
%option noyywrap
%option reentrant
%option noindent
%option fast

space        [ \t\n]+

udec         0 | ([1-9] [0-9]*)
frac         \. \d+

int          [-+]? {udec}
uint         {udec}
float        {int} {frac}?
ufloat       {udec} {frac}?
change       \.\.
ratio        {udec}\/{udec}
key          t(empo)?
multikey     (a(mp)? | c(ue)? | m(ark)? | p(an)? | v(ol)?)[0-9]?
keys         ({key} | {multikey})=
bar_check    bar={udec}
beat_div     div={udec}
fermata      fermata
bar_pos      \@{udec}
bar_repeat   ([1-9] [0-9]*)\*

%%

{space}                 { return ScoreParser::make_SPACE(); }
\|                      { return ScoreParser::make_PIPE(); }
{change}                { return ScoreParser::make_CHANGE(); }
{fermata}               { return ScoreParser::make_FERMATA(); }

{bar_check}             {
                            const auto n = std::strtol(text()+4, nullptr, 10);
                            return ScoreParser::make_BAR_CHECK(n);
                        }

{bar_pos}               {
                            const auto pos = std::strtol(text()+1, nullptr, 10);
                            return ScoreParser::make_AT(pos);
                        }

{bar_repeat}            {
                            const auto n = std::strtol(text(), nullptr, 10);
                            return ScoreParser::make_REPEAT(n);
                        }

{beat_div}              {
                            const auto n = std::strtol(text()+4, nullptr, 10);
                            return ScoreParser::make_BEAT_DIVISION(n);
                        }

{ratio}                 {
                            char* end;
                            const auto num = std::strtol(text(), &end, 10);
                            const auto den = std::strtol(end+1, nullptr, 10);
                            return ScoreParser::make_RATIO(std::pair<int, int>(num, den));
                        }

{udec}                  {
                            const auto num = std::strtol(text(), nullptr, 10);
                            return ScoreParser::make_INT(num);
                        }

{keys}                  {
                            const int len = strlen(text());
                            const char id = text()[0];
                            const char c = text()[len-2];
                            const char n = isdigit(c) ? (c-'0') : 0;
                            return ScoreParser::make_KEY(KeyId(id, n));
                        }
<*> .                   {   }

%%


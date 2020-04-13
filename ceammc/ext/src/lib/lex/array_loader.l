%top {
    # include <string>
    # include <memory>
    # include <cstdint>
    # include <cmath>

    # include "lex/array_loader.parser.hpp"

    using token = ceammc::ArrayLoaderParser::token;
%}

%class {
    private:
        std::size_t pat_count_ = {0};
        std::string pat_prefix_;
        std::string pat_suffix_;
        std::vector<std::string> patterns_;
        std::pair<uint, uint> pat_range_ = {0, 0};
        std::vector<uint8_t> channels_;
        bool finished_ = {false};

        size_t output_indent_ = {0};

    public:
        /** return string for output indent */
        inline std::string indent(size_t n = 0, char c = ' ') const {
            return std::string(output_indent_ + n, c);
        }

        /** set debug output indent */
        inline void setOutputIndent(size_t n) { output_indent_ = n; }
%}

%option freespace unicode
%option namespace=ceammc
%option lexer=ArrayLoaderLexer
%option header-file=array_loader.lexer.h
%option outfile=array_loader.lexer.cpp
/*%option graphs-file*/
%option debug
/*%option perf-report*/
%option bison-cc-namespace="ceammc"
%option bison-cc-parser="ArrayLoaderParser"
%option bison-cc
%option bison-complete
%option bison-locations
%option noyywrap
%option reentrant
%option noindent
%option fast

space       [ \t\n]+

udec        0 | [1-9] [0-9]*
frac        \. \d+

int         [-+]? {udec}
uint        {udec}
float       {int} {frac}?
ufloat      {udec} {frac}?

string      \"([^`\"]|`\"|``|`\.|`:|`\(|`\))*\"
symbol      [^ \t\n]+

smpte       [0-9]{2}:[0-9]{2}(:[0-9]{2})?(.[0-9]{2})?


to          [@]? to
channels    (c | ch | chan | channels)
begin       (b | begin)
end         (e | end)
gain        (g | gain)
length      (l | len | length)
norm        (n | norm | normalize)
resize      (r | resize)
verbose     (v | verbose)
resample    resample
offset      offset


%xstate ARRAY
%xstate PATTERN
%xstate RANGE
%xstate VARIANT
%xstate OPTIONS
%xstate SMPTE

%%

{space}
load/{space}            { return ArrayLoaderParser::make_ACT_LOAD(location()); }

{symbol}                { return ArrayLoaderParser::make_SYMBOL(text(), location()); }
{string}                {
                          std::string str(text() + 1);
                          str.pop_back();
                          return ArrayLoaderParser::make_QUOTED_STRING(str, location());
                        }

{to}{space}             {
                            start(ARRAY);
                            pat_count_ = 0;
                            patterns_.clear();
                            pat_prefix_.clear();
                            pat_suffix_.clear();

                            out() << "array start\n";
                        }

<ARRAY>[^\s\[\]]*       {   // before pattern
                            if(pat_count_ == 0)
                                pat_prefix_ += text();
                            else
                                pat_suffix_ += text();
                        }

<ARRAY>\[               {   // pattern found
                            if(pat_count_++ > 0) {
                                out() << "only one pattern allowed:\n";
                                out() << matcher().line() << "\n";
                                out() << std::string(location().begin.column, '_') << "^\n";
                                return ArrayLoaderParser::make_LEXER_ERROR(location());
                            } else {
                                start(PATTERN);
                                out() << "pattern start\n";
                            }
                        }

<PATTERN>{

\]                      {   // pattern end
                            start(ARRAY);
                            out() << "pattern end\n";

                            if(patterns_.empty()) {
                                for(char c = '0'; c <= '9'; c++)
                                    patterns_.push_back(std::string(1, c));
                            }

                        }

{udec}-                 { start(RANGE); pat_range_.first = std::stoi(text()); }
[0-9a-zA-Z]+/\|         { patterns_.emplace_back(text()); }
[0-9a-zA-Z]+/\]         { patterns_.emplace_back(text()); }
\|
}

<RANGE>{udec}/\]        {
                            start(PATTERN);
                            pat_range_.second = std::stoi(text());

                            if(pat_range_.first > pat_range_.second) {
                                out() << "invalid range";
                                return ArrayLoaderParser::make_LEXER_ERROR(location());
                            }

                            for(uint i = pat_range_.first; i <= pat_range_.second; i++) {
                                patterns_.push_back(std::to_string(i));
                            }
                        }
<ARRAY><<EOF>>          {
                            if(!finished_) {
                                finished_ = true;
                                return ArrayLoaderParser::make_ARRAY_LIST({}, location());
                            }
                            else
                                return ArrayLoaderParser::make_STRING_END(location());
                        }
<ARRAY>{space}          {
                            start(OPTIONS);
                            finished_ = true;
                            return ArrayLoaderParser::make_ARRAY_LIST({}, location());
                        }

<OPTIONS>@{begin}       { return ArrayLoaderParser::make_BEGIN(location()); }
<OPTIONS>@{offset}      {   out() << "@offset option is obsolete, use @begin\n";
                            return ArrayLoaderParser::make_BEGIN(location()); }
<OPTIONS>@{end}         { return ArrayLoaderParser::make_END(location()); }
<OPTIONS>@{gain}        { return ArrayLoaderParser::make_GAIN(location()); }
<OPTIONS>@{length}      { return ArrayLoaderParser::make_LENGTH(location()); }
<OPTIONS>@{norm}        { return ArrayLoaderParser::make_NORMALIZE(location()); }
<OPTIONS>@{resample}    { return ArrayLoaderParser::make_RESAMPLE(location()); }
<OPTIONS>@{resize}      { return ArrayLoaderParser::make_RESIZE(location()); }
<OPTIONS>@{verbose}     { return ArrayLoaderParser::make_VERBOSE(location()); }
<OPTIONS>@[a-z]+        {   out() << "unexpected option: " << text() << "\n";
                            return ArrayLoaderParser::make_LEXER_ERROR(location()); }

<OPTIONS>{space}

<OPTIONS>{smpte}        { start(SMPTE); matcher().less(0); }
<OPTIONS>{int}          { return ArrayLoaderParser::make_INT(std::stoi(text()), location()); }
<OPTIONS>{float}        { return ArrayLoaderParser::make_FLOAT(std::stod(text()), location()); }
<OPTIONS>(ms|msec)      { return ArrayLoaderParser::make_MSEC(location()); }
<OPTIONS>(s|sec)        { return ArrayLoaderParser::make_SEC(location()); }
<OPTIONS>(db)           { return ArrayLoaderParser::make_DB(location()); }
<OPTIONS>(samp|samples) { return ArrayLoaderParser::make_SAMPLES(location()); }
<OPTIONS>\/             { return ArrayLoaderParser::make_FRAC(location()); }

<SMPTE>[0-9][0-9]       { return ArrayLoaderParser::make_UINT(std::stoi(text(), nullptr, 10), location()); }
<SMPTE>:                { return ArrayLoaderParser::make_COLON(location()); }
<SMPTE>\.               { return ArrayLoaderParser::make_DOT(location()); }
<SMPTE>@                { start(OPTIONS); matcher().less(0); /*push back '@' and go to OPTIONS*/  }


<<EOF>>                 { return ArrayLoaderParser::make_STRING_END(location()); }
<*> .                   { return ArrayLoaderParser::make_LEXER_ERROR(location()); /* error */ }

%%

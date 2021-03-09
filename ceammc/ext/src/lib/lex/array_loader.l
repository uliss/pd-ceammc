%top {
    # include <string>

    # include "lex/array_loader.parser.hpp"

    using token = ceammc::al::ArrayLoaderParser::token;
%}

%class {
    private:
        size_t output_indent_ = {0};

    public:
        /** return string for output indent */
        inline std::string indent(size_t n = 0, char c = ' ') const {
            return std::string(output_indent_ + n, c);
        }

        /** set debug output indent */
        inline void setOutputIndent(size_t n) { output_indent_ = n; }

        static const std::vector<std::string>& options() {
            static const std::vector<std::string> opts = {
                "[@ch | @chan | @channel | @channels <value>...]",
                "[@b | @begin <value>]",
                "([@e | @end <value>] | [@l | @len | @length <value>])",
                "[@aoff | @aoffset | @array_offset <value>]",
                "[@r | @resize]",
                "[@g | @gain <value>]",
                "[@n | @norm | @normalize]",
                "[@resample <value>]"
            };

            return opts;
        }
%}

%option freespace unicode
%option namespace=ceammc::al
%option lexer=ArrayLoaderLexer
%option header-file=array_loader.lexer.h
%option outfile=array_loader.lexer.cpp
/*%option graphs-file*/
/*%option debug*/
/*%option perf-report*/
%option bison-cc-namespace=ceammc::al
%option bison-cc-parser=ArrayLoaderParser
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

smpte       [0-9]{2}:[0-9]{2}(:[0-9]{2})?(\.[0-9]{2})?

/* note: keep in sync with options() lexer method */
to          [@]? to
channels    (c | ch | chan | channel | channels)
begin       (b | begin)
end         (e | end)
gain        (g | gain)
length      (l | len | length)
norm        (n | norm | normalize)
resize      (r | resize)
resample    resample
offset      (off | offset)
aoffset     (aoff | aoffset | array_offset)

arr_quoted  \"[^ \"]*\"
arr_pattern [^@ \[\]][^ \[\]]*

%xstate ARRAY
%xstate PATTERN
%xstate OPTIONS
%xstate SMPTE
%xstate RANGE

%%

{space}
{symbol}                { return ArrayLoaderParser::make_SYMBOL(text(), location()); }

{to}{space}             { start(ARRAY); }

<ARRAY>[ ]+/[^@]        { return ArrayLoaderParser::make_ARRAY_DELIM(location()); }
<ARRAY>[ ]+/[@]         { start(OPTIONS); }
<ARRAY>{arr_quoted}     { return ArrayLoaderParser::make_SYMBOL(std::string(text()+1, size()-2), location()); }
<ARRAY>{arr_pattern}    { return ArrayLoaderParser::make_SYMBOL(text(), location()); }
<ARRAY>\[               { start(PATTERN); return ArrayLoaderParser::make_PATTERN_BEGIN(location()); }

<PATTERN>{uint}         { auto n = std::stoi(text());
                          return ArrayLoaderParser::make_UINT(n, location()); }
<PATTERN>\-             { return ArrayLoaderParser::make_RANGE_DELIM(location()); }
<PATTERN>\|             { return ArrayLoaderParser::make_VAR_DELIM(location()); }
<PATTERN>[\w]+          { return ArrayLoaderParser::make_SYMBOL(text(), location()); }
<PATTERN>\]             { start(ARRAY); return ArrayLoaderParser::make_PATTERN_END(location()); }


<OPTIONS>@{begin}       { return ArrayLoaderParser::make_BEGIN(location()); }
<OPTIONS>@{offset}      { return ArrayLoaderParser::make_BEGIN(location()); }
<OPTIONS>@{end}         { return ArrayLoaderParser::make_END(location()); }
<OPTIONS>@{aoffset}     { return ArrayLoaderParser::make_ARRAY_OFFSET(location()); }
<OPTIONS>@{channels}    { return ArrayLoaderParser::make_CHANNELS(location()); }
<OPTIONS>@{gain}        { return ArrayLoaderParser::make_GAIN(location()); }
<OPTIONS>@{length}      { return ArrayLoaderParser::make_LENGTH(location()); }
<OPTIONS>@{norm}        { return ArrayLoaderParser::make_NORMALIZE(location()); }
<OPTIONS>@{resample}    { return ArrayLoaderParser::make_RESAMPLE(location()); }
<OPTIONS>@{resize}      { return ArrayLoaderParser::make_RESIZE(location()); }
<OPTIONS>@[a-z]+        {   out() << "unexpected option: " << text() << "\n";
                            return ArrayLoaderParser::make_LEXER_ERROR(location()); }

<OPTIONS>{space}

<OPTIONS>{smpte}        { start(SMPTE); matcher().less(0); /*push back smpte value and go to SMPTE*/ }
<OPTIONS>{uint}/-{uint} { start(RANGE);
                          auto n = std::stoi(text());
                          return ArrayLoaderParser::make_RANGE_BEGIN(n, location()); }
<OPTIONS>{int}          { return ArrayLoaderParser::make_INT(std::stoi(text()), location()); }
<OPTIONS>{float}        { return ArrayLoaderParser::make_FLOAT(std::stod(text()), location()); }
<OPTIONS>(ms|msec)      { return ArrayLoaderParser::make_MSEC(location()); }
<OPTIONS>(s|sec)        { return ArrayLoaderParser::make_SEC(location()); }
<OPTIONS>(db)           { return ArrayLoaderParser::make_DB(location()); }
<OPTIONS>(samp|samples) { return ArrayLoaderParser::make_SAMPLES(location()); }
<OPTIONS>\/             { return ArrayLoaderParser::make_FRAC(location()); }
<OPTIONS>\$             { return ArrayLoaderParser::make_OFFSET_END(location()); }

<RANGE>-{uint}          { start(OPTIONS);
                          auto n = std::stoi(text()+1);
                          return ArrayLoaderParser::make_RANGE_END(n, location()); }

<SMPTE>[0-9][0-9]       { auto n = std::stol(text(), nullptr, 10);
                          return ArrayLoaderParser::make_UINT(n, location()); }
<SMPTE>:                { return ArrayLoaderParser::make_COLON(location()); }
<SMPTE>\.               { return ArrayLoaderParser::make_DOT(location()); }
<SMPTE>[ ]              { start(OPTIONS); /*go to next option*/  }


<<EOF>>                 { return ArrayLoaderParser::make_STRING_END(location()); }
<*> .                   { return ArrayLoaderParser::make_LEXER_ERROR(location()); /* error */ }

%%


%require "3.5"
%skeleton "lalr1.cc"
%output "array_loader.parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::al}
%define api.parser.class {ArrayLoaderParser}

%define parse.error verbose
%define parse.assert
%define api.value.type variant
%define api.token.constructor
%locations
%define api.location.file "array_loader.location.hpp"
%debug

%parse-param { ceammc::al::ArrayLoaderLexer& lexer }
%parse-param { ceammc::ArrayLoader& loader }

%code requires {
    # include <cstddef>
    # include <cstdint>
    # include <cstdio>
    # include <string>
    # include <vector>

    namespace ceammc {
        namespace al {
        class ArrayLoaderLexer;
        }
        class ArrayLoader;
    }

    using StringList = std::vector<std::string>;
    using ChannelList = std::vector<int>;
}

%code top {
    # undef yylex
    # define yylex lexer.lex

    # include "array_loader.lexer.h"
    # include "array_loader.h"
    # include "ceammc_convert.h"

    using OPT = ceammc::ArrayLoader::OptionType;
    using OFFSET = ceammc::ArrayLoader::OffsetType;

    static StringList generateStringRange(int from, int to) {
        StringList res;
        res.reserve(to - from + 1);
        for(int i = from; i <= to; i++)
            res.emplace_back(std::to_string(i));

        return res;
    }

    static ChannelList generateIntRange(int from, int to) {
        ChannelList res;
        res.reserve(std::abs(from - to) + 1);
        if(from < to) {
            for(int i = from; i <= to; i++)
                res.push_back(i);
        } else {
            for(int i = from; i >= to; i--)
                res.push_back(i);
        }

        return res;
    }
}

%token  <std::string>   SYMBOL
%token                  ARRAY_DELIM
%token                  PATTERN_BEGIN PATTERN_END
%token                  RANGE_DELIM VAR_DELIM
%token                  LENGTH RESIZE GAIN RESAMPLE BEGIN END NORMALIZE CHANNELS ARRAY_OFFSET
%token                  OFFSET_END
%token  <std::string>   SMPTE
%token  <double>        FLOAT
%token  <int32_t>       INT
%token  <uint32_t>      UINT
%token  <uint32_t>      RANGE_BEGIN RANGE_END
%token                  SEC MSEC SAMPLES DB
%token                  COLON ':'  "colon"
%token                  DOT   '.'  "dot"
%token                  FRAC  '/'  "fraction"
%token                  STRING_END 0 "end of string"
%token                  LEXER_ERROR

%nterm  <int32_t>       time smpte
%nterm  <double>        number
%nterm  <StringList>    array_pattern var_list
%nterm  <std::string>   var
%nterm  <ChannelList>   channel_list

%start expr

%%

smpte
    : UINT COLON UINT                     { $$ = loader.smpte2samples(0, $1, $3, 0); }
    | UINT COLON UINT DOT UINT            { $$ = loader.smpte2samples(0, $1, $3, $5); }
    | UINT COLON UINT COLON UINT          { $$ = loader.smpte2samples($1, $3, $5, 0); }
    | UINT COLON UINT COLON UINT DOT UINT { $$ = loader.smpte2samples($1, $3, $5, $7); }
    ;

number
    : FLOAT
    | INT  { $$ = $1; }
    ;

time
    : number          { $$ = $1; }
    | number SEC      { $$ = loader.sec2samples($1); }
    | number MSEC     { $$ = loader.ms2samples($1); }
    | number SAMPLES  { $$ = $1; }
    | smpte           { $$ = $1; }
    ;

channel_list
    : INT                   { $$ = {$1}; }
    | RANGE_BEGIN RANGE_END { $$ = generateIntRange($1, $2); }
    | channel_list INT      { std::copy($1.begin(), $1.end(), std::back_inserter($$)); $$.push_back($2); }
    ;

opt
    : RESIZE                      { loader.setFlagOption(OPT::OPT_RESIZE); }
    | NORMALIZE                   { loader.setFlagOption(OPT::OPT_NORMALIZE); }
    | BEGIN     time              { if(!loader.setBeginOption($2, OFFSET::OFF_BEGIN))
                                        { error(@2, "invalid @begin value"); return 1; }}
    | BEGIN     OFFSET_END time   { if(!loader.setBeginOption($3, OFFSET::OFF_END))
                                        { error(@2, "invalid @begin value"); return 1; }}
    | END       time              { if(!loader.setEndOption($2, OFFSET::OFF_BEGIN))
                                        { error(@2, "invalid @end value"); return 1; }}
    | END       OFFSET_END time   { if(!loader.setEndOption($3, OFFSET::OFF_END))
                                        { error(@2, "invalid @end value"); return 1; }}
    | LENGTH    time              { if(!loader.setLengthOption($2))
                                        { error(@2, "invalid @length value"); return 1; }}
    | GAIN      number            { if(!loader.setGain($2))
                                        { error(@2, "invalid @gain value"); return 1; }}
    | GAIN number DB              { if(!loader.setGain(ceammc::convert::dbfs2amp($2)))
                                        { error(@2, "invalid @gain value"); return 1; }}
    | RESAMPLE                    { if(!loader.setResampleRatio(loader.destSamplerate(), loader.srcSampleRate()))
                                        { error(@1, "invalid ratio"); return 1; }}
    | RESAMPLE number             { if(!loader.setResampleRatio($2 * loader.srcSampleRate(), loader.srcSampleRate()))
                                        { error(@2, "invalid ratio"); return 1; }}
    | RESAMPLE INT FRAC INT       { if(!loader.setResampleRatio($2, $4))
                                        { error(@2, "invalid ratio"); return 1; }}
    | CHANNELS channel_list       { for(auto& c: $2)
                                        loader.addChannel(c); }
    | ARRAY_OFFSET INT            { if(!loader.setArrayOffset($2, OFFSET::OFF_BEGIN))
                                        { error(@2, "non-negative value expected"); return 1; }}
    | ARRAY_OFFSET OFFSET_END     { loader.setArrayOffset(0, OFFSET::OFF_END); }
    | ARRAY_OFFSET OFFSET_END INT { loader.setArrayOffset($3, OFFSET::OFF_END); }
    ;

options
    : %empty
    | options opt
    ;

var
    : SYMBOL
    | UINT   { $$ = std::to_string($1); }
    ;

var_list
    : var VAR_DELIM var       { $$.push_back($1); $$.push_back($3); }
    | var_list VAR_DELIM var  { std::copy($1.begin(), $1.end(), std::back_inserter($$)); $$.push_back($3); }
    ;

array_pattern
    : PATTERN_BEGIN PATTERN_END                         { $$ = generateStringRange(0, 9); }
    | PATTERN_BEGIN var_list PATTERN_END                { $$ = $2; }
    | PATTERN_BEGIN UINT RANGE_DELIM UINT PATTERN_END   {

        if($2 > $4)
            error(@2, "invalid range values");
        else
            $$ = generateStringRange($2, $4);
                                                        }
    ;

array_name
    : SYMBOL                        { loader.addArray($1); }
    | SYMBOL array_pattern          {
        for(auto& s: $2)
            loader.addArray($1 + s);
                                    }
    | array_pattern SYMBOL   {
        for(auto& s: $1)
            loader.addArray(s + $2);
                                    }
    | SYMBOL array_pattern SYMBOL   {
        for(auto& s: $2)
            loader.addArray($1 + s + $3);
                                    }
    ;

array_list
    : array_name
    | array_list ARRAY_DELIM array_name
    ;

expr
    : array_list options
    ;

%%

void ceammc::al::ArrayLoaderParser::error(const location& loc, const std::string& err_message)
{
    auto line = lexer.matcher().line();
    lexer.out() << err_message << ':' << std::endl;
    lexer.out() << lexer.indent()
                << line << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;
}


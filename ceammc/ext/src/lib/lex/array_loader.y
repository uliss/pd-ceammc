%require "3.5"
%skeleton "lalr1.cc"
%output "array_loader.parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc}
%define api.parser.class {ArrayLoaderParser}

%define parse.error verbose
%define parse.assert
%define api.value.type variant
%define api.token.constructor
%locations
%define api.location.file "array_loader.location.hpp"
%debug

%parse-param { ceammc::ArrayLoaderLexer& lexer }
%parse-param { ceammc::ArrayLoader& loader }

%code requires {
    # include <cstddef>
    # include <cstdio>
    # include <vector>
    # include <string>

    namespace ceammc {
        class ArrayLoaderLexer;
        class ArrayLoader;
    }

    using StringList = std::vector<std::string>;
}

%code top {
    # undef yylex
    # define yylex lexer.lex

    # include "array_loader.lexer.h"
    # include "array_loader.h"
    # include "ceammc_convert.h"

    using OPT = ceammc::ArrayLoader::OptionType;

    static StringList generateRange(uint from, uint to) {
        StringList res;
        res.reserve(to - from + 1);
        for(uint i = from; i <= to; i++)
            res.emplace_back(std::to_string(i));

        return res;
    }
}

%token                  ACT_LOAD
%token  <std::string>   QUOTED_STRING
%token  <std::string>   SYMBOL
%token                  ARRAY_DELIM
%token                  PATTERN_BEGIN PATTERN_END
%token                  RANGE_DELIM VAR_DELIM
%token  <std::string>   OPTION
%token                  LENGTH RESIZE GAIN RESAMPLE BEGIN END VERBOSE NORMALIZE
%token  <std::string>   SMPTE
%token  <double>        FLOAT
%token  <int>           INT
%token  <uint>          UINT
%token                  SEC MSEC SAMPLES DB
%token                  COLON ':'  "colon"
%token                  DOT   '.'  "dot"
%token                  FRAC  '/'  "fraction"
%token                  STRING_END 0 "end of string"
%token                  LEXER_ERROR

%nterm  <std::size_t>   time smpte
%nterm  <double>        number
%nterm  <StringList>    array_pattern var_list
%nterm  <std::string>   var

%start EXPR

%%

smpte
    : UINT COLON UINT                     { $$ = loader.smpte2samples(0, $1, $3, 0); }
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

opt
    : RESIZE           { loader.setFlagOption(OPT::OPT_RESIZE); }
    | NORMALIZE        { loader.setFlagOption(OPT::OPT_NORMALIZE); }
    | VERBOSE          { loader.setFlagOption(OPT::OPT_VERBOSE); }
    | BEGIN  time      { if(!loader.setTimeOption(OPT::OPT_BEGIN, $2))      error(@2, "invalid @begin value");}
    | END    time      { if(!loader.setTimeOption(OPT::OPT_END, $2))        error(@2, "invalid @end value");}
    | LENGTH time      { if(!loader.setTimeOption(OPT::OPT_LENGTH, $2))     error(@2, "invalid @length value");}
    | GAIN number      { if(!loader.setGain($2))                            error(@2, "invalid @gain value");}
    | GAIN number DB   { if(!loader.setGain(ceammc::convert::dbfs2amp($2))) error(@2, "invalid @gain value");}
    | RESAMPLE number  {
            if(!loader.setResampleRatio($2/loader.sampleRate()))
                error(@2, "invalid @resample value");
        }
    | RESAMPLE number FRAC number
    ;

options
    : %empty
    | options opt
    ;

filename
    : QUOTED_STRING
    | SYMBOL
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
    : PATTERN_BEGIN PATTERN_END                         { $$ = generateRange(0, 9); }
    | PATTERN_BEGIN var_list PATTERN_END                { $$ = $2; }
    | PATTERN_BEGIN UINT RANGE_DELIM UINT PATTERN_END   {

        if($2 > $4)
            error(@2, "invalid range values");
        else
            $$ = generateRange($2, $4);
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

load
    : ACT_LOAD filename array_list options
    ;

EXPR
    : load
    ;

%%

void ceammc::ArrayLoaderParser::error(const location& loc, const std::string& err_message)
{
    lexer.out() << err_message << std::endl;
    lexer.out() << lexer.indent()
                << lexer.matcher().line() << std::endl;
    lexer.out() << lexer.indent(loc.begin.column)
                << std::string(loc.end.column - loc.begin.column + 1, '^') << std::endl;
}


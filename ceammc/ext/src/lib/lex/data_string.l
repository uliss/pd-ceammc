%top {
    # include <string>
    # include <memory>

    # include "lex/data_string.parser.hpp"
    # include "lex/data_string.location.hpp"
    # include "ceammc_log.h"

    using token = ceammc::DataStringParser::token;
%}

%class {
    public:
        size_t output_indent = {0};

        std::string indent(size_t n = 0) const {
            return std::string(output_indent + n, ' ');
        }
%}

%{
    # include "fmt/format.h"
%}

%option freespace unicode
%option namespace=ceammc
%option lexer=DataStringLexer
%option header-file=data_string.lexer.h
%option outfile=data_string.lexer.cpp
/*%option graphs-file*/
/*%option debug*/
/*%option perf-report*/
%option bison-cc-namespace="ceammc"
%option bison-cc-parser="DataStringParser"
%option bison-cc
%option bison-complete
%option bison-locations
%option noyywrap
%option reentrant

udec       0 | [1-9] [0-9]*
int        [-+]? {udec}
frac       \. [0-9]+
float      {int} {frac}?
hex        [-+]?0[xX][0-9a-fA-F]+
bin        0b[01]+

string     \"([^`\"]|`\"|``|`\.|`:|`\(|`\))*\"

space      [ \t\n]+
data_type  [A-Z][a-zA-Z]*
func_call  [a-z][a-z0-9]*
open_list  \(
close_list \)
open_dict  \[
close_dict \]

symbol0    [a-zA-Z_.@#0-9/%]
symbol_    [^:\(\)\[\]\"\\\` ]
symbol     {symbol0} {symbol_}*

key        [a-zA-Z_.@#0-9]+:

%%

{space}
,                       { return DataStringParser::make_COMMA(location()); }
null                    { return DataStringParser::make_NULL(location()); }
true                    { return DataStringParser::make_FLOAT(1, location()); }
false                   { return DataStringParser::make_FLOAT(0, location()); }
S/{string}              { return DataStringParser::make_DATA_TYPE_STRING(location()); }
{data_type}/{open_list} { return DataStringParser::make_DATA_TYPE(text(), location()); }
{data_type}/{open_dict} { return DataStringParser::make_DATA_TYPE(text(), location()); }
{func_call}/{open_list} { return DataStringParser::make_FUNC_CALL(text(), location()); }
{open_dict}             { return DataStringParser::make_OPEN_DICT_BRACKET(location()); }
{close_dict}            { return DataStringParser::make_CLOSE_DICT_BRACKET(location()); }
{open_list}             { return DataStringParser::make_OPEN_LIST_BRACKET(location()); }
{close_list}            { return DataStringParser::make_CLOSE_LIST_BRACKET(location()); }
{float}                 { return DataStringParser::make_FLOAT(std::strtod(text(), 0), location()); }
{hex}                   {
                          try {
                           long hex = std::stoi(text(), 0, 16);
                           return DataStringParser::make_FLOAT(hex, location());
                          }
                          catch(std::exception& e) {
                             out() << "invalid hex: " << e.what() << " - " << text();
                             return DataStringParser::make_LEXER_ERROR(location());
                          }
                        }
{bin}                   {
                          std::string str(text() + 2);
                          try {
                           long bin = std::stoi(str.c_str(), 0, 2);
                           return DataStringParser::make_FLOAT(bin, location());
                          }
                          catch(std::exception& e) {
                             out() << "invalid bin: " << e.what() << " - " << text();
                             return DataStringParser::make_LEXER_ERROR(location());
                          }
                        }
{key}                   {
                          std::string key(str());
                          key.pop_back(); // remove trailing ':'
                          return DataStringParser::make_KEY(key, location());
                        }
{symbol}                { return DataStringParser::make_SYMBOL(text(), location()); }
{string}                {
                          std::string str(text() + 1);
                          str.pop_back();
                          return DataStringParser::make_STRING(str, location());
                        }

<*> .                   {
                          std::string line = matcher().line();
                          std::string err;

                          if(strcmp("`", text()) == 0)
                            err = " - stray ` found, use quotes: \"``\"";
                          else if(strcmp("\"", text()) == 0)
                            err = " - unmatched \" found, use quotes: \"`\"\"";
                          else if(strcmp("[", text()) == 0)
                            err = " - unmatched [ found, use quotes: \"[\"";
                          else if(strcmp("]", text()) == 0)
                            err = " - unmatched ] found, use quotes: \"]\"";
                          else if(strcmp("(", text()) == 0)
                            err = " - unmatched '(' found, use quotes: \"(\"";
                          else
                            err = fmt::format("unmatched lexer input: '{}'", text());

                          out() << fmt::format("error while parsing '{}': {}", line, err);

                          return DataStringParser::make_LEXER_ERROR(location()); /* error */
                        }

%%

# include "fmt/format.h"


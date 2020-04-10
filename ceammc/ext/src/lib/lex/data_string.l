%top {
    # include <string>
    # include <memory>

    # include "lex/data_string.parser.hpp"
    # include "ceammc_log.h"

    using token = ceammc::DataStringParser::token;
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
open_list  \(
close_list \)
open_dict  \[
close_dict \]

symbol0    [a-zA-Z_.@#0-9]
symbol_    [a-zA-Z_.@#0-9\-+:\?'|/<>=*~]
symbol     {symbol0} {symbol_}*

key        {symbol0}+:

%%

{space}
null                    { return token::TOK_NULL; }
true                    { return DataStringParser::make_FLOAT(1); }
false                   { return DataStringParser::make_FLOAT(0); }
S/{string}              { return DataStringParser::make_DATA_TYPE_STRING(); }
{data_type}/{open_list} { return DataStringParser::make_DATA_TYPE(text()); }
{data_type}/{open_dict} { return DataStringParser::make_DATA_TYPE(text()); }
{open_dict}             { return DataStringParser::make_OPEN_DICT_BRACKET(); }
{close_dict}            { return DataStringParser::make_CLOSE_DICT_BRACKET(); }
{open_list}             { return DataStringParser::make_OPEN_LIST_BRACKET(); }
{close_list}            { return DataStringParser::make_CLOSE_LIST_BRACKET(); }
{float}                 { return DataStringParser::make_FLOAT(std::strtod(text(), 0)); }
{hex}                   {
                          try {
                           long hex = std::stoi(text(), 0, 16);
                           return DataStringParser::make_FLOAT(hex);
                          }
                          catch(std::exception& e) {
                             LIB_ERR << "invalid hex: " << e.what() << " - " << text();
                             return token::TOK_LEXER_ERROR;
                          }
                        }
{bin}                   {
                          std::string str(text() + 2);
                          try {
                           long bin = std::stoi(str.c_str(), 0, 2);
                           return DataStringParser::make_FLOAT(bin);
                          }
                          catch(std::exception& e) {
                             LIB_ERR << "invalid bin: " << e.what() << " - " << text();
                             return token::TOK_LEXER_ERROR;
                          }
                        }
{key}                   {
                          std::string key(str());
                          key.pop_back(); // remove trailing ':'
                          return DataStringParser::make_KEY(key);
                        }
{symbol}                { return DataStringParser::make_SYMBOL(text()); }
{string}                {
                          std::string str(text() + 1);
                          str.pop_back();
                          return DataStringParser::make_STRING(str);
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

                          LIB_ERR << fmt::format("error while parsing '{}': {}", line, err);

                          return token::TOK_LEXER_ERROR; /* error */
                        }

%%

# include "fmt/format.h"


%top {
    # include <cerrno>
    # include <climits>
    # include <cstdlib>
    # include <string>
    # include "tl_cmd_driver.h"
    # include "tl_parser.hpp"

    /* import the parser's token type into a local typedef */
    using token = ceammc::tl::TlParser::token;

    # define TL_NO_UNISTD_H
%}

%option freespace
%option namespace=ceammc::tl
%option lexer=TlLexer
%option bison-cc-namespace=ceammc::tl
%option bison-cc-parser=TlParser
%option bison-cc
%option bison-complete
%option bison-locations
%option noyywrap
%option reentrant
%option fast
%option header-file="tl_lexer.h"
%option outfile="tl_lexer.cpp"
%option stack yylineno

FC              [a-zA-Z_@]
C               [a-zA-Z_!@#$%^&*0-9]
float           [+-]?[0-9]+[.]?[0-9]+
int             [+-]?[0-9]+
time            [0-9]{1,2}":"[0-9]{2}(":"[0-9]{2})?("."[0-9]{1,3})?
unit            "ms"|"msec"|"s"|"sec"
act             "add"|"remove_at"|"remove"|"clear"|"to_event"|"to_time"
frac            [1-9][0-9]*"/"[1-9][0-9]*
word            {FC}{C}+
blank           [ \t]

%%

{time}          { return TlParser::make_CLOCK_TIME(text(), location()); }

{int}           {
                      long v = strtol(text(), NULL, 10);
                      return TlParser::make_INT(v, location());
                }

{float}         {
                      float v = strtof(text(), NULL);
                      return TlParser::make_FLOAT(v, location());
                }

{unit}          { return TlParser::make_UNIT(text(), location()); }
before          { return TlParser::make_BEFORE(location()); }
after           { return TlParser::make_AFTER(location()); }


{act}           { std::string act(text());
                  if(act == "add")
                    return TlParser::make_ADD(location());
                  else if(act == "remove")
                    return TlParser::make_REMOVE(location());
                  else if(act == "remove_at")
                    return TlParser::make_REMOVE_AT(location());
                  else if(act == "clear")
                    return TlParser::make_CLEAR(location());
                  else if(act == "to_event")
                    return TlParser::make_TO_EVENT(location());
                  else if(act == "to_time")
                    return TlParser::make_TO_TIME(location());
                }

{word}          { return TlParser::make_WORD(text(), location()); }
{blank}+        {  }
%%

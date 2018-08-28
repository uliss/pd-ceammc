%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "tl_cmd_driver.h"
#include "tl_parser.hpp"

/* import the parser's token type into a local typedef */
typedef yy::TlCmdParser::token token;
typedef yy::TlCmdParser::token_type token_type;

#define yyterminate() return yy::TlCmdParser::make_END(loc)

// The location of the current token.
static yy::location loc;

#define TL_NO_UNISTD_H
%}

%option header-file="tl_lexer.h"
%option outfile="tl_lexer.cpp"
%option noyywrap warn stack yylineno

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


%{
    // Code run each time a pattern is matched.
    #define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}


{time}          { return yy::TlCmdParser::make_CLOCK_TIME(yytext, loc); }

{int}           {
                      long v = strtol(yytext, NULL, 10);
                      return yy::TlCmdParser::make_INT(v, loc);
                }

{float}         {
                      float v = strtof(yytext, NULL);
                      return yy::TlCmdParser::make_FLOAT(v, loc);
                }

{unit}          { return yy::TlCmdParser::make_UNIT(yytext, loc); }
before          { return yy::TlCmdParser::make_BEFORE(loc); }
after           { return yy::TlCmdParser::make_AFTER(loc); }


{act}           { std::string act(yytext);
                  if(act == "add")
                    return yy::TlCmdParser::make_ADD(loc);
                  else if(act == "remove")
                    return yy::TlCmdParser::make_REMOVE(loc);
                  else if(act == "remove_at")
                    return yy::TlCmdParser::make_REMOVE_AT(loc);
                  else if(act == "clear")
                    return yy::TlCmdParser::make_CLEAR(loc);
                  else if(act == "to_event")
                    return yy::TlCmdParser::make_TO_EVENT(loc);
                  else if(act == "to_time")
                    return yy::TlCmdParser::make_TO_TIME(loc);
                }

{word}          { return yy::TlCmdParser::make_WORD(yytext, loc); }
{blank}+        {  }
%%

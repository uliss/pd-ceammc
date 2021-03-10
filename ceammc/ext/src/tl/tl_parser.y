%skeleton "lalr1.cc"
%require "3.5"
%output "tl_parser.cpp"

%defines
%define api.token.prefix {TOK_}
%define api.namespace {ceammc::tl}
%define api.parser.class {TlParser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.error verbose
%define parse.trace

%locations
%define api.location.file "tl.location.hpp"
%debug

%parse-param { ceammc::tl::TlLexer& lexer }
%parse-param { ceammc::tl::TlCmdDriver& driver }

%code requires
{
    # include <string>
    namespace ceammc {
        namespace tl {
            class TlCmdDriver;
            class TlLexer;
        }
    }
}

%code top {
    # undef yylex
    # define yylex lexer.lex

    # include "tl_lexer.h"
}

// The parsing context.
%code
{
    # include "tl_cmd_driver.h"
}

%token                  END         0	"end of file"
%token                  SPACE       " "
%token                  ADD         "add action"
%token                  REMOVE      "remove action"
%token                  CLEAR       "clear action"
%token                  TO_EVENT    "move to named event"
%token                  TO_TIME     "move to time"
%token                  REMOVE_AT   "remove at specified time ms"
%token <long>           INT         "integer number"
%token                  AFTER
%token                  BEFORE
%token <double>         FLOAT       "float number"
%token <std::string>    WORD        "word"
%token <std::string>    UNIT        "unit"
%token <std::string>    CLOCK_TIME  "clock time in format 00:00:00.000"

%type <double>          time        "time in any format"
%type <double>          ms_time     "time in msec"

%printer { yyoutput << $$; } <*>;

%%
%start command;
command:
  add_command
| remove_command
| remove_command_at
| move_command
| clear_command;

add_command:
  ADD time                      { driver.addAbsEvent("", $2); }
| ADD WORD time                 { driver.addAbsEvent($2, $3); }
| ADD WORD time BEFORE  WORD    { driver.addRelEvent($2, -$3, $5); }
| ADD WORD time AFTER   WORD    { driver.addRelEvent($2, $3, $5); };

remove_command:
  REMOVE WORD                   { driver.removeEvent($2); }
| REMOVE INT                    { driver.removeEventAtPos($2); };

remove_command_at:
  REMOVE_AT time                { driver.removeEventAtTime($2); };

clear_command:  CLEAR           { driver.clearAll(); } ;

move_command:
  TO_EVENT WORD                 { driver.moveToEvent($2); }
| TO_EVENT INT                  { driver.moveToEvent($2); }
| TO_TIME  time                 { driver.moveToTime($2); };

time:
  CLOCK_TIME                    { $$ = driver.parseClock($1); }
| ms_time                       { $$ = double($1); };

ms_time:
  INT UNIT                      { $$ = driver.parseTime($1, $2); }
| FLOAT UNIT                    { $$ = driver.parseTime($1, $2); };

%%

void ceammc::tl::TlParser::error(const ceammc::tl::TlParser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

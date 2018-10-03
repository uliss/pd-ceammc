%{
#include <string.h>
#include <stdio.h>

#include "dict_parser_impl.h"
#define YY_DECL int yylex(t_dict *dict)

#include "m_pd.h"
#include "lex.dict_parser.h"

#include "dict_parser.tab.h"

void yyerror(t_dict* dict, const char* s);
int yylex(t_dict* dict);

int dict_parse_string(t_dict* dict, const char* s);

%}

%code requires {
#include "m_pd.h"
enum ValueType {
    VTYPE_LIST = 0,
    VTYPE_DICT = 1
};

typedef struct dict_token {
    void* sym;
    enum ValueType type;
    int npairs;
} t_dict_token;
}

%define api.prefix {dict_parser_}
%define api.pure false
%parse-param {t_dict *dict}
%lex-param   {t_dict *dict}
%define parse.trace

%union {
    const char* txt;
    t_dict_token tok;
}

%token <txt> TOK_ASSOC TOK_WORD TOK_QSTR TOK_PAIR_BEGIN TOK_PAIR_END TOK_DICT_BEGIN TOK_DICT_END
%type <tok.npairs> pair_list
%type <tok.npairs> pair
%type <tok.sym> word
%type <tok.sym> key
%type <tok.type> value
%type <tok.type> list
%type <tok.sym> dict

%start dict

%%

dict: spair    { dict_store(dict, 1); }
  |  pair_list { dict_store(dict, $1); }
  | TOK_DICT_BEGIN pair_list TOK_DICT_END { dict_store(dict, $2); };

pair_list:
    pair { $$ = 1; }
 |  pair_list pair { $$ = $1 + 1; };

pair: TOK_PAIR_BEGIN key TOK_ASSOC value TOK_PAIR_END {
       if($4 == VTYPE_LIST)
           dict_insert_pair_list(dict, $2);
       else if($4 == VTYPE_DICT)
           dict_insert_pair_dict(dict, $2);
   };

spair: key TOK_ASSOC value {
       if($3 == VTYPE_LIST)
           dict_insert_pair_list(dict, $1);
       else if($3 == VTYPE_DICT)
           dict_insert_pair_dict(dict, $1);
};

word:
   TOK_WORD { $$ = gensym($1); }
 | TOK_QSTR { $$ = gensym($1); };

key: word;

value:
   list { $$ = VTYPE_LIST; }
 | dict { $$ = VTYPE_DICT; };

list:
   word      { dict_push_to_list(dict, $1); }
 | list word { dict_push_to_list(dict, $2); };


%%

void yyerror(t_dict* dict, const char* s)
{
    pd_error(0, "[dict] parse error: %s", s);
}

int dict_parse_string(t_dict* dict, const char* s)
{
    if(strlen(s) == 0)
        return 0;

    YY_BUFFER_STATE b;
    b = dict_parser__scan_string(s);

    dict_clear(dict);
    int ok = yyparse(dict);

    dict_parser__delete_buffer(b);
    return ok;
}


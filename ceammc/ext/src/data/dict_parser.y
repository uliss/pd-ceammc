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

%define api.prefix {dict_parser_}
%define api.pure false
%parse-param {t_dict *dict}
%lex-param   {t_dict *dict}

%union {
    const char* txt;
}

%token <txt> TOK_ASSOC TOK_WORD TOK_QSTR TOK_BEGIN TOK_END
%type <txt> kv
%type <txt> list

%start dict

%%

dict:     spair
        | mpair
        | dict mpair
        ;

kv:       TOK_WORD
        | TOK_QSTR
        ;

list:     kv
        | list kv
        ;

mpair:    TOK_BEGIN spair TOK_END
        ;

spair:    kv TOK_ASSOC list   { dict_lexer_insert_pair(dict); }
        ;

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


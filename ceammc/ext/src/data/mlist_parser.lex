%option noyywrap
%option prefix="mlist_parser_"
%option outfile="lex.mlist_parser.c"

%{
#include <assert.h>
#include <stdlib.h>

#include "mlist_parser_impl.h"
#include "mlist_parser.tab.h"

#define YY_DECL int yylex(mlist_ast *tree)
//#define MSG(s) printf("%s ", s)
#define MSG(s)

int mlist_lex_open_par(mlist_ast* tree)
{
    MSG("(");

    mlist_node* parent = mlist_ast_top(tree);

    if(!parent) {
        // create root node
        mlist_node* root = mlist_node_create();
        mlist_ast_push(tree, root);
    }
    else {
        // append as child to top node
        mlist_node* n = mlist_node_create();
        mlist_node_append(parent, n);
        mlist_ast_push(tree, n);
    }

    return TOK_OPEN_PAR;
}

int mlist_lex_close_par(mlist_ast* tree)
{
    MSG(")");

    mlist_node* n = mlist_ast_pop(tree);

    if(!n) {
        mlist_parser_lval.txt = "mismatched close bracket";
        return TOK_ERROR;
    }

    if (mlist_ast_top(tree) == NULL)
        mlist_ast_set_root(tree, n);

    return TOK_CLOSE_PAR;
}

int mlist_lex_space()
{
    MSG("SPACE");

    return TOK_SPACE;
}

int mlist_lex_atom(mlist_ast* tree, const char* txt, int is_string)
{
    MSG("ATOM");

    mlist_node* parent = mlist_ast_top(tree);
    if(!parent) {
        mlist_parser_lval.txt = "stack is empty";
        return TOK_ERROR;
    }

    mlist_node* n = NULL;

    if(is_string) {
        const size_t len = strlen(txt);
        // at least quotes ""
        assert(len >= 2);

        // remove outer and unescape inner quotes
        char* str = (char*) malloc(len-1);
        char* outp = str;
        for(size_t i = 1; i < len - 1; i++) {
            char c = txt[i];
            char pc = txt[i-1];
            if(c == '"' || c == '`') {
                if(pc == '`')
                    *outp++ = c;
                continue;
            }
            else {
                *outp++ = c;
            }
        }

        *outp = '\0';

        n = mlist_node_text_create(str, is_string);
        free(str);
    }
    else {
        n = mlist_node_text_create(txt, is_string);
    }

    mlist_node_append(parent, n);
    return TOK_ATOM;
}
%}

space      [ ]+
string     \"([^`\"]|\\.)*\"
word       [^ ()]+

%%

[ ]*"("[ ]*    {  return mlist_lex_open_par(tree); }
[ ]*")"[ ]*    {  return mlist_lex_close_par(tree); }
{space}        {  return mlist_lex_space(); }
{string}       {  return mlist_lex_atom(tree, yytext, 1); }
{word}         {  return mlist_lex_atom(tree, yytext, 0); }

%%

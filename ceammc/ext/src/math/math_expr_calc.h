/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef MATH_EXPR_CALC_H
#define MATH_EXPR_CALC_H

#if defined(__cplusplus)
extern "C" {
#endif

/* Data type for links in the chain of symbols. */

struct symrec {
    char name[8]; /* name of symbol */
    int type; /* type of symbol: VAR or FNCT */
    union {
        double var; /* value of a VAR  */
        double (*fnctptr)(double); /* value of a FNCT */
    } value;
    struct symrec* next; /* link field      */
};

typedef struct symrec symrec;

/* The symbol table: a chain of 'struct symrec' */
extern symrec* sym_table;

symrec* math_expr_putvar(const char* var_name, double v);
symrec* math_expr_setvar(const char* var_name, double v);
symrec* math_expr_putsym(const char* sym_name, int sym_type);
symrec* math_expr_getsym(const char* sym_name);

void math_expr_init_table();
int math_expr_calc(const char* s, double* res);

int math_exprlex();

typedef struct ast ast;
typedef struct Node Node;
void math_expr_error(ast *ast, const char* s);
int math_expr_parse_ast(ast* ast, const char* s);

#if defined(__cplusplus)
}
#endif

#endif // MATH_EXPR_CALC_H

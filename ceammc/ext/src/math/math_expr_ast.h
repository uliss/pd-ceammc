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
#ifndef MATH_EXPR_AST_H
#define MATH_EXPR_AST_H

typedef struct ast ast;
typedef double (*UnaryFunc)(double);
typedef double (*BinaryFunc)(double, double);
typedef struct Node Node;

#if defined(__cplusplus)
extern "C" {
#endif

ast* ast_new();
void ast_free(ast* tree);
Node* ast_root(ast* tree);
void ast_print(ast* tree);
int ast_ok(ast* tree);
void ast_invalidate(ast* tree);
int ast_eval(ast* tree, double* res);
void ast_clear_vars(ast* tree);
void ast_bind_var(ast* tree, int idx, double v);
double* ast_ref(ast* tree, int idx);

Node* node_add_cont(Node* parent, Node* c);
Node* node_create_value(double v);
Node* node_create_ref(double* v);
Node* node_create_cont(Node* c);
Node* node_create_ufunc(UnaryFunc fn, Node* arg);
Node* node_create_bfunc(BinaryFunc fn, Node* arg0, Node* arg1);

typedef enum UFuncName {
    UFN_SIN = 0,
    UFN_COS,
    UFN_TAN,
    UFN_SQRT,
    UFN_EXP,
    UFN_LN,
    UFN_LOG2,
    UFN_LOG10,
    UFN_ATAN
} UFuncName;

typedef enum BFuncName {
    BFN_MIN = 0,
    BFN_MAX
} BFuncName;

typedef enum ParseError {
    ERR_UNKNOWN_FUNC = 1000
} ParseError;

UnaryFunc ufnNameToPtr(UFuncName n);
BinaryFunc bfnNameToPtr(BFuncName n);

#if defined(__cplusplus)
}
#endif

#endif // MATH_EXPR_AST_H

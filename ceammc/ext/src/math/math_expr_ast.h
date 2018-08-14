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

typedef struct Ast Ast;
typedef struct Node Node;

typedef double math_float_t;
typedef math_float_t* math_float_ref_t;

typedef math_float_t (*UnaryFloatFunc)(math_float_t);
typedef math_float_t (*BinaryFloatFunc)(math_float_t, math_float_t);

#if defined(__cplusplus)
extern "C" {
#endif

Ast* ast_new();
void ast_free(Ast* tree);
Node* ast_root(Ast* tree);
void ast_print(Ast* tree);
int ast_ok(Ast* tree);
void ast_invalidate(Ast* tree);
int ast_eval(Ast* tree, double* res);
void ast_clear_vars(Ast* tree);
void ast_bind_var(Ast* tree, int idx, double v);
double* ast_ref(Ast* tree, int idx);

Node* node_add_cont(Node* parent, Node* c);
Node* node_create_value_float(math_float_t v);
Node* node_create_ref_float(math_float_ref_t v);
Node* node_create_cont(Node* c);
Node* node_create_ufunc(UnaryFloatFunc fn, Node* arg);
Node* node_create_bfunc(BinaryFloatFunc fn, Node* arg0, Node* arg1);
Node* node_create_afunc(const char* name, Node* idx);

typedef enum UFuncName {
    UFN_SIN = 0,
    UFN_COS,
    UFN_TAN,
    UFN_SQRT,
    UFN_EXP,
    UFN_LN,
    UFN_LOG2,
    UFN_LOG10,
    UFN_ATAN,
    UFN_ABS,
    UFN_SIGN,
    UFN_FACTORIAL,
    UFN_ROUND,
    UFN_CEIL,
    UFN_FLOOR
} UFuncName;

typedef enum BFuncName {
    BFN_MIN = 0,
    BFN_MAX,
    BFN_CMN
} BFuncName;

typedef enum ParseError {
    ERR_UNKNOWN_FUNC = 1000
} ParseError;

UnaryFloatFunc ufnNameToPtr(UFuncName n);
BinaryFloatFunc bfnNameToPtr(BFuncName n);

#if defined(__cplusplus)
}
#endif

#endif // MATH_EXPR_AST_H

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

#include "math_expr_ast.h"

int math_exprlex();

void math_expr_error(ast *ast, const char* s);
int math_expr_parse_ast(ast* ast, const char* s);

#if defined(__cplusplus)
}
#endif

#endif // MATH_EXPR_CALC_H

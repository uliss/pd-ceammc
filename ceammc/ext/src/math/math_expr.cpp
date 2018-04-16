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
#include "math_expr.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "math_expr_ast.h"
#include "math_expr_calc.h"

#include <cassert>
#include <cmath>

MathExpr::MathExpr(const PdArgs& args)
    : BaseObject(args)
    , ast_(0)
{
    expr_ = to_string(positionalArguments(), "");

    updateAST();

    createInlet();
    createOutlet();

    createCbProperty("@expr", &MathExpr::propExpr, &MathExpr::propSetExpr);
}

MathExpr::~MathExpr()
{
    ast_free(ast_);
}

void MathExpr::onFloat(t_float v)
{
    if (!ast_) {
        OBJ_ERR << "NULL AST";
        return;
    }

    if (!ast_ok(ast_)) {
        OBJ_ERR << "invalid AST: not parsed...";
        return;
    }

    ast_clear_vars(ast_);
    ast_bind_var(ast_, 0, v);

    double res = 0;
    int err = ast_eval(ast_, &res);
    if (err) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

void MathExpr::onInlet(size_t n, const AtomList& lst)
{
    expr_ = to_string(lst, "");
    updateAST();
}

void MathExpr::onList(const AtomList& lst)
{
    //    bin vars
    ast_clear_vars(ast_);
    for (size_t i = 0; i < std::min<size_t>(10, lst.size()); i++)
        ast_bind_var(ast_, i, lst[i].asFloat());

    double res = 0;
    int err = ast_eval(ast_, &res);
    if (err) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

AtomList MathExpr::propExpr() const
{
    return Atom(gensym(expr_.c_str()));
}

void MathExpr::propSetExpr(const AtomList& lst)
{
    expr_ = to_string(lst, "");
    updateAST();
}

void MathExpr::updateAST()
{
    if (ast_) {
        ast_free(ast_);
    }

    ast_ = ast_new();

    int err = math_expr_parse_ast(ast_, expr_.c_str());
    if (err)
        return;
}

void setup_math_expr()
{
    ObjectFactory<MathExpr> obj("math.expr");
}

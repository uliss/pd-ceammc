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
    , ast_(nullptr, ast_free)
{
    createCbListProperty(
        "@expr",
        [this]() -> AtomList { return Atom(gensym(expr_.c_str())); },
        [this](const AtomList& l) -> bool {
            expr_ = to_string(l, "");
            updateAST();
            return true;
        })
        ->setArgIndex(0);

    createInlet();
    createOutlet();
}

MathExpr::~MathExpr() = default;

void MathExpr::onFloat(t_float v)
{
    if (!ast_) {
        OBJ_ERR << "NULL AST";
        return;
    }

    if (!ast_ok(ast_.get())) {
        OBJ_ERR << "invalid AST: not parsed...";
        return;
    }

    ast_clear_vars(ast_.get());
    ast_bind_var(ast_.get(), 0, v);

    double res = 0;
    int err = ast_eval(ast_.get(), &res);
    if (err) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

void MathExpr::onInlet(size_t n, const AtomListView& lst)
{
    setProperty("@expr", lst);
}

void MathExpr::onList(const AtomList& lst)
{
    //  bind vars
    ast_clear_vars(ast_.get());
    auto NVARS = std::min<size_t>(10, lst.size());
    for (size_t i = 0; i < NVARS; i++)
        ast_bind_var(ast_.get(), i, lst[i].asFloat());

    double res = 0;
    int err = ast_eval(ast_.get(), &res);
    if (err) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

void MathExpr::updateAST()
{
    ast_.reset(ast_new());

    int err = math_expr_parse_ast(ast_.get(), expr_.c_str());
    if (err)
        return;
}

void setup_math_expr()
{
    ObjectFactory<MathExpr> obj("math.expr");
}

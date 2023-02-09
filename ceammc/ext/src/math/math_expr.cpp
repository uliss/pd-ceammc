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

#include <cassert>
#include <cmath>

MathExpr::MathExpr(const PdArgs& args)
    : BaseObject(args)
{
    createCbListProperty(
        "@expr",
        [this]() -> AtomList { return Atom(gensym(expr_.c_str())); },
        [this](const AtomListView& lv) -> bool {
            expr_ = to_string(lv, "");
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
    using namespace ceammc::math;

    if (!ast_) {
        OBJ_ERR << "NULL AST";
        return;
    }

    if (!ast_->isOk()) {
        OBJ_ERR << "invalid AST: not parsed...";
        return;
    }

    ast_->clearVars();
    ast_->bindVar(0, v);

    math_float_t res = 0;
    if (!ast_->eval(&res)) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

void MathExpr::onInlet(size_t n, const AtomListView& lv)
{
    setProperty("@expr", lv);
}

void MathExpr::onList(const AtomListView& lv)
{
    using namespace ceammc::math;

    if (!ast_) {
        OBJ_ERR << "NULL AST";
        return;
    }

    if (!ast_->isOk()) {
        OBJ_ERR << "invalid AST: not parsed...";
        return;
    }

    //  bind vars
    ast_->clearVars();
    auto NVARS = std::min<size_t>(10, lv.size());
    for (size_t i = 0; i < NVARS; i++)
        ast_->bindVar(i, lv[i].asFloat());

    math_float_t res = 0;
    if (!ast_->eval(&res)) {
        OBJ_ERR << "eval error";
        return;
    }

    floatTo(0, res);
}

void MathExpr::updateAST()
{
    ast_.reset(new ceammc::math::Ast);
    ast_->parse(expr_);
}

void setup_math_expr()
{
    ObjectFactory<MathExpr> obj("math.expr");
    obj.parseArgsMode(PdArgs::PARSE_COPY);
    obj.parsePropsMode(PdArgs::PARSE_COPY);

    obj.setXletsInfo({ "float: input value\n"
                       "list: input values",
                         "list: set new expression" },
        { "float: result expression" });

    obj.setDescription("dynamic [expr]");
    obj.setCategory("math");
}

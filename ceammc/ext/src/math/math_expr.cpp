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
#include "math_expr_calc.h"

#include <cmath>

extern "C" double math_expr_yyparse(const char*);

MathExpr::MathExpr(const PdArgs& args)
    : BaseObject(args)
{
    expr_ = to_string(positionalArguments(), "");

    createInlet();
    createOutlet();

    createCbProperty("@expr", &MathExpr::propExpr, &MathExpr::propSetExpr);
}

void MathExpr::onFloat(t_float v)
{
    math_expr_setvar("$f", v);

    double res = 0;
    int err = math_expr_calc(expr_.c_str(), &res);

    if (!err)
        floatTo(0, res);
}

void MathExpr::onInlet(size_t n, const AtomList& lst)
{
    expr_ = to_string(lst, "");
}

void MathExpr::onList(const AtomList& lst)
{
    AtomList out;

    for (size_t i = 0; i < lst.size(); i++) {
        const Atom& a = lst[i];
        t_float v = 0;
        if (!a.getFloat(&v))
            continue;

        double res = 0;
        math_expr_setvar("$f", v);
        int err = math_expr_calc(expr_.c_str(), &res);
        if (!err)
            out.append(Atom(res));
    }

    listTo(0, out);
}

AtomList MathExpr::propExpr() const
{
    return Atom(gensym(expr_.c_str()));
}

void MathExpr::propSetExpr(const AtomList& lst)
{
    expr_ = to_string(lst, "");
}

void setup_math_expr()
{
    ObjectFactory<MathExpr> obj("math.expr");
    math_expr_init_table();
    math_expr_putvar("$pi", M_PI);
    math_expr_putvar("$e", M_E);
}

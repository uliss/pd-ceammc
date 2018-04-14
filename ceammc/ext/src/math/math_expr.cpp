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
#include "math_expr_spirit.h"

//extern "C" double math_expr_yyparse(const char*);

MathExpr::MathExpr(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createOutlet();
}

void MathExpr::onFloat(t_float v)
{
}

void MathExpr::onList(const AtomList& lst)
{
    OptDouble res = parse(to_string(lst).c_str());

    if (res)
        floatTo(0, *res);
}

void MathExpr::onAny(t_symbol* s, const AtomList& lst)
{
    std::string expr(s->s_name);
    expr += to_string(lst);

    OptDouble res = parse(expr.c_str());

    if (res)
        floatTo(0, *res);
}

void setup_math_expr()
{
    ObjectFactory<MathExpr> obj("math.expr");
}

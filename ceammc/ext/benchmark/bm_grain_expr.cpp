/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "grain_expr_parser.h"

#include <nonius/nonius.h++>

#include <cassert>

using namespace ceammc;

static AtomList l0, l1, l2, l3;
static ByteCode bc0, bc1, bc2;

extern "C" void pd_init();

bool init_data()
{
    pd_init();
    l0 = AtomList::parseString("@t 100");
    l1 = AtomList::parseString("@t abs(0.25*10.0)");
    l2 = AtomList::parseString("@t sin($pi/2)");
    l3 = AtomList::parseString("@t 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1");

    GrainExprParser p;
    p.parse(l3.view());
    bc0 = p.byteCode();
    bc1 = bc0;
    bc1.optimize();
    return true;
}

const static bool init = init_data();

NONIUS_BENCHMARK("GrainExprParser construct", [] {
    GrainExprParser p;
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser parse string", [] {
    GrainExprParser p;
    p.parse("@t +=100");
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser parse @t 100", [] {
    GrainExprParser p;
    p.parse(l0.view());
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser parse @t abs(0.25*10.0)", [] {
    GrainExprParser p;
    p.parse(l1.view());
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser parse @t sin($pi/2)", [] {
    GrainExprParser p;
    p.parse(l2.view());
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser parse @t 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1", [] {
    GrainExprParser p;
    p.parse(l3.view());
    return 0;
})

NONIUS_BENCHMARK("GrainExprParser eval bc: 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1", [] {
    double res = 0;
    bc0.eval(res);
    return res != 0;
})

NONIUS_BENCHMARK("GrainExprParser eval opt bc: 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1", [] {
    double res = 0;
    bc1.eval(res);
    return res != 0;
})

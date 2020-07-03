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
#include "ceammc_atomlist.h"

#include <nonius/nonius.h++>
#include <random>

using namespace ceammc;

AtomList randomList(size_t n)
{
    std::uniform_real_distribution<t_float> gen(1, 100);
    AtomList a = AtomList::filled(0.f, n);
    std::generate(a.begin(), a.end(), std::rand);
    return a;
}

NONIUS_BENCHMARK("AtomList::sum", [] {
    return randomList(100).sum();
})

NONIUS_BENCHMARK("AtomList::reduce +", [] {
    return randomList(100).reduceFloat(0, [](t_float f0, t_float f1) { return f0 + f1; });
})

NONIUS_BENCHMARK("AtomList::product", [] {
    return randomList(100).product();
})

NONIUS_BENCHMARK("AtomList::reduce *", [] {
    return randomList(100).reduceFloat(1, [](t_float f0, t_float f1) { return f0 * f1; });
})

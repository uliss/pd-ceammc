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
#include "ceammc_canvas.h"
#include "ceammc_data.h"
#include "ceammc_pd.h"
#include "datatype_mlist.h"
#include "list/mod_list.h"

#include <nonius/nonius.h++>
#include <random>

using namespace ceammc;
using namespace ceammc::pd;

extern "C" void pd_init();
extern void setup_list_choice();
extern void setup_list_distribution();
extern void setup_list_each();
extern void setup_list_gen();
extern void setup_list_histogram();
extern void setup_list_length();
extern void setup_list_max();
extern void setup_list_mean();
extern void setup_list_mean();
extern void setup_list_min();
extern void setup_list_normalize();
extern void setup_list_pass_if();
extern void setup_list_product();
extern void setup_list_range();
extern void setup_list_reduce();
extern void setup_list_reverse();
extern void setup_list_shuffle();
extern void setup_list_sort();
extern void setup_list_sort_with();
extern void setup_list_stretch();
extern void setup_list_sum();
extern void setup_list_unique();

static bool init()
{
    pd_init();
    setup_list_choice();
    setup_list_distribution();
    setup_list_each();
    setup_list_gen();
    setup_list_histogram();
    setup_list_length();
    setup_list_max();
    setup_list_mean();
    setup_list_min();
    setup_list_normalize();
    setup_list_pass_if();
    setup_list_product();
    setup_list_range();
    setup_list_reduce();
    setup_list_reverse();
    setup_list_shuffle();
    setup_list_sort();
    setup_list_sort_with();
    setup_list_stretch();
    setup_list_sum();
    setup_list_unique();
    return true;
}

static auto cnv = PureData::instance().createTopCanvas("test_canvas");
static bool init_done = init();

static AtomList randomFloatList(size_t n, float from = 0, float to = 1000)
{
    AtomList res;
    res.reserve(n);

    std::default_random_engine engine;
    std::uniform_real_distribution<float> gen(from, to);

    for (size_t i = 0; i < n; i++)
        res.append(gen(engine));

    return res;
}

static AtomList randomIntList(size_t n, int from = 0, int to = 1000)
{
    AtomList res;
    res.reserve(n);

    std::default_random_engine engine;
    std::uniform_int_distribution<int> gen(from, to);

    for (size_t i = 0; i < n; i++)
        res.append(gen(engine));

    return res;
}

static Atom randomFloatMList(size_t n, float from = 0, float to = 1000)
{
    MListAtom res;
    res->reserve(n);

    std::default_random_engine engine;
    std::uniform_real_distribution<float> gen(from, to);

    for (size_t i = 0; i < n; i++)
        res->append(gen(engine));

    return res;
}

NONIUS_BENCHMARK("list.sum", [] {
    External t("list.sum");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.reduce +", [] {
    External t("list.reduce");
    External plus("+");
    t.connectTo(1, plus, 0);
    t.connectFrom(0, plus, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.product", [] {
    External t("list.product");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.reduce *", [] {
    External t("list.reduce");
    External mul("*");
    t.connectTo(1, mul, 0);
    t.connectFrom(0, mul, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.sort", [] {
    External t("list.sort");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.sort mlist", [] {
    External t("list.sort");
    t.sendList(randomFloatMList(100));
})

NONIUS_BENCHMARK("list.sort_with", [] {
    External t("list.sort_with");
    External cmp("<");
    t.connectTo(1, cmp, 0);
    t.connectFrom(0, cmp, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.each", [] {
    External t("list.each");
    External plus("+", { 1 });

    t.connectTo(1, plus, 0);
    t.connectFrom(0, plus, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.each (mlist)", [] {
    External t("list.each");
    External plus("+", { 1 });

    t.connectTo(1, plus, 0);
    t.connectFrom(0, plus, 1);

    t.sendList(randomFloatMList(100));
})

NONIUS_BENCHMARK("list.choice", [] {
    External t("list.choice");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.gen", [] {
    External t("list.gen", { 100 });
    External rnd("random", { 1, 1000 });

    t.connectTo(1, rnd, 0);
    t.connectFrom(0, rnd, 1);

    t.sendBang();
})

NONIUS_BENCHMARK("list.shuffle", [] {
    External t("list.shuffle");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.length", [] {
    External t("list.length");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list length", [] {
    External t("list length");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.mean", [] {
    External t("list.mean");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("mean: list.sum + list.size", [] {
    //    [t f           f]
    //    |              ^|
    //    [list.length] [list.sum]
    //    |             |.
    //    [swap          ]
    //    |   ^|.
    //    [/   ]

    External t("t", AtomList(gensym("l"), gensym("l")));
    External sum("list.sum");
    External len("list.length");
    External swap("swap");
    External div("/");

    t.connectTo(0, len, 0);
    t.connectTo(1, sum, 0);
    len.connectTo(0, swap, 0);
    sum.connectTo(0, swap, 1);
    swap.connectTo(0, div, 0);
    swap.connectTo(1, div, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.reverse", [] {
    External t("list.reverse");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.pass_if", [] {
    External t("list.pass_if");
    External cmp(">", { 3000 });
    t.connectTo(1, cmp, 0);
    t.connectFrom(0, cmp, 1);

    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.min", [] {
    External t("list.min");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.min mlist", [] {
    External t("list.min");
    t.sendList(randomFloatMList(100));
})

NONIUS_BENCHMARK("list.max", [] {
    External t("list.max");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.max mlist", [] {
    External t("list.max");
    t.sendList(randomFloatMList(100));
})

NONIUS_BENCHMARK("list.range", [] {
    External t("list.range");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.range mlist", [] {
    External t("list.range");
    t.sendList(randomFloatMList(100));
})

NONIUS_BENCHMARK("list.distribution", [] {
    External t("list.distribution");
    t.sendList(randomIntList(100, 1, 10));
})

NONIUS_BENCHMARK("list.normalize", [] {
    External t("list.normalize");
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.histogram", [] {
    External t("list.histogram", { 10 });
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.stretch", [] {
    External t("list.stretch", { 11 });
    t.sendList(randomFloatList(100));
})

NONIUS_BENCHMARK("list.unique", [] {
    External t("list.unique");
    t.sendList(randomIntList(100, 1, 10));
})

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
#include "ceammc_pd.h"

#include <nonius/nonius.h++>
#include <random>

#include "array/mod_array.h"

using namespace ceammc;
using namespace ceammc::pd;

extern void setup_array_fill();
extern void setup_array_mean();
extern void setup_array_sum();

class Initializer {
    Initializer()
        : canvas(PureData::instance().createTopCanvas("test_canvas"))
    {
        array = canvas->createArray("array1", 1000);
        setup_array_fill();
        setup_array_mean();
        setup_array_sum();
    }

    Initializer(const Initializer&);

public:
    CanvasPtr canvas;
    ArrayPtr array;

    static Initializer& instance()
    {
        static Initializer instance_;
        return instance_;
    }
};

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

NONIUS_BENCHMARK("...init...", [] {
    auto& instance = Initializer::instance();
})

NONIUS_BENCHMARK("array.fill (value)", [] {
    External t("array.fill", AtomList(gensym("array1")));
    t.sendMessage(gensym("fill"), AtomList(1.5f));
})

NONIUS_BENCHMARK("array.fill (values...)", [] {
    External t("array.fill", AtomList(gensym("array1")));
    t.sendMessage(gensym("fill"), AtomList(1.5f, 2.5));
})

NONIUS_BENCHMARK("array.fill (sin)", [] {
    External t("array.fill", AtomList(gensym("array1")));
    t.sendMessage(gensym("sin"), AtomList(40));
})

NONIUS_BENCHMARK("array.fill (uniform noise)", [] {
    External t("array.fill", AtomList(gensym("array1")));
    t.sendMessage(gensym("uniform"));
})

NONIUS_BENCHMARK("array.fill (gauss noise)", [] {
    External t("array.fill", AtomList(gensym("array1")));
    t.sendMessage(gensym("gauss"));
})

NONIUS_BENCHMARK("array.sum", [] {
    External t("array.sum", AtomList(gensym("array1")));
    t.sendBang();
})

NONIUS_BENCHMARK("array.mean", [] {
    External t("array.mean", AtomList(gensym("array1")));
    t.sendBang();
})

//NONIUS_BENCHMARK("mean: list.sum + list.size", [] {
//    //    [t f           f]
//    //    |              ^|
//    //    [list.length] [list.sum]
//    //    |             |.
//    //    [swap          ]
//    //    |   ^|.
//    //    [/   ]

//    External t("t", AtomList(gensym("l"), gensym("l")));
//    External sum("list.sum");
//    External len("list.length");
//    External swap("swap");
//    External div("/");

//    t.connectTo(0, len, 0);
//    t.connectTo(1, sum, 0);
//    len.connectTo(0, swap, 0);
//    sum.connectTo(0, swap, 1);
//    swap.connectTo(0, div, 0);
//    swap.connectTo(1, div, 1);

//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.pass_if", [] {
//    External t("list.pass_if");
//    External cmp(">", { 3000 });
//    t.connectTo(1, cmp, 0);
//    t.connectFrom(0, cmp, 1);

//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.min", [] {
//    External t("list.min");
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.min mlist", [] {
//    External t("list.min");

//    DataPtr dlst(randomFloatMList(100));
//    t.sendList(dlst.asAtom());
//})

//NONIUS_BENCHMARK("list.max", [] {
//    External t("list.max");
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.max mlist", [] {
//    External t("list.max");

//    DataPtr dlst(randomFloatMList(100));
//    t.sendList(dlst.asAtom());
//})

//NONIUS_BENCHMARK("list.range", [] {
//    External t("list.range");
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.range mlist", [] {
//    External t("list.range");

//    DataPtr dlst(randomFloatMList(100));
//    t.sendList(dlst.asAtom());
//})

//NONIUS_BENCHMARK("list.distribution", [] {
//    External t("list.distribution");
//    t.sendList(randomIntList(100, 1, 10));
//})

//NONIUS_BENCHMARK("list.normalize", [] {
//    External t("list.normalize");
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.histogram", [] {
//    External t("list.histogram", { 10 });
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.stretch", [] {
//    External t("list.stretch", { 11 });
//    t.sendList(randomFloatList(100));
//})

//NONIUS_BENCHMARK("list.unique", [] {
//    External t("list.unique");
//    t.sendList(randomIntList(100, 1, 10));
//})

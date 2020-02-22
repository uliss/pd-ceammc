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
#include "ceammc_atomlist.h"
#include "ceammc_canvas.h"
#include "ceammc_data.h"
#include "ceammc_dataatomlist.h"
#include "ceammc_pd.h"

#include <nonius/nonius.h++>
#include <random>

#include "data/mod_data.h"
#include "datatype_tree.h"
#include "list/mod_list.h"

using namespace ceammc;
using namespace ceammc::pd;

extern "C" void pd_init();

static bool init()
{
    pd_init();
    ceammc_data_setup();
    ceammc_list_setup();
    return true;
}

static auto cnv = PureData::instance().createTopCanvas("test_canvas");
static bool init_done = init();

static t_symbol* SYM_ADD = gensym("add");
static t_symbol* SYM_AT = gensym("at");
static t_symbol* SYM_CLEAR = gensym("clear");

NONIUS_BENCHMARK("tree [bang(", [] {
    External t0("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));
    External t1("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));

    t0.connectTo(0, t1, 0);
    t0.sendBang();
})

NONIUS_BENCHMARK("list.at", [] {
    External tree("data.list", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    External tree_at("list.at", { 8, 1, -1 });

    tree.connectTo(0, tree_at, 0);
    tree.sendBang();
})

NONIUS_BENCHMARK("tree [at(", [] {
    External tree("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));
    tree.sendMessage(SYM_AT, { 8, 1, -1 });
})

NONIUS_BENCHMARK("tree [clear(", [] {
    External t("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));
    t.sendMessage(SYM_CLEAR);
})

NONIUS_BENCHMARK("tree.at", [] {
    External tree("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));
    External tree_at("tree.at", { 8, 1, -1 });

    tree.connectTo(0, tree_at, 0);
    tree.sendBang();
})

NONIUS_BENCHMARK("tree.add", [] {
    External tree("data.tree", Atom(gensym(R"([1,2,3,4,5,6,7,8,9,10])")));
    tree.sendMessage(SYM_ADD, AtomList({ 11 }));
    tree.sendMessage(SYM_ADD, AtomList(SYM_ADD));
})

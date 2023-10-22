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
#include "metro_random.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <ctime>
#include <random>

MetroRandom::MetroRandom(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        bangTo(0);
        clock_.delay(gen());
    })
    , min_(nullptr)
    , max_(nullptr)
    , seed_(nullptr)
{
    min_ = new IntProperty("@min", 100);
    min_->setArgIndex(0);
    min_->checkMin(1);
    min_->setUnitsMs();
    addProperty(min_);

    max_ = new IntProperty("@max", 200);
    max_->setArgIndex(1);
    max_->checkMin(1);
    max_->setUnitsMs();
    addProperty(max_);

    seed_ = new random::SeedProperty(gen_);
    addProperty(seed_);

    createInlet(); // set min or min/max
    createInlet(); // set max
    createOutlet();
}

void MetroRandom::onFloat(t_float f)
{
    if (f > 0)
        clock_.exec();
    else
        clock_.unset();
}

void MetroRandom::onInlet(size_t n, const AtomListView& lst)
{
    switch (n) {
    case 1:
        if (lst.isFloat())
            min_->setValue(lst[0].asFloat());
        else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
            min_->setValue(lst[0].asFloat());
            max_->setValue(lst[1].asFloat());
        } else {
            OBJ_ERR << "MIN MAX interval range expected: " << lst;
        }
        break;
    case 2:
        max_->set(lst);
        break;
    }
}

double MetroRandom::gen()
{
    int a = min_->value();
    int b = max_->value();

    auto minmax = std::minmax(a, b);
    return gen_.gen_uniform_int(minmax.first, minmax.second);
}

void setup_metro_random()
{
    ObjectFactory<MetroRandom> obj("metro.random");

    obj.setDescription("metro random bang with specified range");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "random" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 9);

    obj.setXletsInfo(
        { "float: 1 - start metronome, 0 - stop",
            "float: MIN\n"
            "list:  MIN MAX",
            "float: MAX" },
        { "bang" });
}

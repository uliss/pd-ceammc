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
    , clock_(this, &MetroRandom::tick)
    , min_(nullptr)
    , max_(nullptr)
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

    createInlet();
    createOutlet();
}

void MetroRandom::onFloat(t_float f)
{
    if (f > 0)
        tick();
    else
        clock_.unset();
}

void MetroRandom::onInlet(size_t n, const AtomListView& lst)
{
    if (lst.isFloat())
        min_->setValue(lst[0].asFloat());
    else if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
        min_->setValue(lst[0].asFloat());
        max_->setValue(lst[1].asFloat());
    } else {
        OBJ_ERR << "MIN MAX interval range expected: " << lst;
    }
}

void MetroRandom::tick()
{
    clock_.delay(gen());
    bangTo(0);
}

double MetroRandom::gen() const
{
    static std::mt19937 rnd_gen(time(0));

    int a = min_->value();
    int b = max_->value();

    auto minmax = std::minmax(a, b);
    std::uniform_int_distribution<> uid(minmax.first, minmax.second);
    return uid(rnd_gen);
}

void setup_metro_random()
{
    ObjectFactory<MetroRandom> obj("metro.random");

    obj.setDescription("metro random bang with specified range");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "random" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 9);

    obj.setXletsInfo({ "float: 1 - start metronome, 0 - stop", "float: f - set @min range\n"
                                                               "list:  min max - set \\[@min @max\\] range" },
        { "bang" });
}

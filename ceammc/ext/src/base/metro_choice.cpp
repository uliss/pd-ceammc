/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "metro_choice.h"
#include "ceammc_factory.h"

#include <random>

MetroChoice::MetroChoice(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        auto N = timeset_->value().size();
        if (N == 0) {
            OBJ_ERR << "empty timeset";
            return;
        }

        auto idx = gen_.gen_uniform_uint(0, N - 1);
        auto atom = timeset_->value()[idx];
        auto choice = atom.asFloat(-1);
        if (choice <= 0) {
            OBJ_ERR << "invalid delay time value, expected positive float, got: " << atom;
            return;
        }

        clock_.delay(choice);
        bangTo(0);
    })
    , timeset_(nullptr)
{
    createInlet(); // timeset
    createOutlet();

    timeset_ = new ListProperty("@timeset");
    timeset_->setArgIndex(0);
    timeset_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.allOf([](const Atom& a) { return a.isFloat() && a.asT<t_float>() > 0; });
    },
        "list of positive floats expected");
    timeset_->setUnitsMs();
    addProperty(timeset_);

    addProperty(new random::SeedProperty(gen_));
}

void MetroChoice::onFloat(t_float f)
{
    if (f > 0)
        clock_.exec();
    else
        clock_.unset();
}

void MetroChoice::onInlet(size_t n, const AtomListView& lv)
{
    timeset_->set(lv);
}

void setup_metro_choice()
{
    ObjectFactory<MetroChoice> obj("metro.choice");

    obj.setDescription("metro with random choice from timeset");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "random", "choice", "set", "uniform" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 9);

    obj.setXletsInfo(
        { "float: 1 - start metronome, 0 - stop",
            "list: set timeset" },
        { "bang" });
}

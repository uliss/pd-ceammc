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
        auto N = pattern_->value().size();
        if (N == 0) {
            OBJ_ERR << "empty time choice set";
            return;
        }

        auto idx = gen_.gen_uniform_uint(0, N - 1);
        auto atom = pattern_->value()[idx];
        auto choice = atom.asFloat(-1);
        if (choice <= 0) {
            OBJ_ERR << "invalid delay time value, expected positive float, got: " << atom;
            return;
        }

        clock_.delay(choice);
        bangTo(0);
    })
    , pattern_(nullptr)
{
    createInlet(); // choice set
    createOutlet();

    pattern_ = new ListProperty("@set");
    pattern_->setArgIndex(0);
    pattern_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.allOf([](const Atom& a) { return a.isFloat() && a.asT<t_float>() > 0; });
    },
        "list of positive floats expected");
    pattern_->setUnitsMs();
    addProperty(pattern_);

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
    pattern_->set(lv);
}

void setup_metro_choice()
{
    ObjectFactory<MetroChoice> obj("metro.choice");
}

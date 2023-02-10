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
#include "metro_wchoice.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "lex/parser_units.h"

#include <boost/container/small_vector.hpp>
#include <random>

MetroWChoice::MetroWChoice(const PdArgs& args)
    : BaseObject(args)
    , clock_([this]() {
        auto NT = timeset_->value().size();
        auto NW = weights_->value().size();

        if (NT == 0 || NW == 0) {
            OBJ_ERR << "empty timeset or weights";
            return;
        }

        if (NW != NT) {
            weights_->value().resizePad(NT, Atom(0.));
            updateDistribution(weights_->value());
        }

        auto idx = dist_(gen_.get());
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
    , weights_(nullptr)
{
    createInlet(); // timeset
    createInlet(); // weights
    createOutlet();

    timeset_ = new ListProperty("@t");
    timeset_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.allOf([](const Atom& a) { return a.isFloat() && a.asT<t_float>() > 0; });
    },
        "list of positive floats expected");
    timeset_->setUnitsMs();
    addProperty(timeset_);

    weights_ = new ListProperty("@w");
    weights_->setListCheckFn([this](const AtomListView& lv) -> bool {
        return updateDistribution(lv);
    },
        "list of positive floats expected");
    addProperty(weights_);

    {
        auto args = createCbListProperty(
            "@init", []() { return AtomList(); },
            [this](const AtomListView& lv) -> bool {
                std::vector<AtomList> out(2, {});
                list::deinterleavePadWith(lv, Atom(0.0), out);
                return timeset_->set(out[0]) && weights_->set(out[1]);
            });

        args->setArgIndex(0);
        args->setInitOnly();
    }

    {
        createCbListProperty("@calc_w", [this]() -> AtomList {
            AtomList res;
            res.reserve(weights_->value().size());

            for (auto x : dist_.probabilities())
                res.append(x);

            return res;
        });
    }

    addProperty(new random::SeedProperty(gen_));
}

void MetroWChoice::onFloat(t_float f)
{
    if (f > 0)
        clock_.exec();
    else
        clock_.unset();
}

void MetroWChoice::onInlet(size_t n, const AtomListView& lv)
{
    switch (n) {
    case 1:
        timeset_->set(lv);
        break;
    case 2:
        weights_->set(lv);
        break;
    }
}

bool MetroWChoice::updateDistribution(const AtomListView& lv)
{
    parser::UnitsFullMatch p;
    boost::container::small_vector<float, 16> w;
    w.reserve(lv.size());

    for (auto& a : lv) {
        if (!p.parseAs(a, parser::AtomType::TYPE_PERCENT)) {
            OBJ_ERR << "invalid atom, float or percent value expected, got: " << a;
            return false;
        }

        if (p.value() < 0) {
            OBJ_ERR << "non-negative weight required, got: " << p.value();
            return false;
        }

        w.push_back(p.value());
    }

    dist_.param({ w.begin(), w.end() });
    return true;
}

void setup_metro_wchoice()
{
    ObjectFactory<MetroWChoice> obj("metro.wchoice");

    obj.setDescription("metro with weighted random choice from timeset");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "metro", "random", "choice", "set", "uniform" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 9);

    obj.setXletsInfo(
        { "float: 1 - start metronome, 0 - stop",
            "list: set timeset",
            "list: set weights" },
        { "bang" });
}

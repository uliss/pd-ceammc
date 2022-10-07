/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "flt_a_weight.h"
#include "ceammc_dsp.h"
#include "ceammc_factory.h"

#include <cmath>

static const dsp::TableFunction<t_float, 1024> tab_a_weight(dsp::curves::a_weight, 10, 20000);
static const auto RA_1000 = 20 * std::log10(tab_a_weight.at(1000));

FltAWeight::FltAWeight(const PdArgs& args)
    : BaseObject(args)
    , db_(nullptr)
{
    db_ = new FlagProperty("@db");
    addProperty(db_);

    createOutlet();
}

void FltAWeight::onFloat(t_float f)
{
    t_float res;
    if (!getValue(f, res))
        return;

    floatTo(0, res);
}

void FltAWeight::onList(const AtomListView& lv)
{
    AtomList res;
    res.reserve(lv.size());

    for (auto& a : lv) {
        t_float val = 0;
        if (a.isFloat() && getValue(a.asT<t_float>(), val)) {
            res.append(val);
        } else
            res.append(a);
    }

    listTo(0, res);
}

bool FltAWeight::getValue(t_float freqHz, t_float& res) const
{
    if (freqHz < tab_a_weight.min() || freqHz > tab_a_weight.max())
        return false;

    const auto val = tab_a_weight.at(freqHz);

    if (db_->value())
        res = 20 * std::log10(val) - 2.0;
    else
        res = val;

    return true;
}

void setup_flt_a_weight()
{
    ObjectFactory<FltAWeight> obj("flt.a-weight");
}

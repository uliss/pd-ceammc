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
    if (f < tab_a_weight.min() || f > tab_a_weight.max())
        return;

    if (db_->value())
        floatTo(0, 20 * std::log10(tab_a_weight.at(f)) - 2.0);
    else
        floatTo(0, tab_a_weight.at(f));
}

void setup_flt_a_weight()
{
    ObjectFactory<FltAWeight> obj("flt.a-weight");
}

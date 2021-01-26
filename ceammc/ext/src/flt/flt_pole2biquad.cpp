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
#include "flt_pole2biquad.h"
#include "ceammc_factory.h"

FltPole2Biquad::FltPole2Biquad(const PdArgs& args)
    : BaseObject(args)
    , freq_(nullptr)
    , herz_(nullptr)
    , coeffs_ { 0., 0., 0., 0., 0. }
{
    freq_ = new FloatProperty("@freq", 0);
    freq_->setArgIndex(0);
    addProperty(freq_);

    herz_ = new BoolProperty("@hz", true);
    addProperty(herz_);

    createOutlet();
}

void FltPole2Biquad::onFloat(t_float freq)
{
    if (herz_->value()) {
        const double Fs = sys_getsr();
        freq /= Fs;
    }

    const auto a1 = exp(-2.0 * M_PI * freq);
    const auto b0 = 1.0 - a1;
    coeffs_[0] = b0;
    coeffs_[3] = a1;

    listTo(0, AtomListView(coeffs_, 5));
}

void setup_flt_pole2biquad()
{
    ObjectFactory<FltPole2Biquad> obj("flt.pole2biquad");
    obj.addAlias("pole->biquad");
}

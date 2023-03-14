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
#include "color_mix.h"
#include "ceammc_factory.h"

ColorMix::ColorMix(const PdArgs& args)
    : BaseObject(args)
    , c0_(nullptr)
    , c1_(nullptr)
{
    c0_ = new DataPropertyT<DataTypeColor>("@c0", DataTypeColor());
    c0_->setArgIndex(0);
    addProperty(c0_);

    c1_ = new DataPropertyT<DataTypeColor>("@c1", DataTypeColor());
    c1_->setArgIndex(1);
    addProperty(c1_);

    createInlet();
    createInlet();
    createOutlet();
}

void ColorMix::onFloat(t_float f)
{
    auto new_color = c0_->value().mix(c1_->value(), clip01<float>(f));
    atomTo(0, ColorAtom(new_color));
}

void ColorMix::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        c0_->set(lv);
    else if (n == 2)
        c1_->set(lv);
}

void setup_color_mix()
{
    ObjectFactory<ColorMix> obj("color.mix");
}

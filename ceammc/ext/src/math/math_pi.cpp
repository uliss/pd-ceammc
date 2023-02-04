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
#include "math_pi.h"
#include "ceammc_factory.h"

const static t_float PI = std::acos(t_float(-1));

MathPi::MathPi(const PdArgs& args)
    : BaseObject(args)
    , k_(nullptr)
{
    k_ = new FloatProperty("@k", 1);
    k_->setArgIndex(0);
    k_->setInitOnly();
    addProperty(k_);

    createOutlet();
}

void MathPi::onBang()
{
    floatTo(0, PI * k_->value());
}

MathPiTilde::MathPiTilde(const PdArgs& args)
    : SoundExternal(args)
    , k_(nullptr)
{
    k_ = new FloatProperty("@k", 1);
    k_->setArgIndex(0);
    k_->setInitOnly();
    addProperty(k_);

    createSignalOutlet();
}

void MathPiTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto k = PI * k_->value();
    auto const BS = blockSize();
    for (size_t i = 0; i < BS; i += 8) {
        out[0][0 + i] = k;
        out[0][1 + i] = k;
        out[0][2 + i] = k;
        out[0][3 + i] = k;
        out[0][4 + i] = k;
        out[0][5 + i] = k;
        out[0][6 + i] = k;
        out[0][7 + i] = k;
    }
}

void setup_math_pi()
{
    ObjectFactory<MathPi> obj("math.pi");
    obj.addAlias("pi");

    SoundExternalFactory<MathPiTilde> obj_tilde("math.pi~", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_DEFAULT_INLET);
    obj_tilde.addAlias("pi~");
}

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
#include "conv_samp2ms.h"
#include "ceammc_factory.h"

SampleToMs::SampleToMs(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void SampleToMs::onFloat(t_float v)
{
    floatTo(0, (v * 1000) / sys_getsr());
}

void SampleToMs::onList(const AtomList& lst)
{
    listTo(0, (lst * 1000) / sys_getsr());
}

SampleToMsTilde::SampleToMsTilde(const PdArgs& a)
    : SoundExternal(a)
{
    createSignalOutlet();
}

void SampleToMsTilde::processBlock(const t_sample** in, t_sample** out)
{
    const t_sample k = t_sample(1000) / samplerate();
    const auto BS = blockSize();
    for (size_t i = 0; i < BS; i += 8) {
        out[0][i + 0] = k * in[0][i + 0];
        out[0][i + 1] = k * in[0][i + 1];
        out[0][i + 2] = k * in[0][i + 2];
        out[0][i + 3] = k * in[0][i + 3];
        out[0][i + 4] = k * in[0][i + 4];
        out[0][i + 5] = k * in[0][i + 5];
        out[0][i + 6] = k * in[0][i + 6];
        out[0][i + 7] = k * in[0][i + 7];
    }
}

void setup_conv_samp2ms()
{
    ObjectFactory<SampleToMs> obj("conv.samp2ms");
    obj.addAlias("samp->ms");

    SoundExternalFactory<SampleToMsTilde> obj1("conv.samp2ms~");
    obj1.addAlias("samp->ms~");
}

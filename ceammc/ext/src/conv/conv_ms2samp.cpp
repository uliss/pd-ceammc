/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "conv_ms2samp.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

MsToSample::MsToSample(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void MsToSample::onFloat(t_float v)
{
    floatTo(0, v * sys_getsr() * 0.001);
}

void MsToSample::onList(const AtomListView& lv)
{
    SmallAtomList res;
    res.reserve(lv.size());
    for (auto& a : lv)
        res.push_back(a * sys_getsr() * 0.001);

    listTo(0, res.view());
}

MsToSampleTilde::MsToSampleTilde(const PdArgs& a)
    : SoundExternal(a)
{
    createSignalOutlet();
}

void MsToSampleTilde::processBlock(const t_sample** in, t_sample** out)
{
    const t_sample k = samplerate() * 0.001;
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

void setup_conv_ms2samp()
{
    ObjectFactory<MsToSample> obj("conv.ms2samp");
    obj.addAlias("ms->samp");

    SoundExternalFactory<MsToSampleTilde> obj1("conv.ms2samp~");
    obj1.addAlias("ms->samp~");

    obj.setDescription("convert time in milliseconds to number of samples according to current samplerate");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "samp", "ms"});
}

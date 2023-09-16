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
#include "conv_sig2float.h"
#include "ceammc_factory.h"

#include <algorithm>

ConvSig2Float::ConvSig2Float(const PdArgs& args)
    : SoundExternal(args)
    , clock_([this]() { floatTo(0, value_); })
    , interval_ms_(nullptr)
    , block_offset_(nullptr)
    , period_(0)
    , value_(0)
{
    interval_ms_ = new FloatProperty("@interval", 50);
    interval_ms_->checkMinEq(-1);
    interval_ms_->setArgIndex(0);
    interval_ms_->setUnitsMs();
    addProperty(interval_ms_);

    block_offset_ = new IntProperty("@offset", 0);
    block_offset_->checkClosedRange(0, 8192);
    block_offset_->setUnitsSamp();
    addProperty(block_offset_);

    createInlet();
    createOutlet();
}

void ConvSig2Float::onBang()
{
    clock_.exec();
}

void ConvSig2Float::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    clock_.exec();
}

void ConvSig2Float::onInlet(size_t n, const AtomListView& lv)
{
    interval_ms_->setList(lv);
}

void ConvSig2Float::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    const size_t bs = blockSize();
    const auto sr = samplerate();

    period_ = size_t(((interval_ms_->value() / 1000) * sr) / bs) * bs;
}

void ConvSig2Float::processBlock(const t_sample** in, t_sample**)
{
    const size_t bs = blockSize();
    const size_t offset = block_offset_->value();
    const size_t idx = std::min<size_t>(bs - 1, offset);

    // sample always
    value_ = in[0][idx];

    // output at specified time
    if (sample_counter_ < period_) {
        sample_counter_ += bs;
    } else {
        sample_counter_ = 0;

        if (interval_ms_->value() >= 0)
            clock_.delay(0);
    }
}

void setup_conv_sig2float()
{
    SoundExternalFactory<ConvSig2Float> obj("conv.sig2float~");
    obj.addAlias("sig->float~");
    obj.addAlias("s->f~");
    obj.setXletsInfo({ "signal: input", "float: set calc interval" }, { "float: sample value" });

    obj.useClick();

    obj.setDescription("convert signal to floats");
    obj.setCategory("conv");
    obj.setKeywords({"conv", "signal", "float", "snapshot"});
}

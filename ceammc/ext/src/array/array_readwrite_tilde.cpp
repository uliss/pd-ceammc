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
#include "array_readwrite_tilde.h"
#include "ceammc_factory.h"

#include <cmath>

ArrayReadwriteTilde::ArrayReadwriteTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , mix_(nullptr)
{
    createSignalInlet();
    createSignalInlet();

    createSignalOutlet();

    mix_ = new FloatProperty("@mix", 0);
    mix_->checkClosedRange(0, 1);
    addProperty(mix_);
}

void ArrayReadwriteTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();

    if (!array_.isValid()) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;

        return;
    }

    const std::int64_t ASIZE = array_.size();

    auto read_idx = in[0];
    auto write_sig = in[1];
    auto write_idx = in[2];

    t_sample read_buf[BS];

    auto as_int = [](t_sample s) { return static_cast<std::uint64_t>(s); };

    // read
    for (size_t i = 0; i < BS; i++) {
        t_sample ridx = read_idx[i];
        if (ridx < 0)
            read_buf[i] = 0;
        else if (as_int(ridx) < ASIZE - 2)
            read_buf[i] = readUnsafe3(ridx);
        else if (as_int(ridx) < ASIZE)
            read_buf[i] = readSafe3(ridx);
        else
            read_buf[i] = 0;
    }

    auto mix = mix_->value();
    // write
    for (size_t i = 0; i < BS; i++) {
        std::int64_t widx = std::round(write_idx[i]);
        if (widx < 0 || widx >= ASIZE)
            continue;

        array_.at(widx) = ((1 - mix) * write_sig[i]) + (mix * array_.at(widx));
    }

    // output
    for (size_t i = 0; i < BS; i++)
        out[0][i] = read_buf[i];
}

void ArrayReadwriteTilde::m_redraw(t_symbol* s, const AtomListView& lv)
{
    array_.redraw();
}

void setup_array_readwrite_tilde()
{
    SoundExternalFactory<ArrayReadwriteTilde> obj("array.readwrite~");
    obj.addAlias("array.rw~");
    obj.addMethod("redraw", &ArrayReadwriteTilde::m_redraw);
}

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
#include "base_merge_tilde.h"
#include "ceammc_factory.h"

constexpr int NMIN = 2;
constexpr int NMAX = 64;
constexpr int NDEF = 2;

BaseMergeTilde::BaseMergeTilde(const PdArgs& args)
    : SoundExternal(args)
{
    n_ = new IntProperty("@n", NDEF, PropValueAccess::INITONLY);
    n_->checkClosedRange(NMIN, NMAX);
    n_->setArgIndex(0);
    addProperty(n_);

    createSignalOutlet();
}

void BaseMergeTilde::initDone()
{
    for (int i = 1; i < n_->value(); i++)
        createSignalInlet();
}

void BaseMergeTilde::processBlock(const t_sample** in, t_sample** out)
{
    auto BS = blockSize();
    const size_t N = n_->value();

    for (size_t i = 0; i < BS; i++) {
        auto x = in[0][i];
        for (size_t k = 1; k < N; k++)
            x += in[k][i];

        out[0][i] = x;
    }
}

void setup_base_merge_tilde()
{
    SoundExternalFactory<BaseMergeTilde> obj("merge~");
}

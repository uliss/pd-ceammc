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
#include "conv_lin2lin_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

Lin2LinTilde::Lin2LinTilde(const PdArgs& args)
    : BaseRangeConverterTilde(args, -1, 1, -1, 1)
{
    createSignalOutlet();
}

void Lin2LinTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    auto x0 = in_from();
    auto x1 = in_to();
    auto y0 = out_from();
    auto y1 = out_to();

    auto type = clipType();

    for (size_t i = 0; i < BS; i++) {
        auto f = in[0][i];
        if (doClip(f, type)) {
            out[0][i] = f;
            continue;
        }

        out[0][i] = convert::lin2lin(f, x0, x1, y0, y1);
    }
}

void setup_conv_lin2lin_tilde()
{
    SoundExternalFactory<Lin2LinTilde> obj("conv.lin2lin~");
    obj.addAlias("lin->lin~");
    obj.addAlias("l->l~");
}

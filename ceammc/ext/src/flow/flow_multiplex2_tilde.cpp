/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "flow_multiplex2_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

static PdArgs stereoArgs(const PdArgs& args)
{
    PdArgs res(args);
    res.flags = MULTIPLEX_STEREO;
    return res;
}

Multiplex2Tilde::Multiplex2Tilde(const PdArgs& args)
    : MultiplexTilde(stereoArgs(args))
{
}

void Multiplex2Tilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v0 = 0;
        t_sample v1 = 0;

        for (size_t j = 0; j < N; j++) {
            auto k = gain_[j]();
            v0 += k * in[2 * j][i];
            v1 += k * in[2 * j + 1][i];
        }

        out[0][i] = v0;
        out[1][i] = v1;
    }
}

void setup_flow_multiplex2_tilde()
{
    SoundExternalFactory<Multiplex2Tilde> obj("flow.multiplex2~");
    obj.addAlias("flow.mux2~");
    obj.addAlias("mux2~");
}

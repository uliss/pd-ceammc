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
#include "xfade2_tilde.h"
#include "ceammc_factory.h"

static PdArgs stereoArgs(const PdArgs& args)
{
    PdArgs res(args);
    res.flags = XFADE_STEREO;
    return res;
}

XFade2Tilde::XFade2Tilde(const PdArgs& args)
    : XFadeTilde(stereoArgs(args))
{
}

void XFade2Tilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v0 = 0;
        t_sample v1 = 0;
        for (size_t j = 0; j < N; j++) {
            auto k = gain_[j]();

            t_sample s0 = k * in[2 * j][i];
            t_sample s1 = k * in[2 * j + 1][i];
            v0 += s0;
            v1 += s1;
        }

        out[0][i] = v0;
        out[1][i] = v1;
    }
}

void setup_base_xfade2_tilde()
{
    SoundExternalFactory<XFade2Tilde> obj("xfade2~");

    obj.setDescription("multi stereo-signal crossfade");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"crossfade", "xfade"});
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}

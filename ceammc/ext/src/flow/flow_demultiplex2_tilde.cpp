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
#include "flow_demultiplex2_tilde.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

static char INFO_OUT[DEMUX_MAX_OUTLETS][8] = {};

static PdArgs stereoArgs(const PdArgs& args)
{
    PdArgs res(args);
    res.flags = DEMULTIPLEX_STEREO;
    return res;
}

Demultiplex2Tilde::Demultiplex2Tilde(const PdArgs& args)
    : DemultiplexTilde(stereoArgs(args))
{
}

void Demultiplex2Tilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v0 = in[0][i];
        t_sample v1 = in[1][i];

        for (size_t j = 0; j < N; j++) {
            auto k = gain_[j]();
            out[2 * j][i] = k * v0;
            out[2 * j + 1][i] = k * v1;
        }
    }
}

const char* Demultiplex2Tilde::annotateOutlet(size_t n) const
{
    if (n < DEMUX_MAX_OUTLETS)
        return INFO_OUT[n];
    else
        return "?";
}

void setup_flow_demultiplex2_tilde()
{
    for (int i = 0; i < DEMUX_MAX_OUTLETS; i++)
        fmt::format_to(INFO_OUT[i], "{}\\[{}\\]\0", ((i & 1) ? 'R' : 'L'), i / 2);

    SoundExternalFactory<Demultiplex2Tilde> obj("flow.demultiplex2~");
    obj.addAlias("flow.demux2~");
    obj.addAlias("demux2~");

    obj.setDescription("audio stream stereo demultiplexer");
    obj.setCategory("flow");
    obj.setKeywords({ "flow", "demultiplex" });
    obj.setXletsInfo({ "signal: L", "signal: R", "int: output select" }, {});
}

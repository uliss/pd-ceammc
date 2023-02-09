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
#include "math_cmul_tilde.h"
#include "ceammc_factory.h"

MathComplexMulTilde::MathComplexMulTilde(const PdArgs& args)
    : SoundExternal(args)
{
    createSignalInlet();
    createSignalInlet();
    createSignalInlet();

    createSignalOutlet();
    createSignalOutlet();
}

void MathComplexMulTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        auto a = in[0][i];
        auto b = in[1][i];
        auto c = in[2][i];
        auto d = in[3][i];

        auto x = a * c - b * d;
        auto y = a * d + b * c;

        out[0][i] = x;
        out[1][i] = y;
    }
}

void setup_math_cmul_tilde()
{
    SoundExternalFactory<MathComplexMulTilde> obj("math.cmul~");
    obj.addAlias("cmul~");

    obj.setDescription("multiplication of complex signals");
    obj.setCategory("math");
}

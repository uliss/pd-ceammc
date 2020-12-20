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
#include "math_cexp_tilde.h"
#include "ceammc_factory.h"

#include <cmath>
#include <complex>

MathComplexExpTilde::MathComplexExpTilde(const PdArgs& args)
    : SoundExternal(args)
{
    createSignalInlet();
    createSignalOutlet();
    createSignalOutlet();
}

void MathComplexExpTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        auto v = std::exp(std::complex<t_sample>(in[0][i], in[1][i]));

        out[0][i] = v.real();
        out[1][i] = v.imag();
    }
}

void setup_math_cexp_tilde()
{
    SoundExternalFactory<MathComplexExpTilde> obj("math.cexp~");
    obj.addAlias("cexp~");
}

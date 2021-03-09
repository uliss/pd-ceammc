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
#include "flt_hpf2biquad.h"
#include "ceammc_factory.h"
#include "ceammc_filter.h"

FltHpf2Biquad::FltHpf2Biquad(const PdArgs& args)
    : FltCalcBiquad(args, { 1000, 20, sys_getsr() / 2, flt::m_pi / 2, 0.001, flt::m_pi })
{
}

void FltHpf2Biquad::calc()
{
    calc_hpf();
}

void setup_flt_hpf2biquad()
{
    ObjectFactory<FltHpf2Biquad> obj("flt.c_hpf");
    obj.addAlias("hpf->biquad");
    obj.addMethod("bw", &FltHpf2Biquad::m_bandwidth);

    obj.setXletsInfo({ "float: freq cutoff in herz or radians" },
        { "list: biquad coeffs: b0 b1 b2 a1 a2" });
}

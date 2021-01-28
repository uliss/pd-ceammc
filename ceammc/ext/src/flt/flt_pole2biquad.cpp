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
#include "flt_pole2biquad.h"
#include "ceammc_factory.h"
#include "flt_common.h"

static t_symbol* SYM_HPF;
static t_symbol* SYM_LPF;

FltPole2Biquad::FltPole2Biquad(const PdArgs& args)
    : FltCalcBiquad(args, { 1000, 0, sys_getsr() / 2, flt::m_pi / 2, 0, flt::m_pi })
    , mode_(nullptr)
{
    q_->setInternal();

    mode_ = new SymbolEnumProperty("@mode", { SYM_LPF, SYM_HPF });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@lpf", mode_, SYM_LPF));
    addProperty(new SymbolEnumAlias("@hpf", mode_, SYM_HPF));
}

void FltPole2Biquad::calc()
{
    calc_onepole(mode_->value() == SYM_HPF);
}

void setup_flt_pole2biquad()
{
    SYM_LPF = gensym("lpf");
    SYM_HPF = gensym("hpf");

    ObjectFactory<FltPole2Biquad> obj("flt.c_pole");
    obj.addAlias("pole->biquad");

     obj.setXletsInfo({ "float: freq cutoff" }, { "list: biquad coeffs: b0 b1 b2 a1 a2" });
}

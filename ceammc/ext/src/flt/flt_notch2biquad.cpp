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
#include "flt_notch2biquad.h"
#include "ceammc_factory.h"
#include "flt_common.h"

FltNotch2Biquad::FltNotch2Biquad(const PdArgs& args)
    : FltCalcBiquad(args, { 1000, 20, sys_getsr() / 2, flt::m_pi / 2, 0, flt::m_pi })
{
}

void FltNotch2Biquad::calc()
{
    calc_notch();
}

void setup_flt_c_notch()
{
    ObjectFactory<FltNotch2Biquad> obj("flt.c_notch");
    obj.addAlias("notch->biquad");
}

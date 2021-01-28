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
#include "flt_apf2biquad.h"
#include "ceammc_factory.h"
#include "flt_common.h"

FltApf2Biquad::FltApf2Biquad(const PdArgs& args)
    : FltCalcBiquad(args, { 0, 0, sys_getsr() / 2, 0, 0, flt::m_pi })
{
}

void FltApf2Biquad::calc()
{
    calc_allpass();
}

void setup_flt_apf2biquad()
{
    ObjectFactory<FltApf2Biquad> obj("flt.c_apf");
    obj.addAlias("apf->biquad");
}

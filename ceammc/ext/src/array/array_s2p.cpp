/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "array_s2p.h"

ArraySampleToPhase::ArraySampleToPhase(const PdArgs& a)
    : ArrayBase(a)
    , sr_(0)
{
    createOutlet();
}

void ArraySampleToPhase::onFloat(t_float sample)
{
    floatTo(0, sample / arraySize());
}

void ArraySampleToPhase::onList(const AtomList& lst)
{
    listTo(0, lst.filtered(isFloat) * arraySize());
}

extern "C" void setup_array0x2es2p()
{
    ObjectFactory<ArraySampleToPhase> obj("array.s2p");
    obj.addAlias("array.s->p");
    obj.addAlias("array.sample->phase");
}

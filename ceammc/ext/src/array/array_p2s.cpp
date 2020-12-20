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
#include "array_p2s.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

ArrayPhaseToSample::ArrayPhaseToSample(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
}

void ArrayPhaseToSample::onFloat(t_float phase)
{
    array_.update();
    floatTo(0, clip01(phase) * array_.size());
}

void ArrayPhaseToSample::onList(const AtomList& lst)
{
    array_.update();
    listTo(0, lst.mapFloat([this](t_float v) { return clip01(v) * array_.size(); }, AtomListMapType::FILTER));
}

void setup_array_p2s()
{
    ObjectFactory<ArrayPhaseToSample> obj("array.p2s");
    obj.addAlias("array.p->s");
    obj.addAlias("array.phase->sample");
}

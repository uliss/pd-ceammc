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
#include "ceammc_containers.h"
#include "ceammc_factory.h"

ArraySampleToPhase::ArraySampleToPhase(const PdArgs& a)
    : ArrayBase(a)
{
    createOutlet();
}

void ArraySampleToPhase::onFloat(t_float sample)
{
    if (!array_.update() || array_.size() == 0)
        return;

    floatTo(0, sample / array_.size());
}

void ArraySampleToPhase::onList(const AtomListView& lv)
{
    if (!array_.update() || array_.size() == 0)
        return;

    SmallAtomList val;
    lv.mapFloat([this](t_float f) { return f / array_.size(); }, val, AtomListMapType::FILTER);
    listTo(0, val.view());
}

void setup_array_s2p()
{
    ObjectFactory<ArraySampleToPhase> obj("array.s2p");
    obj.addAlias("array.s->p");
    obj.addAlias("array.sample->phase");
    obj.setXletsInfo({ "float: sample position in array" },
        { "float: position in \\[0..1\\] range" });

    obj.setDescription("convert sample position to phase range [0..1]");
    obj.setCategory("conv");
    obj.setKeywords({"array", "phase", "sample"});
}

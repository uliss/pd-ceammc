/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "data_color.h"
#include "args/argcheck2.h"
#include "ceammc_factory.h"

DataColor::DataColor(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void DataColor::onBang()
{
    atomTo(0, ColorAtom(color_));
}

void DataColor::onDataT(const ColorAtom& a)
{
    color_ = *a;
}

void DataColor::m_brighten(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_.brighten(lv.floatAt(0, 0.1));
}

void DataColor::m_darken(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this);

    color_.darken(lv.floatAt(0, 0.1));
}

void setup_data_color()
{
    ObjectFactory<DataColor> obj("data.color");
    obj.addAlias("color");
    obj.processData<DataTypeColor>();

    obj.addMethod("brighten", &DataColor::m_brighten);
    obj.addMethod("darken", &DataColor::m_darken);
}

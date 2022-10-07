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
#include "dict_is.h"
#include "ceammc_datatypes.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"

IsDict::IsDict(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void IsDict::onFloat(t_float)
{
    boolTo(0, false);
}

void IsDict::onSymbol(t_symbol*)
{
    boolTo(0, false);
}

void IsDict::onList(const AtomListView&)
{
    boolTo(0, false);
}

void IsDict::onAny(t_symbol*, const AtomListView&)
{
    boolTo(0, false);
}

void IsDict::onData(const Atom& d)
{
    if (d.isA<DataTypeDict>()) {
        atomTo(1, d);
        boolTo(0, true);
    } else
        boolTo(0, false);
}

void setup_is_dict()
{
    ObjectFactory<IsDict> obj("is_dict");
    obj.processData();
}

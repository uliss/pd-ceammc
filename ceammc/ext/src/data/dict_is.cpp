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

IsDict::IsDict(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
    createOutlet();
}

void IsDict::onFloat(t_float)
{
    floatTo(0, 0);
}

void IsDict::onSymbol(t_symbol*)
{
    floatTo(0, 0);
}

void IsDict::onList(const AtomList&)
{
    floatTo(0, 0);
}

void IsDict::onAny(t_symbol*, const AtomList&)
{
    floatTo(0, 0);
}

void IsDict::onData(const DataPtr& ptr)
{
    if (ptr.isValid() && ptr->type() == data::DATA_DICT) {
        dataTo(1, ptr);
        floatTo(0, 1);
    } else
        floatTo(0, 0);
}

void setup_is_dict()
{
    ObjectFactory<IsDict> obj("is_dict");
    obj.processData();
}

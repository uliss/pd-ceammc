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
#include "tl_toggle.h"
#include "ceammc_factory.h"

TlToggle::TlToggle(const PdArgs& args)
    : TlBaseObject(args)
{
}

void TlToggle::onCueEnter()
{
    floatTo(0, 1);
}

void TlToggle::onCueLeave()
{
    floatTo(0, 0);
}

void setup_tl_toggle()
{
    ObjectFactory<TlToggle> obj("tl.toggle");
    obj.addAlias("tl.t");
    TlToggle::setDisplaceFn(obj.classPointer());
}

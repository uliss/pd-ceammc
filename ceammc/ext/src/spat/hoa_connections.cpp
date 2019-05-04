/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_connections.h"
#include "ceammc_factory.h"

#include <stdexcept>

HoaIn::HoaIn(const PdArgs& args)
    : BaseObject(args)
    , extra_(0)
{
    auto n = positionalFloatArgument(0, 0);
    if (n < 1) {
        OBJ_ERR << "extra index should be > 0: " << n;
        n = 0;
    }

    extra_ = n;

    createOutlet();
}

void HoaIn::onBang()
{
    bangTo(0);
}

void HoaIn::onFloat(t_float v)
{
    floatTo(0, v);
}

void HoaIn::onSymbol(t_symbol* s)
{
    symbolTo(0, s);
}

void HoaIn::onList(const AtomList& l)
{
    listTo(0, l);
}

void HoaIn::onAny(t_symbol* s, const AtomList& l)
{
    anyTo(0, s, l);
}

t_class* hoa_in_class()
{
    return ObjectFactory<HoaIn>::classPointer();
}

HoaIn* HoaIn::fromObject(t_object* obj)
{
    if (!obj)
        return nullptr;

    if (obj->te_g.g_pd != ObjectFactory<HoaIn>::classPointer())
        return nullptr;

    return reinterpret_cast<ObjectFactory<HoaIn>::ObjectProxy*>(obj)->impl;
}

void setup_spat_hoa_connections()
{
    ObjectFactory<HoaIn> hoa_in("!hoa.in");
}

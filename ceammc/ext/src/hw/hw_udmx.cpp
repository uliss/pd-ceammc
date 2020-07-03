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
#include "hw_udmx.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "udmx_handle.h"

uDMX::uDMX(const PdArgs& args)
    : BaseObject(args)
    , udmx_(new uDMX_Handle)
    , channel_(nullptr)
{
    channel_ = new IntProperty("@ch", positionalFloatArgument(0, 0));
    channel_->checkClosedRange(0, 512);
    createProperty(channel_);

    createInlet();
}

uDMX::~uDMX()
{
    delete udmx_;
}

void uDMX::onFloat(t_float f)
{
    if (!udmx_->sendByte(f, channel_->value()))
        return;
}

void uDMX::onList(const AtomList& lst)
{
    std::vector<unsigned char> data;
    std::transform(lst.begin(), lst.end(), std::back_inserter(data), [](const Atom& a) { return a.asFloat(0); });
    udmx_->send(data, channel_->value());
}

void uDMX::onInlet(size_t n, const AtomList& lst)
{
    channel_->set(lst);
}

void uDMX::m_open(t_symbol* s, const AtomList& lst)
{
    if (!udmx_->open())
        OBJ_ERR << "can't connect to uDMX device";
}

void uDMX::m_close(t_symbol* s, const AtomList& lst)
{
    if (!udmx_->close())
        OBJ_ERR << "can't close uDMX device";
}

void uDMX::m_find(t_symbol* s, const AtomList& lst)
{
    udmx_->findDevice();
}

void setup_hw_udmx()
{
    ObjectFactory<uDMX> obj("hw.udmx");
    obj.addMethod("find", &uDMX::m_find);
    obj.addMethod("open", &uDMX::m_open);
    obj.addMethod("close", &uDMX::m_close);
}

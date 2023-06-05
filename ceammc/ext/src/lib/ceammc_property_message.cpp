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
#include "ceammc_property_message.h"

namespace ceammc {

MessageProperty::MessageProperty(const char* name, const Message& init, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::LIST), access)
{
    if (init.isNone())
        info().setDefault(AtomList());
    else if (init.isList())
        info().setDefault(init.listValue());
    else
        info().setDefault(init.anyValue());
}

AtomList MessageProperty::get() const
{
    if (msg_.isNone() || msg_.isBang())
        return {};
    else if (msg_.isList())
        return msg_.listValue();
    else if (msg_.isAny())
        return msg_.anyValue();
    else
        return msg_.atomValue();
}

bool MessageProperty::setList(const AtomListView& lv)
{
    msg_ = Message::makeTyped(lv);
    return true;
}

bool MessageProperty::getList(AtomList& l) const
{
    l = msg_.anyValue();
    return true;
}

bool MessageProperty::setValue(const Message& m)
{
    msg_ = m;
    return true;
}

const AtomList& MessageProperty::defaultValue() const
{
    return info().defaultList();
}

}

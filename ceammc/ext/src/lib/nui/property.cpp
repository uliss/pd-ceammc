/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "nui/property.h"

namespace ceammc {
namespace ui {

    SizeProperty::SizeProperty(const char* name, const Size& init, PropValueAccess access)
        : SizeProperty(gensym(name), init, access)
    {
    }

    SizeProperty::SizeProperty(t_symbol* name, const Size& init, PropValueAccess access)
        : Property(PropertyInfo(name, PropValueType::LIST, access), access)
        , size_(init)
    {
    }

    AtomList SizeProperty::get() const
    {
        return { (t_float)size_.width(), (t_float)size_.height() };
    }

    bool SizeProperty::setList(const AtomListView& l)
    {
        const bool ok = l.size() == 2 && l[0].isFloat() && l[1].isFloat();
        if (!ok)
            return false;

        size_.setWidth(l[0].asT<t_float>());
        size_.setHeight(l[1].asT<t_float>());
        return true;
    }

    bool SizeProperty::getList(AtomList& l) const
    {
        l = AtomList((t_float)size_.width(), (t_float)size_.height());
        return true;
    }

    bool SizeProperty::setValue(const Size& sz)
    {
        size_ = sz;
        return true;
    }

    static SizePropertyObserver<std::tuple<Size>, 0> ob(0, 0);
}
}

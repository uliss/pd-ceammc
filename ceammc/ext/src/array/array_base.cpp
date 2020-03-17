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
#include "array_base.h"

ArrayBase::ArrayBase(const PdArgs& a)
    : BaseObject(a)
    , array_name_(nullptr)
{
    createCbSymbolProperty(
        "@array",
        [this]() -> t_symbol* { return array_name_; },
        [this](t_symbol* s) -> bool { setArray(s); return true; })
        ->setArgIndex(0);
}

bool ArrayBase::setArray(t_symbol* s)
{
    array_name_ = s;
    if (!array_.open(array_name_)) {
        OBJ_ERR << "array not found: " << s->s_name;
        return false;
    }

    return true;
}

bool ArrayBase::checkArray(bool log)
{
    if (array_name_ == nullptr || !array_.open(array_name_)) {
        if (log) {
            OBJ_ERR << "invalid array: " << array_.name();
        }

        return false;
    }

    return true;
}

ArrayMod::ArrayMod(const PdArgs& a)
    : ArrayBase(a)
    , redraw_(nullptr)
{
    redraw_ = new BoolProperty("@redraw", true);
    createProperty(redraw_);
}

bool ArrayMod::shouldRedraw() const
{
    return redraw_->value();
}

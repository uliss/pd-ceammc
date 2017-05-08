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
    , array_name_(positionalSymbolArgument(0, 0))
{
    if (array_name_)
        array_.open(array_name_);

    createCbProperty("@array", &ArrayBase::propArray, &ArrayBase::propSetArray);
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

AtomList ArrayBase::propArray() const
{
    return AtomList(array_name_);
}

void ArrayBase::propSetArray(const AtomList& l)
{
    if (l.empty() || !l[0].isSymbol()) {
        OBJ_ERR << "array name required";
        return;
    }

    setArray(l[0].asSymbol());
}

bool ArrayBase::check()
{
    if (array_name_ == 0 || !array_.open(array_name_)) {
        OBJ_ERR << "invalid array: " << array_.name();
        return false;
    }

    return true;
}

ArrayMod::ArrayMod(const PdArgs& a)
    : ArrayBase(a)
    , redraw_(0)
{
    redraw_ = new BoolProperty("@redraw", true);
    createProperty(redraw_);
}

bool ArrayMod::shouldRedraw() const
{
    return redraw_->value();
}

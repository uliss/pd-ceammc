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

ArraySoundBase::ArraySoundBase(const PdArgs& args)
    : ArrayReadIFace<SoundExternal>(args)
{
}

bool ArraySoundBase::setArray(t_symbol* aname)
{
    auto rc = ArrayReadIFace<SoundExternal>::setArray(aname);
    if (!rc)
        return rc;

    array_.useInDSP();
    return rc;
}

ArrayMod::ArrayMod(const PdArgs& a)
    : ArrayBase(a)
    , redraw_(nullptr)
{
    redraw_ = new BoolProperty("@redraw", true);
    addProperty(redraw_);
}

bool ArrayMod::shouldRedraw() const
{
    return redraw_->value();
}

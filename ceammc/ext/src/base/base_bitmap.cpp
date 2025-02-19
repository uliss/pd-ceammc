/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "base_bitmap.h"
#include "ceammc_factory.h"

using namespace ceammc;

BaseBitmap::BaseBitmap(const PdArgs& args)
    : BaseObject(args)
{
    w_ = new IntProperty("@w", 8);
    w_->checkClosedRange(1, 256);
    w_->setArgIndex(0);
    addProperty(w_);

    h_ = new IntProperty("@h", 8);
    h_->checkClosedRange(1, 256);
    h_->setArgIndex(1);
    addProperty(h_);

    createOutlet();
}

void setup_base_bitmap()
{
    ObjectFactory<BaseBitmap> obj("bitmap");
}

/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "base_sync.h"
#include "ceammc_factory.h"

constexpr size_t NUM_MIN = 2;
constexpr size_t NUM_MAX = 8;
constexpr size_t NUM_DEFAULT = 2;

BaseSync::BaseSync(const PdArgs& args)
    : BaseObject(args)
    , blocked_(false)
{
    n_ = new IntProperty("@n", NUM_DEFAULT, PropValueAccess::INITONLY);
    n_->checkClosedRange(NUM_MIN, NUM_MAX);
    n_->setArgIndex(0);
    addProperty(n_);
}

void BaseSync::initDone()
{
    for (int i = 0; i < n_->value(); i++) {
        createInlet();
        createOutlet();
    }
}

void BaseSync::onInlet(size_t idx, const AtomListView& lv)
{
    if (blocked_)
        return;

    blocked_ = true;

    for (int i = 0; i < n_->value(); i++) {
        if ((i + 1) == idx)
            continue;

        listTo(i, lv);
    }

    blocked_ = false;
}

void setup_base_sync()
{
    ObjectFactory<BaseSync> obj("sync", OBJECT_FACTORY_NO_DEFAULT_INLET);

    obj.setDescription("value synchronization");
    obj.setCategory("base");
    obj.setKeywords({ "sync" });
}

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
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

constexpr size_t NUM_MIN = 2;
constexpr size_t NUM_MAX = 8;
constexpr size_t NUM_DEFAULT = 2;

BaseSync::BaseSync(const PdArgs& args)
    : BaseObject(args)
    , n_(NUM_DEFAULT)
    , blocked_(false)
{
    const auto& pargs = parsedPosArgs();
    if (!pargs.empty() && !pargs.isInteger()) {
        OBJ_ERR << "number of inputs expected";
    } else {
        int n = parsedPosArgs().intAt(0, NUM_DEFAULT);
        if (n < NUM_MIN || n > NUM_MAX) {
            OBJ_ERR << fmt::format("argument is out of {}..{} range, got: {}, "
                                   "using default value {}",
                NUM_MIN, NUM_MAX, n, NUM_DEFAULT);
        } else
            n_ = n;
    }

    for (size_t i = 0; i < n_; i++) {
        createInlet();
        createOutlet();
    }
}

void BaseSync::onInlet(size_t idx, const AtomListView& lv)
{
    if (blocked_)
        return;

    blocked_ = true;

    for (size_t i = 0; i < n_; i++) {
        if ((i + 1) == idx)
            continue;

        listTo(i, lv);
    }

    blocked_ = false;
}

void setup_base_sync()
{
    ObjectFactory<BaseSync> obj("sync", OBJECT_FACTORY_NO_DEFAULT_INLET);
}

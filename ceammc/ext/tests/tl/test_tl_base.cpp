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
#include "test_tl_base.h"
#include "ceammc_pd.h"
#include "ceammc_timeline.h"
#include "cicm/Sources/ecommon.h"
#include "mod_tl.h"

#define CATCH_CONFIG_EXTERNAL_INTERFACES
#include "catch.hpp"

struct UIRegister : Catch::TestEventListenerBase {
    using TestEventListenerBase::TestEventListenerBase; // inherit constructor

    void testRunStarting(Catch::TestRunInfo const& testRunInfo) override
    {
        ceammc::PureData::instance();
        epd_init();
        ceammc_tl_setup();
        ceammc::tl::CueStorage::clear();
    }
};

CATCH_REGISTER_LISTENER(UIRegister)

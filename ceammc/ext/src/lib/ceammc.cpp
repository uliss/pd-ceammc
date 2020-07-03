/*****************************************************************************
 *
 * Created: 04.12.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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
#include "ceammc.h"
#include "config.h"
#include "fmt/format.h"

namespace ceammc {

InvalidOutlet::InvalidOutlet(OutletIdx n) noexcept
    : Exception("")
    , n_(n)
{
}

const char* InvalidOutlet::what() const noexcept
{
    static char buf[64];
    fmt::format_to(buf, FMT_STRING("invalid outlet index: {}"), n_.n);
    return buf;
}

}

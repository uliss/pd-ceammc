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
#include "ceammc_platform_win.h"

#include <Shlwapi.h>
#include <cstdlib>
#include <cstring>

namespace ceammc {
namespace platform_win {

    bool is_path_relative(const char* path)
    {
        return PathIsRelative(path) == 1;
    }

    std::string basename(const char* path)
    {
        char* buf = strdup(path);
        PathStripPath(buf);
        std::string res(buf);
        free(buf);
        return res;
    }
}
}

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
#include "ceammc_platform.h"

#ifdef __WIN32
#define NS ceammc::platform_win
#include "ceammc_platform_win.h"
#else
#define NS ceammc::platform_unix
#include "ceammc_platform_unix.h"
#endif

namespace ceammc {
namespace platform {

    bool is_path_relative(const char* path)
    {
        return NS::is_path_relative(path);
    }

    const char* platform_name()
    {
#ifdef __APPLE__
        return "macosx";
#elif defined(__WIN32)
        return "windows";
#elif defined(__ANDROID__)
        return "android";
#elif defined(__linux__)
        return "linux";
#elif defined(__FreeBSD__)
        return "freebsd";
#elif defined(__NetBSD__)
        return "netbsd";
#elif defined(__OpenBSD__)
        return "openbsd";
#else
        return "???";
#endif
    }
}
}

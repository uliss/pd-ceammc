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

#include "config.h"

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

namespace ceammc {

bool win_is_path_relative(const char* path)
{
    return PathIsRelative(path) == 1;
}

std::string win_basename(const char* path)
{
#ifdef HAVE_LIBGEN_H
    char* p = strdup(path);
    char* bn = ::basename(p);
    if (!bn)
        return "";

    std::string res(bn);
    free(p);
    return res;
#else
    char* buf = strdup(path);
    PathStripPath(buf);
    std::string res(buf);
    free(buf);
    return res;
#endif
}

std::string win_dirname(const char* path)
{
#ifdef HAVE_LIBGEN_H
    char* p = strdup(path);
    char* dir = ::dirname(p);
    if (!dir)
        return "";

    std::string res(dir);
    free(p);
    return res;
#else
    char* buf = strdup(path);
    PathRemoveFileSpec(buf);
    std::string res(buf);
    free(buf);
    return res;
#endif
}
}

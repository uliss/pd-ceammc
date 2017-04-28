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
#include "ceammc_platform_unix.h"

#include <cstdlib>
#include <cstring>
#include <fnmatch.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>

bool ceammc::unix_is_path_relative(const char* path)
{
    return path[0] != '/';
}

std::string ceammc::unix_basename(const char* path)
{
    char* p = strdup(path);
    char* bn = ::basename(p);
    if (!bn)
        return "";

    std::string res(bn);
    free(p);
    return res;
}

std::string ceammc::unix_dirname(const char* path)
{
    char* p = strdup(path);
    char* dir = ::dirname(p);
    if (!dir)
        return "";

    std::string res(dir);
    free(p);
    return res;
}

bool ceammc::unix_fnmatch(const char* pattern, const char* str)
{
    return ::fnmatch(pattern, str, 0) == 0;
}

bool ceammc::unix_path_exists(const char* path)
{
    return access(path, R_OK) == 0;
}

bool ceammc::unix_mkdir(const char* path, int flags)
{
    if (flags < 0)
        flags = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

    return mkdir(path, static_cast<mode_t>(flags)) == 0;
}

bool ceammc::unix_rmdir(const char* path)
{
    return rmdir(path) == 0;
}

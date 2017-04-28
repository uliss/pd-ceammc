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

#include <cstdlib>

#ifdef __WIN32
#define NS(f) win_##f
#include "ceammc_platform_win.h"
#else
#define NS(f) unix_##f
#include "ceammc_platform_unix.h"
#endif

namespace ceammc {
namespace platform {

    bool is_path_relative(const char* path)
    {
        return NS(is_path_relative(path));
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

    std::string basename(const char* path)
    {
        return NS(basename(path));
    }

    std::string dirname(const char* path)
    {
        return NS(dirname(path));
    }

    static bool findEnvVar(const std::string& str, std::string::size_type from,
        std::string* varname, std::string::size_type* begin, std::string::size_type* end)
    {
        std::string::size_type at1 = str.find_first_of('%', from);
        if (std::string::npos == at1)
            return false;

        std::string::size_type at2 = str.find_first_of('%', at1 + 1);
        if (std::string::npos == at2)
            return false;

        std::string::size_type len = at2 - at1;
        if (len < 2)
            return false;

        *begin = at1;
        *end = at2;
        *varname = str.substr(at1 + 1, len - 1);
        return true;
    }

    std::string expandenv(const char* str)
    {
        std::string s(str);
        std::string::size_type pos = 0;

        std::string::size_type at1 = 0;
        std::string::size_type at2 = 0;
        std::string varname;
        std::string res;

        while (findEnvVar(s, pos, &varname, &at1, &at2)) {
            res += s.substr(pos, at1 - pos);

            char* var = getenv(varname.c_str());
            // if env var not found
            if (!var) {
                // append unmodified
                res += s.substr(at1, at2 - at1 + 1);
            } else {
                res += var;
            }

            pos = at2 + 1;
        }

        res += s.substr(pos, std::string::npos);
        return res;
    }

    bool fnmatch(const char* pattern, const char* str)
    {
        return NS(fnmatch)(pattern, str);
    }

    bool path_exists(const char* path)
    {
        return NS(path_exists)(path);
    }

    bool mkdir(const char* path, int flags)
    {
        return NS(mkdir)(path, flags);
    }

    bool rmdir(const char* path)
    {
        return NS(rmdir)(path);
    }
}
}

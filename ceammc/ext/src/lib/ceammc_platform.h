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
#ifndef CEAMMC_PLATFORM_H
#define CEAMMC_PLATFORM_H

#include <string>

/**
 * Platform dependent function realization
 */

namespace ceammc {
namespace platform {
    const char* platform_name();
    bool is_path_relative(const char* path);

    std::string basename(const char* path);
    std::string dirname(const char* path);
    std::string expandenv(const char* str);

    bool fnmatch(const char* pattern, const char* str);

    /**
     * @brief returns true if given path (file or directory) exists
     */
    bool path_exists(const char* path);

    bool mkdir(const char* path, int flags = -1);
    bool rmdir(const char* path);
}
}

#endif // CEAMMC_PLATFORM_H

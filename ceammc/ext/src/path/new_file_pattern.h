/*****************************************************************************
 * Copyright 2022 Serge Poltavski. All rights reserved.
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
#ifndef NEW_FILE_PATTERN_H
#define NEW_FILE_PATTERN_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace ceammc {
namespace path {

    enum PatternStatus {
        PATTERN_DONE,
        PATTERN_NOMATCH,
        PATTERN_ERROR,
        PATTERN_LENGTH_OVERFLOW,
    };

    PatternStatus expand_filename_pattern(const std::string& pattern, std::string& res, uint32_t idx = 1);

}
}

#endif // NEW_FILE_PATTERN_H

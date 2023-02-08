/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef PARSER_HEXBEAT_H
#define PARSER_HEXBEAT_H

#include <bitset>
#include <cstddef>
#include <cstdint>

namespace ceammc {
namespace parser {

    using HexBeatResult = std::bitset<512>;

    /**
     * @brief parse hexbeat string
     * @param str - input string, starting with '0x' or '#' prefix, non hex digits are ignored
     * @param res - result bitset
     * @return number of parsed bits or 0 on error
     */
    std::uint16_t parse_hexbeat(const char* str, HexBeatResult& res);

}
}

#endif // PARSER_HEXBEAT_H

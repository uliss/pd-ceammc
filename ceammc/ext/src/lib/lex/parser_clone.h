/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef PARSER_CLONE_H
#define PARSER_CLONE_H

#include <cstdint>

namespace ceammc {

namespace parser {

    enum TargetType : uint8_t {
        TARGET_TYPE_NONE,
        TARGET_TYPE_ALL,
        TARGET_TYPE_EXCEPT,
        TARGET_TYPE_RANDOM,
        TARGET_TYPE_EQ,
        TARGET_TYPE_GT,
        TARGET_TYPE_GE,
        TARGET_TYPE_LT,
        TARGET_TYPE_LE,
        TARGET_TYPE_RANGE
    };

    struct TargetMessage {
        int16_t first { 0 };
        int16_t last { -1 };
        int16_t step { 1 };
        int8_t inlet { -1 };
        TargetType type { TARGET_TYPE_NONE };
    };

    enum CloneMessageType {
        MSG_TYPE_NONE,
        MSG_TYPE_SEND,
        MSG_TYPE_SEND_SPREAD,
        MSG_TYPE_DSP_SET,
        MSG_TYPE_DSP_TOGGLE
    };

    bool parse_clone_target(const char* str, TargetMessage& res);
    CloneMessageType parse_clone_message_type(const char* str);
}

}

#endif // PARSER_CLONE_H

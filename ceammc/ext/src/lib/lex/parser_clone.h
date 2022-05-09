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

    enum ArgumentType : uint8_t {
        ARG_TYPE_NONE,
        ARG_TYPE_ALL,
        ARG_TYPE_EXCEPT,
        ARG_TYPE_RANDOM,
        ARG_TYPE_EQ,
        ARG_TYPE_GT,
        ARG_TYPE_GE,
        ARG_TYPE_LT,
        ARG_TYPE_LE,
        ARG_TYPE_RANGE,
        ARG_TYPE_SPREAD
    };

    struct CloneMessage {
        int16_t first;
        int16_t last;
        int16_t step;
        int8_t inlet;
        ArgumentType type;
    };

    enum MessageType {
        MSG_TYPE_NONE,
        MSG_TYPE_SEND,
        MSG_TYPE_SEND_SPREAD,
        MSG_TYPE_DSP_SET,
        MSG_TYPE_DSP_TOGGLE
    };

    bool parse_clone_action(const char* str, CloneMessage& res);
    MessageType parse_clone_message_type(const char* str);
}

}

#endif // PARSER_CLONE_H

/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef PROTO_MIDI_CC_RPN_PARSER_H
#define PROTO_MIDI_CC_RPN_PARSER_H

#include <cstdint>

namespace ceammc {
namespace midi {

    class RPNParser {
    public:
        enum CcNumber {
            CC_DATA_ENTRY_COARSE = 6,
            CC_DATA_ENTRY_FINE = 38,
            CC_DATA_INCREMENT = 96,
            CC_DATA_DECREMENT = 97,
            CC_RPN_COARSE = 101,
            CC_RPN_FINE = 100,
        };

        enum State {
            ST_INIT = 0,
            ST_RPN_NUMBER_MSB,
            ST_RPN_NUMBER_LSB,
            ST_DATA_MSB,
            ST_DONE,
            ST_ERROR
        };

        enum RpnNumber {
            RPN_PITCH_BEND_SENSIVITY = 0,
            RRN_CHANNEL_TUNING_FINE = 1,
            RRN_CHANNEL_TUNING_COARSE = 2,
            RPN_CHANNEL_TUNING_PROG_CHANGE = 3,
            RPN_CHANNEL_TUNING_BANK_SELECT = 4,
            RPN_RESET = 0x3FFF
        };

        enum Error : std::uint8_t {
            NO_ERROR,
            ERR_RPN_FINE_EXPECTED,
            ERR_UNKNOWN_RPN,
            ERR_DATA_COARSE_EXPECTED,
        };

        struct Result {
            State state;
            Error err;
            int16_t rpn;
            uint16_t value;
        };

    private:
        State state_;
        int16_t rpn_number_;
        uint16_t rpn_value_;

    public:
        RPNParser()
            : state_(ST_INIT)
            , rpn_number_(-1)
            , rpn_value_(0)
        {
        }

        void reset()
        {
            state_ = ST_INIT;
            rpn_number_ = -1;
            rpn_value_ = 0;
        }

        Result push(uint8_t cc, uint8_t val)
        {
            switch (state_) {
            case ST_INIT:
                if (cc & CC_RPN_COARSE) {
                    state_ = ST_RPN_NUMBER_MSB;
                    rpn_number_ = (val << 7);
                } else if (cc & CC_DATA_INCREMENT) {
                    if (rpn_number_ < 0)
                        return { state_, ERR_UNKNOWN_RPN, rpn_number_, 0 };

                    state_ = ST_DONE;
                    rpn_value_++;
                    return { state_, NO_ERROR, rpn_number_, rpn_value_ };
                } else if (cc & CC_DATA_DECREMENT) {
                    if (rpn_number_ < 0)
                        return { state_, ERR_UNKNOWN_RPN, rpn_number_, 0 };

                    state_ = ST_DONE;
                    rpn_value_--;
                    return { state_, NO_ERROR, rpn_number_, rpn_value_ };
                }

                return { state_, NO_ERROR, rpn_number_, 0 };

            case ST_RPN_NUMBER_MSB:
                if (cc & CC_RPN_FINE) {
                    state_ = ST_RPN_NUMBER_LSB;
                    rpn_number_ |= val;

                    if (rpn_number_ == RPN_RESET) {
                        state_ = ST_DONE;
                        rpn_number_ = -1;
                    }

                    return { state_, NO_ERROR, rpn_number_, 0 };
                } else
                    return { state_, ERR_RPN_FINE_EXPECTED, 0, 0 };

            case ST_RPN_NUMBER_LSB:
                if (cc & CC_DATA_ENTRY_COARSE) {
                    state_ = ST_DATA_MSB;
                    rpn_value_ = (val << 7);
                    return { state_, NO_ERROR, rpn_number_, rpn_value_ };
                } else
                    return { state_, ERR_DATA_COARSE_EXPECTED, rpn_number_, 0 };

            case ST_DATA_MSB:
                state_ = ST_DONE;
                if (cc & CC_DATA_ENTRY_FINE)
                    rpn_value_ |= val;

                return { state_, NO_ERROR, rpn_number_, rpn_value_ };

            default:
                return { state_, NO_ERROR, rpn_number_, rpn_value_ };
            }
        }
    };

}
}

#endif // PROTO_MIDI_CC_RPN_PARSER_H

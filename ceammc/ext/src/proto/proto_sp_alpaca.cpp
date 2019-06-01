/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "proto_sp_alpaca.h"
#include "ceammc_factory.h"

#include <cstdint>
#include <tuple>

static t_symbol* SYM_DIGITAL;
static t_symbol* SYM_ANALOG;
static t_symbol* SYM_ANALOG_RAW;
static t_symbol* SYM_OK;
static t_symbol* SYM_EMPTY;
static t_symbol* SYM_NO_TARGET;
static t_symbol* SYM_UNKNOWN_TARGET;
static t_symbol* SYM_UNKNOWN_COMMAND;
static t_symbol* SYM_RESPONSE;

enum StateType {
    STATE_START = 0,
    STATE_CMD,
    STATE_DIGITAL_VALUE,
    STATE_ANALOG_VALUE,
    STATE_ANALOG_VALUE_RAW,
    STATE_RESPONSE,
    STATE_END
};

typedef std::function<bool(ProtoSpAlpaca*, uint8_t)> TransitionFn;
typedef std::function<bool(uint8_t)> ValidatorFn;
typedef std::tuple<StateType, ValidatorFn, StateType, TransitionFn> FSMRow;

enum CommandType {
    CMD_START = 0x81,
    CMD_END = 0x80,
    CMD_SEND_DIGITAL = 0x90,
    CMD_SEND_ANALOG = 0xA0,
    CMD_SEND_ANALOG_RAW = 0xB0,
    CMD_RESPONSE = 0xC0
};

static std::vector<FSMRow> fsm = {
    // start->cmd
    { STATE_START,
        [](uint8_t v) { return v == CMD_START; },
        STATE_CMD,
        [](ProtoSpAlpaca* p, uint8_t) { return p->fsm_clear(); } },

    // cmd->digital
    { STATE_CMD,
        [](uint8_t v) { return (0xF0 & v) == CMD_SEND_DIGITAL; },
        STATE_DIGITAL_VALUE,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x0F & v); } },

    // digital->end
    { STATE_DIGITAL_VALUE,
        [](uint8_t v) { return v == CMD_END; },
        STATE_END,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_output_digital(); } },

    // cmd->analog
    { STATE_CMD,
        [](uint8_t v) { return (0xF0 & v) == CMD_SEND_ANALOG; },
        STATE_ANALOG_VALUE,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x0F & v); } },

    // analog->end
    {
        STATE_ANALOG_VALUE,
        [](uint8_t v) { return v == CMD_END; },
        STATE_END,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_output_analog(); } },

    // analog->analog
    { STATE_ANALOG_VALUE,
        nullptr,
        STATE_ANALOG_VALUE,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x7F & v); } },

    // cmd->analog_raw
    { STATE_CMD,
        [](uint8_t v) { return (0xF0 & v) == CMD_SEND_ANALOG_RAW; },
        STATE_ANALOG_VALUE_RAW,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x0F & v); } },

    // analog_raw->end
    { STATE_ANALOG_VALUE_RAW,
        [](uint8_t v) { return v == CMD_END; },
        STATE_END,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_output_analog_raw(); } },

    // analog_raw->analog_raw
    { STATE_ANALOG_VALUE_RAW,
        nullptr,
        STATE_ANALOG_VALUE_RAW,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x7F & v); } },

    // cmd->response
    { STATE_CMD,
        [](uint8_t v) { return (0xF0 & v) == CMD_RESPONSE; },
        STATE_RESPONSE,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x0F & v); } },

    // response->end
    { STATE_RESPONSE,
        nullptr,
        STATE_END,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_output_response(); } },
};

ProtoSpAlpaca::ProtoSpAlpaca(const PdArgs& args)
    : BaseObject(args)
    , state_(STATE_START)
{
    createOutlet();
}

void ProtoSpAlpaca::onFloat(t_float f)
{
    parse(f);
}

bool ProtoSpAlpaca::fsm_clear()
{
    in_cmd_.clear();
    return true;
}

bool ProtoSpAlpaca::fsm_add(uint8_t v)
{
    if (in_cmd_.size() >= PROTO_ALPACA_MAX_CMD_SIZE)
        return false;

    in_cmd_.push_back(v);
    return true;
}

bool ProtoSpAlpaca::fsm_output_digital()
{
    if (in_cmd_.empty())
        return false;

    const uint8_t n = in_cmd_[0] >> 1;
    const uint8_t x = in_cmd_[0] & 1;
    anyTo(0, SYM_DIGITAL, { t_float(n), t_float(x) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_analog()
{
    if (in_cmd_.size() != 3)
        return false;

    const uint8_t n = in_cmd_[0] >> 1;
    const uint8_t v = (in_cmd_[1] << 7) + in_cmd_[2];
    anyTo(0, SYM_ANALOG, { t_float(n), t_float(v) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_analog_raw()
{
    if (in_cmd_.size() != 3)
        return false;

    const uint8_t n = in_cmd_[0] >> 1;
    const uint8_t v1 = (in_cmd_[1] << 7) + in_cmd_[2];
    const uint8_t v2 = (in_cmd_[3] << 7) + in_cmd_[4];
    anyTo(0, SYM_ANALOG_RAW, { t_float(n), t_float(v1), t_float(v2) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_response()
{
    if (in_cmd_.empty())
        return false;

    switch (in_cmd_[0]) {
    case 0:
        anyTo(0, SYM_RESPONSE, SYM_OK);
        break;
    case 1:
        anyTo(0, SYM_RESPONSE, SYM_EMPTY);
        break;
    case 2:
        anyTo(0, SYM_RESPONSE, SYM_NO_TARGET);
        break;
    case 3:
        anyTo(0, SYM_RESPONSE, SYM_UNKNOWN_TARGET);
        break;
    case 4:
        anyTo(0, SYM_RESPONSE, SYM_UNKNOWN_COMMAND);
        break;
    default:
        LIB_ERR << "unknown response code: " << int(in_cmd_[0]);
        break;
    }
    return true;
}

void ProtoSpAlpaca::parse(uint8_t v)
{
    if (in_cmd_.size() >= PROTO_ALPACA_MAX_CMD_SIZE) {
        OBJ_ERR << "max command size exceeded: " << PROTO_ALPACA_MAX_CMD_SIZE;
        in_cmd_.clear();
        state_ = STATE_START;
        return;
    }

    for (auto& row : fsm) {
        if (std::get<0>(row) == state_) {
            auto check_fn = std::get<1>(row);
            if (check_fn && !check_fn(v))
                continue;

            auto fn = std::get<3>(row);
            if (fn(this, v)) {
                state_ = std::get<2>(row);

                // next loop
                if (state_ == STATE_END)
                    state_ = STATE_START;
            } else {
                OBJ_ERR << "transition error";
                state_ = STATE_START;
            }

            return;
        }
    }

    OBJ_ERR << "invalid value: " << int(v);
    state_ = STATE_START;
}

void setup_proto_sp_alpaca()
{
    SYM_DIGITAL = gensym("digital");
    SYM_ANALOG = gensym("analog");
    SYM_ANALOG_RAW = gensym("analog_raw");
    SYM_OK = gensym("ok");
    SYM_EMPTY = gensym("empty");
    SYM_NO_TARGET = gensym("no_target");
    SYM_UNKNOWN_TARGET = gensym("unknown_target");
    SYM_UNKNOWN_COMMAND = gensym("unknown_command");
    SYM_RESPONSE = gensym("response");

    ObjectFactory<ProtoSpAlpaca> obj("proto.sp.alpaca");
}

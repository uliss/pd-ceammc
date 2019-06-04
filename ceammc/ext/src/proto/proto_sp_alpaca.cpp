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
#include "ceammc_format.h"

#include <cctype>
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
static t_symbol* SYM_MODE_NONE;
static t_symbol* SYM_MODE_ANALOG;
static t_symbol* SYM_MODE_DIGITAL1;
static t_symbol* SYM_MODE_DIGITAL2;
static t_symbol* SYM_MODE_DIGITAL_BOTH;
static t_symbol* SYM_MODE_ANALOG_RAW;

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
    // in
    CMD_SEND_DIGITAL = 0x90,
    CMD_SEND_ANALOG = 0xA0,
    CMD_SEND_ANALOG_RAW = 0xB0,
    CMD_RESPONSE = 0xC0,
    // out
    CMD_TARGET = 0xD0,
    CMD_TARGET_JACK0 = 0x0,
    CMD_TARGET_JACK1 = 0x1,
    CMD_TARGET_JACK_BOTH = 0x2,
    CMD_TARGET_MATRIX = 0x3,
    CMD_JACK_SET_MODE = 0x10,
    CMD_MATRIX_SET_BRIGHTNESS = 0x10,
    CMD_MATRIX_SET_PIXEL = 0x20,
    CMD_MATRIX_CLEAR_PIXEL = 0x30,
    CMD_MATRIX_CLEAR = 0x40,
    CMD_MATRIX_FILL = 0x50,
    CMD_MATRIX_DRAW = 0x60,
    CMD_MATRIX_CHAR = 0x70
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
    const uint8_t v = (in_cmd_[2] << 7) + in_cmd_[1];
    anyTo(0, SYM_ANALOG, { t_float(n), t_float(v) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_analog_raw()
{
    if (in_cmd_.size() != 3)
        return false;

    const uint8_t n = in_cmd_[0] >> 1;
    const uint8_t v1 = (in_cmd_[2] << 7) + in_cmd_[1];
    const uint8_t v2 = (in_cmd_[4] << 7) + in_cmd_[3];
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

void ProtoSpAlpaca::m_clear(t_symbol* s, const AtomList& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_CLEAR);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_str(t_symbol* s, const AtomList& l)
{
    auto str = to_string(l);
    if (str.empty()) {
        METHOD_ERR(s) << "empty string";
        return;
    }

    if (str.size() > 2) {
        METHOD_ERR(s) << "max string length is 2: " << str;
        return;
    }

    m_clear(s, l);

    drawChar(toupper(str[0]), 0);
    if (str.size() > 1)
        drawChar(toupper(str[1]), 4);
}

void ProtoSpAlpaca::m_mode(t_symbol* s, const AtomList& l)
{
    if (!checkArgs(ARG_INT, ARG_SYMBOL, s))
        return;

    const int ch = l.intAt(0, 0);
    if (ch < 0 || ch > 1) {
        METHOD_ERR(s) << "invalid channel: " << ch << "; 0 or 1 expected";
        return;
    }

    t_symbol* mode = l.symbolAt(0, &s_);
    if (mode == SYM_MODE_NONE) {

    } else if (mode == SYM_MODE_ANALOG) {

    } else if (mode == SYM_MODE_ANALOG_RAW) {

    } else if (mode == SYM_MODE_DIGITAL1) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | CMD_TARGET_JACK0);
        floatTo(0, CMD_MO);
        floatTo(0, CMD_END);

    } else if (mode == SYM_MODE_DIGITAL2) {

    } else if (mode == SYM_MODE_DIGITAL_BOTH) {

    } else {
        METHOD_ERR(s) << "unknown mode: " << mode->s_name;
    }
}

void ProtoSpAlpaca::m_char(t_symbol* s, const AtomList& l)
{
    if (l.empty() || l.size() > 2) {
        METHOD_ERR(s) << "CHAR [OFFSET] expected";
        return;
    }

    auto str = to_string(l[0]);

    if (str.size() != 1) {
        METHOD_ERR(s) << "single character expected: " << str;
        return;
    }

    if (l.size() == 2 && !l[1].isFloat()) {
        METHOD_ERR(s) << "offset value expected: " << l[1];
        return;
    }

    drawChar(toupper(str[0]), l.size() == 2 ? l[1].asFloat() : 0);
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

void ProtoSpAlpaca::drawChar(int ch, int offset)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_CHAR);

    floatTo(0, 0x3F & ch);
    floatTo(0, 0x3F & (ch >> 6));
    if (offset > 0)
        floatTo(0, offset);
    floatTo(0, CMD_END);
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

    SYM_MODE_NONE = gensym("none");
    SYM_MODE_ANALOG = gensym("analog");
    SYM_MODE_DIGITAL1 = gensym("digital1");
    SYM_MODE_DIGITAL2 = gensym("digital2");
    SYM_MODE_DIGITAL_BOTH = gensym("digital");
    SYM_MODE_ANALOG_RAW = gensym("analog_raw");

    ObjectFactory<ProtoSpAlpaca> obj("proto.sp.alpaca");
    obj.addMethod("clear", &ProtoSpAlpaca::m_clear);
    obj.addMethod("char", &ProtoSpAlpaca::m_char);
    obj.addMethod("str", &ProtoSpAlpaca::m_str);
    obj.addMethod("mode", &ProtoSpAlpaca::m_mode);
}

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

#include "extra/hw_alpaca/constants.h"

using namespace alpaca;

static t_symbol* SYM_ANALOG;
static t_symbol* SYM_ANALOG_RAW;
static t_symbol* SYM_DIGITAL;
static t_symbol* SYM_EMPTY;
static t_symbol* SYM_MODE;
static t_symbol* SYM_MODE_ANALOG;
static t_symbol* SYM_MODE_ANALOG_RAW;
static t_symbol* SYM_MODE_DIGITAL1;
static t_symbol* SYM_MODE_DIGITAL2;
static t_symbol* SYM_MODE_DIGITAL_BOTH;
static t_symbol* SYM_MODE_NONE;
static t_symbol* SYM_NO_TARGET;
static t_symbol* SYM_OK;
static t_symbol* SYM_RESPONSE;
static t_symbol* SYM_UNKNOWN_COMMAND;
static t_symbol* SYM_UNKNOWN_TARGET;
static t_symbol* SYM_VERSION;

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

static std::vector<FSMRow> fsm({ {
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

    // response->response
    { STATE_RESPONSE,
        [](uint8_t v) { return v <= 0x7F; },
        STATE_RESPONSE,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_add(0x7F & v); } },

    // response->end
    { STATE_RESPONSE,
        [](uint8_t v) { return v == CMD_END; },
        STATE_END,
        [](ProtoSpAlpaca* p, uint8_t v) { return p->fsm_output_response(); } },
} });

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
    const uint16_t v = (uint16_t(in_cmd_[2]) << 7) | in_cmd_[1];
    anyTo(0, SYM_ANALOG, { t_float(n), t_float(v) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_analog_raw()
{
    if (in_cmd_.size() != 5)
        return false;

    const uint8_t n = in_cmd_[0] >> 1;
    const uint16_t v1 = (uint16_t(in_cmd_[2]) << 7) | in_cmd_[1];
    const uint16_t v2 = (uint16_t(in_cmd_[4]) << 7) | in_cmd_[3];
    anyTo(0, SYM_ANALOG_RAW, { t_float(n), t_float(v1), t_float(v2) });
    return true;
}

bool ProtoSpAlpaca::fsm_output_response()
{
    if (in_cmd_.empty())
        return false;

    switch (in_cmd_[0]) {
    case 0: {
        if (in_cmd_.size() > 2) {
            const size_t argc = in_cmd_[1];

            if (in_cmd_.size() != argc + 2) { // target, nargs, ....
                OBJ_ERR << "invalid response format...";
                return false;
            }

            OBJ_DBG << "command size: " << in_cmd_.size();
            const auto argv = (&in_cmd_[0]) + 2;

            if (argc == 2 && argv[0] == CMD_DEVICE_VERSION)
                anyTo(0, SYM_VERSION, argv[1]);
            else if (argc == 3 && argv[0] == CMD_DEVICE_MODE) {
                t_symbol* s = SYM_MODE_NONE;
                switch (argv[2]) {
                case MODE_ANALOG:
                    s = SYM_MODE_ANALOG;
                    break;
                case MODE_ANALOG_RAW:
                    s = SYM_MODE_ANALOG_RAW;
                    break;
                case MODE_DIGITAL1:
                    s = SYM_MODE_DIGITAL1;
                    break;
                case MODE_DIGITAL2:
                    s = SYM_MODE_DIGITAL2;
                    break;
                case MODE_DIGITAL_BOTH:
                    s = SYM_MODE_DIGITAL_BOTH;
                    break;
                default:
                    break;
                }

                AtomList l(Atom(argv[1]), s);
                anyTo(0, SYM_MODE, l);
            }
        }

        anyTo(0, SYM_RESPONSE, SYM_OK);
    } break;
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

void ProtoSpAlpaca::m_clear(t_symbol* s, const AtomListView& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_CLEAR);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_col(t_symbol* s, const AtomListView& l)
{
    // args example: 2 0 1 1 0 1 1
    // 3rd col with values 0 1 1 0 1 1
    if (l.size() < 2) {
        METHOD_ERR(s) << "COL_IDX VALUE [VALUE..] expected: " << l;
        return;
    }

    int col = l.intAt(0, 0);
    if (col < 0 || col > 7) {
        METHOD_ERR(s) << "invalid column index range: " << col << "; 0 <= x < 8 expected";
        return;
    }

    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_COL | col);

    // pack first 4 elements of list in format 0 1 1 0 to bits
    int top_part = 0;
    const size_t LEFT_OFF = 1;
    const size_t LEFT_N = std::min<size_t>(4 + LEFT_OFF, l.size());
    for (size_t i = LEFT_OFF; i < LEFT_N; i++) {
        auto v = l[i].asFloat(0);
        if (v != 0)
            top_part |= (1 << (i - LEFT_OFF));
    }

    floatTo(0, top_part);

    // pack  4 elements of list in format 0 1 1 0 to bits
    int bottom_part = 0;
    const size_t RIGHT_OFF = 5;
    const size_t RIGHT_N = std::min<size_t>(4 + RIGHT_OFF, l.size());
    for (size_t i = RIGHT_OFF; i < RIGHT_N; i++) {
        auto v = l[i].asFloat(0);
        if (v != 0)
            bottom_part |= (1 << (i - RIGHT_OFF));
    }

    floatTo(0, bottom_part);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_row(t_symbol* s, const AtomListView& l)
{
    // args example: 2 0 1 1 0 1 1
    // 3rd row with values 0 1 1 0 1 1
    if (l.size() < 2) {
        METHOD_ERR(s) << "ROW_IDX VALUE [VALUE..] expected: " << l;
        return;
    }

    int row = l.intAt(0, 0);
    if (row < 0 || row > 5) {
        METHOD_ERR(s) << "invalid row index range: " << row << "; 0 <= x < 6 expected";
        return;
    }

    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_ROW | row);

    // pack first 4 elements of list in format 0 1 1 0 to bits
    int left_part = 0;
    const size_t LEFT_OFF = 1;
    const size_t LEFT_N = std::min<size_t>(4 + LEFT_OFF, l.size());
    for (size_t i = LEFT_OFF; i < LEFT_N; i++) {
        auto v = l[i].asFloat(0);
        if (v != 0)
            left_part |= (1 << (i - LEFT_OFF));
    }

    floatTo(0, left_part);

    // pack  4 elements of list in format 0 1 1 0 to bits
    int right_part = 0;
    const size_t RIGHT_OFF = 5;
    const size_t RIGHT_N = std::min<size_t>(4 + RIGHT_OFF, l.size());
    for (size_t i = RIGHT_OFF; i < RIGHT_N; i++) {
        auto v = l[i].asFloat(0);
        if (v != 0)
            right_part |= (1 << (i - RIGHT_OFF));
    }

    floatTo(0, right_part);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_fill(t_symbol* s, const AtomListView& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_FILL);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_get_version(t_symbol* s, const AtomListView& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_DEVICE);
    floatTo(0, CMD_DEVICE_VERSION);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_invert(t_symbol* s, const AtomListView& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_INVERT);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_str(t_symbol* s, const AtomListView& l)
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

void ProtoSpAlpaca::m_sync(t_symbol* s, const AtomListView& l)
{
    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_DEVICE);
    floatTo(0, CMD_DEVICE_SYNC);
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_mode(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_INT, ARG_SYMBOL, s))
        return;

    const int ch = l.intAt(0, 0);
    if (ch < 0 || ch > 1) {
        METHOD_ERR(s) << "invalid channel: " << ch << "; 0 or 1 expected";
        return;
    }

    t_symbol* mode = l.symbolAt(1, &s_);
    if (mode == SYM_MODE_NONE) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_NONE);
        floatTo(0, CMD_END);
    } else if (mode == SYM_MODE_ANALOG) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_ANALOG);
        floatTo(0, CMD_END);
    } else if (mode == SYM_MODE_ANALOG_RAW) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_ANALOG_RAW);
        floatTo(0, CMD_END);
    } else if (mode == SYM_MODE_DIGITAL1) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_DIGITAL1);
        floatTo(0, CMD_END);
    } else if (mode == SYM_MODE_DIGITAL2) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_DIGITAL2);
        floatTo(0, CMD_END);
    } else if (mode == SYM_MODE_DIGITAL_BOTH) {
        floatTo(0, CMD_START);
        floatTo(0, CMD_TARGET | ch);
        floatTo(0, CMD_JACK_SET_MODE | MODE_DIGITAL_BOTH);
        floatTo(0, CMD_END);
    } else {
        METHOD_ERR(s) << "unknown mode: " << mode->s_name;
    }
}

void ProtoSpAlpaca::m_brightness(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_FLOAT, s))
        return;

    const float v = l.floatAt(0, 0);
    if (v < 0 || v > 1) {
        METHOD_ERR(s) << "brightness should be in range [0-1]; given value: " << v;
        return;
    }

    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, CMD_MATRIX_SET_BRIGHTNESS);
    floatTo(0, int(v * 127));
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_pixel(t_symbol* s, const AtomListView& l)
{
    if (!checkArgs(l, ARG_INT, ARG_INT, ARG_INT)) {
        METHOD_ERR(s) << " X Y 1|0 args expected: " << l;
        return;
    }

    int x = l.intAt(0, 0);
    int y = l.intAt(1, 0);
    int state = l.intAt(2, 1);

    if (x < 0 || x > 7) {
        METHOD_ERR(s) << "x coordinate should be in [0-7] range: " << x;
        return;
    }

    if (y < 0 || y > 5) {
        METHOD_ERR(s) << "y coordinate should be in [0-5] range: " << y;
        return;
    }

    if (!(state == 0 || state == 1)) {
        METHOD_ERR(s) << "pixel state (0 or 1) expected: " << state;
        return;
    }

    floatTo(0, CMD_START);
    floatTo(0, CMD_TARGET | CMD_TARGET_MATRIX);
    floatTo(0, state ? CMD_MATRIX_SET_PIXEL : CMD_MATRIX_CLEAR_PIXEL);
    floatTo(0, (0x7 & x) | ((0x7 & y) << 4));
    floatTo(0, CMD_END);
}

void ProtoSpAlpaca::m_char(t_symbol* s, const AtomListView& l)
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
    SYM_ANALOG = gensym("analog");
    SYM_ANALOG_RAW = gensym("analog_raw");
    SYM_DIGITAL = gensym("digital");
    SYM_EMPTY = gensym("empty");
    SYM_MODE = gensym("mode");
    SYM_MODE_ANALOG = gensym("analog");
    SYM_MODE_ANALOG_RAW = gensym("analog_raw");
    SYM_MODE_DIGITAL1 = gensym("digital1");
    SYM_MODE_DIGITAL2 = gensym("digital2");
    SYM_MODE_DIGITAL_BOTH = gensym("digital");
    SYM_MODE_NONE = gensym("none");
    SYM_NO_TARGET = gensym("no_target");
    SYM_OK = gensym("ok");
    SYM_RESPONSE = gensym("response");
    SYM_UNKNOWN_COMMAND = gensym("unknown_command");
    SYM_UNKNOWN_TARGET = gensym("unknown_target");
    SYM_VERSION = gensym("version");

    ObjectFactory<ProtoSpAlpaca> obj("proto.sp.alpaca");
    obj.addMethod("brightness", &ProtoSpAlpaca::m_brightness);
    obj.addMethod("char", &ProtoSpAlpaca::m_char);
    obj.addMethod("clear", &ProtoSpAlpaca::m_clear);
    obj.addMethod("col", &ProtoSpAlpaca::m_col);
    obj.addMethod("fill", &ProtoSpAlpaca::m_fill);
    obj.addMethod("invert", &ProtoSpAlpaca::m_invert);
    obj.addMethod("mode", &ProtoSpAlpaca::m_mode);
    obj.addMethod("pixel", &ProtoSpAlpaca::m_pixel);
    obj.addMethod("row", &ProtoSpAlpaca::m_row);
    obj.addMethod("str", &ProtoSpAlpaca::m_str);
    obj.addMethod("sync", &ProtoSpAlpaca::m_sync);
    obj.addMethod("version?", &ProtoSpAlpaca::m_get_version);
}

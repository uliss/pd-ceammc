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
#include "proto_midi_cc.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>
#include <tuple>

enum {
    CC_BANK_SELECT = 0,
    CC_MOD_WHEEL_COARSE = 1,
    //
    CC_PAN_POSITION_COARSE = 10,
    CC_RPN_COARSE = 101,
    CC_RPN_FINE = 100,
    CC_DATA_ENTRY_COARSE = 6,
    CC_DATA_ENTRY_FINE = 38,
    CC_DATA_INCREMENT = 96,
    CC_DATA_DECREMENT = 97,
};

constexpr const char* SEL_BANK_SELECT = "bankselect";
constexpr const char* SEL_MOD_WHEEL_COARSE = "modwheel~";
constexpr const char* SEL_PAN_POSITION_COARSE = "panpos~";
constexpr const char* SEL_RPN = "rpn";
constexpr const char* SEL_RPN_RESET = "rpn_reset";

ProtoMidiCC::ProtoMidiCC(const PdArgs& args)
    : BaseObject(args)
    , as_list_(nullptr)
    , mod_wheel0_(0)
    , mod_wheel1_(0)
    , pan_pos0_(0)
    , pan_pos1_(0)
    , rpn0_(0)
    , rpn1_(0)
{
    createOutlet();

    as_list_ = new FlagProperty("@list");
    addProperty(as_list_);

    using midi::Byte;
    parser_.setControlChangeFn([this](Byte b, Byte c, Byte v) {
        onCC(b, c, v);
    });
}

void ProtoMidiCC::initDone()
{
    if (as_list_->value())
        buffer_.reserve(32);
}

void ProtoMidiCC::onFloat(t_float f)
{
    if (f < 0 || f > 0xff) {
        OBJ_ERR << "byte value expected in 0-255 range, got: " << f;
        return;
    }

    auto res = parser_.push(f);
    if (res.err != midi::MidiParser::NO_ERROR) {
        OBJ_ERR << res.errStr();
    }
}

void ProtoMidiCC::onList(const AtomList& lst)
{
    for (auto& a : lst)
        onFloat(a.asT<t_float>());
}

void ProtoMidiCC::m_bend_sens(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT)) {
        METHOD_ERR(s) << "sensivity in semitones expected, got: " << lv;
        return;
    }

    auto f = lv[0].asT<t_float>();
    const int semitones = int(f);
    const int cents = std::round((f - (int)f) * 100);
    if (semitones < 0) {
        METHOD_ERR(s) << "non-negative value expected, got: " << lv;
        return;
    }

    sendCCBegin();
    sendCC(0, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RPN_PITCH_BEND_SENSIVITY);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_COARSE, semitones);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_FINE, cents);
    sendCCEnd();
}

void ProtoMidiCC::m_tune_bank_select(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_BYTE, s))
        return;

    sendCCBegin();
    sendCC(0, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RPN_CHANNEL_TUNING_BANK_SELECT);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_FINE, lv[0].asInt());
    sendCCEnd();
}

void ProtoMidiCC::m_tune_prog_change(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_BYTE, s))
        return;

    sendCCBegin();
    sendCC(0, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RPN_CHANNEL_TUNING_PROG_CHANGE);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_FINE, lv[0].asInt());
    sendCCEnd();
}

void ProtoMidiCC::m_tune_fine(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    const float tune = lv[0].asT<t_float>();
    if (tune < -100 || tune > 100) {
        METHOD_ERR(s) << "expected fine tuning (in cents) in [-100..+100] range, got: " << lv;
        return;
    }

    sendTuneFine(tune);
}

void ProtoMidiCC::m_tune_coarse(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, s))
        return;

    const int tune = lv[0].asT<int>();
    if (tune < -64 || tune > 63) {
        METHOD_ERR(s) << "expected coarse tuning (in semitones) in [-64..+63] range, got: " << lv;
        return;
    }

    sendTuneCoarse(tune);
}

void ProtoMidiCC::m_tune_semi(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    const auto tune = lv[0].asT<t_float>();
    if (tune < -64 || tune > 63) {
        METHOD_ERR(s) << "expected tuning (in semitones) in [-64..+63.0] range, got: " << lv;
        return;
    }

    t_float semi = 0;
    const t_float cents = std::modf(tune, &semi) * 100;

    sendTuneCoarse(semi);
    sendTuneFine(cents);
}

void ProtoMidiCC::sendCCBegin()
{
    if (as_list_->value())
        buffer_.clear();
}

void ProtoMidiCC::sendCCEnd()
{
    if (as_list_->value()) {
        const auto N = buffer_.size();
        Atom buf[N];
        for (size_t i = 0; i < N; i++)
            buf[i] = buffer_[i];

        listTo(0, AtomListView(buf, N));
        buffer_.clear();
    }
}

void ProtoMidiCC::onCC(int b, int c, int v)
{
    switch (c) {
    case CC_BANK_SELECT:
        return anyTo(0, gensym(SEL_BANK_SELECT), Atom(v));
    case CC_MOD_WHEEL_COARSE:
        mod_wheel0_ = v;
        return anyTo(0, gensym(SEL_MOD_WHEEL_COARSE), Atom(v));
    case CC_PAN_POSITION_COARSE:
        pan_pos0_ = v;
        return anyTo(0, gensym(SEL_PAN_POSITION_COARSE), Atom(v));
    case CC_RPN_COARSE:
    case CC_RPN_FINE:
    case CC_DATA_ENTRY_COARSE:
    case CC_DATA_ENTRY_FINE:
    case CC_DATA_INCREMENT:
    case CC_DATA_DECREMENT: {
        auto res = rpn_parser_.push(c, v);
        if (res.err != midi::RPNParser::NO_ERROR) {
            rpn_parser_.reset();
            OBJ_ERR << "RPN parser error: ";
        } else if (res.state == midi::RPNParser::ST_DONE) {
            rpn_parser_.reset();
            if (res.rpn == midi::RPNParser::RPN_RESET) {
                return anyTo(0, gensym(SEL_RPN_RESET), AtomListView());
            } else {
                Atom val[2];
                val[0] = res.rpn;
                val[1] = res.value;
                return anyTo(0, gensym(SEL_RPN), AtomListView(val, 2));
            }
        }
    } break;
    default:
        break;
    }
}

void ProtoMidiCC::sendCC(int chan, int cc, int v)
{
    if (as_list_->value()) {
        buffer_.push_back(0xB0 | (0xF & chan));
        buffer_.push_back(0x7F & cc);
        buffer_.push_back(0x7F & v);
    } else {
        floatTo(0, 0xB0 | (0xF & chan));
        floatTo(0, 0x7F & cc);
        floatTo(0, 0x7F & v);
    }
}

void ProtoMidiCC::sendTuneFine(float cents)
{
    const int val = std::round(convert::lin2lin_clip<t_float>(cents, -100, 100, 0, 0x3FFF));
    const uint16_t msb = 0x7F & (val >> 7);
    const uint16_t lsb = 0x7F & val;

    sendCCBegin();
    sendCC(0, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RRN_CHANNEL_TUNING_FINE);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_COARSE, msb);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_FINE, lsb);
    sendCCEnd();
}

void ProtoMidiCC::sendTuneCoarse(int semi)
{
    sendCCBegin();
    sendCC(0, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(0, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RRN_CHANNEL_TUNING_COARSE);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_COARSE, semi + 64);
    sendCC(0, midi::RPNParser::CC_DATA_ENTRY_FINE, 0);
    sendCCEnd();
}

void setup_proto_midi_cc()
{
    ObjectFactory<ProtoMidiCC> obj("proto.midi.cc");
    obj.addMethod("bendsens", &ProtoMidiCC::m_bend_sens);
    obj.addMethod("tunebank", &ProtoMidiCC::m_tune_bank_select);
    obj.addMethod("tuneprog", &ProtoMidiCC::m_tune_prog_change);
    obj.addMethod("tunefine", &ProtoMidiCC::m_tune_fine);
    obj.addMethod("tunecoarse", &ProtoMidiCC::m_tune_coarse);
    obj.addMethod("tunesemi", &ProtoMidiCC::m_tune_semi);
}

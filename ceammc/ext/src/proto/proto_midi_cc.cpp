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
#include "ceammc_args.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "midi_names.h"

#include <cmath>
#include <utility>

constexpr const char* SEL_MOD_WHEEL_COARSE = "modwheel~";
constexpr const char* SEL_PAN_POSITION_INT = "pan:i";
constexpr const char* SEL_PAN_POSITION_FLOAT = "pan:f";
constexpr const char* SEL_PAN_POSITION_COARSE = "pan~";
constexpr const char* SEL_PAN_POSITION_FINE = "pan.";
constexpr const char* SEL_RPN = "rpn";
constexpr const char* SEL_RPN_RESET = "rpn_reset";
constexpr const char* SEL_HOLD_PEDAL = "hold";
constexpr const char* SEL_SOSTENUTO_PEDAL = "sostenuto";

ProtoMidiCC::ProtoMidiCC(const PdArgs& args)
    : BaseObject(args)
    , as_list_(nullptr)
    , mod_wheel0_(0)
    , mod_wheel1_(0)
    , pan_pos0_(0)
    , pan_pos1_(0)
    , rpn0_(0)
    , rpn1_(0)
    , banksel0_(0)
    , banksel1_(0)
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
    static ArgChecker chk("f-64..63 i0..15?");

    if (!chk.check(lv)) {
        METHOD_ERR(s) << "usage: TUNE[-64..63] CHAN[0..15]?, got: " << lv;
        return;
    }

    const auto tune = lv.floatAt(0, 0);
    const int chan = lv.intAt(1, 0);

    t_float semi = 0;
    const t_float cents = std::modf(tune, &semi) * 100;

    sendTuneCoarse(semi, chan);
    sendTuneFine(cents, chan);
}

void ProtoMidiCC::m_pan_fine(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_INT)) {
        usage();
        return;
    }

    const auto chan = lv.intAt(0, 0);
    const auto value = lv.intAt(1, 0);

    if (!checkChan(chan)) {
        usage();
        return;
    }

    if (!checkByteValue(value)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_PAN_POSITION_FINE, value);
    sendCCEnd();
}

void ProtoMidiCC::m_pan_coarse(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_INT)) {
        usage();
        return;
    }

    const auto chan = lv.intAt(0, 0);
    const auto value = lv.intAt(1, 0);

    if (!checkChan(chan)) {
        usage();
        return;
    }

    if (!checkByteValue(value)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_PAN_POSITION_COARSE, value);
    sendCCEnd();
}

void ProtoMidiCC::m_pan_float(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE[-1..+1], got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_FLOAT)) {
        usage();
        return;
    }

    const auto chan = lv.intAt(0, 0);
    const auto value = lv.floatAt(1, 0);

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    const auto data = panToBit14(value);
    sendCC(chan, CC_PAN_POSITION_COARSE, data.first);
    sendCC(chan, CC_PAN_POSITION_FINE, data.second);
    sendCCEnd();
}

void ProtoMidiCC::m_pan_int(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN PAN|(MSB LSB), got: " << lv; };

    int chan = 0;
    int msb = 0;
    int lsb = 0;

    if (lv.size() == 2 && lv[0].isInteger() && lv[1].isInteger()) {
        chan = lv.intAt(0, 0);
        const int value = lv.intAt(1, 0);

        if (!checkChan(chan)) {
            usage();
            return;
        }

        if (value < 0 || value > 0x3FFF) {
            METHOD_ERR(s) << "invalid pan value, expected int in [0..0x3FFF] range, got: " << value;
            usage();
            return;
        }

        msb = 0x7F & (value >> 7);
        lsb = 0x7F & value;

    } else if (lv.size() == 3 && lv[0].isInteger() && lv[1].isInteger() && lv[2].isInteger()) {
        chan = lv.intAt(0, 0);

        if (!checkChan(chan)) {
            usage();
            return;
        }

        msb = lv.intAt(1, 0);
        if (!checkByteValue(msb)) {
            usage();
            return;
        }

        lsb = lv.intAt(2, 0);
        if (!checkByteValue(lsb)) {
            usage();
            return;
        }
    } else {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_PAN_POSITION_COARSE, msb);
    sendCC(chan, CC_PAN_POSITION_FINE, lsb);
    sendCCEnd();
}

void ProtoMidiCC::m_banksel_msb(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN BANK_MSB, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BYTE)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int msb = lv[1].asInt();

    if (!checkChan(chan) || msb < 0 || msb > 0x7F) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_BANK_SELECT_MSB, msb);
    sendCCEnd();
}

void ProtoMidiCC::m_banksel_lsb(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN BANK_LSB, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BYTE)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int lsb = lv[1].asInt();

    if (!checkChan(chan) || lsb < 0 || lsb > 0x7F) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_BANK_SELECT_LSB, lsb);
    sendCCEnd();
}

void ProtoMidiCC::m_banksel_int(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN BANK, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_INT)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int val = lv[1].asInt();

    if (!checkChan(chan) || val < 0 || val > 0x3FFF) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_BANK_SELECT_MSB, (val >> 7) & 0x7F);
    sendCCEnd();
    sendCCBegin();
    sendCC(chan, CC_BANK_SELECT_LSB, (val & 0x7F));
    sendCCEnd();
}

void ProtoMidiCC::m_hold_pedal(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE(0|1), got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BOOL)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int on = lv[1].asBool();

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_HOLD_PEDAL, on ? 127 : 0);
    sendCCEnd();
}

void ProtoMidiCC::m_sostenuto_pedal(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE(0|1), got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BOOL)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int on = lv[1].asBool();

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_SOSTENUTO_PEDAL, on ? 127 : 0);
    sendCCEnd();
}

void ProtoMidiCC::m_all_soundsOff(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN, got: " << lv; };

    if (!checkArgs(lv, ARG_INT) || !checkChan(lv[0].asInt())) {
        usage();
        return;
    }

    int chan = lv[0].asInt();

    sendCCBegin();
    sendCC(chan, CC_ALL_SOUND_OFF, 0x7F);
    sendCCEnd();
}

void ProtoMidiCC::m_volume_coarse(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BYTE)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int v = lv[1].asInt();

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_VOLUME_COARSE, v & 0x7F);
    sendCCEnd();
}

void ProtoMidiCC::m_volume_fine(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BYTE)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int v = lv[1].asInt();

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_VOLUME_FINE, v & 0x7F);
    sendCCEnd();
}

void ProtoMidiCC::m_volume_float(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE, got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_FLOAT)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int v = convert::lin2lin_clip<t_float, 0, 1>(lv[1].asFloat(), 0, 0x3FFF);

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_VOLUME_COARSE, (v >> 7) & 0x7F);
    sendCCEnd();
    sendCCBegin();
    sendCC(chan, CC_VOLUME_FINE, v & 0x7F);
    sendCCEnd();
}

void ProtoMidiCC::m_portamento_switch(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN VALUE(0|1), got: " << lv; };

    if (!checkArgs(lv, ARG_INT, ARG_BOOL)) {
        usage();
        return;
    }

    int chan = lv[0].asInt();
    int on = lv[1].asBool();

    if (!checkChan(chan)) {
        usage();
        return;
    }

    sendCCBegin();
    sendCC(chan, CC_PORTAMENO_SWITCH, on ? 127 : 0);
    sendCCEnd();
}

void ProtoMidiCC::m_all_notesOff(t_symbol* s, const AtomListView& lv)
{
    auto usage = [&]() { METHOD_ERR(s) << "usage: CHAN, got: " << lv; };

    if (!checkArgs(lv, ARG_INT) || !checkChan(lv[0].asInt())) {
        usage();
        return;
    }

    int chan = lv[0].asInt();

    sendCCBegin();
    sendCC(chan, CC_ALL_NOTES_OFF, 0x7F);
    sendCCEnd();
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

void ProtoMidiCC::onCC(int chan, int cc, int v)
{
    switch (cc) {
    case CC_BANK_SELECT_MSB: {
        banksel0_ = v;
        handleBankSelectMsb(chan);
        handleBankSelect(chan);
        return;
    }
    case CC_BANK_SELECT_LSB: {
        banksel1_ = v;
        handleBankSelectLsb(chan);
        handleBankSelect(chan);
        return;
    }
    case CC_MOD_WHEEL_COARSE:
        mod_wheel0_ = v;
        return anyTo(0, gensym(SEL_MOD_WHEEL_COARSE), Atom(v));
    case CC_PAN_POSITION_COARSE: {
        pan_pos0_ = v;
        handlePanPositionCoarse(chan);
        handlePanPosition(chan);
        return;
    }
    case CC_PAN_POSITION_FINE: {
        pan_pos1_ = v;
        handlePanPositionFine(chan);
        handlePanPosition(chan);
        return;
    }
    case CC_HOLD_PEDAL: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(SEL_HOLD_PEDAL), AtomListView(data, 2));
    }
    case CC_PORTAMENO_SWITCH: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(M_PORTAMENTO_SWITCH), AtomListView(data, 2));
    }
    case CC_SOSTENUTO_PEDAL: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(SEL_SOSTENUTO_PEDAL), AtomListView(data, 2));
    }
    case CC_ALL_NOTES_OFF: {
        Atom data(chan);
        return anyTo(0, gensym(M_ALL_NOTES_OFF), AtomListView(data));
    }
    case CC_ALL_SOUND_OFF: {
        Atom data(chan);
        return anyTo(0, gensym(M_ALL_SOUND_OFF), AtomListView(data));
    }
    case CC_VOLUME_COARSE: {
        vol0_ = v;
        handleVolumeCoarse(chan);
        handleVolume(chan);
        return;
    }
    case CC_VOLUME_FINE: {
        vol0_ = v;
        handleVolumeFine(chan);
        handleVolume(chan);
        return;
    }
    case CC_RPN_COARSE:
    case CC_RPN_FINE:
    case CC_DATA_ENTRY_COARSE:
    case CC_DATA_ENTRY_FINE:
    case CC_DATA_INCREMENT:
    case CC_DATA_DECREMENT: {
        if (chan < 0 || chan > 15) {
            OBJ_ERR << "invalid midi channel value: " << chan;
            return;
        }

        auto res = rpn_parser_[chan].push(cc, v);
        if (res.err != midi::RPNParser::NO_ERROR) {
            rpn_parser_[chan].reset();
            OBJ_ERR << "RPN parser error: ";
        } else if (res.state == midi::RPNParser::ST_DONE) {
            rpn_parser_[chan].reset();
            if (res.rpn == midi::RPNParser::RPN_RESET) {
                return anyTo(0, gensym(SEL_RPN_RESET), AtomListView(Atom(chan)));
            } else {
                Atom val[3];
                val[0] = chan;
                val[1] = res.rpn;
                val[2] = res.value;
                return anyTo(0, gensym(SEL_RPN), AtomListView(val, 3));
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

void ProtoMidiCC::handleBankSelectMsb(int chan)
{
    const Atom data[2] = { chan, banksel0_ };
    anyTo(0, gensym(M_BANK_SELECT_MSB), AtomListView(data, 2));
}

void ProtoMidiCC::handleBankSelectLsb(int chan)
{
    const Atom data[2] = { chan, banksel1_ };
    anyTo(0, gensym(M_BANK_SELECT_LSB), AtomListView(data, 2));
}

void ProtoMidiCC::handleBankSelect(int chan)
{
    const int val14bit = (pan_pos0_ << 7) | pan_pos1_;
    Atom data[2] = { chan, val14bit };
    anyTo(0, gensym(M_BANK_SELECT), AtomListView(data, 2));
}

void ProtoMidiCC::handleVolumeCoarse(int chan)
{
    const Atom data[2] = { chan, vol0_ };
    anyTo(0, gensym(M_CC_VOLUME_COARSE), AtomListView(data, 2));
}

void ProtoMidiCC::handleVolumeFine(int chan)
{
    const Atom data[2] = { chan, vol1_ };
    anyTo(0, gensym(M_CC_VOLUME_FINE), AtomListView(data, 2));
}

void ProtoMidiCC::handleVolume(int chan)
{
    const t_float vol = convert::lin2lin<t_float, 0, 0x3FFF>((vol0_ << 7) | vol1_, 0, 1);
    const Atom data[2] = { chan, vol };
    anyTo(0, gensym(M_CC_VOLUME_FLOAT), AtomListView(data, 2));
}

void ProtoMidiCC::sendTuneFine(float cents, int chan)
{
    const int val = std::round(convert::lin2lin_clip<t_float>(cents, -100, 100, 0, 0x3FFF));
    const uint16_t msb = 0x7F & (val >> 7);
    const uint16_t lsb = 0x7F & val;

    sendCCBegin();
    sendCC(chan, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(chan, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RRN_CHANNEL_TUNING_FINE);
    sendCC(chan, midi::RPNParser::CC_DATA_ENTRY_COARSE, msb);
    sendCC(chan, midi::RPNParser::CC_DATA_ENTRY_FINE, lsb);
    sendCCEnd();
}

void ProtoMidiCC::sendTuneCoarse(int semi, int chan)
{
    sendCCBegin();
    sendCC(chan, midi::RPNParser::CC_RPN_COARSE, 0);
    sendCC(chan, midi::RPNParser::CC_RPN_FINE, midi::RPNParser::RRN_CHANNEL_TUNING_COARSE);
    sendCC(chan, midi::RPNParser::CC_DATA_ENTRY_COARSE, semi + 64);
    sendCC(chan, midi::RPNParser::CC_DATA_ENTRY_FINE, 0);
    sendCCEnd();
}

void ProtoMidiCC::handlePanPositionCoarse(int chan)
{
    const Atom data[2] = { chan, pan_pos0_ };
    anyTo(0, gensym(SEL_PAN_POSITION_COARSE), AtomListView(data, 2));
}

void ProtoMidiCC::handlePanPositionFine(int chan)
{
    const Atom data[2] = { chan, pan_pos1_ };
    anyTo(0, gensym(SEL_PAN_POSITION_FINE), AtomListView(data, 2));
}

void ProtoMidiCC::handlePanPosition(int chan)
{
    const int val14bit = (pan_pos0_ << 7) | pan_pos1_;
    Atom data[2] = { chan, val14bit };
    anyTo(0, gensym(SEL_PAN_POSITION_INT), AtomListView(data, 2));

    data[1] = bit14ToPan(pan_pos0_, pan_pos1_);
    anyTo(0, gensym(SEL_PAN_POSITION_FLOAT), AtomListView(data, 2));
}

bool ProtoMidiCC::checkChan(int chan) const
{
    if (chan < 0 || chan > 15) {
        OBJ_ERR << "invalid midi channel, expected int in [0..15] range, got: " << chan;
        return false;
    } else
        return true;
}

bool ProtoMidiCC::checkByteValue(int value) const
{
    if (value < 0 || value > 0x7F) {
        OBJ_ERR << "invalid value, expected int in [0..127] range, got: " << value;
        return false;
    } else
        return true;
}

std::pair<uint8_t, uint8_t> ProtoMidiCC::panToBit14(t_float v)
{
    v = clip<t_float, -1, 1>(v);
    const uint16_t pan = (v <= 0)
        ? std::round((1 + v) * 0x2000)
        : std::round(0x1fff * v + 0x2000);

    const uint8_t msb = 0x7F & (pan >> 7);
    const uint8_t lsb = 0x7F & pan;

    return { msb, lsb };
}

t_float ProtoMidiCC::bit14ToPan(uint8_t msb, uint8_t lsb)
{
    const uint16_t bit14 = (msb << 7) | lsb;
    return (bit14 <= 0x2000)
        ? (bit14 - 0x2000) / t_float(0x2000)
        : (bit14 - 0x2000) / t_float(0x1FFF);
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

    obj.addMethod(SEL_PAN_POSITION_COARSE, &ProtoMidiCC::m_pan_coarse);
    obj.addMethod(SEL_PAN_POSITION_FINE, &ProtoMidiCC::m_pan_fine);
    obj.addMethod(SEL_PAN_POSITION_FLOAT, &ProtoMidiCC::m_pan_float);
    obj.addMethod(SEL_PAN_POSITION_INT, &ProtoMidiCC::m_pan_int);

    obj.addMethod(SEL_HOLD_PEDAL, &ProtoMidiCC::m_hold_pedal);
    obj.addMethod(SEL_SOSTENUTO_PEDAL, &ProtoMidiCC::m_sostenuto_pedal);

    obj.addMethod(M_BANK_SELECT_MSB, &ProtoMidiCC::m_banksel_msb);
    obj.addMethod(M_BANK_SELECT_LSB, &ProtoMidiCC::m_banksel_lsb);
    obj.addMethod(M_BANK_SELECT_INT, &ProtoMidiCC::m_banksel_int);
    obj.addMethod(M_BANK_SELECT, &ProtoMidiCC::m_banksel_int);

    obj.addMethod(M_ALL_NOTES_OFF, &ProtoMidiCC::m_all_notesOff);
    obj.addMethod(M_ALL_SOUND_OFF, &ProtoMidiCC::m_all_soundsOff);

    obj.addMethod(M_CC_VOLUME_COARSE, &ProtoMidiCC::m_volume_coarse);
    obj.addMethod(M_CC_VOLUME_FINE, &ProtoMidiCC::m_volume_fine);
    obj.addMethod(M_CC_VOLUME_FLOAT, &ProtoMidiCC::m_volume_float);

    obj.addMethod(M_PORTAMENTO_SWITCH, &ProtoMidiCC::m_portamento_switch);
}

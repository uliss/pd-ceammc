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
#include "args/argcheck2.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "midi_names.h"

#include <cmath>
#include <utility>

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
    , banksel0_(0)
    , banksel1_(0)
    , vol0_(0)
    , vol1_(0)
    , expr0_(0)
    , expr1_(0)
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

void ProtoMidiCC::onList(const AtomListView& lv)
{
    for (auto& a : lv)
        onFloat(a.asT<t_float>());
}

void ProtoMidiCC::m_bend_sens(t_symbol* s, const AtomListView& lv)
{
    int chan = 0;
    t_float sens = 0;

    if (checkArgs(lv, ARG_INT, ARG_FLOAT)) {
        chan = lv[0].asT<int>();
        sens = lv[1].asT<t_float>();
    } else if (checkArgs(lv, ARG_FLOAT)) {
        sens = lv[0].asT<t_float>();
    } else {
        METHOD_ERR(s) << "CHAN[0..15]? SENS expected, got: " << lv;
        return;
    }

    if (!checkChan(chan))
        return;

    const int semitones = int(sens);
    const int cents = std::round((sens - (int)sens) * 100);
    if (semitones < 0) {
        METHOD_ERR(s) << "non-negative value expected, got: " << lv;
        return;
    }

    rpnSend(chan, midi::RPNParser::RPN_PITCH_BEND_SENSIVITY, semitones, cents);
}

void ProtoMidiCC::m_tune_bank_select(t_symbol* s, const AtomListView& lv)
{
    Data2 data;
    if (!getCCByte(s, lv, data))
        return;

    rpnSend(data.chan, midi::RPNParser::RPN_CHANNEL_TUNING_BANK_SELECT, 0, data.value);
}

void ProtoMidiCC::m_tune_select(t_symbol* s, const AtomListView& lv)
{
    int chan = 0;
    int bank = 0;
    int prog = 0;

    if (checkArgs(lv, ARG_INT, ARG_INT, ARG_INT)) {
        chan = lv[0].asT<int>();
        bank = lv[1].asT<int>();
        prog = lv[2].asT<int>();
    } else if (checkArgs(lv, ARG_INT, ARG_INT)) {
        bank = lv[0].asT<int>();
        prog = lv[1].asT<int>();
    } else {
        METHOD_ERR(s) << "CHAN[0..15]? TUNE_BANK[0..127] TUNE_PROG[0..127] expected, got: " << lv;
        return;
    }

    if (!checkChan(chan))
        return;

    if (bank < 0 || bank > 127) {
        METHOD_ERR(s) << "invalid tuning bank value: " << bank;
        return;
    }

    if (prog < 0 || prog > 127) {
        METHOD_ERR(s) << "invalid tuning prog value: " << bank;
        return;
    }

    rpnSend(chan, midi::RPNParser::RPN_CHANNEL_TUNING_BANK_SELECT, 0, bank);
    rpnSend(chan, midi::RPNParser::RPN_CHANNEL_TUNING_PROG_CHANGE, 0, prog);
}

void ProtoMidiCC::m_tune_prog_change(t_symbol* s, const AtomListView& lv)
{
    Data2 data;
    if (!getCCByte(s, lv, data))
        return;

    rpnSend(data.chan, midi::RPNParser::RPN_CHANNEL_TUNING_PROG_CHANGE, 0, data.value);
}

void ProtoMidiCC::m_tune_fine(t_symbol* s, const AtomListView& lv)
{
    const auto data = getCCValue(s, lv);

    if (data.chan < 0 || (data.value < -100 || data.value > 100)) {
        METHOD_ERR(s) << "expected fine tuning (in cents) in [-100..+100] range, got: " << lv;
        return;
    }

    sendTuneFine(data.value, data.chan);
}

void ProtoMidiCC::m_tune_coarse(t_symbol* s, const AtomListView& lv)
{
    const auto data = getCCValue(s, lv);

    if (data.chan < 0 || (data.value < -64 || data.value > 63)) {
        METHOD_ERR(s) << "usage: CHAN[0..15]? TUNE[-64..63], got: " << lv;
        return;
    }

    sendTuneCoarse(data.value, data.chan);
}

void ProtoMidiCC::m_tune_semi(t_symbol* s, const AtomListView& lv)
{
    const auto data = getCCValue(s, lv);

    if (data.chan < 0 || (data.value < -64 || data.value > 63)) {
        METHOD_ERR(s) << "usage: CHAN[0..15]? TUNE[-64..63], got: " << lv;
        return;
    }

    t_float semi = 0;
    const t_float cents = std::modf(data.value, &semi) * 100;

    sendTuneCoarse(semi, data.chan);
    sendTuneFine(cents, data.chan);
}

void ProtoMidiCC::m_pan_fine(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_PAN_POSITION_FINE);
}

void ProtoMidiCC::m_pan_coarse(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_PAN_POSITION_COARSE);
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

    ccBegin();
    const auto data = panToBit14(value);
    ccSet(chan, CC_PAN_POSITION_COARSE, data.first);
    ccSet(chan, CC_PAN_POSITION_FINE, data.second);
    ccSend();
}

void ProtoMidiCC::m_pan_int(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCInt14(s, lv);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_PAN_POSITION_COARSE, CC_PAN_POSITION_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_banksel_msb(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_BANK_SELECT_MSB);
}

void ProtoMidiCC::m_banksel_lsb(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_BANK_SELECT_LSB);
}

void ProtoMidiCC::m_banksel_int(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCInt14(s, lv);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_BANK_SELECT_MSB, CC_BANK_SELECT_LSB, data.value);
    ccSend();
}

void ProtoMidiCC::m_hold_pedal(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCBool(s, lv);
    if (data.chan < 0)
        return;

    ccSend(data.chan, CC_HOLD_PEDAL, data.value);
}

void ProtoMidiCC::m_sostenuto_pedal(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCBool(s, lv);
    if (data.chan < 0)
        return;

    ccSend(data.chan, CC_SOSTENUTO_PEDAL, data.value);
}

void ProtoMidiCC::m_allSoundOff(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,15]?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, -1);
    if (chan < 0) {
        for (int i = 0; i < 16; i++)
            ccSend(i, CC_ALL_SOUND_OFF, 0x7F);
    } else {
        ccSend(chan, CC_ALL_SOUND_OFF, 0x7F);
    }
}

void ProtoMidiCC::m_volume_coarse(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_VOLUME_COARSE);
}

void ProtoMidiCC::m_volume_fine(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_VOLUME_FINE);
}

void ProtoMidiCC::m_volume_float(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCFloat(s, lv, 0, 0x3FFF);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_VOLUME_COARSE, CC_VOLUME_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_volume_int(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCInt14(s, lv);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_VOLUME_COARSE, CC_VOLUME_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_portamento(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_PORTAMENTO);
}

void ProtoMidiCC::m_portamento_switch(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCBool(s, lv);
    if (data.chan < 0)
        return;

    ccSend(data.chan, CC_PORTAMENTO_SWITCH, data.value);
}

void ProtoMidiCC::m_mod_fine(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_MOD_WHEEL_FINE);
}

void ProtoMidiCC::m_mod_coarse(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_MOD_WHEEL_COARSE);
}

void ProtoMidiCC::m_mod_float(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCFloat(s, lv, 0, 0x3FFF);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_MOD_WHEEL_COARSE, CC_MOD_WHEEL_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_mod_int(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCInt14(s, lv);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_MOD_WHEEL_COARSE, CC_MOD_WHEEL_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_exp_fine(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_EXPRESSION_FINE);
}

void ProtoMidiCC::m_exp_coarse(t_symbol* s, const AtomListView& lv)
{
    sendCCbyte(s, lv, CC_EXPRESSION_COARSE);
}

void ProtoMidiCC::m_exp_float(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCFloat(s, lv, 0, 0x3FFF);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_EXPRESSION_COARSE, CC_EXPRESSION_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_exp_int(t_symbol* s, const AtomListView& lv)
{
    auto data = getCCInt14(s, lv);
    if (data.chan < 0)
        return;

    ccBegin();
    ccSet14(data.chan, CC_EXPRESSION_COARSE, CC_EXPRESSION_FINE, data.value);
    ccSend();
}

void ProtoMidiCC::m_allNotesOff(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,15]?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, -1);
    if (chan < 0) {
        for (int i = 0; i < 16; i++)
            ccSend(i, CC_ALL_NOTES_OFF, 0x7F);
    } else {
        ccSend(chan, CC_ALL_NOTES_OFF, 0x7F);
    }
}

void ProtoMidiCC::ccBegin()
{
    if (as_list_->value())
        buffer_.clear();
}

void ProtoMidiCC::ccSend()
{
    if (as_list_->value()) {
        const auto N = buffer_.size();
        AtomList256 buf;
        buf.reserve(N);

        for (size_t i = 0; i < N; i++)
            buf.push_back(buffer_[i]);

        listTo(0, buf.view());
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
    case CC_MOD_WHEEL_COARSE: {
        mod_wheel0_ = v;
        handleModWheelCoarse(chan);
        handleModWheel(chan);
        return;
    }
    case CC_MOD_WHEEL_FINE: {
        mod_wheel1_ = v;
        handleModWheelFine(chan);
        handleModWheel(chan);
        return;
    }
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
    case CC_EXPRESSION_COARSE: {
        expr0_ = v;
        handleExpressionCoarse(chan);
        handleExpression(chan);
        return;
    }
    case CC_EXPRESSION_FINE: {
        expr1_ = v;
        handleExpressionFine(chan);
        handleExpression(chan);
        return;
    }
    case CC_HOLD_PEDAL: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(M_HOLD_PEDAL), AtomListView(data, 2));
    }
    case CC_PORTAMENTO: {
        AtomArray<2> data { chan, v };
        return anyTo(0, gensym(M_PORTAMENTO), data.view());
    }
    case CC_PORTAMENTO_SWITCH: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(M_PORTAMENTO_SWITCH), AtomListView(data, 2));
    }
    case CC_SOSTENUTO_PEDAL: {
        Atom data[2] = { chan, v > 63 };
        return anyTo(0, gensym(M_SOSTENUTO_PEDAL), AtomListView(data, 2));
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
        vol1_ = v;
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

void ProtoMidiCC::ccSet(int chan, int cc, int v)
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

void ProtoMidiCC::ccSet14(int chan, int cc0, int cc1, int v)
{
    ccSet(chan, cc0, 0x7F & (v >> 7));
    ccSet(chan, cc1, 0x7F & v);
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
    auto v = (vol0_ << 7) | vol1_;
    Atom data[2] = { chan, v };
    anyTo(0, gensym(M_CC_VOLUME_INT), AtomListView(data, 2));

    data[1] = t_float(v) / 0x3FFF;
    anyTo(0, gensym(M_CC_VOLUME_FLOAT), AtomListView(data, 2));
}

void ProtoMidiCC::sendTuneFine(float cents, int chan)
{
    const int val = std::round(convert::lin2lin_clip<t_float>(cents, -100, 100, 0, 0x3FFF));
    const uint16_t msb = 0x7F & (val >> 7);
    const uint16_t lsb = 0x7F & val;

    rpnSend(chan, midi::RPNParser::RRN_CHANNEL_TUNING_FINE, msb, lsb);
}

void ProtoMidiCC::sendTuneCoarse(int semi, int chan)
{
    rpnSend(chan, midi::RPNParser::RRN_CHANNEL_TUNING_COARSE, semi + 64, 0);
}

void ProtoMidiCC::handlePanPositionCoarse(int chan)
{
    const Atom data[2] = { chan, pan_pos0_ };
    anyTo(0, gensym(M_PAN_POSITION_COARSE), AtomListView(data, 2));
}

void ProtoMidiCC::handlePanPositionFine(int chan)
{
    const Atom data[2] = { chan, pan_pos1_ };
    anyTo(0, gensym(M_PAN_POSITION_FINE), AtomListView(data, 2));
}

void ProtoMidiCC::handlePanPosition(int chan)
{
    const int val14bit = (pan_pos0_ << 7) | pan_pos1_;
    Atom data[2] = { chan, val14bit };
    anyTo(0, gensym(M_PAN_POSITION_INT), AtomListView(data, 2));

    data[1] = bit14ToPan(pan_pos0_, pan_pos1_);
    anyTo(0, gensym(M_PAN_POSITION_FLOAT), AtomListView(data, 2));
}

void ProtoMidiCC::handleModWheelFine(int chan)
{
    const Atom data[2] = { chan, mod_wheel1_ };
    anyTo(0, gensym(M_MODWHEEL_FINE), AtomListView(data, 2));
}

void ProtoMidiCC::handleModWheelCoarse(int chan)
{
    const Atom data[2] = { chan, mod_wheel0_ };
    anyTo(0, gensym(M_MODWHEEL_COARSE), AtomListView(data, 2));
}

void ProtoMidiCC::handleModWheel(int chan)
{
    auto v = (mod_wheel0_ << 7) | mod_wheel1_;
    Atom data[2] = { chan, v };
    anyTo(0, gensym(M_MODWHEEL_INT), AtomListView(data, 2));

    data[1] = t_float(v) / 0x3FFF;
    anyTo(0, gensym(M_MODWHEEL_FLOAT), AtomListView(data, 2));
}

void ProtoMidiCC::handleExpressionFine(int chan)
{
    const Atom data[2] = { chan, expr1_ };
    anyTo(0, gensym(M_EXPRESSION_FINE), AtomListView(data, 2));
}

void ProtoMidiCC::handleExpressionCoarse(int chan)
{
    const Atom data[2] = { chan, expr0_ };
    anyTo(0, gensym(M_EXPRESSION_COARSE), AtomListView(data, 2));
}

void ProtoMidiCC::handleExpression(int chan)
{
    auto v = (expr0_ << 7) | expr1_;
    Atom data[2] = { chan, v };
    anyTo(0, gensym(M_EXPRESSION_INT), AtomListView(data, 2));

    data[1] = t_float(v) / 0x3FFF;
    anyTo(0, gensym(M_EXPRESSION_FLOAT), AtomListView(data, 2));
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

void ProtoMidiCC::sendCCbyte(t_symbol* s, const AtomListView& lv, uint8_t cc)
{
    Data2 data;
    if (!getCCByte(s, lv, data))
        return;

    ccSend(data.chan, cc, data.value);
}

void ProtoMidiCC::ccSend(int chan, int cc, int v)
{
    ccBegin();
    ccSet(chan, cc, v);
    ccSend();
}

void ProtoMidiCC::rpnSend(int chan, int rpn, int msb, int lsb)
{
    ccBegin();
    ccSet(chan, CC_RPN_COARSE, 0);
    ccSet(chan, midi::RPNParser::CC_RPN_FINE, rpn);
    ccSet(chan, midi::RPNParser::CC_DATA_ENTRY_COARSE, msb);
    ccSet(chan, midi::RPNParser::CC_DATA_ENTRY_FINE, lsb);
    ccSend();
}

ProtoMidiCC::Data2 ProtoMidiCC::getCCBool(t_symbol* s, const AtomListView& lv) const
{
    ProtoMidiCC::Data2 res { -1, -1 };

    if (lv.size() == 1 && lv[0].isInteger()) {
        res.chan = 0;
        res.value = lv[0].asT<bool>() ? 0x7F : 0;
    } else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isInteger()) {
        res.chan = lv[0].asT<int>();
        res.value = lv[1].asT<bool>() ? 0x7F : 0;
    } else {
        METHOD_ERR(s) << "expected CHAN[0..15]? VALUE(0|1), got: " << lv;
        return res;
    }

    if (res.chan < 0 || res.chan > 15) {
        METHOD_ERR(s) << "channel value should be in [0..15] range";
        return { -1, -1 };
    }

    return res;
}

bool ProtoMidiCC::getCCByte(t_symbol* s, const AtomListView& lv, Data2& res) const
{
    if (lv.size() == 1 && lv[0].isInteger()) {
        res.chan = 0;
        res.value = lv[0].asT<int>();
    } else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isInteger()) {
        res.chan = lv[0].asT<int>();
        res.value = lv[1].asT<int>();
    } else {
        METHOD_ERR(s) << "expected CHAN[0..15]? VALUE[0..127], got: " << lv;
        return false;
    }

    if (res.chan < 0 || res.chan > 15) {
        METHOD_ERR(s) << "channel should be in [0..15] range";
        return false;
    }

    if (res.value < 0 || res.value > 127) {
        METHOD_ERR(s) << "value should be in [0..127] range";
        return false;
    }

    return true;
}

ProtoMidiCC::Data2 ProtoMidiCC::getCCInt14(t_symbol* s, const AtomListView& lv) const
{
    ProtoMidiCC::Data2 res { -1, -1 };

    if (lv.size() == 1 && lv[0].isInteger()) {
        res.chan = 0;
        res.value = lv[0].asT<int>();
    } else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isInteger()) {
        res.chan = lv[0].asT<int>();
        res.value = lv[1].asT<int>();
    } else {
        METHOD_ERR(s) << "expected CHAN[0..15]? VALUE[0..16384], got: " << lv;
        return res;
    }

    if (res.chan < 0 || res.chan > 15) {
        METHOD_ERR(s) << "channel should be in [0..15] range";
        return { -1, -1 };
    }

    if (res.value < 0 || res.value > 16384) {
        METHOD_ERR(s) << "value should be in [0..16384] range";
        return { -1, -1 };
    }

    return res;
}

ProtoMidiCC::FData ProtoMidiCC::getCCValue(t_symbol* s, const AtomListView& lv) const
{
    ProtoMidiCC::FData res { -1, -1 };

    t_float value = 0;

    if (lv.size() == 1 && lv[0].isFloat()) {
        res.chan = 0;
        value = lv[0].asT<t_float>();
    } else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isFloat()) {
        res.chan = lv[0].asT<int>();
        value = lv[1].asT<t_float>();
    } else {
        METHOD_ERR(s) << "expected CHAN[0..15]? VALUE, got: " << lv;
        return res;
    }

    if (res.chan < 0 || res.chan > 15) {
        METHOD_ERR(s) << "channel should be in [0..15] range";
        return { -1, -1 };
    }

    res.value = value;
    return res;
}

ProtoMidiCC::Data2 ProtoMidiCC::getCCFloat(t_symbol* s, const AtomListView& lv, int from, int to) const
{
    ProtoMidiCC::Data2 res { -1, -1 };

    t_float value = 0;

    if (lv.size() == 1 && lv[0].isFloat()) {
        res.chan = 0;
        value = lv[0].asT<t_float>();
    } else if (lv.size() == 2 && lv[0].isInteger() && lv[1].isFloat()) {
        res.chan = lv[0].asT<int>();
        value = lv[1].asT<t_float>();
    } else {
        METHOD_ERR(s) << "expected CHAN[0..15]? VALUE[0..1], got: " << lv;
        return res;
    }

    if (res.chan < 0 || res.chan > 15) {
        METHOD_ERR(s) << "channel should be in [0..15] range";
        return { -1, -1 };
    }

    if (value < 0 || value > 1) {
        METHOD_ERR(s) << "value should be in [0..1] range";
        return { -1, -1 };
    }

    res.value = static_cast<int>(convert::lin2lin<t_float>(value, 0, 1, from, to));
    return res;
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

    obj.addMethod(M_TUNE_BANK, &ProtoMidiCC::m_tune_bank_select);
    obj.addMethod(M_TUNE_PROG, &ProtoMidiCC::m_tune_prog_change);

    obj.addMethod(M_TUNE_SELECT, &ProtoMidiCC::m_tune_select);
    obj.addMethod(M_TUNE_FINE, &ProtoMidiCC::m_tune_fine);
    obj.addMethod(M_TUNE_CENTS, &ProtoMidiCC::m_tune_fine);
    obj.addMethod(M_TUNE_COARSE, &ProtoMidiCC::m_tune_coarse);
    obj.addMethod(M_TUNE_SEMITONES, &ProtoMidiCC::m_tune_semi);

    obj.addMethod(M_BEND_SENSIVITY, &ProtoMidiCC::m_bend_sens);

    obj.addMethod(M_PAN_POSITION_COARSE, &ProtoMidiCC::m_pan_coarse);
    obj.addMethod(M_PAN_POSITION_FINE, &ProtoMidiCC::m_pan_fine);
    obj.addMethod(M_PAN_POSITION_FLOAT, &ProtoMidiCC::m_pan_float);
    obj.addMethod(M_PAN_POSITION_INT, &ProtoMidiCC::m_pan_int);

    obj.addMethod(M_HOLD_PEDAL, &ProtoMidiCC::m_hold_pedal);
    obj.addMethod(M_SOSTENUTO_PEDAL, &ProtoMidiCC::m_sostenuto_pedal);

    obj.addMethod(M_BANK_SELECT_MSB, &ProtoMidiCC::m_banksel_msb);
    obj.addMethod(M_BANK_SELECT_LSB, &ProtoMidiCC::m_banksel_lsb);
    obj.addMethod(M_BANK_SELECT_INT, &ProtoMidiCC::m_banksel_int);
    obj.addMethod(M_BANK_SELECT, &ProtoMidiCC::m_banksel_int);

    obj.addMethod(M_ALL_NOTES_OFF, &ProtoMidiCC::m_allNotesOff);
    obj.addMethod(M_ALL_SOUND_OFF, &ProtoMidiCC::m_allSoundOff);
    obj.addMethod(M_PANIC, &ProtoMidiCC::m_allNotesOff);

    obj.addMethod(M_CC_VOLUME_COARSE, &ProtoMidiCC::m_volume_coarse);
    obj.addMethod(M_CC_VOLUME_FINE, &ProtoMidiCC::m_volume_fine);
    obj.addMethod(M_CC_VOLUME_FLOAT, &ProtoMidiCC::m_volume_float);
    obj.addMethod(M_CC_VOLUME_INT, &ProtoMidiCC::m_volume_int);

    obj.addMethod(M_PORTAMENTO_SWITCH, &ProtoMidiCC::m_portamento_switch);
    obj.addMethod(M_PORTAMENTO, &ProtoMidiCC::m_portamento);

    obj.addMethod(M_MODWHEEL_COARSE, &ProtoMidiCC::m_mod_coarse);
    obj.addMethod(M_MODWHEEL_FINE, &ProtoMidiCC::m_mod_fine);
    obj.addMethod(M_MODWHEEL_FLOAT, &ProtoMidiCC::m_mod_float);
    obj.addMethod(M_MODWHEEL_INT, &ProtoMidiCC::m_mod_int);

    obj.addMethod(M_EXPRESSION_COARSE, &ProtoMidiCC::m_exp_coarse);
    obj.addMethod(M_EXPRESSION_FINE, &ProtoMidiCC::m_exp_fine);
    obj.addMethod(M_EXPRESSION_FLOAT, &ProtoMidiCC::m_exp_float);
    obj.addMethod(M_EXPRESSION_INT, &ProtoMidiCC::m_exp_int);
}

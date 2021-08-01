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
#ifndef PROTO_MIDI_CC_H
#define PROTO_MIDI_CC_H

#include <cstdint>

#include "ceammc_object.h"
#include "proto_midi_cc_rpn_parser.h"
#include "proto_midi_parser.h"
using namespace ceammc;

enum {
    CC_BANK_SELECT_MSB = 0,
    CC_MOD_WHEEL_COARSE = 1,
    CC_PORTAMENTO = 5,
    CC_VOLUME_COARSE = 7,
    //
    CC_PAN_POSITION_COARSE = 10,
    CC_BANK_SELECT_LSB = 32,
    CC_VOLUME_FINE = 39,
    CC_PAN_POSITION_FINE = 42,
    CC_HOLD_PEDAL = 64,
    CC_SOSTENUTO_PEDAL = 66,
    CC_SOFT_PEDAL = 67,
    CC_LEGATO_PEDAL = 67,
    CC_RPN_COARSE = 101,
    CC_RPN_FINE = 100,
    CC_DATA_ENTRY_COARSE = 6,

    CC_DATA_ENTRY_FINE = 38,
    CC_DATA_INCREMENT = 96,
    CC_DATA_DECREMENT = 97,
    CC_ALL_NOTES_OFF = 123,
    CC_ALL_SOUND_OFF = 120,
};

class ProtoMidiCC : public BaseObject {
    FlagProperty* as_list_;
    std::vector<uint8_t> buffer_;
    midi::MidiParser parser_;
    midi::RPNParser rpn_parser_[16];
    uint8_t mod_wheel0_, mod_wheel1_;
    uint8_t pan_pos0_, pan_pos1_;
    uint8_t rpn0_, rpn1_;
    uint8_t banksel0_, banksel1_;
    uint8_t vol0_, vol1_;

public:
    ProtoMidiCC(const PdArgs& args);
    void initDone() override;

    void onFloat(t_float f) override;
    void onList(const AtomList& lst) override;

    void m_bend_sens(t_symbol* s, const AtomListView& lv);
    void m_tune_bank_select(t_symbol* s, const AtomListView& lv);
    void m_tune_prog_change(t_symbol* s, const AtomListView& lv);
    void m_tune_fine(t_symbol* s, const AtomListView& lv);
    void m_tune_coarse(t_symbol* s, const AtomListView& lv);
    void m_tune_semi(t_symbol* s, const AtomListView& lv);

    void m_pan_fine(t_symbol* s, const AtomListView& lv);
    void m_pan_coarse(t_symbol* s, const AtomListView& lv);
    void m_pan_float(t_symbol* s, const AtomListView& lv);
    void m_pan_int(t_symbol* s, const AtomListView& lv);

    void m_banksel_msb(t_symbol* s, const AtomListView& lv);
    void m_banksel_lsb(t_symbol* s, const AtomListView& lv);
    void m_banksel_int(t_symbol* s, const AtomListView& lv);

    void m_hold_pedal(t_symbol* s, const AtomListView& lv);
    void m_sostenuto_pedal(t_symbol* s, const AtomListView& lv);

    void m_all_notesOff(t_symbol* s, const AtomListView& lv);
    void m_all_soundsOff(t_symbol* s, const AtomListView& lv);

    void m_volume_coarse(t_symbol* s, const AtomListView& lv);
    void m_volume_fine(t_symbol* s, const AtomListView& lv);
    void m_volume_float(t_symbol* s, const AtomListView& lv);

public:
    static std::pair<uint8_t, uint8_t> panToBit14(t_float v);
    static t_float bit14ToPan(uint8_t msb, uint8_t lsb);

private:
    void sendCCBegin();
    void sendCCEnd();
    void onCC(int chan, int cc, int v);
    void sendCC(int chan, int cc, int v);

    void handleBankSelectMsb(int chan);
    void handleBankSelectLsb(int chan);
    void handleBankSelect(int chan);

    void handleVolumeCoarse(int chan);
    void handleVolumeFine(int chan);
    void handleVolume(int chan);

    void sendTuneFine(float cents, int chan = 0);
    void sendTuneCoarse(int semi, int chan = 0);

    void handlePanPositionFine(int chan);
    void handlePanPositionCoarse(int chan);
    void handlePanPosition(int chan);

    bool checkChan(int chan) const;
    bool checkByteValue(int value) const;
};

void setup_proto_midi_cc();

#endif // PROTO_MIDI_CC_H

/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef FLUID_H
#define FLUID_H

#include "ceammc_sound_external.h"
#include "proto/proto_midi_parser.h"

using namespace ceammc;

struct _fluid_synth_t;
typedef struct _fluid_synth_t fluid_synth_t;

class FluidSynthProperty;

/**
 * @note MIDI channels are 1-based in PureData
 */
class Fluid : public SoundExternal {
    fluid_synth_t* synth_;
    t_symbol* sound_font_;
    midi::MidiParser midi_parser_;

public:
    Fluid(const PdArgs& args);
    ~Fluid() override;

    void initDone() override;

    void onList(const AtomList& lst) final;

    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    bool propSetSoundFont(t_symbol* s);
    AtomList propVersion() const;
    AtomList propSoundFonts() const;

    void m_note(t_symbol* s, const AtomListView& lst);
    void m_cc(t_symbol* s, const AtomListView& lst);
    void m_prog(t_symbol* s, const AtomListView& lst);
    void m_bank(t_symbol* s, const AtomListView& lst);
    void m_gen(t_symbol* s, const AtomListView& lst);
    void m_panic(t_symbol* s, const AtomListView& lst);
    void m_reset(t_symbol* s, const AtomListView& lst);
    void m_notesOff(t_symbol* s, const AtomListView& lst);
    void m_soundsOff(t_symbol* s, const AtomListView& lst);
    void m_sysex(t_symbol* s, const AtomListView& lv);
    void m_midi(t_symbol* s, const AtomListView& lv);

    void m_get_bend_sens(t_symbol* s, const AtomListView& lv);
    void m_set_bend_sens(t_symbol* s, const AtomListView& lv);

    void m_tune_cent(t_symbol* s, const AtomListView& lv);
    void m_tune_semi(t_symbol* s, const AtomListView& lv);

    void m_tune_set_octave(t_symbol* s, const AtomListView& lv);
    void m_tune_select(t_symbol* s, const AtomListView& lv);

    void m_pan(t_symbol* s, const AtomListView& lv);
    void m_hold_pedal(t_symbol* s, const AtomListView& lv);
    void m_sostenuto_pedal(t_symbol* s, const AtomListView& lv);
    void m_soft_pedal(t_symbol* s, const AtomListView& lv);
    void m_legato_pedal(t_symbol* s, const AtomListView& lv);

    void dump() const override;

    /**
     * @note bend value in 0..0x3fff range, with center: 0x2000
     */
    void m_bend(t_symbol* s, const AtomListView& lv);

    /**
     * @note bend value in -0x2000..0x1fff range, with center: 0
     */
    void m_bend_int(t_symbol* s, const AtomListView& lv);

    /**
     * @note bend value in -1..+1 range, with center: 0
     */
    void m_bend_float(t_symbol* s, const AtomListView& lv);

private:
    void select_tune(int bank, int prog);
    void bindMidiParser();

    void set_bend(int chan, int value);
};

void setup_misc_fluid();

#endif // FLUID_H

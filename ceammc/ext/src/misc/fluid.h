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

#include <tuple>

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

    void m_note(t_symbol* s, const AtomListView& lv);
    void m_cc(t_symbol* s, const AtomListView& lv);
    void m_prog(t_symbol* s, const AtomListView& lv);
    void m_bank(t_symbol* s, const AtomListView& lv);
    void m_gen(t_symbol* s, const AtomListView& lst);
    void m_panic(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_notesOff(t_symbol* s, const AtomListView& lv);
    void m_soundsOff(t_symbol* s, const AtomListView& lv);
    void m_sysex(t_symbol* s, const AtomListView& lv);
    void m_midi(t_symbol* s, const AtomListView& lv);
    void m_aftertouch(t_symbol* s, const AtomListView& lv);
    void m_polytouch(t_symbol* s, const AtomListView& lv);

    void m_tune_cent(t_symbol* s, const AtomListView& lv);
    void m_tune_semi(t_symbol* s, const AtomListView& lv);
    void m_tune_octave(t_symbol* s, const AtomListView& lv);
    void m_tune_select(t_symbol* s, const AtomListView& lv);

    void m_pan(t_symbol* s, const AtomListView& lv);
    void m_pan_float(t_symbol* s, const AtomListView& lv);
    void m_pan_int(t_symbol* s, const AtomListView& lv);

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

    void m_set_bend_sens(t_symbol* s, const AtomListView& lv);

private:
    void selectTune(int bank, int prog);
    void bindMidiParser();

    void setBend(t_symbol* method, int chan, int value, const AtomListView& args);
    void setPan(t_symbol* method, int chan, float fluid_pan_value, const AtomListView& args);

    template <typename Fn, typename V>
    void callFluidChannelFn(t_symbol* method, int chan, Fn fn, V value, const char* valueName, const AtomListView& args)
    {
        if (!synth_)
            return;

        constexpr int ALL_CHANNELS = -1;

        const int n = countMidiChannels();

        if (chan == ALL_CHANNELS) {
            for (int i = 0; i < n; i++) {
                if (!fn(synth_, i, value)) {
                    METHOD_ERR(method) << "can't set " << valueName << " for channel[" << (i + 1) << "]: " << args;
                    return;
                }
            }
        } else if (chan >= 0 && chan < n) {
            if (!fn(synth_, chan, value)) {
                METHOD_ERR(method) << "can't set " << valueName << " for channel[" << (chan + 1) << "]: " << args;
                return;
            }
        } else {
            METHOD_ERR(method) << "invalid channel: " << (chan + 1);
        }
    }

    int countMidiChannels() const;

    template <typename T>
    struct ChanValue {
        T value;
        int chan;
        bool ok;
    };

    template <typename T>
    ChanValue<T> channelValue(const AtomListView& lv) const
    {
        if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat())
            return { lv[1].asT<T>(), lv[0].asT<int>() - 1, true };
        else if (lv.size() == 1 && lv[0].isFloat())
            return { lv[0].asT<T>(), -1, true };
        else
            return { 0, 0, false };
    }

    template <typename T>
    bool checkChanValue(t_symbol* method, const char* valueName, const ChanValue<T>& ch, T min, T max) const
    {
        if (ch.value < min || ch.value > max) {
            METHOD_ERR(method) << valueName << " in [" << min << ".." << max << "] range expected, got: " << ch.value;
            return false;
        } else
            return true;
    }

    struct MidiChanValue3 {
        int chan;
        uint8_t n;
        uint16_t value;
        bool ok;
    };

    MidiChanValue3 midiByteValue3(t_symbol* method, const char* nName, const char* vName, const AtomListView& lv) const;

    struct MidiChanValue2 {
        int chan;
        uint16_t value;
        bool ok;
    };

    MidiChanValue2 midiByteValue2(t_symbol* method, const char* valueName, const AtomListView& lv, int maxValue = 127) const;
};

void setup_misc_fluid();

#endif // FLUID_H

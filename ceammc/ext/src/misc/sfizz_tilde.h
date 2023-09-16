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
#ifndef SFIZZ_TILDE_H
#define SFIZZ_TILDE_H

#include "ceammc_data.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
#include "datatype_dict.h"
#include "proto/proto_midi_parser.h"
#include "sfizz.hpp"
using namespace ceammc;

#include <string>
#include <vector>

class SfizzTilde : public SoundExternal {
    sfz::Sfizz sfz_;
    midi::MidiParser midi_parser_;
    SymbolProperty* sf_path_;
    DictAtom cc_lables_;
    DictAtom key_lables_;
    SymbolEnumProperty* tuning_;

public:
    SfizzTilde(const PdArgs& args);

    void setupDSP(t_signal** sig) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void dump() const override;

    void m_note(t_symbol* s, const AtomListView& lv);
    void m_cc(t_symbol* s, const AtomListView& lv);
    void m_prog(t_symbol* s, const AtomListView& lv);
    void m_midi(t_symbol* s, const AtomListView& lv);
    void m_notesOff(t_symbol* s, const AtomListView& lv);
    void m_soundOff(t_symbol*, const AtomListView&);
    void m_aftertouch(t_symbol* s, const AtomListView& lv);
    void m_polytouch(t_symbol* s, const AtomListView& lv);

    /// tune
    void m_tune_octave(t_symbol* s, const AtomListView& lv);
    void m_tune_select(t_symbol* s, const AtomListView& lv);

    /// bend
    /**
     * @note value in 0..0x3fff range with 0x2000 center
     */
    void m_bend(t_symbol* s, const AtomListView& lv);

    /**
     * @note value in -0x2000..0x1fff range with 0x0 center
     */
    void m_bend_int(t_symbol* s, const AtomListView& lv);

    /**
     * @note value in -1..+1 range with 0 center
     */
    void m_bend_float(t_symbol* s, const AtomListView& lv);

    void m_set_bend_sens(t_symbol* s, const AtomListView& lv);

    /// pan
    void m_pan(t_symbol* s, const AtomListView& lv);
    void m_pan_float(t_symbol* s, const AtomListView& lv);
    void m_pan_int(t_symbol* s, const AtomListView& lv);

    /// pedals
    void m_hold_pedal(t_symbol* s, const AtomListView& lv);
    void m_sostenuto_pedal(t_symbol* s, const AtomListView& lv);
    void m_soft_pedal(t_symbol* s, const AtomListView& lv);
    void m_legato_pedal(t_symbol* s, const AtomListView& lv);

private:
    void updateLabels();
    void bindMidiParser();
    void setPan(uint16_t vuint14);

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

public:
    using TuningPreset = std::vector<std::string>;
    using TuningPresetPtr = std::unique_ptr<TuningPreset>;
    using TuningBank = std::vector<TuningPresetPtr>;

private:
    static TuningBank tuning_bank_;
    static bool saveTuning(size_t bank, size_t preset, const std::string& info);
    bool selectTuning(size_t bank, size_t preset);
};

void setup_misc_sfizz_tilde();

#endif // SFIZZ_TILDE_H

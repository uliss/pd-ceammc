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
#include "sfizz.hpp"
using namespace ceammc;

class SfizzTilde : public SoundExternal {
    sfz::Sfizz sfz_;
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
    void m_soundsOff(t_symbol*, const AtomListView&);

    void m_tune_set_octave(t_symbol* s, const AtomListView& lv);

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

private:
    void updateLabels();
};

void setup_misc_sfizz_tilde();

#endif // SFIZZ_TILDE_H

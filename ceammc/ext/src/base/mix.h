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
#ifndef MIX_H
#define MIX_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class Mix : public SoundExternal {
    typedef SmoothLinT<t_float> t_smooth;
    std::vector<t_smooth> gain_;
    std::vector<t_smooth> mute_;
    std::vector<t_smooth> solo_;
    std::vector<int> solo_values_;
    bool is_solo_;
    const size_t n_;
    t_float xfade_time_;

public:
    Mix(const PdArgs& args);

    void onList(const AtomListView& lv) override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** in) override;

    AtomList propValue() const;
    void setPropValue(const AtomListView& lv);
    AtomList propDb() const;
    void setPropDb(const AtomListView& lv);
    AtomList propMute() const;
    void setPropMute(const AtomListView& lv);
    AtomList propSolo() const;
    void setPropSolo(const AtomListView& lv);

    void m_mute(t_symbol* s, const AtomListView& lv);
    void m_solo(t_symbol* s, const AtomListView& lv);
};

void setup_base_mix();

#endif // MIX_H

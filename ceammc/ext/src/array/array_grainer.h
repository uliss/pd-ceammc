/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef ARRAY_GRAINER_H
#define ARRAY_GRAINER_H

#include "array_base.h"
#include "ceammc_array.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
#include "grain.h"
#include "grain_cloud.h"

using namespace ceammc;


class ArrayGrainer : public ArraySoundBase {
    SymbolEnumProperty* sync_;
    FloatProperty* sync_interval_;
    FloatProperty* sync_prob_;
    GrainCloud cloud_;
    std::vector<size_t> onsets_;

public:
    ArrayGrainer(const PdArgs& args);

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void onBang() override;

    void dump() const override;

    void m_align(t_symbol* s, const AtomListView& lv);
    void m_append(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_grain(t_symbol* s, const AtomListView& lv);
    void m_onsets(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
    void m_slice(t_symbol* s, const AtomListView& lv);
    void m_spread(t_symbol* s, const AtomListView& lv);
    void m_shuffle(t_symbol* s, const AtomListView& lv);

    const GrainCloud& cloud() const { return cloud_; }

private:
    void updateGrains();

    void appendGrains(int n, const AtomListView& args);
};

void setup_array_grainer();

#endif // ARRAY_GRAINER_H

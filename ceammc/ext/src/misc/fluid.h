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

using namespace ceammc;

struct _fluid_synth_t;
typedef struct _fluid_synth_t fluid_synth_t;

class Fluid : public SoundExternal {
    fluid_synth_t* synth_;
    t_symbol* sound_font_;

public:
    Fluid(const PdArgs& args);
    ~Fluid() override;

    void onList(const AtomList& lst) final;

    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    AtomList propSoundFont() const;
    void propSetSoundFont(const AtomList& lst);
    AtomList propVersion() const;

    void m_note(t_symbol* s, const AtomList& lst);
    void m_cc(t_symbol* s, const AtomList& lst);
    void m_prog(t_symbol* s, const AtomList& lst);
    void m_bank(t_symbol* s, const AtomList& lst);
    void m_bend(t_symbol* s, const AtomList& lst);
    void m_gen(t_symbol* s, const AtomList& lst);
    void m_panic(t_symbol* s, const AtomList& lst);
    void m_reset(t_symbol* s, const AtomList& lst);
    void m_notesOff(t_symbol* s, const AtomList& lst);
    void m_soundsOff(t_symbol* s, const AtomList& lst);
};

void setup_misc_fluid();

#endif // FLUID_H

/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef MISC_MIKMOD_H
#define MISC_MIKMOD_H

#include "ceammc_object.h"

#include <libmodplug/modplug.h>

using namespace ceammc;

class ModPlug : public SoundExternal {
    t_symbol* path_;
    ModPlugFile* file_;
    bool play_;
    PointerProperty<bool>* play_prop_;
    float pos_;
    t_symbol* func_on_end_;

public:
    ModPlug(const PdArgs& a);
    ~ModPlug();
    void onSymbol(t_symbol* s);
    void processBlock(const t_sample**, t_sample** out);
    void setupDSP(t_signal** sp);

    void m_play(t_symbol*, const AtomList&);
    void m_stop(t_symbol*, const AtomList&);
    void m_pause(t_symbol*, const AtomList&);

    AtomList p_pos() const;
    void p_set_pos(const AtomList& pos);
    AtomList p_name() const;
    AtomList p_len() const;

    AtomList p_rel_pos() const;
    void p_set_rel_pos(const AtomList& pos);

    AtomList p_on_end() const;
    void p_set_on_end(const AtomList& fn);

private:
    void load();
    void unload();

    bool isOpened() const;
};

extern "C" void setup_misc0x2emodplug_tilde();

#endif // MISC_MIKMOD_H

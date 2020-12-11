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

#include "ceammc_sound_external.h"

#include <libmodplug/modplug.h>

using namespace ceammc;

class ModPlug : public SoundExternal {
    t_symbol* path_;
    ModPlugFile* file_;
    PointerProperty<bool>* play_prop_;
    float pos_;
    t_symbol* func_on_end_;
    bool play_;
    bool log_error_;

public:
    ModPlug(const PdArgs& a);
    ~ModPlug();
    void onSymbol(t_symbol* s) override;
    void processBlock(const t_sample**, t_sample** out) override;

    void m_play(t_symbol*, const AtomListView&);
    void m_stop(t_symbol*, const AtomListView&);
    void m_pause(t_symbol*, const AtomListView&);

    t_float p_pos() const;
    bool p_set_pos(t_float pos);
    t_symbol* p_name() const;
    t_float p_len() const;

    t_float p_rel_pos() const;
    bool p_set_rel_pos(t_float pos);
    bool p_set_on_end(t_symbol* s);

    void initDone() override;

private:
    void load();
    void unload();

    bool isOpened() const;
};

void setup_misc_modplug_tilde();

#endif // MISC_MIKMOD_H

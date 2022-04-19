/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef BASE_CLONE_H
#define BASE_CLONE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class BaseClone : public SoundExternal {
    IntProperty* num_;
    SymbolProperty* patch_;
    ListProperty* args_;
    t_canvas* canvas_cont_;
    t_canvas* clone_pattern_;

public:
    BaseClone(const PdArgs& args);
    ~BaseClone();

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;

    void initDone() override;

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) final;

public:
    virtual void onSave(t_binbuf* b);
    virtual void onRestore(const AtomListView& lv);

private:
    bool initInstances(t_symbol* name, const AtomListView& patch_args);
};

void setup_base_clone();

#endif // BASE_CLONE_H

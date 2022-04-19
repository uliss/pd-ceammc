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

class CloneInstance {
    size_t idx_;
    t_canvas* canvas_;

    CloneInstance(const CloneInstance&) = delete;

public:
    CloneInstance(size_t idx, t_canvas* owner);
    CloneInstance(CloneInstance&& ci);
    ~CloneInstance();

    void fillWithPattern(const t_binbuf* pattern);
    void loadbang();
    void clear();
    void open();

    size_t index() const { return idx_; }
};

class BaseClone : public SoundExternal {
    IntProperty* num_;
    ListProperty* args_;
    t_canvas* wrapper_;
    t_canvas* pattern_;
    std::vector<CloneInstance> instances_;

public:
    BaseClone(const PdArgs& args);
    ~BaseClone();

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;

    void initDone() override;

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) final;

    void m_open(t_symbol* s, const AtomListView& lv);

public:
    virtual void onSave(t_binbuf* b);
    virtual void onRestore(const AtomListView& lv);

private:
    bool initInstances(const AtomListView& patch_args);
    bool initInstance(size_t idx, const AtomListView& args);
    void updateInstances();
};

void setup_base_clone();

#endif // BASE_CLONE_H

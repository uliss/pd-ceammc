/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef TL_CUE_NEW_H
#define TL_CUE_NEW_H

#include "ceammc_timeline.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class TlCue : public UIObject {
private:
    tl::CueData data_;
    UIFont font_;
    UITextLayout txt_;
    bool first_draw_;

public:
    TlCue();
    ~TlCue();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void okSize(t_rect* newrect);
    void setDrawParams(t_object*, t_edrawparams* params);
    void paint(t_object* view);
    void updatePos();

public:
    static void setup();

private:
    bool updateCues();
    void redrawCues();
    bool isLayoutFinished();
};

void setup_tl_cue();

#endif // TL_CUE_NEW_H

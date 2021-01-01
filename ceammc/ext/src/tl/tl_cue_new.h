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

#include <unordered_map>

using namespace ceammc;

class TlCue : public UIObject {
private:
    static size_t ref_counter_;
    static std::unordered_map<t_canvas*, int> draw_counter_;

private:
    tl::CueData data_;
    UIFont font_;
    UITextLayout txt_;
    t_symbol* bind_sym_;
    bool first_draw_;
    bool line_created_;

public:
    TlCue();
    ~TlCue();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();
    void updatePos();
    void onZoom(t_float z);
    void onPropChange(t_symbol* prop_name);

    void m_updateLine(const AtomListView& l);
    void syncXPos();
    bool updateCues();

    void visIncrement();
    bool visLast();

public:
    static void setup();

private:
    bool isLayoutFinished();
    void redrawCues();
    void createLine();
    void deleteLine();
    void updateLineCoords();
    void updateLineBackground();
    int lineHeight() const;
};

void setup_tl_cue();

#endif // TL_CUE_NEW_H

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
#ifndef UI_ARRAYVIEW_H
#define UI_ARRAYVIEW_H

#include "ceammc_array.h"
#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

struct WaveInfo {
    float peak_max;
    float peak_min;
    float rms;
};

class UIArrayView : public UIObject {
    std::vector<WaveInfo> buffer_;
    mutable Array array_;
    UILayer control_layer_;
    ClockMemberFunction<UIArrayView> render_clock_;
    size_t render_index_;
    long cursor_sample_pos_;
    UIFont font_;
    UITextLayout label_top_left_;
    UITextLayout label_top_right_;
    UITextLayout label_bottom_left_;
    UITextLayout label_bottom_right_;
    std::string str_label_top_right_;
    std::string str_label_bottom_right_;

public:
    t_symbol* prop_array;
    t_rgba prop_color_wave;
    t_rgba prop_color_cursor;
    int prop_show_labels;
    int prop_show_rms;

public:
    UIArrayView();

    void paint(t_object* view);
    void drawBackground();
    void drawCursor();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void okSize(t_rect* newrect);
    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);

    void onMouseDown(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);

    void onBang();
    void onFloat(t_float f);

    void m_update();
    void m_set(const AtomList& lst);

private:
    bool quickRender();
    void renderTick();
    void renderRange(size_t pos, size_t len);
    void output();

    t_float cursorPosSample() const;
    void setCursorPosSample(t_float pos);

    t_float cursorPosPhase() const;
    void setCursorPosPhase(t_float phase);

    t_float cursorPosMs() const;
    void setCursorPosMs(t_float ms);

    t_float cursorPosSec() const;
    void setCursorPosSec(t_float pos);

    bool checkArray();

public:
    t_float sizeSamples() const;
    t_float sizeSec() const;
    t_float sizeMs() const;

    AtomList labelTopRight() const;
    void setLabelTopRight(const AtomList& lst);
    AtomList labelBottomRight() const;
    void setLabelBottomRight(const AtomList& lst);

    AtomList propArray() const;
    void propSetArray(const AtomList& lst);

public:
    static void setup();
};

void setup_ui_arrayview();

#endif // UI_ARRAYVIEW_H

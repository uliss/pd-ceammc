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

enum EditMode {
    SELECTION_NONE = 0,
    SELECTION_CURSOR,
    SELECTION_RANGE,
    MOVE_RANGE,
    CHANGE_RANGE_LEFT,
    CHANGE_RANGE_RIGHT
};

enum class ErrorMessageMode {
    HIDE,
    SHOW
};

class SelectionRange {
    typedef std::pair<long, long> Range;
    Range range_;

public:
    SelectionRange(long from = 0, long to = 0)
        : range_({ from, to })
    {
    }

    long& from() { return range_.first; }
    long& to() { return range_.second; }
    const long& from() const { return range_.first; }
    const long& to() const { return range_.second; }

    bool operator==(const SelectionRange& r) const
    {
        return range_ == r.range_;
    }

    void set(long from, long to)
    {
        range_.first = from;
        range_.second = to;
    }

    long length() const { return range_.second - range_.first; }
    size_t absLength() const { return std::abs(range_.second - range_.first); }

    bool isNull() const { return length() == 0; }

    void normalize()
    {
        if (range_.second < range_.first)
            std::swap(range_.first, range_.second);
    }
};

class UIArrayView : public UIObject {
    std::vector<WaveInfo> buffer_;
    mutable Array array_;
    UILayer cursor_layer_;
    UILayer wave_layer_;
    ClockMemberFunction<UIArrayView> render_clock_;
    size_t render_index_;
    t_int cursor_sample_pos_;
    UIFont font_;
    UITextLayout label_top_left_;
    UITextLayout label_top_right_;
    UITextLayout label_bottom_left_;
    UITextLayout label_bottom_right_;
    std::string str_label_top_right_;
    std::string str_label_bottom_right_;
    SelectionRange selection_;
    EditMode selection_mode_;
    t_pt prev_mouse_pt_;

public:
    t_symbol* prop_array;
    t_rgba prop_color_wave;
    t_rgba prop_color_cursor;
    t_rgba prop_color_selection;
    int prop_show_labels;
    int prop_show_rms;

public:
    UIArrayView();

    void paint();
    void drawWaveform();
    void drawLabels();
    void drawCursor();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void onZoom(t_float z);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);

    void onBang();
    void onFloat(t_float f);

    void m_update();
    void m_selectSamples(const AtomListView& lv);

private:
    bool quickRender();
    void renderTick();
    void renderRange(size_t pos, size_t len);

    /*
     * draw part of waveform
     * @param p - painter
     * @param pixel_begin - begin pixel x-coord
     * @param pixel_end - end pixel x-coord
     * @param color - waveform color
     */
    void drawWaveformSegment(UIPainter& p, int pixel_begin, int pixel_end, const t_rgba& color);
    void output();

    t_int cursorPosSample() const;
    void setCursorPosSample(t_int pos);

    t_float cursorPosPhase() const;
    void setCursorPosPhase(t_float phase);

    t_float cursorPosMs() const;
    void setCursorPosMs(t_float ms);

    t_float cursorPosSec() const;
    void setCursorPosSec(t_float pos);

    AtomList selectPosSample() const;
    void setSelectPosSample(const AtomListView& pos);

    AtomList selectPosPhase() const;
    void setSelectPosPhase(const AtomListView& pos);

    AtomList selectPosMs() const;
    void setSelectPosMs(const AtomListView& pos);

    AtomList selectPosSec() const;
    void setSelectPosSec(const AtomListView& pos);

    bool isValidArray(ErrorMessageMode msg = ErrorMessageMode::SHOW);

    void invalidateWaveform();
    void invalidateCursor();
    void invalidateAll();

    void redrawSelection();
    void setSelection(long begin, long end);

    EditMode keyMod2EditMode(long mod, int x) const;

public:
    t_int sizeSamples() const;
    t_float sizeSec() const;
    t_float sizeMs() const;

    t_symbol* labelTopRight() const;
    void setLabelTopRight(t_symbol* s);
    t_symbol* labelBottomRight() const;
    void setLabelBottomRight(t_symbol* s);

    AtomList propArray() const;
    void propSetArray(const AtomListView& lv);

public:
    static void setup();

private:
    bool openArray() const;
};

void setup_ui_arrayview();

#endif // UI_ARRAYVIEW_H

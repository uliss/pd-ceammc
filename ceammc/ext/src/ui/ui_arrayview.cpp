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
#include "ui_arrayview.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"

constexpr const char* SYM_ARRAY_NAME = "array";
constexpr const char* SYM_ATTR_SIZE = "size";
constexpr const char* SYM_ATTR_WAVE_COLOR = "wave_color";
constexpr const char* SYM_ATTR_CURSOR_COLOR = "cursor_color";
constexpr const char* SYM_ATTR_SELECTION_COLOR = "selection_color";
constexpr const char* SYM_ATTR_BG_COLOR = PROP_BACKGROUND_COLOR;
constexpr const char* SYM_ATTR_SHOW_LABELS = "show_labels";
constexpr const char* SYM_CURSOR_SAMPLE = "@cursor_samp";
constexpr const char* SYM_CURSOR_PHASE = "@cursor_phase";
constexpr const char* SYM_CURSOR_MS = "@cursor_ms";
constexpr const char* SYM_CURSOR_SEC = "@cursor_sec";

constexpr const char* SYM_SELECT_SAMPLE = "@select_samp";
constexpr const char* SYM_SELECT_PHASE = "@select_phase";
constexpr const char* SYM_SELECT_MS = "@select_ms";
constexpr const char* SYM_SELECT_SEC = "@select_sec";
constexpr const char* SYM_BEGIN = "@begin";
constexpr const char* SYM_END = "@end";

constexpr size_t RENDER_CHUNK = 44100 * 5;
constexpr size_t RENDER_CHUNK_PERIOD = 100;

UIArrayView::UIArrayView()
    : cursor_layer_(asEBox(), gensym("control_layer"))
    , wave_layer_(asEBox(), gensym("wave_layer"))
    , render_clock_(this, &UIArrayView::renderTick)
    , render_index_(0)
    , selection_mode_(SELECTION_NONE)
    , prop_array(&s_)
    , prop_color_wave(rgba_blue)
    , prop_color_cursor(rgba_blue)
    , prop_show_labels(0)
    , prop_show_rms(0)
    , cursor_sample_pos_(0)
    , font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , label_top_left_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , label_top_right_(font_.font(), ColorRGBA::black(), ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP)
    , label_bottom_left_(font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , label_bottom_right_(font_.font(), ColorRGBA::black(), ETEXT_DOWN_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP)
{
    createOutlet();

    appendToLayerList(&wave_layer_);
    appendToLayerList(&cursor_layer_);
}

void UIArrayView::paint()
{
    drawWaveform();
    drawLabels();
    drawCursor();
}

void UIArrayView::drawWaveform()
{
    UIPainter p = wave_layer_.painter(rect());
    if (!p)
        return;

    const int WD = width();

    // no selection
    if (selection_.isNull()) {
        drawWaveformSegment(p, 0, WD, prop_color_wave);
    } else {
        const double N = array_.size();
        // division by zero check
        if (N < 1) {
            UI_ERR << "empty array: " << prop_array;
            return;
        }

        int pixel_sel_start = std::round((selection_.from() * WD) / N);
        int pixel_sel_end = std::round((selection_.to() * WD) / N);

        if (pixel_sel_end < pixel_sel_start)
            std::swap(pixel_sel_start, pixel_sel_end);

        auto sel_color = rgba_color_sum(&prop_color_wave, &prop_color_selection, 0.4);
        auto sel_rect_color = rgba_color_sum(&prop_color_background, &prop_color_selection, 0.9);

        // draw before selection if exists
        if (pixel_sel_start > 1)
            drawWaveformSegment(p, 0, pixel_sel_start, prop_color_wave);

        // draw selection
        // selection rectangle
        p.setColor(sel_rect_color);
        p.drawRect(pixel_sel_start, 0, (pixel_sel_end - pixel_sel_start), height());
        p.fill();
        // selection waveform segment
        drawWaveformSegment(p, pixel_sel_start, pixel_sel_end, sel_color);

        // draw after selection
        if (pixel_sel_end <= (WD - 1))
            drawWaveformSegment(p, pixel_sel_end, WD, prop_color_wave);
    }
}

void UIArrayView::drawLabels()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p || !prop_show_labels)
        return;

    // top left
    label_top_left_.setColor(prop_color_border);
    label_top_left_.set(array_.name()->s_name, 5, 3, 0, 0);
    p.drawText(label_top_left_);

    // top right
    label_top_right_.setColor(prop_color_border);
    label_top_right_.set(str_label_top_right_.c_str(), r.width - 10, 3, 0, 0);
    p.drawText(label_top_right_);

    // bottom right
    label_bottom_right_.setColor(prop_color_border);
    label_bottom_right_.set(str_label_bottom_right_.c_str(), r.width - 10, r.height - 3, 0, 0);
    p.drawText(label_bottom_right_);
}

void UIArrayView::drawCursor()
{
    const auto r = rect();
    UIPainter p = cursor_layer_.painter(r);

    if (!p)
        return;

    if (!isValidArray(ErrorMessageMode::HIDE))
        return;

    float x = roundf((cursor_sample_pos_ * r.width) / (array_.size() - 1));
    p.setLineWidth(2);
    p.setColor(prop_color_cursor);
    p.drawLine(x, 0, x, r.height);

    if (prop_show_labels) {
        const double SR = sys_getsr();
        if (!SR)
            return;

        label_bottom_left_.setColor(prop_color_border);
        label_bottom_left_.set(convert::time::sec2str(double(cursor_sample_pos_) / SR, true).c_str(),
            5, r.height - 3, 0, 0);
        p.drawText(label_bottom_left_);
    }
}

void UIArrayView::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    buffer_.reserve(1000);

    if (args.size() > 0 && args[0].isSymbol() && !args[0].isProperty())
        propSetArray(args);
}

void UIArrayView::okSize(t_rect* newrect)
{
    newrect->width = clip<float>(newrect->width, 10, 5000);
    newrect->height = clip<float>(newrect->height, 10, 1000);
}

void UIArrayView::onPropChange(t_symbol* prop_name)
{
    if (prop_name == gensym(SYM_ATTR_SIZE)) {
        // width changed
        if (buffer_.size() != width()) {
            buffer_.resize(width());
            m_update();
        } else {
            // height changed: no need to update buffer, just redraw
            bg_layer_.invalidate();
            cursor_layer_.invalidate();
            wave_layer_.invalidate();
            redraw();
        }
    } else if (prop_name == gensym(SYM_ATTR_WAVE_COLOR) || prop_name == gensym(SYM_ATTR_SELECTION_COLOR)) {
        wave_layer_.invalidate();
        redraw();
    } else if (prop_name == gensym(SYM_ATTR_BG_COLOR) || prop_name == gensym(SYM_ATTR_SHOW_LABELS)) {
        bg_layer_.invalidate();
        wave_layer_.invalidate();
        redraw();
    } else if (prop_name == gensym(SYM_ATTR_CURSOR_COLOR)) {
        cursor_layer_.invalidate();
        redraw();
    } else if (prop_name == gensym(SYM_ARRAY_NAME)) {
        m_update();
    }
}

void UIArrayView::onZoom(t_float z)
{
    cursor_layer_.invalidate();
    bg_layer_.invalidate();
    wave_layer_.invalidate();
}

void UIArrayView::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (!isValidArray(ErrorMessageMode::HIDE))
        return;

    auto x = clip<float>(pt.x, 0, width());
    auto sample_pos = (x * array_.size()) / width();
    selection_mode_ = keyMod2EditMode(modifiers, x);

    switch (selection_mode_) {
    case SELECTION_CURSOR:
        cursor_sample_pos_ = sample_pos;
        cursor_layer_.invalidate();
        redraw();
        break;
    case SELECTION_RANGE:
        selection_.from() = sample_pos;
        break;
    case CHANGE_RANGE_LEFT:
        selection_.from() = sample_pos;
        selection_.normalize();
        invalidateCursor();
        invalidateWaveform();
        redraw();
        setCursor(ECURSOR_LEFT_SIDE);
        break;
    case CHANGE_RANGE_RIGHT:
        selection_.to() = sample_pos;
        selection_.normalize();
        invalidateCursor();
        invalidateWaveform();
        redraw();
        setCursor(ECURSOR_RIGHT_SIDE);
        break;
    case MOVE_RANGE:
        prev_mouse_pt_ = pt;
        setCursor(ECURSOR_MOVE);
        break;
    default:
        break;
    }

    output();
}

void UIArrayView::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    switch (selection_mode_) {
    case SELECTION_RANGE: {
        auto x = clip<float>(pt.x, 0, width());
        selection_.to() = ((x * array_.size()) / width());
        selection_.normalize();
        invalidateWaveform();
        invalidateCursor();
        redraw();
    } break;
    case CHANGE_RANGE_LEFT:
    case CHANGE_RANGE_RIGHT:
    case MOVE_RANGE:
        selection_.normalize();
        invalidateWaveform();
        invalidateCursor();
        redraw();
        break;
    default:
        break;
    }

    output();
    selection_mode_ = SELECTION_NONE;
    setCursor(ECURSOR_LEFT_PTR);
}

void UIArrayView::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    if (!isValidArray(ErrorMessageMode::HIDE))
        return;

    auto x = clip<float>(pt.x, 0, width());
    auto sample_pos = (x * array_.size()) / width();

    switch (keyMod2EditMode(modifiers, x)) {
    case MOVE_RANGE: {
        if (selection_.from() <= sample_pos && sample_pos <= selection_.to())
            setCursor(ECURSOR_MOVE);
    } break;
    case CHANGE_RANGE_LEFT: {
        setCursor(ECURSOR_LEFT_SIDE);
    } break;
    case CHANGE_RANGE_RIGHT: {
        setCursor(ECURSOR_RIGHT_SIDE);
    } break;
    default:
        break;
    }
}

void UIArrayView::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    switch (selection_mode_) {
    case SELECTION_RANGE: {
        auto x = clip<float>(pt.x, 0, width());
        selection_.to() = ((x * array_.size()) / width());
        selection_.normalize();
        invalidateWaveform();
        invalidateCursor();
        redraw();
        selection_mode_ = SELECTION_NONE;
    } break;
    default:
        break;
    }

    setCursor(ECURSOR_LEFT_PTR);
}

void UIArrayView::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    if (!isValidArray(ErrorMessageMode::HIDE))
        return;

    const size_t N = array_.size();
    double x = clip<double>(pt.x, 0, width());
    auto samp_pos = std::min<size_t>((x * array_.size()) / width(), N - 1);

    switch (selection_mode_) {
    case SELECTION_RANGE: {
        selection_.to() = samp_pos;
        invalidateWaveform();
        redraw();
    } break;
    case SELECTION_CURSOR: {
        cursor_sample_pos_ = samp_pos;
        invalidateCursor();
        redraw();
    } break;
    case CHANGE_RANGE_LEFT:
        selection_.from() = samp_pos;
        selection_.normalize();
        invalidateWaveform();
        invalidateCursor();
        redraw();
        break;
    case CHANGE_RANGE_RIGHT:
        selection_.to() = samp_pos;
        selection_.normalize();
        invalidateWaveform();
        invalidateCursor();
        redraw();
        break;
    case MOVE_RANGE: {
        long sample_delta = ((pt.x - prev_mouse_pt_.x) * array_.size()) / width();
        long new_from = selection_.from() + sample_delta;
        long new_to = selection_.to() + sample_delta;
        long N = array_.size();

        if (new_from >= 0 && new_from <= N
            && new_to >= 0 && new_to <= N) {
            selection_.from() = new_from;
            selection_.to() = new_to;
            invalidateWaveform();
            invalidateCursor();
            redraw();
        }

        prev_mouse_pt_ = pt;
    } break;
    default:
        break;
    }

    output();
}

void UIArrayView::onBang()
{
    output();
}

void UIArrayView::onFloat(t_float f)
{
    if (f < 0) {
        UI_ERR << "invalid sample value: " << f;
        return;
    }

    if (!isValidArray())
        return;

    cursor_sample_pos_ = std::min<size_t>(static_cast<size_t>(f), array_.size());
    cursor_layer_.invalidate();
    redraw();
}

void UIArrayView::m_update()
{
    render_index_ = 0;
    render_clock_.unset();

    if (!quickRender())
        return;

    cursor_layer_.invalidate();
    wave_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();

    render_clock_.delay(RENDER_CHUNK_PERIOD);
}

void UIArrayView::m_selectSamples(const AtomListView& lv)
{
    if (lv.empty()) {
        // remove selection
        selection_ = SelectionRange(-1, -1);
        invalidateWaveform();
        redraw();
        return;
    } else if (lv.size() != 2) {
        UI_ERR << "usage: select (BEGIN END) | NONE";
        return;
    }

    if (!isValidArray())
        return;

    const long N = array_.size();
    auto& atom_from = lv[0];
    auto& atom_to = lv[1];

    long from = -1;
    long to = -1;

    // range start
    if (!atom_from.isFloat()) {
        UI_ERR << "float sample value expected: " << atom_from;
        return;
    } else {
        from = atom_from.asFloat();
        if (from < 0)
            from += N;

        if (from < 0)
            from = 0;

        if (from >= N) {
            UI_ERR << "invalid selection start value: should be < " << N << ": " << atom_from;
            return;
        }
    }

    // range end
    if (!atom_to.isFloat()) {
        UI_ERR << "float sample value expected: " << atom_to;
        return;
    } else {
        to = atom_to.asFloat();
        if (to < 0)
            to += N;

        if (to < 1) {
            UI_ERR << "invalid selection end value: should be > 0: " << atom_to;
            return;
        }

        if (to >= N)
            to = N - 1;
    }

    selection_ = SelectionRange(from, to);
    selection_.normalize();

    invalidateWaveform();
    bg_layer_.invalidate();
    redraw();
}

bool UIArrayView::quickRender()
{
    if (!openArray()) {
        render_clock_.unset();
        return false;
    }

    const size_t N = array_.size();
    const int W = width();

    buffer_.resize(W);

    for (int x = 0; x < W; x++) {
        size_t idx = (x * (N - 1)) / (W - 1);

        // invariant
        assert(idx < N);
        buffer_[x].peak_max = buffer_[x].peak_min = array_[idx];
    }

    return true;
}

void UIArrayView::renderTick()
{
    if (!isValidArray()) {
        render_index_ = 0;
        return;
    }

    const size_t LEN = array_.size();

    if (LEN == 0)
        return;

    if (render_index_ + RENDER_CHUNK < LEN) {
        renderRange(render_index_, RENDER_CHUNK);
        render_index_ += RENDER_CHUNK;
        render_clock_.delay(RENDER_CHUNK_PERIOD);
        wave_layer_.invalidate();
        redraw();
    } else {
        renderRange(render_index_, LEN - render_index_);
        render_index_ = 0;
    }
}

template <typename ForwardIter>
float rms(ForwardIter first, ForwardIter last)
{
    const size_t N = last - first;
    if (!N)
        return 0.f;

    float sum = 0;

    for (ForwardIter it = first; it != last; ++it)
        sum += (*it) * (*it);

    return sqrtf(sum / N);
}

void UIArrayView::renderRange(size_t pos, size_t len)
{
    if (!len || buffer_.empty())
        return;

    const size_t T = pos + len;
    const size_t W = buffer_.size();
    const size_t N = array_.size();

    if (N < 2)
        return;

    const size_t PIXEL_FROM = (pos * (W - 1)) / (N - 1);
    const size_t PIXEL_TO = ((pos + len) * (W - 1)) / (N - 1);
    const size_t SAMPLES_IN_PIXEL = ceilf(N / float(W));

    if (PIXEL_TO >= W)
        return;

    for (size_t i = PIXEL_FROM; i < PIXEL_TO; i++) {
        size_t samp_from = (i * (N - 1)) / (W - 1);
        size_t samp_to = std::min<size_t>(samp_from + SAMPLES_IN_PIXEL, T);

        auto from(array_.begin() + samp_from);
        auto to(array_.begin() + samp_to);
        auto peak = std::minmax_element(from, to);

        buffer_[i].peak_min = *peak.first;
        buffer_[i].peak_max = *peak.second;
        buffer_[i].rms = rms(from, to);
    }
}

void UIArrayView::drawWaveformSegment(UIPainter& p, int pixel_begin, int pixel_end, const t_rgba& color)
{
    const int idx_begin = pixel_begin / zoom();
    const int idx_end = pixel_end / zoom();
    const int z = zoom();
    const t_rect r = rect();
    const int len = idx_end - idx_begin;
    const int N = buffer_.size();

    if (!p || len < 1 || len > N || idx_end > N || idx_begin >= N)
        return;

    // draw peak
    p.setColor(color);
    p.moveTo(idx_begin * z, convert::lin2lin<float>(buffer_[idx_begin].peak_min, 1, -1, 0, r.height));
    p.drawLineTo(idx_begin * z, convert::lin2lin<float>(buffer_[idx_begin].peak_max, 1, -1, 0, r.height));

    // assume: (x < idx_end) && (idx_end <= N) ----> (x < N)
    for (int x = idx_begin + 1; x < idx_end; x++) {
        p.drawLineTo(x * z, convert::lin2lin<float>(buffer_[x].peak_min, 1, -1, 0, r.height));
        p.drawLineTo(x * z, convert::lin2lin<float>(buffer_[x].peak_max, 1, -1, 0, r.height));
    }

    p.stroke();

    if (prop_show_rms) {
        // draw rms
        p.setColor(rgba_addContrast(color, 0.23f));
        p.moveTo(idx_begin * z, convert::lin2lin<float>(buffer_[idx_begin].rms, 1, -1, 0, r.height));
        p.drawLineTo(idx_begin * z, convert::lin2lin<float>(-buffer_[idx_begin].rms, 1, -1, 0, r.height));

        // assume: (x < idx_end) && (idx_end <= N) ----> (x < N)
        for (int x = idx_begin + 1; x < idx_end; x++) {
            p.drawLineTo(x * z, convert::lin2lin<float>(buffer_[x].rms, 1, -1, 0, r.height));
            p.drawLineTo(x * z, convert::lin2lin<float>(-buffer_[x].rms, 1, -1, 0, r.height));
        }

        p.stroke();
    }
}

void UIArrayView::output()
{
    const size_t N = array_.size();
    const size_t SR = sys_getsr();
    if (N == 0 || SR == 0)
        return;

    anyTo(0, gensym(SYM_CURSOR_SAMPLE), Atom(cursor_sample_pos_));
    anyTo(0, gensym(SYM_CURSOR_PHASE), Atom(double(cursor_sample_pos_) / double(N - 1)));
    anyTo(0, gensym(SYM_CURSOR_MS), Atom(double(cursor_sample_pos_ * 1000) / SR));
    anyTo(0, gensym(SYM_CURSOR_SEC), Atom(double(cursor_sample_pos_) / SR));

    // output selection
    if (!selection_.isNull() && selection_.absLength() > 0) {
        anyTo(0, gensym(SYM_SELECT_SAMPLE), selectPosSample());
        anyTo(0, gensym(SYM_SELECT_PHASE), selectPosPhase());
        anyTo(0, gensym(SYM_SELECT_MS), selectPosMs());
        anyTo(0, gensym(SYM_SELECT_SEC), selectPosSec());
        anyTo(0, gensym(SYM_BEGIN), Atom(selection_.from()));
        anyTo(0, gensym(SYM_END), Atom(selection_.to()));
    }
}

t_int UIArrayView::cursorPosSample() const
{
    return cursor_sample_pos_;
}

void UIArrayView::setCursorPosSample(t_int pos)
{
    if (pos < 0) {
        UI_ERR << "invalid sample value: " << pos;
        return;
    }

    if (!isValidArray())
        return;

    cursor_sample_pos_ = std::min<size_t>(static_cast<size_t>(pos), array_.size() - 1);

    // update view
    cursor_layer_.invalidate();
    redraw();
}

t_float UIArrayView::cursorPosPhase() const
{
    const size_t N = array_.size();
    if (N <= 1)
        return 0;

    return cursor_sample_pos_ / double(N - 1);
}

void UIArrayView::setCursorPosPhase(t_float phase)
{
    if (phase < 0 || phase > 1) {
        UI_ERR << "invalid phase value: " << phase << ". Should be in [0,1] range";
        return;
    }

    const size_t N = array_.size();
    if (N < 1)
        return;

    cursor_sample_pos_ = phase * (N - 1);
    // update view
    cursor_layer_.invalidate();
    redraw();
}

t_float UIArrayView::cursorPosMs() const
{
    return cursorPosSec() * 1000;
}

void UIArrayView::setCursorPosMs(t_float ms)
{
    const t_float SR = sys_getsr();
    if (ms < 0) {
        UI_ERR << "invalid time value: " << ms;
        return;
    }

    if (array_.size() < 1)
        return;

    size_t sample = roundf(SR * ms / 1000);
    cursor_sample_pos_ = std::min<size_t>(array_.size() - 1, sample);
    // update view
    cursor_layer_.invalidate();
    redraw();
}

t_float UIArrayView::cursorPosSec() const
{
    const size_t SR = sys_getsr();
    if (SR == 0)
        return 0;

    return cursor_sample_pos_ / double(SR);
}

void UIArrayView::setCursorPosSec(t_float pos)
{
    setCursorPosMs(pos * 1000);
}

AtomList UIArrayView::selectPosSample() const
{
    return AtomList(selection_.from(), selection_.to());
}

void UIArrayView::setSelectPosSample(const AtomListView& pos)
{
    bool ok = pos.size() == 2 && pos[0].isFloat() && pos[1].isFloat();
    if (!ok) {
        UI_ERR << "begin and end sample positions are expected: " << pos;
        return;
    }

    const long N = long(array_.size());
    if (N == 0) {
        UI_ERR << "empty array: " << prop_array;
        return;
    }

    long from = pos[0].asFloat();
    long to = pos[1].asFloat();

    setSelection(from, to);
    redrawSelection();
}

AtomList UIArrayView::selectPosPhase() const
{
    const double N = long(array_.size()) - 1;
    if (N == 0)
        return AtomList();

    return AtomList(selection_.from() / N, selection_.to() / N);
}

void UIArrayView::setSelectPosPhase(const AtomListView& pos)
{
    bool ok = pos.size() == 2 && pos[0].isFloat() && pos[1].isFloat();
    if (!ok) {
        UI_ERR << "begin and end phases are expected: " << pos;
        return;
    }

    t_float from = pos[0].asFloat();
    t_float to = pos[1].asFloat();

    if (from < 0.f || from > 1.f || to < 0.f || to > 1.f) {
        UI_ERR << "phase values should be in range [0-1]: " << pos;
        return;
    }

    const double N = long(array_.size()) - 1;

    selection_.set(from * N, to * N);
    selection_.normalize();
    redrawSelection();
}

AtomList UIArrayView::selectPosMs() const
{
    const double SR = sys_getsr();
    if (SR == 0)
        return AtomList();

    return AtomList(selection_.from() * 1000.0 / SR, selection_.to() * 1000.0 / SR);
}

void UIArrayView::setSelectPosMs(const AtomListView& pos)
{
    bool ok = pos.size() == 2 && pos[0].isFloat() && pos[1].isFloat();
    if (!ok) {
        UI_ERR << "begin and end values (ms) are expected: " << pos;
        return;
    }

    t_float from_ms = pos[0].asFloat();
    t_float to_ms = pos[1].asFloat();

    const double SR = sys_getsr();
    // some unit-test case
    if (SR == 0)
        return;

    const long N = long(array_.size());
    if (N == 0) {
        UI_ERR << "empty array: " << prop_array;
        return;
    }

    long from = std::round((from_ms * SR) / 1000.f);
    long to = std::round((to_ms * SR) / 1000.f);

    setSelection(from, to);
    redrawSelection();
}

AtomList UIArrayView::selectPosSec() const
{
    const double SR = sys_getsr();
    if (SR == 0)
        return AtomList();

    return AtomList(selection_.from() / SR, selection_.to() / SR);
}

void UIArrayView::setSelectPosSec(const AtomListView& pos)
{
    setSelectPosMs(AtomList(pos) * 1000.f);
}

bool UIArrayView::isValidArray(ErrorMessageMode msg)
{
    if (prop_array == 0 || !openArray()) {
        if (msg == ErrorMessageMode::SHOW)
            UI_ERR << "invalid array: " << prop_array;

        return false;
    }

    return true;
}

void UIArrayView::invalidateAll()
{
    invalidateCursor();
    invalidateWaveform();
}

void UIArrayView::redrawSelection()
{
    invalidateWaveform();
    bg_layer_.invalidate();
    redraw();
}

void UIArrayView::setSelection(long begin, long end)
{
    const long N = long(array_.size());
    if (N == 0)
        return;

    long from = begin;
    long to = end;

    // negative offset
    if (from < 0)
        from += N;

    if (from < 0 || from >= N) {
        UI_ERR << "invalid begin range value: " << begin;
        return;
    }

    if (to < 0)
        to += N;

    if (to < 0 || to >= N) {
        UI_ERR << "invalid end range value: " << end;
        return;
    }

    selection_.set(from, to);
    selection_.normalize();
}

EditMode UIArrayView::keyMod2EditMode(long mod, int x) const
{
#ifdef __APPLE__
    if (mod & EMOD_CMD)
        return SELECTION_RANGE;
#else
    if (mod & EMOD_CTRL)
        return SELECTION_RANGE;
#endif
    else if (mod & EMOD_SHIFT)
        return MOVE_RANGE;
    else if (mod & EMOD_ALT) {
        long sel_samp = (x * array_.size()) / width();
        auto dx0 = std::abs(selection_.from() - sel_samp);
        auto dx1 = std::abs(selection_.to() - sel_samp);
        return (dx0 < dx1) ? CHANGE_RANGE_LEFT : CHANGE_RANGE_RIGHT;
    } else
        return SELECTION_CURSOR;
}

void UIArrayView::invalidateWaveform()
{
    wave_layer_.invalidate();
}

void UIArrayView::invalidateCursor()
{
    cursor_layer_.invalidate();
}

t_int UIArrayView::sizeSamples() const
{
    if (!openArray())
        return 0;

    return array_.size();
}

t_float UIArrayView::sizeSec() const
{
    const t_float SR = sys_getsr();
    if (!SR)
        return 0;

    return sizeSamples() / SR;
}

t_float UIArrayView::sizeMs() const
{
    return sizeSec() * 1000;
}

t_symbol* UIArrayView::labelTopRight() const
{
    return gensym(str_label_top_right_.c_str());
}

void UIArrayView::setLabelTopRight(t_symbol* s)
{
    auto str = s->s_name;
    if (str != str_label_top_right_) {
        str_label_top_right_ = str;
        bg_layer_.invalidate();
        redraw();
    }
}

t_symbol* UIArrayView::labelBottomRight() const
{
    return gensym(str_label_bottom_right_.c_str());
}

void UIArrayView::setLabelBottomRight(t_symbol* s)
{
    auto str = s->s_name;
    if (str != str_label_bottom_right_) {
        str_label_bottom_right_ = str;
        bg_layer_.invalidate();
        redraw();
    }
}

AtomList UIArrayView::propArray() const
{
    return Atom(prop_array);
}

void UIArrayView::propSetArray(const AtomListView& lv)
{
    t_symbol* name = lv.symbolAt(0, 0);
    if (!name)
        return;

    prop_array = name;
    openArray();
    m_update();
}

void UIArrayView::setup()
{
    UIObjectFactory<UIArrayView> obj("ui.aview");

    obj.setDefaultSize(300, 100);
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_MOVE | UI_MOUSE_LEAVE | UI_MOUSE_DRAG);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_ON);
    obj.useFloat();
    obj.useBang();
    obj.hideLabelInner();

    obj.addSymbolProperty("array", _("Array name"), "", &UIArrayView::prop_array, _("Main"));

    obj.setPropertyAccessor("array", &UIArrayView::propArray, &UIArrayView::propSetArray);
    obj.addProperty("wave_color", _("Wave Color"), "0.3 0.3 0.3 1", &UIArrayView::prop_color_wave);
    obj.addProperty("cursor_color", _("Cursor Color"), DEFAULT_ACTIVE_COLOR, &UIArrayView::prop_color_cursor);
    obj.addProperty("selection_color", _("Selection Color"), DEFAULT_ACTIVE_COLOR, &UIArrayView::prop_color_selection);

    obj.addProperty("show_labels", _("Show Labels"), false, &UIArrayView::prop_show_labels);
    obj.addProperty("show_rms", _("Show RMS"), false, &UIArrayView::prop_show_rms);

    obj.addProperty("cursor_samp", &UIArrayView::cursorPosSample, &UIArrayView::setCursorPosSample);
    obj.addProperty("cursor_phase", &UIArrayView::cursorPosPhase, &UIArrayView::setCursorPosPhase);
    obj.setPropertyRange("cursor_phase", 0, 1);
    obj.addProperty("cursor_sec", &UIArrayView::cursorPosSec, &UIArrayView::setCursorPosSec);
    obj.addProperty("cursor_ms", &UIArrayView::cursorPosMs, &UIArrayView::setCursorPosMs);
    obj.setPropertyUnits(gensym("cursor_samp"), gensym("samp"));
    obj.setPropertyUnits(gensym("cursor_sec"), gensym("sec"));
    obj.setPropertyUnits(gensym("cursor_ms"), gensym("msec"));

    obj.addProperty("select_samp", &UIArrayView::selectPosSample, &UIArrayView::setSelectPosSample);
    obj.addProperty("select_phase", &UIArrayView::selectPosPhase, &UIArrayView::setSelectPosPhase);
    obj.addProperty("select_sec", &UIArrayView::selectPosSec, &UIArrayView::setSelectPosSec);
    obj.addProperty("select_ms", &UIArrayView::selectPosMs, &UIArrayView::setSelectPosMs);
    obj.setPropertyUnits(gensym("select_samp"), gensym("samp"));
    obj.setPropertyUnits(gensym("select_sec"), gensym("sec"));
    obj.setPropertyUnits(gensym("select_ms"), gensym("msec"));

    obj.addProperty("label_top", &UIArrayView::labelTopRight, &UIArrayView::setLabelTopRight);
    obj.addProperty("label_bottom", &UIArrayView::labelBottomRight, &UIArrayView::setLabelBottomRight);

    obj.addProperty("size_samp", &UIArrayView::sizeSamples);
    obj.setPropertyMin("size_samp", 0);
    obj.addProperty("size_sec", &UIArrayView::sizeSec);
    obj.setPropertyMin("size_sec", 0);
    obj.addProperty("size_ms", &UIArrayView::sizeMs);
    obj.setPropertyMin("size_ms", 0);
    obj.setPropertyUnits(gensym("size_samp"), gensym("samp"));
    obj.setPropertyUnits(gensym("size_sec"), gensym("sec"));
    obj.setPropertyUnits(gensym("size_ms"), gensym("msec"));

    obj.addMethod("update", &UIArrayView::m_update);
    obj.addMethod("select", &UIArrayView::m_selectSamples);
}

bool UIArrayView::openArray() const
{
    if (!prop_array)
        return false;

    const auto N = strlen(prop_array->s_name);

    // on stack string
    char str[N + 1];
    for (size_t i = 0; i < (N + 1); i++) { // copy C-string ending
        char c = prop_array->s_name[i];
        str[i] = (c == '#') ? '$' : c;
    }

    auto array_name = canvas_realizedollar(canvas(), gensym(str));
    return array_.open(array_name);
}

void setup_ui_arrayview()
{
    UIArrayView::setup();
}

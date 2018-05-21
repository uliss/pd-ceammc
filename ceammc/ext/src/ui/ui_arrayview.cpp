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
#include "ceammc_format.h"
#include "ceammc_ui.h"

#include <boost/algorithm/minmax_element.hpp>

static t_symbol* SYM_ARRAY_NAME = gensym("array");
static t_symbol* SYM_ATTR_SIZE = gensym("size");
static t_symbol* SYM_ATTR_WAVE_COLOR = gensym("wave_color");
static t_symbol* SYM_CURSOR_SAMPLE = gensym("@cursor_samp");
static t_symbol* SYM_CURSOR_PHASE = gensym("@cursor_phase");
static t_symbol* SYM_CURSOR_MS = gensym("@cursor_ms");
static t_symbol* SYM_CURSOR_SEC = gensym("@cursor_sec");

static const size_t RENDER_CHUNK = 44100 * 5;
static const size_t RENDER_CHUNK_PERIOD = 100;

UIArrayView::UIArrayView()
    : control_layer_(asEBox(), gensym("control_layer"))
    , render_clock_(this, &UIArrayView::renderTick)
    , render_index_(0)
    , cursor_sample_pos_(0)
    , mouse_mode_(MOUSE_MODE_CURSOR)
    , font_(FONT_FAMILY, FONT_SIZE_SMALL)
    , label_top_left_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , label_top_right_(font_.font(), ColorRGBA::black(), ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP)
    , label_bottom_left_(font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , label_bottom_right_(font_.font(), ColorRGBA::black(), ETEXT_DOWN_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP)
    , prop_array(&s_)
    , prop_color_wave(rgba_blue)
    , prop_color_cursor(rgba_blue)
    , prop_show_labels(0)
    , prop_show_rms(0)
{
    createOutlet();
}

void UIArrayView::paint(t_object* /*view*/)
{
    drawBackground();
    drawCursor();
}

void UIArrayView::drawBackground()
{
    const t_rect& r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    // draw selection
    if (cursor_selection_.isValid()) {
        auto from = convert::lin2lin<float>(cursor_selection_.begin(), 0, array_.size(), 0, r.width);
        auto to = convert::lin2lin<float>(cursor_selection_.end(), 0, array_.size(), 0, r.width);

        p.setColor(prop_color_select);
        p.drawRect(from, 0, to - from, r.height);
        p.fill();
    }

    // draw peak
    p.moveTo(0, convert::lin2lin<float>(buffer_[0].peak_min, 1, -1, 0, r.height));
    p.drawLineTo(0, convert::lin2lin<float>(buffer_[0].peak_max, 1, -1, 0, r.height));

    if (cursor_selection_.isValid()) {
        enum {
            out_of_selection,
            in_selection
        };

        t_rgba wave_select_color = rgba_addContrast(prop_color_wave, 0.1);
        int pixel_state = out_of_selection;
        int color_in = 0;

        p.setColor(prop_color_wave);
        Selection sel(cursor_selection_);

        for (int x = 1; x < r.width; x++) {
            size_t sample = convert::lin2lin<double>(x, 0, r.width, 0, array_.size());

            if (sel.contains(sample)) {
                pixel_state = in_selection;
                color_in++;

                // change color only on first in
                if (color_in == 1)
                    p.setColor(wave_select_color);
            } else {
                pixel_state = out_of_selection;
                // change color only on first out
                if (color_in > 0) {
                    color_in = 0;
                    p.setColor(prop_color_wave);
                }
            }

            p.drawLineTo(x, convert::lin2lin<float>(buffer_[x].peak_min, 1, -1, 0, r.height));
            p.drawLineTo(x, convert::lin2lin<float>(buffer_[x].peak_max, 1, -1, 0, r.height));
        }

    } else { // no selection

        p.setColor(prop_color_wave);

        for (int x = 1; x < r.width; x++) {
            p.drawLineTo(x, convert::lin2lin<float>(buffer_[x].peak_min, 1, -1, 0, r.height));
            p.drawLineTo(x, convert::lin2lin<float>(buffer_[x].peak_max, 1, -1, 0, r.height));
        }
    }
    p.stroke();

    if (prop_show_rms) {
        // draw rms
        p.setColor(rgba_addContrast(prop_color_wave, 0.23f));
        p.moveTo(0, convert::lin2lin<float>(buffer_[0].rms, 1, -1, 0, r.height));
        p.drawLineTo(0, convert::lin2lin<float>(-buffer_[0].rms, 1, -1, 0, r.height));

        for (int x = 1; x < r.width; x++) {
            p.drawLineTo(x, convert::lin2lin<float>(buffer_[x].rms, 1, -1, 0, r.height));
            p.drawLineTo(x, convert::lin2lin<float>(-buffer_[x].rms, 1, -1, 0, r.height));
        }

        p.stroke();
    }

    if (prop_show_labels) {
        // top left
        label_top_left_.setColor(prop_color_border);
        label_top_left_.set(array_.name().c_str(), 5, 3, 0, 0);
        p.drawText(label_top_left_);

        // top right
        label_top_right_.setColor(prop_color_border);
        label_top_right_.set(str_label_top_right_.c_str(), 3, 3, 0, 0);
        p.drawText(label_top_right_);

        // bottom right
        label_bottom_right_.setColor(prop_color_border);
        label_bottom_right_.set(str_label_bottom_right_.c_str(), 3, r.height - 3, 0, 0);
        p.drawText(label_bottom_right_);
    }
}

void UIArrayView::drawCursor()
{
    const t_rect& r = rect();
    UIPainter p = control_layer_.painter(r);

    if (!p)
        return;

    if (!checkArray())
        return;

    float x = roundf((cursor_sample_pos_ * r.width) / (array_.size() - 1));
    p.setLineWidth(2);
    p.setColor(prop_color_cursor);
    p.drawLine(x, 0, x, r.height);

    if (prop_show_labels) {
        const auto SR = sys_getsr();
        if (SR <= 0)
            return;

        label_bottom_left_.setColor(prop_color_border);
        label_bottom_left_.set(convert::time::sec2str(double(cursor_sample_pos_) / SR, true).c_str(),
            5, r.height - 3, 0, 0);
        p.drawText(label_bottom_left_);
    }
}

void UIArrayView::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    buffer_.reserve(1000);
}

void UIArrayView::okSize(t_rect* newrect)
{
    newrect->width = clip<float>(newrect->width, 10, 5000);
    newrect->height = clip<float>(newrect->height, 10, 1000);
}

t_pd_err UIArrayView::notify(t_symbol* attr_name, t_symbol* msg)
{
    if (msg == s_attr_modified) {
        if (attr_name == SYM_ATTR_SIZE) {
            if (buffer_.size() != width()) {
                buffer_.resize(width());
                m_update();
            } else {
                bg_layer_.invalidate();
                redraw();
            }
        } else if (attr_name == SYM_ATTR_WAVE_COLOR) {
            bg_layer_.invalidate();
            redraw();
        }
    }
    return 0;
}

void UIArrayView::onMouseDown(t_object* /*view*/, const t_pt& pt, long modifiers)
{
    if (!checkArray())
        return;

    setMouseMode(modifiers);

    long sample_pos = ((pt.x * array_.size()) / width());

    switch (mouse_mode_) {
    case MOUSE_MODE_CURSOR:
        cursor_sample_pos_ = sample_pos;
        break;
    case MOUSE_MODE_SELECTION:
        cursor_selection_.set(sample_pos, sample_pos);
        bg_layer_.invalidate();
        break;
    case MOUSE_MODE_EDIT_SELECTION:
        break;
    default:
        break;
    }

    control_layer_.invalidate();
    redraw();

    output();
}

void UIArrayView::onMouseUp(t_object* /*view*/, const t_pt& pt, long modifiers)
{
    if (!checkArray())
        return;

    switch (mouse_mode_) {
    case MOUSE_MODE_SELECTION:
        bg_layer_.invalidate();
        cursor_selection_.setEnd((pt.x * array_.size()) / width());
        break;
    default:
        break;
    }

    cursor_selection_.fix();
}

void UIArrayView::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    if (!checkArray())
        return;
}

void UIArrayView::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    cursor_selection_.fix();
    mouse_mode_ = MOUSE_MODE_NONE;
    setCursor(ECURSOR_LEFT_PTR);
}

void UIArrayView::onMouseDrag(t_object* /*view*/, const t_pt& pt, long modifiers)
{
    if (!checkArray())
        return;

    const size_t N = array_.size();
    double x = std::max<double>(0, pt.x);

    switch (mouse_mode_) {
    case MOUSE_MODE_CURSOR:
        cursor_sample_pos_ = std::min<size_t>((x * array_.size()) / width(), N - 1);
        break;
    case MOUSE_MODE_SELECTION:
        cursor_selection_.setEnd((x * array_.size()) / width());
        bg_layer_.invalidate();
        break;
    default:
        break;
    }

    control_layer_.invalidate();
    redraw();

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

    if (!checkArray())
        return;

    cursor_sample_pos_ = std::min<size_t>(static_cast<size_t>(f), array_.size());
    control_layer_.invalidate();
    redraw();
}

void UIArrayView::m_update()
{
    render_index_ = 0;
    render_clock_.unset();

    if (!quickRender())
        return;

    control_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();

    render_clock_.delay(RENDER_CHUNK_PERIOD);
}

void UIArrayView::m_set(const AtomList& lst)
{
    t_symbol* sel = lst.symbolAt(0, 0);
    if (sel == 0) {
        UI_ERR << "invalid arguments";
    } else {
        UI_ERR << "unknown selector: " << sel;
    }
}

bool UIArrayView::quickRender()
{
    if (!array_.open(prop_array)) {
        render_clock_.unset();
        UI_DBG << "invalid array: " << array_.name();
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
    if (!checkArray()) {
        render_index_ = 0;
        return;
    }

    const size_t LEN = array_.size();

    if (render_index_ + RENDER_CHUNK < LEN) {
        renderRange(render_index_, RENDER_CHUNK);
        render_index_ += RENDER_CHUNK;
        render_clock_.delay(RENDER_CHUNK_PERIOD);
        bg_layer_.invalidate();
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
    if (!len)
        return;

    typedef Array::const_iterator Iter;
    const size_t T = pos + len;
    const size_t W = buffer_.size();
    const size_t N = array_.size();

    if (N < 2)
        return;

    const size_t PIXEL_FROM = (pos * (W - 1)) / (N - 1);
    const size_t PIXEL_TO = ((pos + len) * (W - 1)) / (N - 1);
    const size_t SAMPLES_IN_PIXEL = ceilf(N / float(W));

    assert(PIXEL_TO < W);

    for (size_t i = PIXEL_FROM; i < PIXEL_TO; i++) {
        size_t samp_from = (i * (N - 1)) / (W - 1);
        size_t samp_to = std::min<size_t>(samp_from + SAMPLES_IN_PIXEL, T);

        Iter from = array_.begin() + samp_from;
        Iter to = array_.begin() + samp_to;
        std::pair<Iter, Iter> peak = boost::minmax_element(from, to);

        buffer_[i].peak_min = *peak.first;
        buffer_[i].peak_max = *peak.second;
        buffer_[i].rms = rms(from, to);
    }
}

void UIArrayView::output()
{
    const size_t N = array_.size();
    const size_t SR = sys_getsr();
    if (N == 0 || SR == 0) {
        return;
    }

    anyTo(0, SYM_CURSOR_SAMPLE, Atom(cursor_sample_pos_));
    anyTo(0, SYM_CURSOR_PHASE, Atom(double(cursor_sample_pos_) / double(N - 1)));
    anyTo(0, SYM_CURSOR_MS, Atom(double(cursor_sample_pos_ * 1000) / SR));
    anyTo(0, SYM_CURSOR_SEC, Atom(double(cursor_sample_pos_) / SR));
}

t_float UIArrayView::cursorPosSample() const
{
    return cursor_sample_pos_;
}

void UIArrayView::setCursorPosSample(t_float pos)
{
    if (pos < 0) {
        UI_ERR << "invalid sample value: " << pos;
        return;
    }

    if (!checkArray())
        return;

    cursor_sample_pos_ = std::min<size_t>(static_cast<size_t>(pos), array_.size() - 1);

    // update view
    control_layer_.invalidate();
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
    control_layer_.invalidate();
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
    control_layer_.invalidate();
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

bool UIArrayView::checkArray()
{
    if (prop_array == nullptr || !array_.open(prop_array)) {
        UI_ERR << "invalid array: " << prop_array;
        return false;
    }

    return true;
}

void UIArrayView::setMouseMode(long mod)
{
    if (mod == EMOD_SHIFT)
        mouse_mode_ = MOUSE_MODE_SELECTION;
    else
        mouse_mode_ = MOUSE_MODE_CURSOR;
}

AtomList UIArrayView::propSelection() const
{
    return AtomList(cursor_selection_.begin(), cursor_selection_.end());
}

void UIArrayView::propSetSelection(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "selection begin and end (in samples) expected: " << lst;
        return;
    }

    cursor_selection_.set(lst[0].asInt(), lst[1].asInt());
}

t_float UIArrayView::sizeSamples() const
{
    if (!array_.open(prop_array))
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

AtomList UIArrayView::labelTopRight() const
{
    return Atom(gensym(str_label_top_right_.c_str()));
}

void UIArrayView::setLabelTopRight(const AtomList& lst)
{
    str_label_top_right_ = to_string(lst);
}

AtomList UIArrayView::labelBottomRight() const
{
    return Atom(gensym(str_label_bottom_right_.c_str()));
}

void UIArrayView::setLabelBottomRight(const AtomList& lst)
{
    str_label_bottom_right_ = to_string(lst);
}

AtomList UIArrayView::propArray() const
{
    return Atom(prop_array);
}

void UIArrayView::propSetArray(const AtomList& lst)
{
    t_symbol* name = lst.symbolAt(0, 0);
    if (!name)
        return;

    prop_array = name;
    array_.open(prop_array);
    m_update();
}

void UIArrayView::setup()
{
    UIObjectFactory<UIArrayView> obj("ui.aview");

    obj.setDefaultSize(480, 120);
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_MOVE | UI_MOUSE_LEAVE | UI_MOUSE_DRAG);
    obj.useFloat();
    obj.useBang();

    obj.addSymbolProperty("array", _("Array name"), "", &UIArrayView::prop_array);
    obj.setPropertyAccessor("array", &UIArrayView::propArray, &UIArrayView::propSetArray);
    obj.addProperty("wave_color", _("Wave Color"), "0.3 0.3 0.3 1", &UIArrayView::prop_color_wave);
    obj.addProperty("cursor_color", _("Cursor Color"), DEFAULT_ACTIVE_COLOR, &UIArrayView::prop_color_cursor);
    obj.addProperty("select_color", _("Select Color"), "1 1 1 1", &UIArrayView::prop_color_select);

    obj.addProperty("show_labels", _("Show Labels"), false, &UIArrayView::prop_show_labels);
    obj.addProperty("show_rms", _("Show RMS"), false, &UIArrayView::prop_show_rms);

    obj.addProperty("cursor_samp", &UIArrayView::cursorPosSample, &UIArrayView::setCursorPosSample);
    obj.addProperty("cursor_phase", &UIArrayView::cursorPosPhase, &UIArrayView::setCursorPosPhase);
    obj.addProperty("cursor_sec", &UIArrayView::cursorPosSec, &UIArrayView::setCursorPosSec);
    obj.addProperty("cursor_ms", &UIArrayView::cursorPosMs, &UIArrayView::setCursorPosMs);

    obj.addProperty("label_top_right", &UIArrayView::labelTopRight, &UIArrayView::setLabelTopRight);
    obj.addProperty("label_bottom_right", &UIArrayView::labelBottomRight, &UIArrayView::setLabelBottomRight);

    obj.addProperty("size_samp", &UIArrayView::sizeSamples, 0);
    obj.addProperty("size_sec", &UIArrayView::sizeSec, 0);
    obj.addProperty("size_ms", &UIArrayView::sizeMs, 0);

    obj.addProperty("selection", &UIArrayView::propSelection, &UIArrayView::propSetSelection);

    obj.addMethod("update", &UIArrayView::m_update);
    obj.addMethod("set", &UIArrayView::m_set);
}

void setup_ui_arrayview()
{
    UIArrayView::setup();
}

Selection::Selection(long begin, long end)
{
    set(begin, end);
}

Selection::Selection(const Selection& s)
{
    set(s.begin(), s.end());
}

void Selection::set(long begin, long end)
{
    auto p = std::minmax(begin, end);
    begin_ = p.first;
    end_ = p.second;
}

long Selection::length() const
{
    return end_ - begin_;
}

bool Selection::contains(long v) const
{
    return begin_ <= v && v <= end_;
}

void Selection::setBegin(long v)
{
    begin_ = v;
}

void Selection::setEnd(long v)
{
    end_ = v;
}

bool Selection::isValid() const
{
    return begin_ >= 0 && end_ >= 0;
}

void Selection::fix()
{
    set(begin_, end_);
}

void Selection::clear()
{
    begin_ = end_ = -1;
}

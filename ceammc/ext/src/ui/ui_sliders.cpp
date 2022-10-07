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

#include <algorithm>
#include <chrono>
#include <random>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_sliders.h"
#include "ui_sliders.tcl.h"

using namespace ceammc;

static const int MAX_SLIDERS_NUM = 1024;

constexpr const char * SYM_SLIDER = "slider";
constexpr const char * SYM_VSLIDERS = "ui.vsliders";
constexpr const char * SYM_NONE = "none";
constexpr const char * SYM_BOTH = "both";
constexpr const char * SYM_MIN = "min";
constexpr const char * SYM_MAX = "max";

static decltype(std::chrono::system_clock::now().time_since_epoch().count()) random_seed;

UISliders::UISliders()
    : select_idx_(-1)
    , is_vertical_(false)
    , prop_slider_color(rgba_greydark)
    , prop_select_color(rgba_blue)
    , prop_min(0)
    , prop_max(1)
    , prop_auto_count(0)
    , prop_show_range(1)
    , prop_count(8)
    , c_min()
    , c_max()
{
    createOutlet();

    initPopupMenu("sliders",
        { { _("fill with max"), [this](const t_pt&) {
                std::fill(std::begin(pos_values_), std::end(pos_values_), 1);
                redrawAll();
                outputList(); } },
            { _("fill with mean"), [this](const t_pt&) {
                 std::fill(std::begin(pos_values_), std::end(pos_values_), 0.5);
                 redrawAll();
                 outputList();
             } },
            { _("fill with min"), [this](const t_pt&) {
                 std::fill(std::begin(pos_values_), std::end(pos_values_), 0);
                 redrawAll();
                 outputList();
             } },
            { _("linear up"), [this](const t_pt&) {
                 m_linup();
                 outputList();
             } },
            { _("linear down"), [this](const t_pt&) {
                 m_lindown();
                 outputList();
             } },
            { _("random"), [this](const t_pt&) {
                  m_random();
                  outputList(); } } });
}

void UISliders::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    int n = args.intAt(0, -1);

    if (n > 0) {
        prop_count = clip<int>(n, 1, MAX_SLIDERS_NUM);
        pos_values_.resize(prop_count, 0);
    }

    if (name == gensym(SYM_VSLIDERS))
        std::swap(asEBox()->b_rect.width, asEBox()->b_rect.height);
}

void UISliders::okSize(t_rect* newrect)
{
    is_vertical_ = newrect->width < newrect->height;

    newrect->width = std::max(20.f, newrect->width);
    newrect->height = std::max(20.f, newrect->height);
}

void UISliders::paint()
{
    sys_vgui("ui::sliders_delete %s %lx\n",
        asEBox()->b_canvas_id->s_name, asEBox());

    paintSliders();

    if (prop_show_range) {
        sys_vgui("ui::sliders_draw_labels %s %lx %d %d %d #%6.6x {%s} {%s}\n",
            asEBox()->b_canvas_id->s_name, asEBox(),
            (int)width(), (int)height(), (int)zoom(),
            rgba_to_hex_int(prop_color_label),
            c_min, c_max);
    }

    if (prop_show_lines) {
        sys_vgui("ui::sliders_draw_lines %s %lx %d %d %d #%6.6x\n",
            asEBox()->b_canvas_id->s_name, asEBox(),
            (int)width(), (int)height(), (int)zoom(),
            rgba_to_hex_int(prop_slider_color));
    }
}

void UISliders::paintSliders()
{
    constexpr size_t MAX_BAR_NUM_WD = 4;
    char buf[MAX_SLIDERS_NUM * MAX_BAR_NUM_WD + 1] = "";
    const size_t N = pos_values_.size();
    size_t idx = 0;
    for (size_t i = 0; i < N; i++) {
        auto n = snprintf(buf + idx, MAX_BAR_NUM_WD + 1, "%d ", (int)std::round(pos_values_[i] * 999));
        idx += n;
    }
    buf[idx] = '\0';

    sys_vgui("ui::sliders_draw_bars %s %lx %d %d %d "
             "#%6.6x #%6.6x %d %d %s\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_slider_color), rgba_to_hex_int(prop_select_color),
        (int)is_vertical_, (int)select_idx_, buf);
}

void UISliders::onBang()
{
    outputList();
}

void UISliders::onList(const AtomListView& lv)
{
    if (!setRealValues(lv))
        return;

    outputList();
}

static AtomList interp_lists(const AtomListView& lv0, const AtomListView& lv1, size_t n, float k)
{
    Atom res[n];

    for (size_t i = 0; i < n; i++) {
        auto v0 = lv0.floatAt(i, 0);
        auto v1 = lv1.floatAt(i, 0);
        res[i] = v0 * (1 - k) + v1 * k;
    }

    return AtomList(AtomListView(res, n));
}

void UISliders::interpPreset(t_float idx)
{
    const size_t N = pos_values_.size();
    Atom def[N];
    for (size_t i = 0; i < N; i++)
        def[i] = realValueAt(i);

    auto lv0 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx), AtomListView(def, N));
    auto lv1 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx) + 1, AtomListView(def, N));

    float k = (static_cast<float>(idx) - static_cast<int>(idx));
    onList(interp_lists(lv0, lv1, N, k));
}

void UISliders::loadPreset(size_t idx)
{
    auto lv = PresetStorage::instance().listValueAt(presetId(), idx);
    onList(lv);
}

void UISliders::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValues());
}

void UISliders::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    const t_rect r = rect();
    const size_t N = pos_values_.size();

    size_t idx = 0;
    t_float val = 0.f;

    // locked movement
    if (modifiers & EMOD_ALT) {
        click_phase_ = (is_vertical_)
            ? convert::lin2lin<t_float>(pt.x, 0, width(), 0, 1)
            : convert::lin2lin<t_float>(pt.y, 0, height(), 1, 0);

        prev_pos_values_ = pos_values_;
        return;
    } else if (modifiers & EMOD_SHIFT) {
        auto val = sliderPosValueAt(pt);
        auto* old_val = val.first;
        auto new_val = val.second;
        if (old_val) {
            // crossing middle line
            if ((*old_val < 0.5 && new_val >= 0.5)
                || (*old_val == 0.5 && new_val != 0.5)
                || (*old_val > 0.5 && new_val <= 0.5)) {
                *old_val = 0.5;
                outputList();
                redrawAll();
                return;
            }
        }
    }

    if (is_vertical_) {
        if (pt.y < 0)
            return;

        idx = clip<size_t>(floorf(pt.y / r.height * N), 0, N - 1);
        val = clip01<t_float>(pt.x / r.width);
    } else {
        if (pt.x < 0)
            return;

        idx = clip<size_t>(floorf(pt.x / r.width * N), 0, N - 1);
        val = clip01<t_float>(1.f - pt.y / r.height);
    }

    pos_values_[idx] = val;
    outputList();
    redrawAll();
}

void UISliders::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    outputList();
}

void UISliders::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    // locked movement
    if (modifiers & EMOD_ALT) {
        t_float new_mouse_phase = (is_vertical_)
            ? convert::lin2lin<t_float>(pt.x, 0, width(), 0, 1)
            : convert::lin2lin<t_float>(pt.y, 0, height(), 1, 0);

        const t_float delta = new_mouse_phase - click_phase_;
        const auto mm = std::minmax_element(prev_pos_values_.begin(), prev_pos_values_.end());
        if (mm.first == prev_pos_values_.end() || *mm.first + delta < 0 || *mm.second + delta > 1)
            return;

        for (size_t i = 0; i < std::min(prev_pos_values_.size(), pos_values_.size()); i++)
            pos_values_[i] = prev_pos_values_[i] + delta;

        outputList();
        redrawAll();
    } else
        onMouseDown(view, pt, pt, modifiers);
}

std::pair<t_float*, t_float> UISliders::sliderPosValueAt(const t_pt& pt)
{
    const t_rect r = rect();
    const int N = pos_values_.size();

    if (is_vertical_) {
        if (pt.y < 0)
            return { nullptr, 0 };

        const auto idx = clip<size_t>(floorf(pt.y / r.height * N), 0, N - 1);
        return { &pos_values_[idx], clip01<t_float>(pt.x / r.width) };
    } else {
        if (pt.x < 0)
            return { nullptr, 0 };

        const auto idx = clip<size_t>(floorf(pt.x / r.width * N), 0, N - 1);
        return { &pos_values_[idx], clip01<t_float>(1.f - pt.y / r.height) };
    }
}

void UISliders::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    t_canvas* c = reinterpret_cast<t_canvas*>(view);

    if (c->gl_edit)
        resize(height() / zoom(), width() / zoom());
}

void UISliders::m_get(const AtomListView& l)
{
    if (l.size() > 0 && l[0].isSymbol()) {
        if (l.size() == 2
            && l[0].asSymbol() == gensym(SYM_SLIDER)
            && l[1].isFloat()) {

            int idx = l[1].asFloat();
            if (idx < 0 || idx >= pos_values_.size()) {
                UI_ERR << "invalid slider index: " << idx;
                return;
            }

            Atom res[2];
            res[0] = idx;
            res[1] = realValueAt(idx);
            anyTo(0, gensym(SYM_SLIDER), AtomListView(res, 2));
        } else {
            UI_ERR << "usage: get slider IDX";
            return;
        }
    }
}

AtomList UISliders::realValues() const
{
    AtomList res;
    res.fill(0., pos_values_.size());

    for (size_t i = 0; i < pos_values_.size(); i++)
        res[i] = realValueAt(i);

    return res;
}

void UISliders::setRealValueAt(size_t n, t_float v)
{
    pos_values_[n] = clip<t_float, 0, 1>((v - prop_min) / (prop_max - prop_min));
}

t_float UISliders::realValueAt(size_t n) const
{
    return pos_values_[n] * (prop_max - prop_min) + prop_min;
}

bool UISliders::setRealValues(const AtomListView& l)
{
    t_float range = prop_max - prop_min;

    if (l.empty()) {
        UI_ERR << "empty list given";
        return false;
    }

    if (prop_auto_range_mode == gensym(SYM_BOTH)) {
        auto mm = std::minmax_element(l.begin(), l.end(),
            [](const Atom& a, const Atom& b) {
                return a.toT<t_float>(0) < b.toT<t_float>(0);
            });
        auto min = mm.first->asFloat(0);
        auto max = mm.second->asFloat(1);
        range = max - min;

        if (range == 0.) {
            UI_ERR << "zero value range";
            return false;
        }

        prop_min = min;
        prop_max = max;
        generateTxtLabels();
    } else if (prop_auto_range_mode == gensym(SYM_MAX)) {
        const size_t N = prop_auto_count
            ? std::min<size_t>(l.size(), MAX_SLIDERS_NUM)
            : std::min<size_t>(l.size(), prop_count);

        auto amax = std::max_element(l.begin(), l.begin() + N,
            [](const Atom& a, const Atom& b) { return a.toT<t_float>(0) < b.toT<t_float>(0); });

        auto max = amax->asFloat(1);
        auto new_range = max - prop_min;

        if (new_range == 0.) {
            UI_ERR << "zero value range";
            return false;
        }

        prop_max = max;
        generateTxtLabels();
    } else if (prop_auto_range_mode == gensym(SYM_MIN)) {
        const size_t N = prop_auto_count
            ? std::min<size_t>(l.size(), MAX_SLIDERS_NUM)
            : std::min<size_t>(l.size(), prop_count);

        auto amin = std::min_element(l.begin(), l.begin() + N,
            [](const Atom& a, const Atom& b) { return a.toT<t_float>(0) < b.toT<t_float>(0); });

        auto min = amin->asFloat(1);
        auto new_range = prop_max - min;

        if (new_range == 0.) {
            UI_ERR << "zero value range";
            return false;
        }

        prop_min = min;
        generateTxtLabels();
    } else if (range == 0.f) {
        UI_ERR << "zero value range";
        return false;
    }

    if (prop_auto_count) {
        pos_values_.resize(std::min<size_t>(MAX_SLIDERS_NUM, l.size()), prop_min);
        prop_count = l.size();
    }

    const size_t N = std::min(l.size(), pos_values_.size());

    for (size_t i = 0; i < N; i++) {
        t_float f = 0;

        if (!l[i].getFloat(&f))
            UI_ERR << "float value expected: " << l[i];

        setRealValueAt(i, f);
    }

    redrawAll();
    return true;
}

t_float UISliders::propCount() const
{
    return pos_values_.size();
}

void UISliders::setPropCount(t_float f)
{
    prop_count = clip<int>(f, 1, MAX_SLIDERS_NUM);
    pos_values_.resize(prop_count, prop_min);
    redrawAll();
}

AtomList UISliders::propAutoRangeMode() const
{
    return AtomList(prop_auto_range_mode);
}

void UISliders::setPropAutoRangeMode(const AtomListView& lv)
{
    static const char* opts[] = { SYM_NONE, SYM_BOTH, SYM_MAX, SYM_MIN };

    auto sym = lv.asSymbol();
    auto it = std::find_if(std::begin(opts), std::end(opts), [sym](const char* s) { return gensym(s) == sym; });
    if (it == std::end(opts)) {
        UI_ERR << SYM_NONE << ", " << SYM_BOTH << ", " << SYM_MAX << ", " << SYM_MIN << " expected, got: " << lv;
        return;
    }

    prop_auto_range_mode = sym;
    if (isPatchLoading())
        return;

    if (prop_auto_range_mode == gensym(SYM_BOTH))
        normalize();
    else if (prop_auto_range_mode == gensym(SYM_MIN))
        normalizeMin();
    else if (prop_auto_range_mode == gensym(SYM_MAX))
        normalizeMax();
}

t_float UISliders::propRange() const
{
    return prop_max - prop_min;
}

AtomList UISliders::propValue() const
{
    return realValues();
}

void UISliders::generateTxtLabels()
{
    snprintf(c_min, 15, "%g", prop_min);
    snprintf(c_max, 15, "%g", prop_max);
}

void UISliders::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void UISliders::normalize()
{
    if (pos_values_.empty())
        return;

    auto mm = std::minmax_element(pos_values_.begin(), pos_values_.end());
    const t_float range = prop_max - prop_min;
    const t_float real_min = (*mm.first) * range + prop_min;
    const t_float real_max = (*mm.second) * range + prop_min;
    const t_float new_range = real_max - real_min;

    if (*mm.second == *mm.first) {
        UI_ERR << "normalize error - invalid range: " << new_range;
        return;
    }

    for (size_t i = 0; i < pos_values_.size(); i++)
        pos_values_[i] = (((pos_values_[i] * range) + prop_min) - real_min) / new_range;

    prop_min = real_min;
    prop_max = real_max;
    setProperty(gensym("min"), Atom(real_min));
    setProperty(gensym("max"), Atom(real_max));
}

void UISliders::normalizeMin()
{
    if (pos_values_.empty())
        return;

    auto min = *std::min_element(pos_values_.begin(), pos_values_.end());
    const t_float range = prop_max - prop_min;
    const t_float real_min = min * range + prop_min;
    const t_float new_range = prop_max - real_min;

    if (new_range == 0) {
        UI_ERR << "normalize error - invalid range: " << new_range;
        return;
    }

    for (size_t i = 0; i < pos_values_.size(); i++) {
        auto old_real_val = (pos_values_[i] * range) + prop_min;
        auto new_real_val = (old_real_val - real_min) / new_range;
        pos_values_[i] = clip_max(new_real_val, prop_max);
    }

    prop_min = real_min;
    setProperty(gensym("min"), Atom(real_min));
}

void UISliders::normalizeMax()
{
    if (pos_values_.empty())
        return;

    auto max = *std::max_element(pos_values_.begin(), pos_values_.end());
    const t_float range = prop_max - prop_min;
    const t_float real_max = max * range + prop_min;
    const t_float new_range = real_max - prop_min;

    if (new_range == 0) {
        UI_ERR << "normalize error - invalid range: " << new_range;
        return;
    }

    for (size_t i = 0; i < pos_values_.size(); i++) {
        auto old_real_val = (pos_values_[i] * range) + prop_min;
        auto new_real_val = (old_real_val - prop_min) / new_range;
        pos_values_[i] = new_real_val;
    }

    prop_max = real_max;
    setProperty(gensym("max"), Atom(real_max));
}

void UISliders::m_set(const AtomListView& l)
{
    // set slider IDX VALUE
    // or
    // set VALUES...
    if (l.size() > 0 && l[0].isSymbol()) {
        if (l.size() == 3
            && l[0].asSymbol() == gensym(SYM_SLIDER)
            && l[1].isFloat()
            && l[2].isFloat()) {

            int idx = l[1].asFloat();
            t_float v = l[2].asFloat();

            if (idx < 0 || idx >= pos_values_.size()) {
                UI_ERR << "invalid slider index: " << idx;
                return;
            }

            setRealValueAt(idx, v);
            redrawAll();
        } else {
            UI_ERR << "usage: set slider INDEX VALUE";
            return;
        }
    } else {
        setRealValues(l);
    }
}

void UISliders::m_select(const AtomListView& l)
{
    select_idx_ = l.floatAt(0, -1);
    redrawAll();
}

void UISliders::m_plus(t_float f)
{
    m_set(realValues() + f);
}

void UISliders::m_minus(t_float f)
{
    m_set(realValues() - f);
}

void UISliders::m_mul(t_float f)
{
    m_set(realValues() * f);
}

void UISliders::m_div(t_float f)
{
    if (f == 0) {
        UI_ERR << "division by zero";
        return;
    }

    m_set(realValues() / f);
}

void UISliders::m_increment()
{
    m_set(realValues() + 1);
}

void UISliders::m_decrement()
{
    m_set(realValues() - 1);
}

void UISliders::m_fill(t_float v)
{
    m_set(AtomList::filled(v, prop_count));
}

void UISliders::m_random()
{
    static std::uniform_real_distribution<t_float> dist(0, 1);
    static std::default_random_engine gen(random_seed);

    std::generate(std::begin(pos_values_), std::end(pos_values_), []() { return dist(gen); });
    redrawAll();
}

void UISliders::m_linup()
{
    int i = 0;
    int n = prop_count - 1;
    std::generate(std::begin(pos_values_), std::end(pos_values_), [&i, &n]() { return t_float(i++) / n; });
    redrawAll();
}

void UISliders::m_lindown()
{
    int n = prop_count - 1;
    int i = n;
    std::generate(std::begin(pos_values_), std::end(pos_values_), [&i, &n]() { return t_float(i--) / n; });
    redrawAll();
}

void UISliders::outputList()
{
    const auto N = pos_values_.size();
    Atom res[N];
    for (size_t i = 0; i < N; i++)
        res[i] = realValueAt(i);

    AtomListView lv(res, N);
    listTo(0, lv);
    send(lv);
}

void UISliders::onPropChange(t_symbol* prop_name)
{
    generateTxtLabels();
    redrawAll();
}

void UISliders::setup()
{
    sys_gui(ui_sliders_tcl);

    random_seed = std::chrono::system_clock::now().time_since_epoch().count();

    UIObjectFactory<UISliders> obj("ui.sliders");
    obj.addAlias("ui.vsliders");

    obj.usePresets();
    obj.useList();
    obj.useBang();
    obj.usePopup();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.setDefaultSize(150, 100);

    obj.addProperty("slider_color", _("Slider color"), "0.75 0.75 0.75 1.0", &UISliders::prop_slider_color);
    obj.addProperty("select_color", _("Select color"), DEFAULT_ACTIVE_COLOR, &UISliders::prop_select_color);

    obj.addProperty("count", _("Sliders count"), 8, &UISliders::prop_count, "Main");
    obj.setPropertyRange("count", 1, MAX_SLIDERS_NUM);
    obj.setPropertyAccessor("count", &UISliders::propCount, &UISliders::setPropCount);

    obj.addFloatProperty("min", _("Minimum Value"), 0., &UISliders::prop_min, "Bounds");
    obj.addFloatProperty("max", _("Maximum Value"), 1., &UISliders::prop_max, "Bounds");

    obj.addMenuProperty("auto_range_mode", _("Auto range mode"), "none", &UISliders::prop_auto_range_mode, "none both min max", "Main");
    obj.setPropertyAccessor("auto_range_mode", &UISliders::propAutoRangeMode, &UISliders::setPropAutoRangeMode);

    obj.addProperty("show_range", _("Show range"), true, &UISliders::prop_show_range, "Main");
    obj.addProperty("auto_count", _("Auto count"), false, &UISliders::prop_auto_count, "Main");
    obj.addProperty("show_lines", _("Show lines"), false, &UISliders::prop_show_lines, "Main");

    obj.addProperty("range", &UISliders::propRange, 0);
    obj.addProperty("value", &UISliders::propValue, 0);

    obj.addMethod("select", &UISliders::m_select);
    obj.addMethod("set", &UISliders::m_set);
    obj.addMethod("get", &UISliders::m_get);

    obj.addMethod("+", &UISliders::m_plus);
    obj.addMethod("-", &UISliders::m_minus);
    obj.addMethod("*", &UISliders::m_mul);
    obj.addMethod("/", &UISliders::m_div);
    obj.addMethod("++", &UISliders::m_increment);
    obj.addMethod("--", &UISliders::m_decrement);
    obj.addMethod("fill", &UISliders::m_fill);
    obj.addMethod("random", &UISliders::m_random);
    obj.addMethod("linup", &UISliders::m_linup);
    obj.addMethod("lindown", &UISliders::m_lindown);
}

void setup_ui_sliders()
{
    UISliders::setup();
}

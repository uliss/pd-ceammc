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
#include <boost/algorithm/minmax_element.hpp>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_fn_list.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_sliders.h"

using namespace ceammc;

static const int MAX_SLIDERS_NUM = 1024;
static t_symbol* SYM_SLIDER = gensym("slider");

UISliders::UISliders()
    : txt_font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , txt_min_(txt_font_.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt_max_(txt_font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JRIGHT, ETEXT_NOWRAP)
    , select_idx_(-1)
    , is_vertical_(false)
    , sliders_layer_(asEBox(), gensym("main_layer"))
    , prop_slider_color(rgba_greydark)
    , prop_select_color(rgba_blue)
    , prop_min(0)
    , prop_max(1)
    , prop_auto_range(0)
    , prop_show_range(1)
    , prop_count(8)
    , c_min()
    , c_max()
{
    prependToLayerList(&sliders_layer_);
    createOutlet();
}

void UISliders::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    int n = args.intAt(0, -1);

    if (n > 0) {
        prop_count = clip<int>(n, 1, MAX_SLIDERS_NUM);
        pos_values_.resize(prop_count, 0);
    }
}

void UISliders::okSize(t_rect* newrect)
{
    is_vertical_ = newrect->width < newrect->height;

    newrect->width = std::max(20.f, newrect->width);
    newrect->height = std::max(20.f, newrect->height);
}

void UISliders::paint()
{
    paintSliders();
    paintLabels();
}

void UISliders::paintLabels()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    if (prop_show_range) {
        const float z = zoom();
        const float xoff = 3 * z;
        const float yoff = 3 * z;

        if (is_vertical_) {
            txt_min_.setAnchor(ETEXT_UP_LEFT);
            txt_min_.setJustify(ETEXT_JLEFT);
            txt_min_.set(c_min, xoff, yoff, r.width * 2, r.height / 2);

            txt_max_.setAnchor(ETEXT_UP_RIGHT);
            txt_max_.setJustify(ETEXT_JRIGHT);
            txt_max_.set(c_max, r.width - xoff, yoff, r.width * 2, r.height / 2);
        } else {
            txt_min_.setAnchor(ETEXT_DOWN_LEFT);
            txt_min_.setJustify(ETEXT_JLEFT);
            txt_min_.set(c_min, xoff, r.height - yoff, r.width * 2, r.height / 2);

            txt_max_.setAnchor(ETEXT_UP_LEFT);
            txt_max_.setJustify(ETEXT_JLEFT);
            txt_max_.set(c_max, xoff, yoff, r.width * 2, r.height / 2);
        }

        p.drawText(txt_min_);
        p.drawText(txt_max_);
    }
}

void UISliders::paintSliders()
{
    const t_rect r = rect();
    UIPainter p = sliders_layer_.painter(r);

    if (!p)
        return;

    // draw bins
    const size_t N = pos_values_.size();
    for (size_t i = 0; i < N; i++) {
        const float v = pos_values_[i];

        float x, y, w, h;

        if (is_vertical_) {
            x = 0;
            y = i * r.height / N;
            w = v * r.width;
            h = r.height / N - 1;
        } else {
            y = (1 - v) * (r.height);
            x = i * r.width / N;
            h = r.height - 1;
            w = r.width / N - 1;
        }

        p.setColor(i == select_idx_ ? prop_select_color : prop_slider_color);
        p.drawRect(x, y, w, h);
        p.fill();

        if (i == select_idx_) {
            p.setLineWidth(2);
            p.setColor(prop_color_border);
            p.drawRect(x, y, w - 1, h);
            p.stroke();
        }
    }
}

void UISliders::onBang()
{
    outputList();
}

void UISliders::onList(const AtomList& lst)
{
    if (!setRealValues(lst))
        return;

    outputList();
}

void UISliders::loadPreset(size_t idx)
{
    AtomList lst = PresetStorage::instance().listValueAt(presetId(), idx);
    onList(lst);
}

void UISliders::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValues());
}

void UISliders::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    // right click
    if (modifiers & EMOD_RIGHT) {
        UIPopupMenu menu(asEObj(), "popup", abs_pt);
        char buf[64];
        snprintf(buf, sizeof(buf), _("fill with %f"), prop_max);
        menu.addItem(buf);
        snprintf(buf, sizeof(buf), _("fill with %f"), (prop_max - prop_min) * 0.5);
        menu.addItem(buf);
        snprintf(buf, sizeof(buf), _("fill with %f"), prop_min);
        menu.addItem(buf);
    }

    const t_rect r = rect();
    const size_t N = pos_values_.size();

    size_t idx = 0;
    float val = 0.f;

    if (is_vertical_) {
        if (pt.y < 0)
            return;

        idx = clip<size_t>(floorf(pt.y / r.height * N), 0, N - 1);
        val = clip<float>(pt.x / r.width, 0, 1);
    } else {
        if (pt.x < 0)
            return;

        idx = clip<size_t>(floorf(pt.x / r.width * N), 0, N - 1);
        val = clip<float>(1.f - pt.y / r.height, 0, 1);
    }

    pos_values_[idx] = val;
    outputList();
    redrawAll();
}

void UISliders::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    onMouseDown(view, pt, pt, modifiers);
}

void UISliders::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    t_canvas* c = reinterpret_cast<t_canvas*>(view);
    if (c->gl_edit)
        resize(height() / zoom(), width() / zoom());
}

void UISliders::onPopup(t_symbol* menu_name, long item_idx)
{
    if (menu_name != gensym("popup"))
        return;

    switch (item_idx) {
    case 0:
        m_fill(prop_max);
        break;
    case 1:
        m_fill((prop_max - prop_min) * 0.5);
        break;
    case 2:
        m_fill(prop_min);
        break;
    default:
        UI_ERR << "unknown popup menu item: " << item_idx;
        break;
    }
}

void UISliders::m_get(const AtomList& l)
{
    if (l.size() > 0 && l[0].isSymbol()) {
        if (l.size() == 2
            && l[0].asSymbol() == SYM_SLIDER
            && l[1].isFloat()) {

            int idx = l[1].asFloat();
            if (idx < 0 || idx >= pos_values_.size()) {
                UI_ERR << "invalid slider index: " << idx;
                return;
            }

            anyTo(0, SYM_SLIDER, AtomList(idx, realValueAt(idx)));
        } else {
            UI_ERR << "usage: get slider IDX";
            return;
        }
    }
}

AtomList UISliders::realValues() const
{
    AtomList res;
    res.fill(0.f, pos_values_.size());

    for (size_t i = 0; i < pos_values_.size(); i++)
        res[i] = realValueAt(i);

    return res;
}

void UISliders::setRealValueAt(size_t n, t_float v)
{
    pos_values_[n] = clip<float>((v - prop_min) / (prop_max - prop_min), 0, 1);
}

float UISliders::realValueAt(size_t n) const
{
    return pos_values_[n] * (prop_max - prop_min) + prop_min;
}

bool UISliders::setRealValues(const AtomList& l)
{
    float range = prop_max - prop_min;

    if (l.empty()) {
        UI_ERR << "empty list given";
        return false;
    }

    if (prop_auto_range) {
        float min = l.min()->asFloat(0);
        float max = l.max()->asFloat(1);
        range = max - min;

        if (range == 0.f) {
            UI_ERR << "zero value range";
            return false;
        }

        prop_min = min;
        prop_max = max;
        generateTxtLabels();
        pos_values_.resize(std::min<size_t>(MAX_SLIDERS_NUM, l.size()), min);
    } else if (range == 0.f) {
        UI_ERR << "zero value range";
        return false;
    }

    const size_t N = std::min(l.size(), pos_values_.size());
    for (size_t i = 0; i < N; i++) {
        float f = 0;

        if (!l[i].getFloat(&f))
            UI_ERR << "float value expected: " << l[i];

        setRealValueAt(i, f);
    }

    redrawAll();
    return true;
}

float UISliders::propCount() const
{
    return pos_values_.size();
}

void UISliders::setPropCount(float f)
{
    prop_count = clip<int>(f, 1, MAX_SLIDERS_NUM);
    pos_values_.resize(prop_count, prop_min);
    redrawAll();
}

float UISliders::propAutoRange() const
{
    return prop_auto_range;
}

void UISliders::setPropAutoRange(float f)
{
    prop_auto_range = (f) ? 1 : 0;
    if (prop_auto_range)
        normalize();
}

float UISliders::propRange() const
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
    sliders_layer_.invalidate();
    redraw();
}

void UISliders::normalize()
{
    if (pos_values_.empty())
        return;

    typedef std::vector<float>::iterator Iterator;

    std::pair<Iterator, Iterator> mm = boost::minmax_element(pos_values_.begin(), pos_values_.end());
    const float range = prop_max - prop_min;
    const float real_min = (*mm.first) * range + prop_min;
    const float real_max = (*mm.second) * range + prop_min;
    const float new_range = real_max - real_min;

    if (*mm.second == *mm.first) {
        UI_ERR << "normalize error - invalid range: " << new_range;
        return;
    }

    for (size_t i = 0; i < pos_values_.size(); i++)
        pos_values_[i] = (((pos_values_[i] * range) + prop_min) - real_min) / new_range;

    prop_min = real_min;
    prop_max = real_max;
    setProperty(gensym("min"), AtomList(real_min));
    setProperty(gensym("max"), AtomList(real_max));
}

void UISliders::m_set(const AtomList& l)
{
    // set slider IDX VALUE
    // or
    // set VALUES...
    if (l.size() > 0 && l[0].isSymbol()) {
        if (l.size() == 3
            && l[0].asSymbol() == SYM_SLIDER
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

void UISliders::m_select(const AtomList& l)
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

void UISliders::outputList()
{
    AtomList v = realValues();
    listTo(0, v);
    send(v);
}

void UISliders::setup()
{
    UIObjectFactory<UISliders> obj("ui.sliders");

    obj.usePresets();
    obj.useList();
    obj.useBang();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK);

    obj.setDefaultSize(150, 100);

    obj.addProperty("slider_color", _("Slider color"), "0.75 0.75 0.75 1.0", &UISliders::prop_slider_color);
    obj.addProperty("select_color", _("Select color"), DEFAULT_ACTIVE_COLOR, &UISliders::prop_select_color);

    obj.addProperty("count", _("Sliders count"), 8, &UISliders::prop_count, "Main");
    obj.setPropertyRange("count", 1, MAX_SLIDERS_NUM);
    obj.setPropertyAccessor("count", &UISliders::propCount, &UISliders::setPropCount);

    obj.addProperty("min", _("Minimum Value"), 0, &UISliders::prop_min, "Bounds");
    obj.addProperty("max", _("Maximum Value"), 1, &UISliders::prop_max, "Bounds");

    obj.addProperty("auto_range", _("Auto range"), false, &UISliders::prop_auto_range, "Main");
    obj.setPropertyAccessor("auto_range", &UISliders::propAutoRange, &UISliders::setPropAutoRange);
    obj.addProperty("show_range", _("Show range"), true, &UISliders::prop_show_range, "Main");

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
}

void UISliders::onPropChange(t_symbol* prop_name)
{
    generateTxtLabels();
    redrawAll();
}

void setup_ui_sliders()
{
    UISliders::setup();
}

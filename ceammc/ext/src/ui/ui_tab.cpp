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
#include "ui_tab.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

static const size_t MAX_ITEMS = 128;

UITab::UITab()
    : item_selected_(-1)
    , item_hover_(-1)
    , prop_is_vertical(0)
    , prop_toggle_mode(0)
    , prop_color_text(rgba_black)
    , prop_color_hover(rgba_grey)
    , prop_color_active(rgba_blue)
{
    createOutlet();
}

void UITab::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    AtomList items;
    for (auto& a : args) {
        if (a.isProperty())
            break;

        items.append(a);
    }

    if (items.size() > 0)
        propSetItems(items);
}

void UITab::okSize(t_rect* newrect)
{
    if (prop_is_vertical) {
        newrect->width = pd_clip_min(newrect->width, 8);
        newrect->height = pd_clip_min(newrect->height, pd_clip_min(items_.size(), 1) * 20);
    } else {
        newrect->width = pd_clip_min(newrect->width, pd_clip_min(items_.size(), 1) * 20);
        newrect->height = pd_clip_min(newrect->height, 8);
    }
}

void UITab::paint()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    const size_t N = items_.size();
    if (N == 0)
        return;

    if (prop_is_vertical) {

        // draw cells
        float cell_h = roundf(r.height / N);
        for (size_t i = 0; i < N; i++) {
            // draw cell
            if (isSelected(i) || i == item_hover_) {
                p.setColor(isSelected(i) ? prop_color_active : prop_color_hover);
                p.drawRect(-1, i * cell_h, r.width + 1, cell_h);
                p.fill();
            }

            // draw separator
            if (i > 0) {
                p.setColor(prop_color_border);
                p.drawLine(-1, i * cell_h, r.width + 1, i * cell_h);
            }

            // draw label
            Layout& l = layouts_[i];
            std::string& s = labels_[i];

            l->set(s.c_str(), r.width / 2, (i + 0.5) * cell_h, 0, 0);
            l->setColor(prop_color_text);
            p.drawText(*l.get());
        }

    } else {
        // draw cells
        float cell_w = roundf(r.width / N);
        for (size_t i = 0; i < N; i++) {
            // draw cell
            if (isSelected(i) || i == item_hover_) {
                p.setColor(isSelected(i) ? prop_color_active : prop_color_hover);
                p.drawRect(i * cell_w, -1, cell_w, r.height + 1);
                p.fill();
            }

            // draw separator
            if (i > 0) {
                p.setColor(prop_color_border);
                p.drawLine(i * cell_w, -1, i * cell_w, r.height + 1);
            }

            // draw label
            Layout& l = layouts_[i];
            std::string& s = labels_[i];

            l->set(s.c_str(), (i + 0.5) * cell_w, r.height / 2, 0, 0);
            l->setColor(prop_color_text);
            p.drawText(*l.get());
        }
    }
}

void UITab::onBang()
{
    output();
}

void UITab::onFloat(t_float f)
{
    if (prop_toggle_mode) {
        UI_ERR << "unexpected float in toggle mode: " << f;
        return;
    }

    if (!selectSingle(f))
        return;

    output();
    redrawBGLayer();
}

void UITab::onSymbol(t_symbol* s)
{
    int idx = findIndex(s);
    if (idx < 0) {
        UI_ERR << "tab was not found by title: " << s;
        return;
    }

    if (prop_toggle_mode) {
        toggles_.flip(idx);
        item_selected_ = idx;
    } else {
        item_selected_ = idx;
    }

    output();
    redrawBGLayer();
}

void UITab::onAny(t_symbol* s, const AtomListView&)
{
    onSymbol(s);
}

void UITab::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    const size_t N = items_.size();
    if (!N)
        return;

    int idx = -1;

    if (prop_is_vertical) {
        const float cell_h = height() / N;
        idx = floorf(pt.y / cell_h);
    } else {
        const float cell_w = width() / N;
        idx = floorf(pt.x / cell_w);
    }

    if (idx < 0 || idx >= MAX_ITEMS)
        return;

    if (prop_toggle_mode) {
        toggles_.flip(idx);
        item_selected_ = idx;
    } else
        item_selected_ = idx;

    redrawBGLayer();
    output();
}

void UITab::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
}

void UITab::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    const size_t N = items_.size();
    if (!N)
        return;

    if (prop_is_vertical) {
        float cell_h = height() / N;
        item_hover_ = floorf(pt.y / cell_h);
    } else {
        float cell_w = width() / N;
        item_hover_ = floorf(pt.x / cell_w);
    }

    redrawBGLayer();
}

void UITab::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    item_hover_ = -1;
    redrawBGLayer();
}

void UITab::loadPreset(size_t idx)
{
    if (prop_toggle_mode) {
        AtomList l = PresetStorage::instance().listValueAt(presetId(), idx);
        const size_t t = std::min<size_t>(items_.size(), l.size());
        for (size_t i = 0; i < t; i++) {
            bool v = l[i].asFloat(0);
            toggles_.set(i, v);
        }
    } else {
        float v = PresetStorage::instance().floatValueAt(presetId(), idx);
        if (v < 0 || v >= items_.size()) {
            UI_ERR << "invalid value from preset: " << v;
            return;
        }

        item_selected_ = v;
    }

    redrawBGLayer();
}

void UITab::storePreset(size_t idx)
{
    if (prop_toggle_mode)
        PresetStorage::instance().setListValueAt(presetId(), idx, propSelected());
    else
        PresetStorage::instance().setFloatValueAt(presetId(), idx, item_selected_);
}

AtomList UITab::propItems() const
{
    return items_;
}

void UITab::propSetItems(const AtomListView& lst)
{
    items_ = lst;
    layouts_.reserve(lst.size());

    for (size_t i = layouts_.size(); i < lst.size(); i++) {
        layouts_.push_back(std::make_shared<UITextLayout>(&asEBox()->b_font, prop_color_text,
            ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP));
    }

    syncLabels();
    redrawBGLayer();
}

AtomList UITab::propSelected() const
{
    const size_t N = items_.size();
    AtomList res;
    res.reserve(N);

    if (prop_toggle_mode) {
        for (size_t i = 0; i < N; i++)
            res.append(Atom(toggles_.test(i) ? 1 : 0));

        return res;
    } else {
        res.fill(Atom(0.f), N);
        if (item_selected_ >= 0 && item_selected_ < N)
            res[item_selected_] = Atom(1);
    }

    return res;
}

t_float UITab::propCount() const
{
    return items_.size();
}

t_float UITab::propCurrent() const
{
    if (prop_toggle_mode)
        return -1;
    else
        return item_selected_;
}

void UITab::m_select(const AtomListView& lst)
{
    if (lst.empty()) {
        UI_ERR << "select: index or symbol expected";
        return;
    }

    const Atom& a = lst[0];
    if (a.isFloat()) {
        int idx = a.asFloat();
        if (idx < 0 || idx >= items_.size()) {
            UI_ERR << "invalid index: " << idx;
            return;
        }

        item_selected_ = idx;
        redrawBGLayer();
    } else if (a.isSymbol()) {
        int idx = findIndex(a);
        if (idx < 0)
            return;

        item_selected_ = idx;
        redrawBGLayer();
    }
}

void UITab::m_clear()
{
    item_selected_ = -1;
    items_.clear();
    redrawBGLayer();
}

void UITab::m_append(const AtomListView& lst)
{
    if (lst.empty())
        return;

    propSetItems(items_ + lst);
    resize(width(), height());
}

void UITab::m_set_item(const AtomListView& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "usage: set_item INDEX VALUE";
        return;
    }

    int idx = lst[0].asFloat(-1);
    if (idx < 0 || idx >= items_.size()) {
        UI_ERR << "set_item: invalid index " << lst[0];
        return;
    }

    items_[idx] = lst[1];
    syncLabels();
    redrawBGLayer();
}

void UITab::m_delete(t_float f)
{
    int idx = f;

    if (idx < 0 || idx >= items_.size()) {
        UI_ERR << "set_item: invalid index " << idx;
        return;
    }

    items_.remove(idx);
    syncLabels();
    redrawBGLayer();
}

void UITab::m_insert(const AtomListView& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "usage: insert INDEX VALUE";
        return;
    }

    int idx = lst[0].asFloat(-1);
    if (idx < 0) {
        UI_ERR << "insert: invalid index " << lst[0];
        return;
    }

    if (!items_.insert(idx, lst[1])) {
        UI_ERR << "insert error: " << idx;
        return;
    }

    propSetItems(items_);
}

bool UITab::isSelected(size_t idx) const
{
    if (idx >= MAX_ITEMS)
        return false;

    if (prop_toggle_mode)
        return toggles_.test(idx);
    else
        return (idx == item_selected_);
}

void UITab::output()
{
    if (prop_toggle_mode) {
        const size_t N = items_.size();
        Atom res[N];
        for (size_t i = 0; i < N; i++) {
            res[i] = toggles_.test(i) ? 1 : 0;
        }

        t_symbol* SYM_PROP_SELECTED = gensym("@selected");
        anyTo(0, SYM_PROP_SELECTED, AtomListView(res, N));
        send(SYM_PROP_SELECTED, AtomListView(res, N));

        if (item_selected_ < 0 || item_selected_ >= items_.size())
            return;

        Atom sel[2];
        sel[0] = item_selected_;
        sel[1] = toggles_.test(item_selected_);
        listTo(0, AtomListView(sel, 2));
        send(AtomListView(sel, 2));
    } else {
        if (item_selected_ < 0 || item_selected_ >= items_.size()) {
            UI_ERR << "no item selected";
            return;
        }

        Atom sel[2];
        sel[0] = item_selected_;
        sel[1] = items_[item_selected_];
        listTo(0, AtomListView(sel, 2));
        send(AtomListView(sel, 2));
    }
}

bool UITab::selectSingle(int idx)
{
    if (idx < 0 || idx >= items_.size()) {
        UI_ERR << "invalid tab index: " << idx;
        return false;
    }

    item_selected_ = idx;
    return true;
}

static inline t_symbol* toSymbol(const Atom& atom)
{
    t_symbol* res = 0;
    if (!atom.getSymbol(&res))
        res = gensym(to_string(atom).c_str());

    assert(res);

    return res;
}

int UITab::findIndex(const Atom& name)
{
    t_symbol* n = toSymbol(name);

    for (size_t i = 0; i < items_.size(); i++) {
        t_symbol* item = toSymbol(items_[i]);

        if (item == n)
            return i;
    }

    return -1;
}

void UITab::syncLabels()
{
    labels_.resize(items_.size());

    for (size_t i = 0; i < items_.size(); i++)
        labels_[i] = to_string(items_[i]);
}

void UITab::setup()
{
    UIObjectFactory<UITab> obj("ui.tab");
    obj.setDefaultSize(100, 15);
    obj.useAny();
    obj.useBang();
    obj.useFloat();
    obj.useSymbol();
    obj.usePresets();
    obj.hideLabelInner();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_MOVE | UI_MOUSE_LEAVE);

    obj.showProperty("fontname");
    obj.showProperty("fontsize");
    obj.showProperty("fontweight");

    obj.addProperty("orientation", _("Vertical Orientation"), false, &UITab::prop_is_vertical, "Main");
    obj.addProperty("toggle", _("Toggle Mode"), false, &UITab::prop_toggle_mode, "Main");
    obj.addProperty("items", &UITab::propItems, &UITab::propSetItems);
    obj.showProperty("items");
    obj.setPropertyCategory("items", "Main");
    obj.setPropertyLabel("items", _("Items"));
    obj.setPropertySave("items");
    obj.addProperty("count", &UITab::propCount, 0);

    obj.addProperty("selected", &UITab::propSelected, 0);
    obj.addProperty("current", &UITab::propCurrent, 0);

    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UITab::prop_color_text);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UITab::prop_color_active);
    obj.addProperty("hover_color", _("Hover Color"), "0.5 0.5 0.5 1", &UITab::prop_color_hover);

    obj.addMethod("append", &UITab::m_append);
    obj.addMethod("clear", &UITab::m_clear);
    obj.addMethod("delete", &UITab::m_delete);
    obj.addMethod("insert", &UITab::m_insert);
    obj.addMethod("select", &UITab::m_select);
    obj.addMethod("set", &UITab::propSetItems);
    obj.addMethod("set_item", &UITab::m_set_item);
}

void setup_ui_tab()
{
    UITab::setup();
}

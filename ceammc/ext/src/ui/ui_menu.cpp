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
#include "ui_menu.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "ceammc_preset.h"
#include "ceammc_string.h"
#include "ceammc_ui.h"

#include <memory>

UIMenu::UIMenu()
    : prop_color_text(rgba_black)
    , prop_color_active(rgba_blue)
    , prop_max_item_length(100)
    , is_open_(false)
    , item_height_(16)
    , current_idx_(-1)
    , hover_idx_(-1)
{
    createOutlet();
}

void UIMenu::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    AtomList items;
    for (auto& a : args) {
        if (a.isProperty())
            break;

        items.append(a);
    }

    if (items.size() > 0) {
        propSetItems(items);
        current_idx_ = 0;
        syncLabels();
    }
}

void UIMenu::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, ebox_fontwidth(asEBox()) * 3 / zoom() + 8);
    newrect->h = fontSize() + 5;

#ifdef __WIN32
    newrect->h += 4;
#endif

    item_height_ = newrect->h;

    if (newrect->w < newrect->h * 2)
        newrect->w = newrect->h * 2;

    if (is_open_)
        newrect->h = item_height_ * (items_.size() + 1);
}

void UIMenu::paint()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    if (is_open_) {
        p.setColor(prop_color_border);

        for (size_t i = 0; i < items_.size(); i++) {
            float y = itemHeightZoomed() * (i + 1);
            p.drawLine(-1, y, r.w + 1, y);
        }

        t_rgba current_color = rgba_addContrast(prop_color_background, -0.2);

        assert(items_.size() == labels_.size());
        assert(items_.size() == layouts_.size());
        for (size_t i = 0; i < items_.size(); i++) {
            // fill current item
            if (i == current_idx_ || i == hover_idx_) {
                p.setColor(i == hover_idx_ ? prop_color_active : current_color);
                p.drawRect(0, itemHeightZoomed() * (i + 1), r.w, itemHeightZoomed());
                p.fill();
            }

            const std::string& lbl = labels_[i];
            TextPtr& ptxt = layouts_[i];
            ptxt->set(lbl.c_str(), 3, itemHeightZoomed() * (i + 1.5), 0, 0);

            p.drawText(*ptxt);
        }

        // draw separator
        const float x = r.w - itemHeightZoomed();
        p.setColor(prop_color_border);
        p.setLineWidth(1);
        p.drawLine(x, -1, x, itemHeightZoomed());

        // draw knob
        p.moveTo(x + itemHeightZoomed() * 0.25, itemHeightZoomed() * 0.75);
        p.drawLineTo(x + itemHeightZoomed() * 0.75, itemHeightZoomed() * 0.75);
        p.drawLineTo(x + itemHeightZoomed() * 0.5, itemHeightZoomed() * 0.25);
        p.drawLineTo(x + itemHeightZoomed() * 0.25, itemHeightZoomed() * 0.75);
        p.fill();

    } else {
        assert(items_.size() == labels_.size());
        assert(items_.size() == layouts_.size());

        if (current_idx_ >= 0 && current_idx_ < items_.size()) {
            const std::string& lbl = labels_[current_idx_];

            TextPtr& ptxt = layouts_[current_idx_];
            ptxt->set(lbl.c_str(), 3, r.h / 2, 0, 0);

            ptxt->setColor(prop_color_text);
            p.drawText(*ptxt);
        }

        const float x = r.w - r.h;
        // draw separator
        p.setColor(prop_color_border);
        p.setLineWidth(1);
        p.drawLine(x, -1, x, r.h + 1);

        // draw knob
        p.moveTo(x + r.h * 0.25, r.h * 0.25);
        p.drawLineTo(x + r.h * 0.75, r.h * 0.25);
        p.drawLineTo(x + r.h * 0.5, r.h * 0.75);
        p.drawLineTo(x + r.h * 0.25, r.h * 0.25);
        p.fill();
    }
}

AtomList UIMenu::propItems() const
{
    return items_;
}

void UIMenu::propSetItems(const AtomListView& lv)
{
    items_ = lv;
    current_idx_ = -1;
    syncLabels();
    setOpen(false);
    redrawBGLayer();
}

void UIMenu::onBang()
{
    output();
}

void UIMenu::onFloat(t_float f)
{
    if (f < 0 || f >= items_.size()) {
        UI_ERR << "invalid menu index: " << f;
        return;
    }

    current_idx_ = f;
    redrawBGLayer();
    output();
}

void UIMenu::onSymbol(t_symbol* s)
{
    if (!selectByValue(AtomListView(s)))
        return;

    output();
}

void UIMenu::onAny(t_symbol* s, const AtomListView&)
{
    onSymbol(s);
}

void UIMenu::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (is_open_) {
        int idx = findIndex(pt.y);
        if (idx != -1) {
            current_idx_ = idx;
        }
    }

    setOpen(!is_open_);

    if (!is_open_)
        output();
}

void UIMenu::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    if (!is_open_)
        return;

    int idx = findIndex(pt.y);
    if (idx < 0)
        return;

    hover_idx_ = idx;
    redrawBGLayer();
}

void UIMenu::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    hover_idx_ = -1;
}

t_int UIMenu::propCount() const
{
    return items_.size();
}

t_int UIMenu::propIndex() const
{
    return current_idx_;
}

void UIMenu::propSetIndex(t_int f)
{
    if (f < 0 || f >= items_.size()) {
        UI_ERR << "invalid menu index: " << f;
        return;
    }

    current_idx_ = f;
    redrawBGLayer();
}

Atom UIMenu::propValue() const
{
    if (current_idx_ < 0 || current_idx_ >= items_.size())
        return &s_;

    return items_[current_idx_];
}

void UIMenu::propSetValue(const Atom& v)
{
    if (!selectByValue(v))
        return;

    redrawBGLayer();
}

bool UIMenu::propOpen() const
{
    return is_open_;
}

void UIMenu::propSetOpen(bool v)
{
    if (is_open_ != v) {
        is_open_ = v;
        adjustSize();
    }
}

t_symbol* UIMenu::propTitle() const
{
    if (current_idx_ < 0 || current_idx_ >= labels_.size())
        return &s_;

    return gensym(labels_[current_idx_].c_str());
}

void UIMenu::loadPreset(size_t idx)
{
    const t_float INVALID = std::numeric_limits<t_float>::max();
    t_float v = PresetStorage::instance().floatValueAt(presetId(), idx, INVALID);
    if (v == INVALID)
        return;

    onFloat(v);
}

void UIMenu::storePreset(size_t idx)
{
    if (current_idx_ < 0 || current_idx_ >= items_.size())
        return;

    PresetStorage::instance().setFloatValueAt(presetId(), idx, current_idx_);
}

void UIMenu::m_append(const AtomListView& lv)
{
    if (lv.size() < 1) {
        UI_ERR << "items expected";
        return;
    }

    items_.append(lv);
    syncLabels();
    adjustSize();
    redrawBGLayer();
}

void UIMenu::m_clear()
{
    items_.clear();
    current_idx_ = -1;
    syncLabels();
    adjustSize();
    redrawBGLayer();
}

void UIMenu::m_delete(t_float f)
{
    int idx = f;
    if (idx < 0 || idx >= items_.size()) {
        UI_ERR << "invalid index: " << f;
        return;
    }

    items_.remove(idx);
    syncLabels();

    if (is_open_ || current_idx_ == idx) {
        if (current_idx_ == idx)
            current_idx_ = -1;

        redrawBGLayer();
    }

    adjustSize();
}

void UIMenu::m_set(const AtomListView& lv)
{
    if (lv.size() < 1) {
        UI_ERR << "menu index or item title expected";
        return;
    }

    const Atom& a = lv[0];
    if (a.isFloat()) {
        propSetIndex(a.asFloat());
    } else if (a.isSymbol()) {
        propSetValue(a);
    }
}

void UIMenu::m_insert(const AtomListView& lv)
{
    bool ok = (lv.size() == 2 && lv[0].isFloat());
    if (!ok) {
        UI_ERR << "invalid arguments: " << lv;
        UI_ERR << "usage: insert INDEX VALUE";
        return;
    }

    int idx = lv[0].asInt();
    if (idx < 0 || idx > items_.size()) {
        UI_ERR << "invalid index: " << idx;
        return;
    }

    items_.insert(idx, lv[1]);
    syncLabels();
    adjustSize();
    redrawBGLayer();
}

void UIMenu::m_set_item(const AtomListView& lv)
{
    bool ok = (lv.size() == 2 && lv[0].isFloat());
    if (!ok) {
        UI_ERR << "invalid arguments: " << lv;
        UI_ERR << "usage: set_item INDEX VALUE";
        return;
    }

    int idx = lv[0].asInt();
    if (idx < 0 || idx >= items_.size()) {
        UI_ERR << "invalid index: " << idx;
        return;
    }

    items_[idx] = lv[1];
    syncLabels();
    redrawBGLayer();
}

void UIMenu::m_file_glob(const AtomListView& lv)
{
    if (lv.size() < 1) {
        UI_ERR << "usage: file_glob PATTERN [PATH]";
        return;
    }

    std::string path, pattern;
    pattern = to_string(lv[0]);
    if (lv.size() > 1)
        path = to_string(lv[1]);

    // expand unix $HOME: ~
    path = platform::expand_tilde_path(path);

    if (!sys_isabsolutepath(path.c_str())) {
        if (canvas()) {
            t_symbol* canvas_dir = canvas_getdir(canvas());
            if (canvas_dir)
                path = std::string(canvas_dir->s_name) + "/" + path;
        }
    }

    platform::DirList res = platform::list_directory(path.c_str(), pattern.c_str());
    if (!res) {
        UI_ERR << "can't open directory: " << path;
        return;
    }

    for (size_t i = 0; i < res->size(); i++) {
        std::string p = path + "/" + res->at(i);
        items_.append(gensym(p.c_str()));
    }

    syncLabels();
    adjustSize();
    redrawBGLayer();
}

void UIMenu::syncLabels()
{
    labels_.resize(items_.size());
    layouts_.reserve(items_.size());

    if (layouts_.size() > items_.size())
        layouts_.resize(items_.size());

    t_efont* f = &asEBox()->b_font;
    for (size_t i = layouts_.size(); i < items_.size(); i++) {
        layouts_.emplace_back(std::make_shared<UITextLayout>(f, ColorRGBA::black(),
            ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP));
    }

    for (size_t i = 0; i < items_.size(); i++) {
        std::string s = to_string(items_[i]);
        const size_t LEN = string::utf8_strlen(s.c_str());
        if (LEN > prop_max_item_length) {
            const size_t N = prop_max_item_length / 2;

            std::string res = string::utf8_substr(s.c_str(), 0, N < 2 ? 1 : N - 2);
            res += "...";
            res += string::utf8_substr(s.c_str(), -N, N);
            s = res;
        }

        labels_[i] = s;
    }
}

int UIMenu::findIndex(int y)
{
    if (!is_open_)
        return -1;

    if (y <= itemHeightZoomed() || y > height())
        return -1;

    int res = int(y / itemHeightZoomed()) - 1;
    if (res >= items_.size())
        return -1;

    return res;
}

bool UIMenu::selectByValue(const AtomListView& lv)
{
    if (lv.size() < 1) {
        UI_ERR << "item title expected";
        return false;
    }

    std::string title = to_string(lv);
    LabelList::iterator it = std::find(labels_.begin(), labels_.end(), title);
    if (it == labels_.end()) {
        UI_ERR << "item is not found: " << title;
        return false;
    }

    current_idx_ = std::distance(labels_.begin(), it);
    return true;
}

void UIMenu::output()
{
    if (current_idx_ < 0 || current_idx_ >= items_.size()) {
        UI_ERR << "no item selected";
        return;
    }

    Atom res[2];
    res[0] = current_idx_;
    res[1] = items_[current_idx_];

    listTo(0, AtomListView(res, 2));
    send(AtomListView(res, 2));
}

void UIMenu::setOpen(bool v)
{
    if (is_open_ == v)
        return;

    is_open_ = v;
    adjustSize();
    redrawBGLayer();
}

void UIMenu::adjustSize()
{
    resize(width() / zoom(), height() / zoom());
}

void UIMenu::setup()
{
    UIObjectFactory<UIMenu> obj("ui.menu");
    obj.setDefaultSize(100, 16);

    obj.usePresets();
    obj.useBang();
    obj.useFloat();
    obj.useAny();
    obj.useSymbol();
    obj.hideLabelInner();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_MOVE | UI_MOUSE_LEAVE);

    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UIMenu::prop_color_text);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIMenu::prop_color_active);
    obj.addProperty("max_length", _("Maximum item length"), 100, &UIMenu::prop_max_item_length);
    obj.setPropertyRange("max_length", 10, 1024);

    obj.addProperty("index", &UIMenu::propIndex, &UIMenu::propSetIndex);
    obj.addProperty("value", &UIMenu::propValue, &UIMenu::propSetValue);
    obj.addProperty("count", &UIMenu::propCount);
    obj.addProperty("open", &UIMenu::propOpen, &UIMenu::propSetOpen);
    obj.setPropertyDefaultValue("open", "0");
    obj.addProperty("title", &UIMenu::propTitle);

    obj.addMethod("append", &UIMenu::m_append);
    obj.addMethod("clear", &UIMenu::m_clear);
    obj.addMethod("delete", &UIMenu::m_delete);
    obj.addMethod("set", &UIMenu::m_set);
    obj.addMethod("insert", &UIMenu::m_insert);
    obj.addMethod("set_item", &UIMenu::m_set_item);
    obj.addMethod("file_glob", &UIMenu::m_file_glob);

    obj.addProperty("items", &UIMenu::propItems, &UIMenu::propSetItems);
    obj.setPropertyLabel("items", _("Items"));
    obj.setPropertyCategory("items", _("Main"));
    obj.showProperty("items");
    obj.setPropertySave("items");
}

void setup_ui_menu()
{
    UIMenu::setup();
}

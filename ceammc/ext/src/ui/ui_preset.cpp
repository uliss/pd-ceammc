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
#include "ui_preset.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"

UIPreset::UIPreset()
    : prop_color_text(rgba_black)
    , prop_color_empty(rgba_grey)
    , prop_color_stored(rgba_greydark)
    , prop_color_active(rgba_blue)
    , button_size_(10)
    , selected_index_(-1)
    , mouse_over_index_(-1)
{
    initPopupMenu("preset",
        { { _("read"), [this](const t_pt&) { m_read(AtomList()); updateIndexes(); } },
            { _("write"), [this](const t_pt&) { m_write(AtomList()); } },
            { _("duplicate all"), [this](const t_pt&) { m_duplicate(AtomList()); } } });
}

void UIPreset::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    bindTo(PresetStorage::instance().SYM_PRESET_UPDATE_INDEX_ADDR);
    updateIndexes();
}

void UIPreset::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 15.);
    newrect->h = pd_clip_min(newrect->h, 15.);
}

void UIPreset::paint()
{
#ifdef __APPLE__
    const static int FNT_SZ = FONT_SIZE_SMALL;
#elif __WIN32__
    const static int FNT_SZ = FONT_SIZE_SMALL + 2;
#else
    const static int FNT_SZ = FONT_SIZE_SMALL + 2;
#endif

    // NO multithreading!!!
    static std::vector<UITextLayout*> numbers_;
    static UIFont font(gensym(FONT_FAMILY), FNT_SZ);

    if (numbers_.empty()) {
        for (size_t i = 0; i < 256; i++)
            numbers_.push_back(new UITextLayout(font.font(), ColorRGBA::black(),
                ETEXT_CENTER, ETEXT_JCENTER, ETEXT_NOWRAP));
    }

    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    button_size_ = FNT_SZ * zoom() * 2.5;
    int btn_x = button_size_ / 2;
    int btn_y = button_size_ / 2;

    char number[256];
    t_rgba color;

    for (int i = 0; i < presets_.size(); i++) {
        if (btn_y + button_size_ * 0.25 >= r.h)
            break;

        if (presets_[i]) {
            if (selected_index_ == i)
                color = rgba_addContrast(prop_color_active, 0.1f);
            else
                color = rgba_addContrast(prop_color_stored, -0.1f);
        } else
            color = rgba_addContrast(prop_color_empty, 0.1f);

        // on mouse over
        if (mouse_over_index_ == i)
            color = rgba_addContrast(color, 0.08f);

        // draw button
        p.setColor(color);
        p.drawCircle(btn_x, btn_y, button_size_ * 0.4);
        p.fill();

        // draw button text
        sprintf(number, "%i", i);
        numbers_[i]->setColor(prop_color_text);
        numbers_[i]->set(number, btn_x, btn_y, 40, 0);
        p.drawText(*numbers_[i]);

        btn_x += button_size_;
        if (btn_x + button_size_ * 0.25 > r.w) {
            // start next row
            btn_x = button_size_ / 2;
            btn_y += button_size_;
        }
    }
}

void UIPreset::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    auto index = buttonIndexAt(pt.x, pt.y);

    if (modifiers & EMOD_ALT)
        clearIndex(index);
    else if (modifiers & EMOD_SHIFT)
        storeIndex(index);
    else
        loadIndex(index);
}

void UIPreset::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    auto index = buttonIndexAt(pt.x, pt.y);

    if (mouse_over_index_ != index) {
        mouse_over_index_ = index;
        redrawLayer(bg_layer_);
    }
}

void UIPreset::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    mouse_over_index_ = -1;
    redrawLayer(bg_layer_);
}

int UIPreset::buttonIndexAt(float x, float y) const
{
    float n;
    const float rem = modff(width() / button_size_, &n);
    const int num_cols = n + (rem >= 0.75);
    const int col = std::min<int>(x / button_size_, num_cols - 1);
    const int row = int(y / button_size_);
    return row * num_cols + col;
}

void UIPreset::m_read(const AtomListView& lv)
{
    if (PresetStorage::instance().read(canvas(), to_string(lv))) {
        selected_index_ = -1;

        redrawLayer(bg_layer_);
    }
}

void UIPreset::m_write(const AtomListView& lv)
{
    PresetStorage::instance().write(canvas(), to_string(lv));
}

void UIPreset::m_load(const AtomListView& lv)
{
    loadIndex(lv.floatAt(0, 0));
}

void UIPreset::m_interp(const AtomListView& lv)
{
    interpIndex(lv.floatAt(0, 0));
}

void UIPreset::m_store(const AtomListView& lv)
{
    storeIndex(lv.floatAt(0, 0));
}

void UIPreset::m_clear(const AtomListView& lv)
{
    for (auto x : lv) {
        if (x.isFloat())
            clearIndex(x.asFloat());
    }
}

void UIPreset::m_clearall(const AtomListView&)
{
    for (size_t i = 0; i < presets_.size(); i++) {
        if (!presets_.test(i))
            continue;

        PresetStorage::instance().clearAll(i);
        redrawLayer(bg_layer_);
    }

    selected_index_ = -1;
    presets_.reset();
    redrawLayer(bg_layer_);
}

void UIPreset::m_duplicate(const AtomListView& lv)
{
    if (lv.empty())
        PresetStorage::instance().duplicateAll();
}

t_int UIPreset::propCurrent() const
{
    return selected_index_;
}

void UIPreset::indexAdd(const AtomListView& lv)
{
    if (lv.isFloat()) {
        int idx = lv.asT<t_int>();
        if (!checkIndex(idx))
            return;

        // update only if changed
        if (!presets_.test(idx)) {
            presets_.set(idx, true);
            redrawLayer(bg_layer_);
        }
    }
}

void UIPreset::indexRemove(const AtomListView& lv)
{
    if (lv.isFloat()) {
        int idx = lv.asT<t_int>();
        if (!checkIndex(idx))
            return;

        // update only if changed
        if (presets_.test(idx)) {
            presets_.set(idx, false);
            redrawLayer(bg_layer_);
        }
    }
}

void UIPreset::updateIndexes()
{
    PresetStorage& s = PresetStorage::instance();
    for (size_t i = 0; i < presets_.size(); i++)
        presets_.set(i, s.hasIndex(i));
}

void UIPreset::loadIndex(int idx)
{
    if (!checkIndex(idx))
        return;

    if (presets_.test(idx)) {
        selected_index_ = idx;
        PresetStorage::instance().loadAll(idx);
        redrawLayer(bg_layer_);
    }
}

void UIPreset::storeIndex(int idx)
{
    if (!checkIndex(idx))
        return;

    PresetStorage::instance().storeAll(idx);
    presets_.set(idx, true);
    selected_index_ = idx;
}

void UIPreset::clearIndex(int idx)
{
    if (!checkIndex(idx))
        return;

    if (presets_.test(idx)) {
        if (selected_index_ == idx)
            selected_index_ = -1;

        PresetStorage::instance().clearAll(idx);
        presets_.set(idx, false);
        redrawLayer(bg_layer_);
    }
}

void UIPreset::interpIndex(t_float idx)
{
    if (!checkIndex(idx))
        return;

    PresetStorage::instance().interpAll(idx);
}

bool UIPreset::checkIndex(float idx) const
{
    if (idx < 0 || idx >= presets_.size()) {
        UI_ERR << "invalid preset index: " << idx;
        return false;
    } else
        return true;
}

void UIPreset::setup()
{
    UIObjectFactory<UIPreset> obj("ui.preset");
    obj.setDefaultSize(102, 42);

    obj.internalProperty("send");
    obj.hideLabelInner();

    obj.addProperty("text_color", _("Text Color"), "0. 0. 0. 1.", &UIPreset::prop_color_text);
    obj.addProperty("empty_color", _("Empty Button Color"), "0.86 0.86 0.86 1.", &UIPreset::prop_color_empty);
    obj.addProperty("stored_color", _("Stored Button Color"), "0.5 0.5 0.5 1.", &UIPreset::prop_color_stored);
    obj.addProperty(sym::props::name_active_color, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIPreset::prop_color_active);
    obj.addProperty("current", &UIPreset::propCurrent);

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_MOVE | UI_MOUSE_LEAVE);
    obj.usePopup();
    obj.addMethod(PresetStorage::instance().SYM_PRESET_INDEX_ADD, &UIPreset::indexAdd);
    obj.addMethod(PresetStorage::instance().SYM_PRESET_INDEX_REMOVE, &UIPreset::indexRemove);

    obj.addMethod("clear", &UIPreset::m_clear);
    obj.addMethod("clearall", &UIPreset::m_clearall);
    obj.addMethod("duplicate", &UIPreset::m_duplicate);
    obj.addMethod("load", &UIPreset::m_load);
    obj.addMethod("read", &UIPreset::m_read);
    obj.addMethod("store", &UIPreset::m_store);
    obj.addMethod("write", &UIPreset::m_write);
    obj.addMethod("interp", &UIPreset::m_interp);
}

void setup_ui_preset()
{
    UIPreset::setup();
}

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
#include "ui_icon.h"
#include "ceammc_crc32.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
#include "icons/generated_icons.h"
#include "ui_icon.tcl.h"

#include <sstream>

CEAMMC_DEFINE_SYM_HASH(ceammc);
CEAMMC_DEFINE_HASH(bang);
CEAMMC_DEFINE_SYM_HASH(button);
CEAMMC_DEFINE_SYM_HASH(toggle);

static const t_rgba COLOR_DISABLED(hex_to_rgba("#D0D0D0"));

constexpr const int SPEC_SIZE = 64;

static icon_entry* iconByName(t_symbol* name)
{
    for (size_t i = 0;; i++) {
        icon_entry& entry = icons_list[i];
        if (!entry.name)
            return 0;

        if (name == entry.name)
            return &entry;
    }

    return 0;
}

static AtomList allIcons()
{
    AtomList res;

    for (size_t i = 0;; i++) {
        const icon_entry& entry = icons_list[i];

        if (!entry.name)
            break;

        res.append(entry.name);
    }

    return res;
}

UIIcon::UIIcon()
    : clock_(this, &UIIcon::clockTick)
    , current_(&icons_list[0])
    , prop_icon(gensym("help"))
    , prop_mode(sym_button())
    , prop_color_active(rgba_blue)
    , prop_size(24)
    , is_active_(false)
    , is_enabled_(true)
{
    createOutlet();
}

void UIIcon::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    hideXlets(true);
}

void UIIcon::onPropChange(t_symbol* prop_name)
{
    if (prop_name == gensym("icon_size")) {
        resize(prop_size, prop_size);
    }

    bg_layer_.invalidate();
    updateIconProp();
}

void UIIcon::okSize(t_rect* newrect)
{
    if (isSpecialIcon()) {
        newrect->w = SPEC_SIZE;
        newrect->h = SPEC_SIZE;
        return;
    }

    float w = (newrect->w);

    int w0 = roundf(w / 6);
    if (w0 == 3 || w0 == 4 || w0 == 6 || w0 == 8) {
        prop_size = w0 * 6;
    } else if (w0 == 5 || w0 == 7) {
        prop_size = roundf(w / 12) * 12;
    } else if (w0 > 8) {
        prop_size = 48;
    } else {
        prop_size = 18;
    }

    newrect->w = prop_size;
    newrect->h = prop_size;
}

void UIIcon::paint()
{
    char name[32] = {};
    int size = pd_clip_max(prop_size * zoom(), 48);
    snprintf(name, sizeof(name) - 1, "%s_%d", current_->name->s_name, size);

    int color = 0;
    if (!is_enabled_)
        color = rgba_to_hex_int(COLOR_DISABLED);
    else
        color = rgba_to_hex_int(is_active_ ? prop_color_active : prop_color_background);

    sys_vgui("::ui::icon::update %s %lx %d %d {%s} #%6.6x\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(),
        name, color);
}

void UIIcon::onBang()
{
    if (!is_enabled_)
        return;

    if (prop_mode == sym::sym_bang() || prop_mode == sym_toggle()) {
        t_pt pt;
        onMouseDown(0, pt, pt, 0);
    } else {
        UI_ERR << "bang is not expected in button mode";
    }
}

void UIIcon::onFloat(t_float f)
{
    if (!is_enabled_)
        return;

    if (prop_mode == sym::sym_bang()) {
        onBang();
    } else {
        is_active_ = (f != 0.f);
        outputState(is_active_);
        redrawAll();
    }
}

void UIIcon::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (modifiers & EMOD_ALT) {
        for (auto& a : allIcons())
            UI_POST << a;

        return;
    }

    if (!is_enabled_)
        return;

    auto mode = crc32_hash(prop_mode);

    if (mode == ::hash_bang) {
        is_active_ = true;
        clock_.delay(100);
        outputBang();
        redrawAll();
    } else if (mode == hash_button) {
        is_active_ = true;
        outputState(1);
        redrawAll();
    } else if (mode == hash_toggle) {
        is_active_ = !is_active_;
        outputState(is_active_ ? 1 : 0);
        redrawAll();
    } else {
        UI_ERR << "unknown mode: " << prop_mode;
    }
}

void UIIcon::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (!is_enabled_)
        return;

    if (modifiers & EMOD_ALT)
        return;

    if (crc32_hash(prop_mode) == hash_button) {
        is_active_ = false;
        outputState(0);
        redrawAll();
    }
}

void UIIcon::m_set(const AtomListView& lv)
{
    if (lv.empty()) {
        UI_ERR << "icon name expected";
        return;
    }

    if (!lv[0].isSymbol()) {
        UI_ERR << "icon name expected: " << lv;
        UI_ERR << "   available values are: " << allIcons();
        return;
    }

    t_symbol* name = lv[0].asSymbol();
    icon_entry* entry = iconByName(name);

    if (entry == current_)
        return;

    if (!entry) {
        UI_ERR << "invalid icon name: " << name;
        UI_ERR << "   available values are: " << allIcons();
        return;
    }

    prop_icon = entry->name;
    current_ = entry;

    redraw();
}

void UIIcon::loadPreset(size_t idx)
{
    prop_icon = PresetStorage::instance().symbolValueAt(presetId(), idx, gensym("help"));
    updateIconProp();
}

void UIIcon::storePreset(size_t idx)
{
    PresetStorage::instance().setSymbolValueAt(presetId(), idx, prop_icon);
}

void UIIcon::updateIconProp()
{
    icon_entry* entry = iconByName(prop_icon);
    if (!entry) {
        UI_ERR << "invalid icon name: " << prop_icon;
        // roll back
        prop_icon = current_->name;
        return;
    } else {
        current_ = entry;
    }

    if (isSpecialIcon())
        resize(SPEC_SIZE, SPEC_SIZE);

    redraw();
}

void UIIcon::outputBang()
{
    if (!is_enabled_)
        return;

    bangTo(0);
    sendBang();
}

void UIIcon::outputState(bool state)
{
    if (!is_enabled_)
        return;

    floatTo(0, state ? 1 : 0);
    send(state ? 1 : 0);
}

bool UIIcon::isSpecialIcon() const
{
    return prop_icon == sym_ceammc();
}

void UIIcon::clockTick()
{
    if (is_active_) {
        is_active_ = false;
        redrawAll();
    }
}

void UIIcon::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

bool UIIcon::propEnabled() const
{
    return is_enabled_ ? 1 : 0.f;
}

void UIIcon::propSetEnabled(bool v)
{
    if (is_enabled_ != v) {
        is_enabled_ = v;
        redrawAll();
    }
}

void UIIcon::setup()
{
    UIObjectFactory<UIIcon> obj("ui.icon", EBOX_GROWLINK);
    obj.setDefaultSize(24, 24);
    obj.internalProperty("size");
    obj.hideLabelInner();
    obj.usePresets();
    obj.useBang();
    obj.useFloat();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);

    obj.setPropertyDefaultValue(sym::props::name_background_color, "1 1 1 1");
    obj.setPropertyDefaultValue(sym::props::name_border_color, "1 1 1 1");

    obj.addProperty("icon", _("Icon"), "help", &UIIcon::prop_icon, icons_string, _("Main"));
    obj.addPropertyIntMenu("icon_size", _("Size"), "24", &UIIcon::prop_size, "18 24 36 48", _("Basic"));
    obj.addProperty(sym::props::name_active_color, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIIcon::prop_color_active);
    obj.setPropertyArgIndex("icon", 0);

    obj.addProperty("mode", _("Mode"), "button", &UIIcon::prop_mode, "toggle button bang", _("Main"));
    obj.addProperty("enabled", &UIIcon::propEnabled, &UIIcon::propSetEnabled);
    obj.setPropertyDefaultValue("enabled", "1");

    obj.addMethod("set", &UIIcon::m_set);
}

void setup_ui_icon()
{
    init_icon_list();

    ui_icon_tcl_output();

    for (auto& i : icons_list) {
        if (!i.name)
            break;

        sys_vgui("::ui::icon::create_image %s {%s} {%s} {%s} {%s}\n",
            i.name->s_name, i.data[0], i.data[1], i.data[2], i.data[3]);
    }

    UIIcon::setup();
}

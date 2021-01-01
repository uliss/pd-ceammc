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
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "icons/generated_icons.h"

#include <sstream>

static t_symbol* SYM_CEAMMC;
static t_symbol* SYM_MODE_BANG;
static t_symbol* SYM_MODE_BUTTON;
static t_symbol* SYM_MODE_TOGGLE;

static const t_rgba COLOR_DISABLED(hex_to_rgba("#D0D0D0"));

static const int SPEC_SIZE = 64;

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
    , image_layer_(asEBox(), gensym("image_layer"))
    , image_(0)
    , current_(&icons_list[0])
    , prop_icon(gensym("help"))
    , prop_mode(SYM_MODE_BUTTON)
    , prop_color_active(rgba_blue)
    , prop_size(24)
    , is_active_(false)
    , is_enabled_(true)
{
    auto ver = eclass_tcl_version();
    if (ver.first == 8 && ver.second < 6) {
        std::ostringstream ss;
        ss << "too old TCL version (" << ver.first << '.' << ver.second
           << "). PNG icons are not supported";
        throw std::runtime_error(ss.str());
    }

    createOutlet();
}

UIIcon::~UIIcon()
{
    eimage_destroy(image_);
}

void UIIcon::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    asEBox()->b_boxparameters.d_hideiolets = true;

    image_ = eimage_create(icon_default_24, 0, 0, ETEXT_UP_LEFT);

    if (args.size() > 0 && args[0].isSymbol() && !args[0].isProperty())
        m_set(args[0]);
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
        newrect->width = SPEC_SIZE;
        newrect->height = SPEC_SIZE;
        return;
    }

    float w = (newrect->width);

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

    newrect->width = prop_size;
    newrect->height = prop_size;
}

static int sizeToIdx(int s)
{
    switch (s) {
    case 18:
        return 0;
    case 24:
        return 1;
    case 36:
        return 2;
    case 48:
        return 3;
    default:
        return 1;
    }
}

void UIIcon::paint()
{
    drawBackground();
    drawImage();
}

void UIIcon::drawBackground()
{
    const auto r = rect();

    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    if (!is_enabled_)
        p.fillLayer(COLOR_DISABLED);
    else
        p.fillLayer(is_active_ ? prop_color_active : prop_color_background);
}

void UIIcon::drawImage()
{
    const auto r = rect();
    UIPainter p = image_layer_.painter(r);

    if (!p)
        return;

    if (image_ && current_) {
        // zoom icons 18 and 24 pix
        int idx = (prop_size <= 24) ? prop_size * zoom() : prop_size;
        eimage_set_base64_data(image_, current_->data[sizeToIdx(idx)]);
        egraphics_image(p.layer(), 0, 0, image_);
        egraphics_stroke(p.layer());
    }
}

void UIIcon::onBang()
{
    if (!is_enabled_)
        return;

    if (prop_mode == SYM_MODE_BANG || prop_mode == SYM_MODE_TOGGLE) {
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

    if (prop_mode == SYM_MODE_BANG) {
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
        UI_DBG << "available icons: ";
        UI_DBG << allIcons();
        return;
    }

    if (!is_enabled_)
        return;

    if (prop_mode == SYM_MODE_BANG) {
        is_active_ = true;
        clock_.delay(100);
        outputBang();
        redrawAll();
    } else if (prop_mode == SYM_MODE_BUTTON) {
        is_active_ = true;
        outputState(1);
        redrawAll();
    } else if (prop_mode == SYM_MODE_TOGGLE) {
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

    if (prop_mode == SYM_MODE_BUTTON) {
        is_active_ = false;
        outputState(0);
        redrawAll();
    }
}

void UIIcon::m_set(const AtomListView& lst)
{
    if (lst.empty()) {
        UI_ERR << "icon name expected";
        return;
    }

    if (!lst[0].isSymbol()) {
        UI_ERR << "icon name expected: " << lst;
        UI_ERR << "   available values are: " << allIcons();
        return;
    }

    t_symbol* name = lst[0].asSymbol();
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

    image_layer_.invalidate();
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

void UIIcon::setup()
{
    UIObjectFactory<UIIcon> obj("ui.icon", EBOX_GROWLINK);
    obj.setDefaultSize(24, 24);
    obj.hideProperty("size");
    obj.hideLabelInner();
    obj.usePresets();
    obj.useBang();
    obj.useFloat();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);

    obj.setPropertyDefaultValue(PROP_BACKGROUND_COLOR, "1 1 1 1");
    obj.setPropertyDefaultValue(PROP_BORDER_COLOR, "1 1 1 1");

    obj.addProperty("icon", _("Icon"), "help", &UIIcon::prop_icon, icons_string, _("Main"));
    obj.addPropertyIntMenu("icon_size", _("Size"), "24", &UIIcon::prop_size, "48 36 24 18", _("Basic"));
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIIcon::prop_color_active);

    obj.addProperty("mode", _("Mode"), "button", &UIIcon::prop_mode, "toggle button bang", _("Main"));
    obj.addProperty("enabled", &UIIcon::propEnabled, &UIIcon::propSetEnabled);

    obj.addMethod("set", &UIIcon::m_set);
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

    image_layer_.invalidate();
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
    return prop_icon == SYM_CEAMMC;
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
    image_layer_.invalidate();
    redraw();
}

AtomList UIIcon::propEnabled() const
{
    return Atom(is_enabled_ ? 1 : 0.f);
}

void UIIcon::propSetEnabled(const AtomListView& lst)
{
    bool v = lst.floatAt(0, 0);

    if (is_enabled_ != v) {
        is_enabled_ = v;
        redrawAll();
    }
}

void setup_ui_icon()
{
// on MacOSX with Tcl 8.6 where's crash on image scale
// there's build script "build_deps.sh" in ceammc/distrib/mac, that applies patch
// to prevent this crash. So this check is required while running on vanilla Pd as external library.
// Because current vanilla version now is not 0.47.1.
#ifdef __APPLE__
    int major = 0, minor = 0, bugfix = 0;
    sys_getversion(&major, &minor, &bugfix);
    bool ok = (major == PD_MAJOR_VERSION)
        && (minor == PD_MINOR_VERSION)
        && (bugfix == PD_BUGFIX_VERSION);

    if (!ok) {
        LIB_ERR << "[ui.icon] is not compatible with your distribution version: "
                << major << '.' << minor << '.' << bugfix;
        return;
    }
#endif

    SYM_CEAMMC = gensym("ceammc");
    SYM_MODE_BANG = gensym("bang");
    SYM_MODE_BUTTON = gensym("button");
    SYM_MODE_TOGGLE = gensym("toggle");

    init_icon_list();

    UIIcon::setup();
}

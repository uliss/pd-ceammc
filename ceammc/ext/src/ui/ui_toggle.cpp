#include "ui_toggle.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_toggle.tcl.h"

UIToggle::UIToggle()
    : prop_color_active(rgba_black)
    , prop_value_on_(1)
    , prop_value_off_(0)
    , value_(false)
{
    createOutlet();
}

t_float UIToggle::value() const
{
    return value_ ? 1 : 0;
}

void UIToggle::setValue(t_float v)
{
    value_ = (v != 0);
}

void UIToggle::flip()
{
    value_ = !value_;
}

void UIToggle::output()
{
    const t_float v = value() ? prop_value_on_ : prop_value_off_;
    floatTo(0, v);
    send(v);
}

void UIToggle::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8);
    newrect->height = pd_clip_min(newrect->height, 8);
}

void UIToggle::paint()
{
    sys_vgui("ui::toggle_update %s %lx %d %d %d #%6.6x %d\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_active), (int)value_);
}

void UIToggle::onMouseDown(t_object*, const t_pt&, const t_pt& abs_pt, long)
{
    flip();
    output();
    redrawAll();
}

void UIToggle::onBang()
{
    flip();
    output();
    redrawAll();
}

void UIToggle::onFloat(t_float f)
{
    setValue(f);
    redrawAll();
    output();
}

void UIToggle::m_set(t_float f)
{
    setValue(f);
    redrawAll();
}

void UIToggle::loadPreset(size_t idx)
{
    t_float f = PresetStorage::instance().floatValueAt(presetId(), idx);
    onFloat(f);
}

void UIToggle::storePreset(size_t idx)
{
    PresetStorage::instance().setFloatValueAt(presetId(), idx, value());
}

void UIToggle::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void UIToggle::setup()
{
    sys_vgui(ui_toggle_tcl);

    UIObjectFactory<UIToggle> obj("ui.toggle", EBOX_GROWLINK);
    obj.addAlias("ui.t");

    obj.setDefaultSize(15, 15);

    obj.useBang();
    obj.useFloat();
    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN);

    obj.addMethod("set", &UIToggle::m_set);

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIToggle::prop_color_active);
    obj.addProperty("value", &UIToggle::value, &UIToggle::setValue);

    obj.addFloatProperty("on_value", _("On value"), 1, &UIToggle::prop_value_on_, _("Main"));
    obj.addFloatProperty("off_value", _("Off value"), 0, &UIToggle::prop_value_off_, _("Main"));
}

void setup_ui_toggle()
{
    UIToggle::setup();
}

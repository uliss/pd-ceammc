#include "ui_toggle.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

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
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    if (value_) {
        float x = r.width - 1;
        float y = r.height - 1;

        p.setLineWidth(2);
        p.setColor(prop_color_active);

        p.drawLine(1, 1, x, y);
        p.drawLine(1, y, x, 1);
    }
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

void UIToggle::setup()
{
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

void UIToggle::redrawAll()
{
    bg_layer_.invalidate();
    redrawInnerArea();
}

void setup_ui_toggle()
{
    UIToggle::setup();
}

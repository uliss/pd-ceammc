#include "ui_knob.h"
#include "ceammc_cicm.h"
#include "ceammc_proxy.h"
#include "ceammc_ui.h"
#include "ceammc_ui_object.h"

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_preset.h"

extern "C" {
#include "m_imp.h"
}

static t_symbol* SYM_PLUS;
static t_symbol* SYM_MINUS;
static t_symbol* SYM_MUL;
static t_symbol* SYM_DIV;
static t_symbol* SYM_INC;
static t_symbol* SYM_DEC;

static const int KNOB_MIN_SIZE = 20;
static t_rgba BIND_MIDI_COLOR = hex_to_rgba("#FF3377");

using namespace ceammc;

static void draw_knob_arc(UIPainter& p, float cx, float cy, float r,
    float angle0, float angle1, float width, const t_rgba& color)
{
    p.setCapStyle(ECAPSTYLE_SQUARE);
    p.setLineWidth(width);
    p.setColor(color);
    p.drawCircleArc(cx, cy, r, angle0, angle1);
    p.setCapStyle(ECAPSTYLE_BUTT);
}

static void draw_knob_line(UIPainter& p, float cx, float cy, float r, float angle, float width, const t_rgba& color)
{
    p.setLineWidth(width);
    p.setCapStyle(ECAPSTYLE_ROUND);
    p.setColor(color);

    const float lx = r * cosf(angle);
    const float ly = r * sinf(angle);

    p.drawLine(cx, cy, cx + lx, cy - ly);
    p.setCapStyle(ECAPSTYLE_BUTT);
}

void UIKnob::setup()
{
    UIObjectFactory<UIKnob> obj("ui.knob", EBOX_GROWLINK);

    obj.useBang();
    obj.useFloat();
    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK);

    obj.addMethod("+", &UISingleValue::m_plus);
    obj.addMethod("-", &UISingleValue::m_minus);
    obj.addMethod("*", &UISingleValue::m_mul);
    obj.addMethod("/", &UISingleValue::m_div);
    obj.addMethod("++", &UISingleValue::m_increment);
    obj.addMethod("--", &UISingleValue::m_decrement);
    obj.addMethod("set", &UISingleValue::m_set);

    obj.setDefaultSize(40, 40);

    obj.addProperty("scale_color", _("Scale Color"), "0.6 0.6 0.6 1.0", &UIKnob::prop_scale_color);
    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIKnob::prop_knob_color);

    obj.addProperty("min", _("Minimum Value"), 0, &UISingleValue::prop_min, "Bounds");
    obj.addProperty("max", _("Maximum Value"), 1, &UISingleValue::prop_max, "Bounds");
    obj.addProperty("show_range", _("Show range"), false, &UIKnob::show_range_);
    obj.addProperty("active_scale", _("Draw active scale"), false, &UIKnob::draw_active_scale_);
    obj.addProperty("midi_channel", _("MIDI channel"), 0, &UISingleValue::prop_midi_chn, "MIDI");
    obj.setPropertyRange("midi_channel", 0, 16);
    obj.addProperty("midi_control", _("MIDI control"), 0, &UISingleValue::prop_midi_ctl, "MIDI");
    obj.setPropertyRange("midi_control", 0, 128);
    obj.addProperty("midi_pickup", _("MIDI pickup"), true, &UISingleValue::prop_midi_pickup, "MIDI");

    obj.addProperty("value", &UISingleValue::realValue, &UISingleValue::setRealValue);
}

UIKnob::UIKnob()
    : txt_font(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , txt_min(txt_font.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , txt_max(txt_font.font(), ColorRGBA::black(), ETEXT_DOWN_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP)
    , show_range_(0)
    , draw_active_scale_(0)
    , prop_knob_color(rgba_black)
    , prop_scale_color(rgba_black)
{
    click_pos_.x = 0;
    click_pos_.y = 0;
}

void UIKnob::paint()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (p) {
        if (show_range_) {
            const float xoff = (1 + (r.width > 50)) * zoom();
            const float yoff = (1 + (r.height > 50)) * zoom();

            char buf[10];
            sprintf(buf, "%g", minValue());

            txt_min.set(buf, xoff, r.height - yoff, r.width * 2, r.height / 2);
            p.drawText(txt_min);

            sprintf(buf, "%g", maxValue());
            txt_max.set(buf, r.width - xoff, r.height - yoff, r.width, r.height / 2);
            p.drawText(txt_max);
        }
    }

    UIPainter kp = knob_layer_.painter(r);
    if (kp) {
        const float cx = r.width * 0.5f;
        const float cy = r.height * 0.5f;

        float radius_scale = 0.85f;
        const float radius = cx * radius_scale;
        const float arc_scale = 0.78f;
        const float arc_full = -(EPD_2PI)*arc_scale;
        const float arc_angle_offset = -(EPD_PI2 + (1 - arc_scale) * EPD_PI);
        const float arc_begin = arc_angle_offset;
        const float arc_end = arc_full + arc_angle_offset;
        const float value_angle = prop_value * arc_full + arc_angle_offset;

        // adjust knob
        float line_width = int(r.height / 20) + 1;

#ifdef __WIN32
        line_width *= 0.5;
#endif

        if (r.height < 30) {
            radius_scale = 0.55f;
        }

        if (draw_active_scale_) {
            // draw active arc
            draw_knob_arc(kp, cx, cy, radius, arc_begin, value_angle, line_width, prop_knob_color);

            // draw passive arc
            draw_knob_arc(kp, cx, cy, radius, value_angle, arc_end, line_width, prop_scale_color);
        } else {
            // draw full arc
            draw_knob_arc(kp, cx, cy, radius, arc_begin, arc_end, line_width, prop_scale_color);
        }

        // draw knob line
        draw_knob_line(kp, cx, cy, radius, value_angle, line_width, prop_knob_color);
    }
}

void UIKnob::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, KNOB_MIN_SIZE);
    newrect->height = pd_clip_min(newrect->height, KNOB_MIN_SIZE);
}

void UIKnob::onMouseDrag(t_object*, const t_pt& pt, long)
{
    t_float delta = (click_pos_.y - pt.y) / height();
    setValue(value() + delta);
    click_pos_ = pt;
    redrawKnob();
    output();
}

void UIKnob::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (modifiers & EMOD_RIGHT) {
        UIPopupMenu menu(asEObj(), "popup", abs_pt);
        menu.addItem(_("min"));
        menu.addItem(_("center"));
        menu.addItem(_("max"));
        return;
    }

    click_pos_ = pt;
}

void UIKnob::onPopup(t_symbol* menu_name, long item_idx)
{
    if (menu_name != gensym("popup"))
        return;

    switch (item_idx) {
    case 0:
        onFloat(prop_min);
        break;
    case 1:
        onFloat(convert::lin2lin<t_float>(0.5, 0, 1, prop_min, prop_max));
        break;
    case 2:
        onFloat(prop_max);
        break;
    default:
        UI_ERR << "unknown popup menu index: " << item_idx;
        break;
    }
}

void setup_ui_knob()
{
    SYM_PLUS = gensym("+");
    SYM_MINUS = gensym("-");
    SYM_MUL = gensym("*");
    SYM_DIV = gensym("/");
    SYM_INC = gensym("++");
    SYM_DEC = gensym("--");

    UIKnob::setup();
}

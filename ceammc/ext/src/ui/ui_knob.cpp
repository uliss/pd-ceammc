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

#include "ui_knob.tcl.h"

static t_symbol* SYM_PLUS;
static t_symbol* SYM_MINUS;
static t_symbol* SYM_MUL;
static t_symbol* SYM_DIV;
static t_symbol* SYM_INC;
static t_symbol* SYM_DEC;
static t_symbol* SYM_POPUP_LINEAR;
static t_symbol* SYM_POPUP_LOG;

constexpr int KNOB_MIN_SIZE = 20;
static t_rgba BIND_MIDI_COLOR = hex_to_rgba("#FF3377");
constexpr int MIDI_CTL_NONE = -1;
constexpr int MIDI_CTL_MIN = 0;
constexpr int MIDI_CTL_MAX = 127;

namespace ceammc {

UIKnob::UIKnob()
    : show_range_(0)
    , show_value_(0)
    , draw_active_scale_(0)
    , prop_knob_color(rgba_black)
    , prop_scale_color(rgba_black)
{
    click_pos_.x = 0;
    click_pos_.y = 0;

    initPopupMenu("knob",
        { { _("min"), [this](const t_pt&) { onFloat(prop_min); } },
            { _("center"), [this](const t_pt&) { onFloat(convert::lin2lin<t_float>(0.5, 0, 1, prop_min, prop_max)); } },
            { _("max"), [this](const t_pt&) { onFloat(prop_max); } } });
}

void UIKnob::paint()
{
    sys_vgui("ui::knob::update %s %lx "
             "%d %d %d "
             "%f %f %f "
             "%d %d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        minValue(), maxValue(), value(),
        show_range_, show_value_, draw_active_scale_,
        rgba_to_hex_int(prop_color_border),
        rgba_to_hex_int(prop_scale_color),
        rgba_to_hex_int(prop_knob_color),
        rgba_to_hex_int(prop_color_label));
}

void UIKnob::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, KNOB_MIN_SIZE);
    newrect->height = pd_clip_min(newrect->height, KNOB_MIN_SIZE);
}

void UIKnob::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    output();
}

void UIKnob::onMouseDrag(t_object*, const t_pt& pt, long modifiers)
{
    t_float delta = (click_pos_.y - pt.y) / height();
    if (modifiers & EMOD_SHIFT)
        delta *= 0.1;

    setKnobPhase(knobPhase() + delta);
    click_pos_ = pt;
    redrawKnob();
    output();
}

void UIKnob::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    click_pos_ = pt;
}

void UIKnob::onMouseWheel(const t_pt& pt, long modifiers, double delta)
{
    float k = 0.01;
    if (modifiers & EMOD_SHIFT)
        k *= 0.1;

    setKnobPhase(knobPhase() + delta * k);
    redrawKnob();
    output();
}

void UIKnob::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    UIPopupMenu menu(asEObj(), popup_menu_list_.front(), abs_pt, pt);
    if (scaleMode() != LINEAR)
        menu.disable(_("center"));
}

void UIKnob::setup()
{
    UISingleValue::setup();

    UIObjectFactory<UIKnob> obj("ui.knob", EBOX_GROWLINK);
    obj.addAlias("ui.k");

    obj.useBang();
    obj.useFloat();
    obj.usePresets();
    obj.usePopup();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK | UI_MOUSE_WHEEL);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addMethod("+", &UISingleValue::m_plus);
    obj.addMethod("-", &UISingleValue::m_minus);
    obj.addMethod("*", &UISingleValue::m_mul);
    obj.addMethod("/", &UISingleValue::m_div);
    obj.addMethod("++", &UISingleValue::m_increment);
    obj.addMethod("--", &UISingleValue::m_decrement);
    obj.addMethod("set", &UISingleValue::m_set);
    obj.addMethod("random", &UISingleValue::m_random);

    obj.setDefaultSize(40, 40);

    obj.addProperty("scale_color", _("Scale Color"), "0.6 0.6 0.6 1.0", &UIKnob::prop_scale_color);
    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIKnob::prop_knob_color);

    obj.addFloatProperty("min", _("Minimum Value"), 0, &UISingleValue::prop_min, "Bounds");
    obj.addFloatProperty("max", _("Maximum Value"), 1, &UISingleValue::prop_max, "Bounds");
    obj.addBoolProperty("show_range", _("Show range"), false, &UIKnob::show_range_);
    obj.addBoolProperty("show_value", _("Show value"), false, &UIKnob::show_value_);
    obj.addProperty("scale", _("Scale Mode"), "linear", &UISingleValue::prop_scale, "linear log", "Main");

    obj.addBoolProperty("active_scale", _("Draw active scale"), false, &UIKnob::draw_active_scale_);
    obj.addIntProperty("midi_channel", _("MIDI channel"), 0, &UISingleValue::prop_midi_chn, "MIDI");
    obj.setPropertyRange("midi_channel", 0, 16);
    obj.addIntProperty("midi_control", _("MIDI control"), MIDI_CTL_NONE, &UISingleValue::prop_midi_ctl, "MIDI");
    obj.setPropertyRange("midi_control", MIDI_CTL_NONE, MIDI_CTL_MAX);
    obj.addBoolProperty("midi_pickup", _("MIDI pickup"), true, &UISingleValue::prop_pickup_midi, "MIDI");

    obj.addProperty("value", &UISingleValue::value, &UISingleValue::setValue);
    obj.showProperty("value");
}

}

void setup_ui_knob()
{
    sys_gui(ui_knob_tcl);

    SYM_PLUS = gensym("+");
    SYM_MINUS = gensym("-");
    SYM_MUL = gensym("*");
    SYM_DIV = gensym("/");
    SYM_INC = gensym("++");
    SYM_DEC = gensym("--");

    SYM_POPUP_LINEAR = gensym("popup_lin");
    SYM_POPUP_LOG = gensym("popup_log");

    ceammc::UIKnob::setup();
}

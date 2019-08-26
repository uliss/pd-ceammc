#include "ui_slider.h"
#include "ceammc_cicm.h"
#include "ceammc_convert.h"
#include "ceammc_ui.h"

static const float ALPHA_BLEND = 0.4;

UISlider::UISlider()
    : is_horizontal_(false)
    , value_last_(0)
    , value_ref_(0)
    , font_(gensym(FONT_FAMILY), FONT_SIZE - 2)
    , txt_value_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT)
    , prop_knob_color(hex_to_rgba(DEFAULT_ACTIVE_COLOR))
    , prop_text_color(hex_to_rgba(DEFAULT_BORDER_COLOR))
    , prop_rel_mode(0)
    , prop_active_scale(0)
    , prop_value_pos(gensym("center"))
    , prop_value_precision(2)
{
}

void UISlider::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UISingleValue::init(name, args, usePresets);
    if (name == gensym("ui.hsl")) {
        is_horizontal_ = true;
        std::swap(asEBox()->b_rect.width, asEBox()->b_rect.height);
    }
}

void UISlider::paint()
{
    const t_rect r = rect();
    UIPainter kp = knob_layer_.painter(r);
    if (kp) {
        kp.setColor(prop_knob_color);
        kp.setLineWidth(2 * zoom());

        if (is_horizontal_) { // horizontal
            float x = value() * r.width;
            kp.drawLine(x, 0, x, r.height);

            if (prop_active_scale) {
                kp.setColor(rgba_color_sum(&prop_color_background, &prop_knob_color, ALPHA_BLEND));
                kp.drawRect(0, 0, x, r.height);
                kp.fill();
            }

            if (prop_show_value) {
                char fmt[] = "%..f";
                fmt[2] = char(prop_value_precision) + '0';
                char buf[16];
                snprintf(buf, sizeof(buf), fmt, value());

                txt_value_.setColor(prop_text_color);
                if (prop_value_pos == gensym("left")) {
                    txt_value_.setAnchor(ETEXT_LEFT);
                    txt_value_.setJustify(ETEXT_JLEFT);
                    txt_value_.set(buf, 2, height() / 2, width() / 2, height());
                } else if (prop_value_pos == gensym("center")) {
                    txt_value_.setAnchor(ETEXT_CENTER);
                    txt_value_.setJustify(ETEXT_JCENTER);
                    txt_value_.set(buf, width() / 2, height() / 2, width() / 2, height());
                } else if (prop_value_pos == gensym("right")) {
                    txt_value_.setAnchor(ETEXT_RIGHT);
                    txt_value_.setJustify(ETEXT_JRIGHT);
                    txt_value_.set(buf, width() - 2, height() / 2, width() / 2, height());
                }

                kp.drawText(txt_value_);
            }
        } else {
            float y = (1.0 - value()) * r.height;
            kp.drawLine(0, y, r.width, y);

            if (prop_active_scale) {
                kp.setColor(rgba_color_sum(&prop_color_background, &prop_knob_color, ALPHA_BLEND));
                kp.drawRect(0, y, r.width, r.height * value());
                kp.fill();
            }
        }
    }
}

void UISlider::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8.);
    newrect->height = pd_clip_min(newrect->height, 8.);

    is_horizontal_ = (newrect->width > newrect->height);

    if (is_horizontal_)
        newrect->width = pd_clip_min(newrect->width, 50.);
    else
        newrect->height = pd_clip_min(newrect->height, 50.);
}

void UISlider::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long)
{
    t_rect r = rect();

    if (prop_rel_mode) {
        value_last_ = prop_value;

        if (is_horizontal_)
            value_ref_ = clip<float>(pt.x / r.width, 0, 1);
        else
            value_ref_ = clip<float>(1.0 - (pt.y / r.height), 0, 1);
    } else {
        if (is_horizontal_)
            setValue(pt.x / r.width);
        else
            setValue(1.0 - (pt.y / r.height));

        output();

        // redraw immidiately
        redrawKnob();
    }
}

void UISlider::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    t_rect r = rect();

    if (prop_rel_mode) {
        float newvalue;
        if (is_horizontal_)
            newvalue = pt.x / r.width;
        else
            newvalue = (1.0 - (pt.y / r.height));

        setValue(value_last_ + newvalue - value_ref_);

        if (prop_value == prop_min || prop_value == prop_max) {
            value_last_ = prop_value;
            value_ref_ = newvalue;
        }
    } else {
        if (is_horizontal_)
            setValue(pt.x / r.width);
        else
            setValue(1.0 - (pt.y / r.height));
    }

    output();
    redrawKnob();
}

void UISlider::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    output();
}

void UISlider::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    if (modifiers == EMOD_SHIFT)
        return UISingleValue::onDblClick(view, pt, modifiers);

    t_canvas* c = reinterpret_cast<t_canvas*>(view);
    if (c->gl_edit)
        resize(height() / zoom(), width() / zoom());
}

void UISlider::setup()
{
    UIObjectFactory<UISlider> obj("ui.slider", EBOX_GROWINDI);
    obj.addAlias("ui.hsl");
    obj.addAlias("ui.vsl");

    obj.useBang();
    obj.useFloat();
    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_DBL_CLICK | UI_MOUSE_UP);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addMethod("+", &UISingleValue::m_plus);
    obj.addMethod("-", &UISingleValue::m_minus);
    obj.addMethod("*", &UISingleValue::m_mul);
    obj.addMethod("/", &UISingleValue::m_div);
    obj.addMethod("++", &UISingleValue::m_increment);
    obj.addMethod("--", &UISingleValue::m_decrement);
    obj.addMethod("set", &UISingleValue::m_set);

    obj.setDefaultSize(15, 120);

    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UISlider::prop_knob_color);
    obj.addProperty("text_color", _("Text Color"), DEFAULT_TEXT_COLOR, &UISlider::prop_text_color);

    obj.addProperty("mode", _("Relative Mode"), false, &UISlider::prop_rel_mode, "Main");
    obj.addProperty("min", _("Minimum Value"), 0, &UISingleValue::prop_min, "Bounds");
    obj.addProperty("max", _("Maximum Value"), 1, &UISingleValue::prop_max, "Bounds");

    obj.addProperty("midi_channel", _("MIDI channel"), 0, &UISingleValue::prop_midi_chn, "MIDI");
    obj.setPropertyRange("midi_channel", 0, 16);
    obj.addProperty("midi_control", _("MIDI control"), 0, &UISingleValue::prop_midi_ctl, "MIDI");
    obj.setPropertyRange("midi_control", 0, 128);
    obj.addProperty("midi_pickup", _("MIDI pickup"), true, &UISingleValue::prop_midi_pickup, "MIDI");
    obj.addProperty("active_scale", _("Draw active scale"), false, &UISlider::prop_active_scale, "Main");
    obj.addProperty("show_value", _("Show value in horizontal mode"), false, &UISingleValue::prop_show_value, "Misc");
    obj.addProperty("value_pos", _("Value position"), "center", &UISlider::prop_value_pos, "left center right", "Misc");
    obj.addProperty("value_precision", _("Precision"), 2, &UISlider::prop_value_precision, "Main");
    obj.setPropertyRange("value_precision", 0, 7);

    obj.addProperty("value", &UISingleValue::realValue, &UISingleValue::setRealValue);
}

void setup_ui_slider()
{
    UISlider::setup();
}

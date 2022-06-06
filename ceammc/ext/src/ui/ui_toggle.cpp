#include "ui_toggle.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_toggle.tcl.h"

static t_symbol* midi_ctl_sym()
{
    static t_symbol* sym = gensym("#ctlin");
    return sym;
}

static t_rgba BIND_MIDI_COLOR = hex_to_rgba("#FF3377");

UIToggle::UIToggle()
    : midi_proxy_(this, &UIToggle::onMidiCtrl)
    , listen_midi_ctrl_(false)
    , prop_midi_chn(0)
    , prop_midi_ctl(0)
    , prop_color_active(rgba_black)
    , prop_value_on_(1)
    , prop_value_off_(0)
    , value_(false)
{
    createOutlet();
}

void UIToggle::startListenMidi()
{
    listen_midi_ctrl_ = true;
    midi_proxy_.bind(midi_ctl_sym());
    LIB_DBG << "move MIDI control to bind";

    asEBox()->b_boxparameters.d_bordercolor = BIND_MIDI_COLOR;
    asEBox()->b_ready_to_draw = true;
    redraw();
}

void UIToggle::stopListenMidi()
{
    listen_midi_ctrl_ = false;
    asEBox()->b_boxparameters.d_bordercolor = prop_color_border;
    asEBox()->b_ready_to_draw = true;
    redraw();
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

void UIToggle::onDblClick(t_object* x, const t_pt& pt, long mod)
{
    if (!(mod & EMOD_SHIFT))
        return onMouseDown(x, pt, pt, mod);

    if (!listen_midi_ctrl_)
        startListenMidi();
    else
        stopListenMidi();
}

void UIToggle::onPropChange(t_symbol *prop_name)
{
    UIObject::onPropChange(prop_name);

    if (prop_name == gensym("midi_control")) {
        if (prop_midi_ctl != 0) {
            // info
            std::ostringstream ss;
            ss << "binded to MIDI ctl #"
               << prop_midi_ctl
               << (prop_midi_chn == 0 ? " on all channels" : " on channel: ");

            if (prop_midi_chn > 0)
                ss << prop_midi_chn;

            UI_DBG << ss.str();
            midi_proxy_.bind(midi_ctl_sym());
        } else
            midi_proxy_.unbind();
    }

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

void UIToggle::onMidiCtrl(const AtomListView& l)
{
    // invalid format
    if (l.size() != 3)
        return;

    const int CTL_NUM = l[0].asInt();
    const int CTL_CHAN = l[2].asInt();
    const t_float CTL_VAL = l[1].asFloat();

    if (listen_midi_ctrl_) {
        stopListenMidi();
        UI_DBG << "binded to CTL #" << CTL_NUM;
        prop_midi_ctl = CTL_NUM;
    } else {
        // skip all
        if (prop_midi_ctl == 0)
            return;

        // skip others
        if (CTL_NUM != prop_midi_ctl)
            return;

        if (prop_midi_chn > 0 && CTL_CHAN != prop_midi_chn)
            return;

        setValue(CTL_VAL > 63);
        redrawAll();
        output();
    }
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
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DBL_CLICK);

    obj.addMethod("set", &UIToggle::m_set);

    obj.addProperty("active_color", _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIToggle::prop_color_active);
    obj.addProperty("value", &UIToggle::value, &UIToggle::setValue);

    obj.addFloatProperty("on_value", _("On value"), 1, &UIToggle::prop_value_on_, _("Main"));
    obj.addFloatProperty("off_value", _("Off value"), 0, &UIToggle::prop_value_off_, _("Main"));

    obj.addProperty("midi_channel", _("MIDI channel"), 0, &UIToggle::prop_midi_chn, "MIDI");
    obj.setPropertyRange("midi_channel", 0, 16);
    obj.addProperty("midi_control", _("MIDI control"), -1, &UIToggle::prop_midi_ctl, "MIDI");
    obj.setPropertyRange("midi_control", -1, 127);
}

void setup_ui_toggle()
{
    UIToggle::setup();
}

#include "ui_single_value.h"
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

static t_rgba BIND_MIDI_COLOR = hex_to_rgba("#FF3377");

using namespace ceammc;

static t_symbol* midi_ctl_sym()
{
    static t_symbol* sym = gensym("#ctlin");
    return sym;
}

UISingleValue::UISingleValue()
    : midi_proxy_(this, &UISingleValue::onMidiCtrl)
    , pick_value_side_(0)
    , listen_midi_ctrl_(false)
    , midi_value_picked_(false)
    , prop_value(0)
    , prop_min(0)
    , prop_max(1)
    , prop_midi_chn(0)
    , prop_midi_ctl(0)
    , prop_midi_pickup(0)
    , prop_show_value(0)
    , knob_layer_(asEBox(), gensym("knob_layer"))
{
    createOutlet();
}

t_float UISingleValue::range() const { return prop_max - prop_min; }

t_float UISingleValue::value() const { return prop_value; }

t_float UISingleValue::minValue() const { return prop_min; }

t_float UISingleValue::maxValue() const { return prop_max; }

void UISingleValue::setValue(t_float v) { prop_value = clip<t_float>(v, 0, 1); }

int UISingleValue::midiChannel() const { return prop_midi_chn; }

int UISingleValue::midiControl() const { return prop_midi_ctl; }

t_float UISingleValue::realValue() const
{
    const t_float r = range();
    return r < 0 ? (prop_value - 1) * r + prop_max : prop_value * r + prop_min;
}

void UISingleValue::setRealValue(t_float v)
{
    const t_float r = range();
    if (r < 0)
        prop_value = ((clip(v, prop_max, prop_min) - prop_max) / r) + 1;
    else if (r > 0)
        prop_value = (clip(v, prop_min, prop_max) - prop_min) / r;
    else
        UI_ERR << "zero range";
}

void UISingleValue::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    if (prop_midi_chn > 0 || prop_midi_ctl > 0)
        midi_proxy_.bind(midi_ctl_sym());
}

void UISingleValue::onPropChange(t_symbol* prop_name)
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

    redrawKnob();
}

void UISingleValue::output()
{
    floatTo(0, realValue());
    send(realValue());
}

void UISingleValue::onBang()
{
    output();
}

void UISingleValue::onFloat(t_float f)
{
    setRealValue(f);
    redrawKnob();
    output();
}

void UISingleValue::onMidiCtrl(const AtomList& l)
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

        float v = CTL_VAL / 127;
        // control value is set and we need to pickup midi control value
        // do it once
        if (prop_midi_pickup && !midi_value_picked_) {
            // simple case - pickup equal value
            if (v == prop_value) {
                midi_value_picked_ = true;
                return;
            }

            // pickup when value is over
            char side = (v < prop_value) ? -1 : 1;
            // init state
            if (pick_value_side_ == 0)
                pick_value_side_ = side;
            // pickup the value
            else if (pick_value_side_ == -side) {
                midi_value_picked_ = true;
                setValue(v);
                redrawKnob();
                output();
            }

            return;
        }

        setValue(v);
        redrawKnob();
        output();
    }
}

void UISingleValue::m_set(t_float f)
{
    setRealValue(f);
    redrawKnob();
}

void UISingleValue::m_plus(t_float f)
{
    m_set(realValue() + f);
}

void UISingleValue::m_minus(t_float f)
{
    m_set(realValue() - f);
}

void UISingleValue::m_mul(t_float f)
{
    m_set(realValue() * f);
}

void UISingleValue::m_div(t_float f)
{
    if (f == 0) {
        UI_ERR << "division by zero";
        return;
    }

    m_set(realValue() / f);
}

void UISingleValue::m_increment()
{
    m_set(realValue() + 1);
}

void UISingleValue::m_decrement()
{
    m_set(realValue() - 1);
}

void UISingleValue::loadPreset(size_t idx)
{
    t_float f = PresetStorage::instance().floatValueAt(presetId(), idx);
    onFloat(f);
}

void UISingleValue::storePreset(size_t idx)
{
    PresetStorage::instance().setFloatValueAt(presetId(), idx, realValue());
}

void UISingleValue::onDblClick(t_object*, const t_pt&, long mod)
{
    if (!(mod & EMOD_SHIFT)) {
        onFloat(minValue() + maxValue() / 2);
        return;
    }

    if (!listen_midi_ctrl_)
        startListenMidi();
    else
        stopListenMidi();
}

void UISingleValue::setDrawParams(t_edrawparams* params)
{
    UIObject::setDrawParams(params);
    params->d_bordercolor = listen_midi_ctrl_ ? BIND_MIDI_COLOR : prop_color_border;
}

void UISingleValue::startListenMidi()
{
    listen_midi_ctrl_ = true;
    midi_proxy_.bind(midi_ctl_sym());
    LIB_DBG << "move MIDI control to bind";

    asEBox()->b_boxparameters.d_bordercolor = BIND_MIDI_COLOR;
    asEBox()->b_ready_to_draw = true;
    redraw();
}

void UISingleValue::stopListenMidi()
{
    listen_midi_ctrl_ = false;
    asEBox()->b_boxparameters.d_bordercolor = prop_color_border;
    asEBox()->b_ready_to_draw = true;
    redraw();
}

void UISingleValue::redrawKnob()
{
    knob_layer_.invalidate();
    redrawInnerArea();
}

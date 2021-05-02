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

// see ui_gain.cpp
static t_rgba BIND_MIDI_COLOR = hex_to_rgba("#FF3377");
static t_rgba PICKUP_MIDI_COLOR = hex_to_rgba("#3377FF");
static t_symbol* SYM_MIN;
static t_symbol* SYM_MAX;
static t_symbol* SYM_LINEAR;
static t_symbol* SYM_LOG;

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
    , midi_pickup_done_(false)
    , knob_phase(0)
    , prop_value(0)
    , prop_min(0)
    , prop_max(1)
    , prop_midi_chn(0)
    , prop_midi_ctl(0)
    , prop_pickup_midi(0)
    , prop_show_value(0)
    , prop_scale(SYM_LINEAR)
    , knob_layer_(asEBox(), gensym("knob_layer"))
{
    createOutlet();
}

t_float UISingleValue::range() const { return prop_max - prop_min; }

t_float UISingleValue::knobPhase() const { return knob_phase; }

t_float UISingleValue::minValue() const { return prop_min; }

t_float UISingleValue::maxValue() const { return prop_max; }

void UISingleValue::setKnobPhase(t_float v)
{
    knob_phase = clip<t_float>(v, 0, 1);

    if (prop_scale == SYM_LINEAR)
        prop_value = convert::lin2lin<t_float, 0, 1>(knob_phase, prop_min, prop_max);
    else if (prop_scale == SYM_LOG) {
        const bool ok = (prop_min > 0) && (prop_max > prop_min);
        if (!ok) {
            UI_ERR << "invalid min/max bounds: " << prop_min << " " << prop_max << " in logarithm mode";
            return;
        }

        prop_value = convert::lin2exp(knob_phase, 0, 1, prop_min, prop_max);
    }
}

int UISingleValue::midiChannel() const { return prop_midi_chn; }

int UISingleValue::midiControl() const { return prop_midi_ctl; }

t_float UISingleValue::value() const
{
    return prop_value;
}

void UISingleValue::setValueLin(t_float v)
{
    const t_float r = range();
    if (r < 0) {
        prop_value = clip(v, prop_max, prop_min);
        knob_phase = ((prop_value - prop_max) / r) + 1;
    } else if (r > 0) {
        prop_value = clip(v, prop_min, prop_max);
        knob_phase = (prop_value - prop_min) / r;
    } else {
        prop_value = prop_min;
        knob_phase = 0;
        UI_ERR << "zero range";
    }
}

void UISingleValue::setValue(t_float v)
{
    if (prop_scale == SYM_LINEAR) {
        setValueLin(v);
    } else if (prop_scale == SYM_LOG) {
        bool ok = prop_min > 0 && prop_max > prop_min;
        if (!ok) {
            UI_ERR << "invalid property range: " << prop_min << " " << prop_max
                   << ". @min should be >0 in log mode";
            return;
        }

        prop_value = clip(v, prop_min, prop_max);
        knob_phase = convert::exp2lin(prop_value, prop_min, prop_max, 0, 1);
    }
}

void UISingleValue::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    if (prop_midi_chn > 0 || prop_midi_ctl > 0)
        midi_proxy_.bind(midi_ctl_sym());

    // process min positional args
    const auto has_min = args.size() > 0 && args[0].isFloat();
    if (has_min)
        prop_min = args[0].asT<t_float>();

    // process max positional args
    if (has_min && args.size() > 1 && args[1].isFloat())
        prop_max = args[1].asT<t_float>();

    prop_value = prop_min;
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
    } else if (prop_name == SYM_MAX || prop_name == SYM_MIN) {
        // put value in valid range
        prop_value = clip_any<float>(prop_value, prop_min, prop_max);
        knob_layer_.invalidate();
    }

    redrawKnob();
}

void UISingleValue::output()
{
    floatTo(0, value());
    send(value());
}

void UISingleValue::onBang()
{
    output();
}

void UISingleValue::onFloat(t_float f)
{
    setValue(f);
    redrawKnob();
    output();
}

void UISingleValue::onMidiCtrl(const AtomListView& l)
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

        const float w = convert::lin2lin_clip<float, 0, 127>(CTL_VAL, 0, 1);

        // control value is set and we need to pickup midi control value
        // do it once
        if (prop_pickup_midi && !midi_pickup_done_) {
            // simple case - pickup equal value
            if (w == knob_phase) {
                midi_pickup_done_ = true;
            } else {
                // pickup when value is over
                char side = (w < knob_phase) ? -1 : 1;
                // init state
                if (pick_value_side_ == 0)
                    pick_value_side_ = side;
                // pickup the value
                else if (pick_value_side_ == -side) {
                    midi_pickup_done_ = true;
                    setKnobPhase(w);
                    redrawKnob();
                    output();
                }
            }

            if (midi_pickup_done_) {
                setKnobPhase(w);

                asEBox()->b_boxparameters.d_bordercolor = prop_color_border;
                redrawKnob();
                output();
            } else {
                asEBox()->b_boxparameters.d_bordercolor = PICKUP_MIDI_COLOR;
                redraw();
            }

            return;
        }

        setKnobPhase(w);
        redrawKnob();
        output();
    }
}

void UISingleValue::m_set(t_float f)
{
    setValue(f);
    redrawKnob();
}

void UISingleValue::m_plus(t_float f)
{
    m_set(value() + f);
}

void UISingleValue::m_minus(t_float f)
{
    m_set(value() - f);
}

void UISingleValue::m_mul(t_float f)
{
    m_set(value() * f);
}

void UISingleValue::m_div(t_float f)
{
    if (f == 0) {
        UI_ERR << "division by zero";
        return;
    }

    m_set(value() / f);
}

void UISingleValue::m_increment()
{
    m_set(value() + 1);
}

void UISingleValue::m_decrement()
{
    m_set(value() - 1);
}

void UISingleValue::loadPreset(size_t idx)
{
    t_float f = PresetStorage::instance().floatValueAt(presetId(), idx);
    onFloat(f);
}

void UISingleValue::interpPreset(t_float idx)
{
    t_float f = PresetStorage::instance().floatValueAt(presetId(), idx);
    onFloat(f);
}

void UISingleValue::storePreset(size_t idx)
{
    PresetStorage::instance().setFloatValueAt(presetId(), idx, value());
}

void UISingleValue::onDblClick(t_object*, const t_pt&, long mod)
{
    if (!(mod & EMOD_SHIFT)) { // center value
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
    redraw();
}

UISingleValue::ScaleMode UISingleValue::scaleMode() const
{
    if (prop_scale == SYM_LINEAR)
        return LINEAR;
    else if (prop_scale == SYM_LOG)
        return LOG;
    else
        return LINEAR;
}

void UISingleValue::setup()
{
    if (!SYM_MIN)
        SYM_MIN = gensym("min");

    if (!SYM_MAX)
        SYM_MAX = gensym("max");

    if (!SYM_LINEAR)
        SYM_LINEAR = gensym("linear");

    if (!SYM_LOG)
        SYM_LOG = gensym("log");
}

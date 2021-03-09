#ifndef UI_SINGLE_VALUE_H
#define UI_SINGLE_VALUE_H

#include "ceammc_cicm.h"
#include "ceammc_proxy.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UISingleValue : public UIObject {
    PdListProxy<UISingleValue> midi_proxy_;

    char pick_value_side_;
    bool listen_midi_ctrl_;
    bool midi_pickup_done_;
    t_float knob_phase;

public:
    t_float prop_value;
    t_float prop_min;
    t_float prop_max;
    int prop_midi_chn;
    int prop_midi_ctl;
    int prop_pickup_midi;
    int prop_show_value;
    t_symbol* prop_scale;

    enum ScaleMode {
        LINEAR,
        LOG
    };

public:
    UISingleValue();

    t_float range() const;
    t_float minValue() const;
    t_float maxValue() const;

    int midiChannel() const;
    int midiControl() const;

    t_float knobPhase() const;
    void setKnobPhase(t_float v);

    t_float value() const;
    void setValue(t_float v);

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void onPropChange(t_symbol* prop_name);
    void output();
    void setDrawParams(t_edrawparams* params);

    void onBang();
    void onFloat(t_float f);
    void onMidiCtrl(const AtomListView& l);
    void onDblClick(t_object*, const t_pt&, long mod);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    void m_set(t_float f);
    void m_plus(t_float f);
    void m_minus(t_float f);
    void m_mul(t_float f);
    void m_div(t_float f);
    void m_increment();
    void m_decrement();

    void startListenMidi();
    void stopListenMidi();

    void redrawKnob();

    ScaleMode scaleMode() const;

protected:
    UILayer knob_layer_;

    static void setup();

private:
    void setValueLin(t_float v);
};

#endif // UI_SINGLE_VALUE_H

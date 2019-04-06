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
    bool midi_value_picked_;

public:
    t_float prop_value;
    t_float prop_min;
    t_float prop_max;
    int prop_midi_chn;
    int prop_midi_ctl;
    int prop_midi_pickup;
    int prop_show_value;

public:
    UISingleValue();

    t_float range() const;
    t_float value() const;
    t_float minValue() const;
    t_float maxValue() const;
    void setValue(t_float v);
    int midiChannel() const;
    int midiControl() const;

    t_float realValue() const;
    void setRealValue(t_float v);

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);
    void output();
    void setDrawParams(t_edrawparams* params);

    void onBang();
    void onFloat(t_float f);
    void onMidiCtrl(const AtomList& l);
    void onDblClick(t_object*, const t_pt&, long mod);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

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

protected:
    UILayer knob_layer_;
};

#endif // UI_SINGLE_VALUE_H

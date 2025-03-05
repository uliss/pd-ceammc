#ifndef HW_RPI_ROTENC_H
#define HW_RPI_ROTENC_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiRotaryEncoder : public DispatchedObject<BaseObject> {
    ceammc_hw_gpio_rotenc* enc_ { nullptr };
    IntProperty* dt_pin_ { nullptr };
    IntProperty* clk_pin_ { nullptr };
    IntProperty* btn_pin_ { nullptr };
    FloatProperty* init_ { nullptr };
    FloatProperty* step_ { nullptr };
    FloatProperty* min_ { nullptr };
    FloatProperty* max_ { nullptr };

public:
    explicit HwRpiRotaryEncoder(const PdArgs& args);
    ~HwRpiRotaryEncoder();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_get(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
};

void setup_hw_gpio_rotenc();

#endif // HW_RPI_ROTENC_H

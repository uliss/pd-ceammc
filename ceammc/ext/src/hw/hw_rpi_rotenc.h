#ifndef HW_RPI_ROTENC_H
#define HW_RPI_ROTENC_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiRotaryEncoder : public RustDispatchedObject<BaseObject> {
    ceammc_hw_gpio_rotenc* enc_ { nullptr };
    GpioPinProperty* dt_pin_ { nullptr };
    GpioPinProperty* clk_pin_ { nullptr };
    GpioPinProperty* btn_pin_ { nullptr };
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
    void onFloat(t_float f) final;
    void onInlet(size_t idx, const AtomListView& lv) final;

    void m_get(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_rotenc();

#endif // HW_RPI_ROTENC_H

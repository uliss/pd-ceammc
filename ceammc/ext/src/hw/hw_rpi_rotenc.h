#ifndef HW_RPI_ROTENC_H
#define HW_RPI_ROTENC_H

#ifdef WITH_RPI

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiRotaryEncoder : public HwRpiDevice<ceammc_hw_gpio_rotenc> {
    GpioPinProperty* dt_pin_ { nullptr };
    GpioPinProperty* clk_pin_ { nullptr };
    GpioPinProperty* btn_pin_ { nullptr };
    FloatProperty* init_ { nullptr };
    FloatProperty* step_ { nullptr };
    FloatProperty* min_ { nullptr };
    FloatProperty* max_ { nullptr };

public:
    explicit HwRpiRotaryEncoder(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onInlet(size_t idx, const AtomListView& lv) final;

    void m_get(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_rotenc();

#else

#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(HwRpiRotaryEncoder, 1, 1, "compiled without RPi support");
OBJECT_STUB_SETUP(HwRpiRotaryEncoder, hw_rpi_rotenc, "hw.rpi.rotenc");

#endif

#endif // HW_RPI_ROTENC_H

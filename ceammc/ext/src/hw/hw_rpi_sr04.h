#ifndef HW_RPI_SR04_H
#define HW_RPI_SR04_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;


class HwRpiSr04 : public DispatchedObject<BaseObject> {
    ceammc_hw_gpio_sr04* sr04_ { nullptr };
    IntProperty* trigger_pin_ { nullptr };
    IntProperty* echo_pin_ { nullptr };
    IntProperty* poll_interval_ { nullptr };

public:
    explicit HwRpiSr04(const PdArgs& args);
    ~HwRpiSr04();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_sr04();

#endif // HW_RPI_SR04_H

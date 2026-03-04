#ifndef HW_RPI_SENSOR_SR04_H
#define HW_RPI_SENSOR_SR04_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiSensorSr04 : public HwRpiDevice<ceammc_hw_gpio_sr04> {
    IntProperty* trigger_pin_ { nullptr };
    IntProperty* echo_pin_ { nullptr };
    IntProperty* poll_interval_ { nullptr };

public:
    explicit HwRpiSensorSr04(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_sensor_sr04();

#endif // HW_RPI_SENSOR_SR04_H

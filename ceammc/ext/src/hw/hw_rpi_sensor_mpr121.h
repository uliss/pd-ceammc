#ifndef HW_RPI_SENSOR_MPR121_H
#define HW_RPI_SENSOR_MPR121_H

#ifdef WITH_RPI

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiSensorMpr121 : public HwRpiDevice<ceammc_hw_sensor_mpr121> {
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };
    GpioPinProperty* irq_pin_ { nullptr };
    ListProperty* debounce_ { nullptr };
    ListProperty* threshold_ { nullptr };

public:
    explicit HwRpiSensorMpr121(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;

    void m_baseline(t_symbol* s, const AtomListView& lv);
    void m_filtered(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;

private:
    std::pair<std::uint8_t, std::uint8_t> debounce() const;
    std::pair<std::uint8_t, std::uint8_t> threshold() const;
};

void setup_hw_rpi_sensor_mpr121();

#else

#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(HwRpiSensorMpr121, 1, 1, "compiled without RPi support");
OBJECT_STUB_SETUP(HwRpiSensorMpr121, hw_rpi_sensor_mpr121, "hw.rpi.sensor.mpr121");

#endif

#endif // HW_RPI_SENSOR_MPR121_H

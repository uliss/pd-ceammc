#ifndef HW_RPI_SENSOR_DHT11_H
#define HW_RPI_SENSOR_DHT11_H

#ifdef WITH_RPI

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiSensorDht11 : public HwRpiDevice<ceammc_hw_gpio_dht11> {
    GpioPinProperty* pin_ { nullptr };

public:
    explicit HwRpiSensorDht11(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_sensor_dht11();

#else

#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(HwRpiSensorDht11, 1, 1, "compiled without RPi support");
OBJECT_STUB_SETUP(HwRpiSensorDht11, hw_rpi_sensor_dht11, "hw.rpi.sensor.dht11");

#endif

#endif // HW_RPI_SENSOR_DHT11_H

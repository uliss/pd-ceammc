#ifndef HW_RPI_SENSOR_DHT11_H
#define HW_RPI_SENSOR_DHT11_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorDht11 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_gpio_dht11* dht_ { nullptr };
    GpioPinProperty* pin_ { nullptr };

public:
    explicit HwRpiSensorDht11(const PdArgs& args);
    ~HwRpiSensorDht11();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_sensor_dht11();

#endif // HW_RPI_SENSOR_DHT11_H

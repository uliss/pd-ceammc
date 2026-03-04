#ifndef HW_RPI_SENSOR_DHT11_H
#define HW_RPI_SENSOR_DHT11_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorDht11 : public RustDispatchedObject<BaseObject> {
    using Sensor = std::unique_ptr<ceammc_hw_gpio_dht11, void (*)(ceammc_hw_gpio_dht11*)>;

    Sensor dht_;
    GpioPinProperty* pin_ { nullptr };
    BoolProperty* connect_ { nullptr };

public:
    explicit HwRpiSensorDht11(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);

private:
    bool check_connected(bool print_err);
    void gpio_connect(bool state);
};

void setup_hw_rpi_sensor_dht11();

#endif // HW_RPI_SENSOR_DHT11_H

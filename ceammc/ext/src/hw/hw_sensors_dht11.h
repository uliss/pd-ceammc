#ifndef HW_SENSORS_DHT11_H
#define HW_SENSORS_DHT11_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

using HwDht11Base = DispatchedObject<BaseObject>;

class HwSensorDht11 : public HwDht11Base {
    ceammc_hw_gpio_dht11* dht_ { nullptr };
    IntProperty* pin_ { nullptr };

public:
    HwSensorDht11(const PdArgs& args);
    ~HwSensorDht11();

    void initDone() final;

    void onBang() final;
    bool notify(int code) final;
};

void setup_hw_sensor_dht11();

#endif // HW_SENSORS_DHT11_H

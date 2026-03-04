#ifndef HW_RPI_SENSOR_SR04_H
#define HW_RPI_SENSOR_SR04_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorSr04 : public RustDispatchedObject<BaseObject> {
    using Sensor = std::unique_ptr<ceammc_hw_gpio_sr04, void (*)(ceammc_hw_gpio_sr04*)>;

    Sensor sr04_;
    IntProperty* trigger_pin_ { nullptr };
    IntProperty* echo_pin_ { nullptr };
    IntProperty* poll_interval_ { nullptr };
    BoolProperty* connect_ { nullptr };

public:
    explicit HwRpiSensorSr04(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);

private:
    bool check_connected(bool print_err);
    void gpio_connect(bool state);
};

void setup_hw_rpi_sensor_sr04();

#endif // HW_RPI_SENSOR_SR04_H

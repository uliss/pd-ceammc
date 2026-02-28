#ifndef HW_RPI_SENSOR_SR04_H
#define HW_RPI_SENSOR_SR04_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorSr04 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_gpio_sr04* sr04_ { nullptr };
    IntProperty* trigger_pin_ { nullptr };
    IntProperty* echo_pin_ { nullptr };
    IntProperty* poll_interval_ { nullptr };

public:
    explicit HwRpiSensorSr04(const PdArgs& args);
    ~HwRpiSensorSr04();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);

private:
    bool check_no_pins(bool output_warning = true) const;
};

void setup_hw_rpi_sensor_sr04();

#endif // HW_RPI_SENSOR_SR04_H

#ifndef HW_RPI_SENSOR_IR_H
#define HW_RPI_SENSOR_IR_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorIR : public RustDispatchedObject<BaseObject> {
    using Sensor = std::unique_ptr<ceammc_hw_infrared, void (*)(ceammc_hw_infrared*)>;
    Sensor ir_;
    GpioPinProperty* pin_ { nullptr };
    SymbolProperty* proto_ { nullptr };

public:
    explicit HwRpiSensorIR(const PdArgs& args);

    bool notify(int code) final;
    void m_poll(t_symbol* s, const AtomListView& lv);

private:
    void startSensor();
};

void setup_hw_rpi_sensor_ir();

#endif // HW_RPI_SENSOR_IR_H

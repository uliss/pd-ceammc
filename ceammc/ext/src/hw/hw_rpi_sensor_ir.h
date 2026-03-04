#ifndef HW_RPI_SENSOR_IR_H
#define HW_RPI_SENSOR_IR_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiSensorIR : public HwRpiDevice<ceammc_hw_infrared> {
    GpioPinProperty* pin_ { nullptr };
    SymbolProperty* proto_ { nullptr };

public:
    explicit HwRpiSensorIR(const PdArgs& args);

    bool notify(int code) final;
    void m_poll(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_sensor_ir();

#endif // HW_RPI_SENSOR_IR_H

#ifndef HW_RPI_SENSOR_MPR121_H
#define HW_RPI_SENSOR_MPR121_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiSensorMpr121 : public HwRpiDevice<ceammc_hw_sensor_mpr121> {
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };

public:
    explicit HwRpiSensorMpr121(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;

//    void m_poll(t_symbol* s, const AtomListView& lv);
//    void m_address(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_sensor_mpr121();

#endif // HW_RPI_SENSOR_MPR121_H

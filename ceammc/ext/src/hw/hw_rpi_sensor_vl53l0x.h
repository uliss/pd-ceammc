#ifndef HW_RPI_SENSOR_VL53L0X_H
#define HW_RPI_SENSOR_VL53L0X_H

#include "ceammc_object.h"
#include "rust_dispatched_object.h"
using namespace ceammc;

using VL53L0XPtr = std::unique_ptr<ceammc_hw_sensor_vl53l0x, void (*)(ceammc_hw_sensor_vl53l0x*)>;

class HwRpiSensorVl53l0x : public RustDispatchedObject<BaseObject> {
    VL53L0XPtr vc_;
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };
    BoolProperty* connect_ { nullptr };

public:
    explicit HwRpiSensorVl53l0x(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;

    void m_poll(t_symbol* s, const AtomListView& lv);
    void m_address(t_symbol* s, const AtomListView& lv);

private:
    bool check_connected(bool print_err);
    void i2c_connect(bool state);
};

void setup_hw_rpi_sensor_vl53l0x();

#endif // HW_RPI_SENSOR_VL53L0X_H

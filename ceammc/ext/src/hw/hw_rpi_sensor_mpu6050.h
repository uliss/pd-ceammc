#ifndef HW_RPI_SENSOR_MPU6050_H
#define HW_RPI_SENSOR_MPU6050_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorMpu6050 : public RustDispatchedObject<BaseObject> {
    using Sensor = std::unique_ptr<ceammc_hw_mpu6050, void (*)(ceammc_hw_mpu6050*)>;
    Sensor mpu_;
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };
    BoolProperty* connect_ { nullptr };

public:
    explicit HwRpiSensorMpu6050(const PdArgs& args);

    bool notify(int code) final;

    void m_calibrate(t_symbol* s, const AtomListView& lv);
    void m_poll(t_symbol* s, const AtomListView& lv);

private:
    void output_data(float yaw, float pitch, float roll);
    bool check_connected(bool print_err);
    void i2c_connect(bool state);
};

void setup_hw_rpi_sensor_mpu6050();

#endif // HW_RPI_SENSOR_MPU6050_H

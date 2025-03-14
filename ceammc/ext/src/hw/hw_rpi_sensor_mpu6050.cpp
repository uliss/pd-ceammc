#include "hw_rpi_sensor_mpu6050.h"
#include "ceammc_factory.h"

HwRpiSensorMpu6050::HwRpiSensorMpu6050(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    i2c_bus_ = new IntProperty("@bus", ceammc_HW_I2C_DEFAULT_BUS);
    i2c_bus_->setInitOnly();
    i2c_bus_->checkClosedRange(-1, 16);
    addProperty(i2c_bus_);
}

HwRpiSensorMpu6050::~HwRpiSensorMpu6050()
{
    ceammc_hw_mpu6050_free(mpu_);
}

void HwRpiSensorMpu6050::initDone()
{
    mpu_ = ceammc_hw_mpu6050_new(i2c_bus_->value(), ceammc_HW_I2C_DEFAULT_ADDR, on_notify(), on_err());
}

bool HwRpiSensorMpu6050::notify(int code)
{
    return true;
}

void setup_hw_rpi_sensor_mpu6050()
{
    ObjectFactory<HwRpiSensorMpu6050> obj("hw.rpi.sensor.mpu6050");
}

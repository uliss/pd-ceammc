#include "hw_rpi_sensor_mpu6050.h"
#include "ceammc_containers.h"
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
    mpu_ = ceammc_hw_mpu6050_new(i2c_bus_->value(),
        ceammc_HW_I2C_DEFAULT_ADDR,
        on_notify(),
        on_err(),
        { this, [](void* user, float yaw, float pitch, float roll, float temp) {
             auto obj = static_cast<HwRpiSensorMpu6050*>(user);
             if (!obj)
                 return;

             obj->outputData(yaw, pitch, roll, temp);
         } });
}

bool HwRpiSensorMpu6050::notify(int code)
{
    return ceammc_hw_mpu6050_process_reply(mpu_);
}

void HwRpiSensorMpu6050::outputData(float yaw, float pitch, float roll, float temp)
{
    AtomArray<3> ypr { yaw, pitch, roll };
    anyTo(0, gensym("ypr"), ypr.view());

    anyTo(0, gensym("temp"), temp);
}

void setup_hw_rpi_sensor_mpu6050()
{
    ObjectFactory<HwRpiSensorMpu6050> obj("hw.rpi.sensor.mpu6050");
}

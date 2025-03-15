#include "hw_rpi_sensor_mpu6050.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwRpiSensorMpu6050::HwRpiSensorMpu6050(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    i2c_bus_ = addI2cBusProperty();
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
        on_message(),
        { this,
            [](void* user, float yaw, float pitch, float roll) {
                auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                if (!obj)
                    return;

                obj->outputData(yaw, pitch, roll);
            },
            [](void* user, float temp) {
                auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                if (!obj)
                    return;

                obj->anyTo(0, gensym("temp"), temp);
            } });
}

bool HwRpiSensorMpu6050::notify(int code)
{
    return ceammc_hw_mpu6050_process_reply(mpu_);
}

void HwRpiSensorMpu6050::m_calibrate(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_mpu6050_calibrate(mpu_);
}

void HwRpiSensorMpu6050::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_mpu6050_poll(mpu_, lv.boolAt(0, false));
}

void HwRpiSensorMpu6050::outputData(float yaw, float pitch, float roll)
{
    AtomArray<3> ypr { yaw, pitch, roll };
    anyTo(0, gensym("ypr"), ypr.view());
}

void setup_hw_rpi_sensor_mpu6050()
{
    ObjectFactory<HwRpiSensorMpu6050> obj("hw.rpi.sensor.mpu6050");

    CEAMMC_OBJECT_ADD_METHOD(obj, HwRpiSensorMpu6050, poll)
    CEAMMC_OBJECT_ADD_METHOD(obj, HwRpiSensorMpu6050, calibrate)
}

#include "hw_rpi_sensor_mpu6050.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwRpiSensorMpu6050::HwRpiSensorMpu6050(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_mpu6050>(&ceammc_hw_mpu6050_free, args)
{
    createOutlet();

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();
}

bool HwRpiSensorMpu6050::notify(int code)
{
    return ceammc_hw_mpu6050_process_reply(device());
}

void HwRpiSensorMpu6050::m_calibrate(t_symbol* s, const AtomListView& lv)
{
    if (!check_connected(true, s))
        return;

    ceammc_hw_mpu6050_calibrate(device());
}

void HwRpiSensorMpu6050::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    if (!check_connected(true, s))
        return;

    ceammc_hw_mpu6050_poll(device(), lv.boolAt(0, false));
}

HwRpiSensorMpu6050::HwRpiDevice::Device HwRpiSensorMpu6050::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is no set";
        return nullDevice();
    }

    return Device(ceammc_hw_mpu6050_new(bus,
                      i2c_addr_->value(),
                      on_notify(),
                      on_message(),
                      { this,
                          [](void* user, float yaw, float pitch, float roll) {
                              auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                              if (!obj)
                                  return;

                              obj->output_data(yaw, pitch, roll);
                          },
                          [](void* user, float temp) {
                              auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                              if (!obj)
                                  return;

                              obj->anyTo(0, gensym("temp"), temp);
                          } }),
        freeDeviceFn());
}

void HwRpiSensorMpu6050::output_data(float yaw, float pitch, float roll)
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

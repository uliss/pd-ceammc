#include "hw_rpi_sensor_mpr121.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwRpiSensorMpr121::HwRpiSensorMpr121(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_sensor_mpr121>(&ceammc_hw_sensor_mpr121_free, args)
{
    createOutlet();
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();
}

bool HwRpiSensorMpr121::notify(int code)
{
    return ceammc_hw_sensor_mpr121_proc_reply(device());
}

void HwRpiSensorMpr121::onBang()
{
    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_sensor_mpr121_readall(device());
}

HwRpiSensorMpr121::HwRpiDevice::Device HwRpiSensorMpr121::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is not specified";
        return nullDevice();
    }

    return Device(ceammc_hw_sensor_mpr121_new(bus,
                      i2c_addr_->value(),
                      on_notify(),
                      on_message(),
                      ceammc_hw_mpr121_key_cb {
                          this,
                          [](void* user, std::uint16_t state) {
                              auto obj = static_cast<HwRpiSensorMpr121*>(user);
                              if (obj) {
                                  SmallAtomListN<12> data;
                                  for (int i = 0; i < 12; i++) {
                                      if ((1 << i) & state) {
                                          obj->anyTo(0, gensym("touch"), Atom(i));
                                          data.push_back(Atom(i));
                                      }
                                  }

                                  if (!data.empty())
                                      obj->listTo(0, data.view());
                                  else
                                      obj->bangTo(1);
                              }
                          },
                      }),
        freeDeviceFn());
}

void setup_hw_rpi_sensor_mpr121()
{
    ObjectFactory<HwRpiSensorMpr121> obj("hw.rpi.sensor.mpr121");

    //    obj.addMethod("poll", &HwRpiSensorMpr121::m_poll);
    //    obj.addMethod("address", &HwRpiSensorMpr121::m_address);
}

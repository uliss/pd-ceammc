#include "hw_rpi_sensor_mpr121.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "hw_rpi_sensor_mpr121_args.hpp"

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

/// @function "set the touch and release threshold for all channels" {
///  #touch   int "touch threshold"     { check: [0..30] }
///  #release int "release threshold"   { check: [0..30] }
/// }
void HwRpiSensorMpr121::m_threshold(t_symbol* s, const AtomListView& lv)
{
    m_threshold_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_sensor_mpr121_set_thresholds(device(), args.touch, args.release);
}

/// @function "the number of consecutive samples needed to confirm a touch/release, helping to filter out noise" {
///  #touch   int "touch threshold"     { check: [0..7] }
///  #release int "release threshold"   { check: [0..7] }
/// }
void HwRpiSensorMpr121::m_debounce(t_symbol* s, const AtomListView& lv)
{
    m_debounce_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, nullptr))
        return;

    auto on = lv.intAt(0, 0);
    auto off = lv.intAt(1, 0);
    ceammc_hw_sensor_mpr121_set_debounce(device(), args.touch, args.release);
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
                      {
                          this,
                          [](void* user, std::uint16_t touched, std::uint16_t previous) {
                              auto obj = static_cast<HwRpiSensorMpr121*>(user);
                              if (obj && (touched != previous)) {
                                  obj->anyTo(1, gensym("touches"), Atom(touched));

                                  AtomArray<2> data;
                                  for (int i = 0; i < 12; i++) {
                                      auto old_bit = (1 << i) & previous;
                                      auto new_bit = (1 << i) & touched;
                                      if (old_bit != new_bit) {
                                          data[0] = i;
                                          data[1] = new_bit > 0;
                                          obj->listTo(0, data.view());
                                      }
                                  }
                              }
                          },
                          [](void* user, std::uint8_t channel, std::uint8_t data) {
                              auto obj = static_cast<HwRpiSensorMpr121*>(user);
                              AtomArray<2> pair;
                              pair[0] = channel;
                              pair[1] = data;
                              obj->anyTo(1, gensym("baseline"), pair.view());
                          },
                          [](void* user, std::uint8_t channel, std::uint16_t data) {
                              auto obj = static_cast<HwRpiSensorMpr121*>(user);
                              AtomArray<2> pair;
                              pair[0] = channel;
                              pair[1] = data;
                              obj->anyTo(1, gensym("filtered"), pair.view());
                          },
                      }),
        freeDeviceFn());
}

void setup_hw_rpi_sensor_mpr121()
{
    ObjectFactory<HwRpiSensorMpr121> obj("hw.rpi.sensor.mpr121");
    obj.addMethod("debounce", &HwRpiSensorMpr121::m_debounce);
    obj.addMethod("threshold", &HwRpiSensorMpr121::m_threshold);
}

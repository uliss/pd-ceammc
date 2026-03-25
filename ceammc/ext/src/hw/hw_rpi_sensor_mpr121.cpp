#include "hw_rpi_sensor_mpr121.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "hw_rpi_sensor_mpr121_args.hpp"

constexpr t_float DEFAULT_DEBOUNCE_ON = 0;
constexpr t_float DEFAULT_DEBOUNCE_OFF = 0;
constexpr t_float DEFAULT_THRESHOLD_ON = ceammc_HW_RPI_MPR121_DEFAULT_TOUCH_THRESHOLD;
constexpr t_float DEFAULT_THRESHOLD_OFF = ceammc_HW_RPI_MPR121_DEFAULT_RELEASE_THRESHOLD;

HwRpiSensorMpr121::HwRpiSensorMpr121(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_sensor_mpr121>(&ceammc_hw_sensor_mpr121_free, args)
{
    createOutlet();
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();

    irq_pin_ = addGpioPinProperty("@irq");

    debounce_ = new ListProperty("@debounce", AtomList({
                                                  DEFAULT_DEBOUNCE_ON,
                                                  DEFAULT_DEBOUNCE_OFF,
                                              }));
    debounce_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.size() == 2
            && lv[0].isIntInClosedInterval(0, ceammc_HW_RPI_MPR121_DEBOUNCE_MAX)
            && lv[1].isIntInClosedInterval(0, ceammc_HW_RPI_MPR121_DEBOUNCE_MAX);
    });
    debounce_->setSuccessFn([this](Property*) {
        if (device()) {
            auto v = debounce();
            ceammc_hw_sensor_mpr121_set_debounce(device(), v.first, v.second);
        }
    });
    addProperty(debounce_);

    threshold_ = new ListProperty("@threshold", AtomList({
                                                    DEFAULT_THRESHOLD_ON,
                                                    DEFAULT_THRESHOLD_OFF,
                                                }));
    threshold_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.size() == 2
            && lv[0].isIntInClosedInterval(0, 255)
            && lv[1].isIntInClosedInterval(0, 255);
    });
    threshold_->setSuccessFn([this](Property*) {
        if (device()) {
            auto v = debounce();
            ceammc_hw_sensor_mpr121_set_thresholds(device(), v.first, v.second);
        }
    });
    addProperty(threshold_);
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

/// @function "performs a software reset on the device, resetting the MPR121 Touch sensor back to default configuration" {
/// }
void HwRpiSensorMpr121::m_reset(t_symbol* s, const AtomListView& lv)
{
    m_reset_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_sensor_mpr121_reset(device());
}

/// @function "get filtered signal from the channel" {
///  #channel   int "sensor channel"    { check: [0..11] }
/// }
void HwRpiSensorMpr121::m_filtered(t_symbol* s, const AtomListView& lv)
{
    m_filtered_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_sensor_mpr121_get_filtered(device(), args.channel);
}

/// @function "reads the baseline data for the channel. Note that this has only a resolution of 8bit." {
///  #channel   int "sensor channel"    { check: [0..11] }
/// }
void HwRpiSensorMpr121::m_baseline(t_symbol* s, const AtomListView& lv)
{
    m_baseline_args args;
    if (!args.parse_args(lv, this))
        return;

    if (!check_connected(true, nullptr))
        return;

    ceammc_hw_sensor_mpr121_get_baseline(device(), args.channel);
}

HwRpiSensorMpr121::HwRpiDevice::Device HwRpiSensorMpr121::createDevice()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus)) {
        OBJ_ERR << "i2c bus is not specified";
        return nullDevice();
    }

    std::uint8_t pin = irq_pin_->value();
    const auto* pin_ptr = irq_pin_->value() < 0 ? nullptr : &pin;

    Device dev(ceammc_hw_sensor_mpr121_new(bus,
                   i2c_addr_->value(),
                   pin_ptr,
                   on_notify(),
                   on_message(),
                   { this,
                       [](void* user, std::uint16_t touched, std::uint16_t previous, bool over_current) {
                           auto obj = static_cast<HwRpiSensorMpr121*>(user);
                           if (obj && over_current)
                               obj->anyTo(1, gensym("over_current"), Atom(1));

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
                       [](void* user) {
                           auto obj = static_cast<HwRpiSensorMpr121*>(user);
                           obj->freeDevice();
                           Error(obj) << "disconnected";
                       } }),
        freeDeviceFn());

    if (dev) {
        auto deb = debounce();
        ceammc_hw_sensor_mpr121_set_debounce(dev.get(), deb.first, deb.second);
        auto thr = threshold();
        ceammc_hw_sensor_mpr121_set_thresholds(dev.get(), thr.first, thr.second);
    }

    return dev;
}

std::pair<uint8_t, uint8_t> HwRpiSensorMpr121::debounce() const
{
    auto v = debounce_->value();
    return std::make_pair(v.intAt(0, 0), v.intAt(1, 0));
}

std::pair<uint8_t, uint8_t> HwRpiSensorMpr121::threshold() const
{
    auto v = threshold_->value();
    return std::make_pair(v.intAt(0, 0), v.intAt(1, 0));
}

void setup_hw_rpi_sensor_mpr121()
{
    ObjectFactory<HwRpiSensorMpr121> obj("hw.rpi.sensor.mpr121");

    obj.addMethod("baseline", &HwRpiSensorMpr121::m_baseline);
    obj.addMethod("filtered", &HwRpiSensorMpr121::m_filtered);
    obj.addMethod("reset", &HwRpiSensorMpr121::m_reset);
}

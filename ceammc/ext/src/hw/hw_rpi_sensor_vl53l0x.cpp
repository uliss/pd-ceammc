#include "hw_rpi_sensor_vl53l0x.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiSensorVl53l0x::HwRpiSensorVl53l0x(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
    , vc_(nullptr, &ceammc_hw_sensor_vl53l0x_free)
{
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();

    connect_ = new BoolProperty("@connect", false);
    connect_->setSuccessFn([this](Property*) {
        i2c_connect(connect_->value());
    });
    addProperty(connect_);
}

bool HwRpiSensorVl53l0x::notify(int code)
{
    return ceammc_hw_sensor_vl53l0x_proc_reply(vc_.get());
}

void HwRpiSensorVl53l0x::onBang()
{
    if (!check_connected(true))
        return;

    ceammc_hw_sensor_vl53l0x_read_mm(vc_.get());
}

void HwRpiSensorVl53l0x::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (!check_connected(true))
        return;

    ceammc_hw_sensor_vl53l0x_poll(vc_.get(), lv.boolAt(0, true));
}

void HwRpiSensorVl53l0x::m_address(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (!check_connected(true))
        return;

    ceammc_hw_sensor_vl53l0x_set_address(vc_.get(), lv.intAt(0, true));
}

void HwRpiSensorVl53l0x::i2c_connect(bool state)
{
    if (state) {
        std::int8_t bus = 0;
        if (!i2c_bus_->getBus(bus)) {
            OBJ_ERR << "i2c bus is not specified";
            return;
        }

        vc_.reset(ceammc_hw_sensor_vl53l0x_new(bus,
            i2c_addr_->value(),
            on_notify(), //
            { this, [](void* user, std::uint16_t mm) {
                 auto obj = static_cast<HwRpiSensorVl53l0x*>(user);
                 if (obj)
                     obj->floatTo(0, mm);
             } },
            on_message()));

        if (!vc_)
            OBJ_ERR << "can't connect to i2c bus";
    } else {
        vc_.reset();
    }
}

bool HwRpiSensorVl53l0x::check_connected(bool print_err)
{
    if (!vc_ && print_err) {
        OBJ_ERR << "device is not connected to the I2C bus";
    }

    return vc_.get();
}

void setup_hw_rpi_sensor_vl53l0x()
{
    ObjectFactory<HwRpiSensorVl53l0x> obj("hw.rpi.sensor.vl53l0x");

    obj.addMethod("poll", &HwRpiSensorVl53l0x::m_poll);
    obj.addMethod("address", &HwRpiSensorVl53l0x::m_address);
}

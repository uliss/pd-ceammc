#include "hw_rpi_sensor_vl53l0x.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiSensorVl53l0x::HwRpiSensorVl53l0x(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    i2c_addr_ = addI2cAddrProperty();
    i2c_bus_ = addI2cBusProperty();
}

HwRpiSensorVl53l0x::~HwRpiSensorVl53l0x()
{
    ceammc_hw_sensor_vl53l0x_free(vc_);
}

void HwRpiSensorVl53l0x::initDone()
{
    vc_ = ceammc_hw_sensor_vl53l0x_new(i2c_bus_->value(), //
        i2c_addr_->value(),
        on_notify(), //
        { this, [](void* user, std::uint16_t mm) {
             auto obj = static_cast<HwRpiSensorVl53l0x*>(user);
             if (obj)
                 obj->floatTo(0, mm);
         } },
        on_message());
}

bool HwRpiSensorVl53l0x::notify(int code)
{
    return ceammc_hw_sensor_vl53l0x_proc_reply(vc_);
}

void HwRpiSensorVl53l0x::onBang()
{
    ceammc_hw_sensor_vl53l0x_read_mm(vc_);
}

void HwRpiSensorVl53l0x::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_sensor_vl53l0x_poll(vc_, lv.boolAt(0, true));
}

void HwRpiSensorVl53l0x::m_address(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:b");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_sensor_vl53l0x_set_address(vc_, lv.intAt(0, true));
}

void setup_hw_rpi_sensor_vl53l0x()
{
    ObjectFactory<HwRpiSensorVl53l0x> obj("hw.rpi.sensor.vl53l0x");

    obj.addMethod("poll", &HwRpiSensorVl53l0x::m_poll);
    obj.addMethod("address", &HwRpiSensorVl53l0x::m_address);
}

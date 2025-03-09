#include "hw_rpi_sensor_vc53l0x.h"
#include "ceammc_factory.h"

HwRpiSensorVc53l0x::HwRpiSensorVc53l0x(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();
}

HwRpiSensorVc53l0x::~HwRpiSensorVc53l0x()
{
    ceammc_hw_sensor_vl53l0x_free(vc_);
}

void HwRpiSensorVc53l0x::initDone()
{
    vc_ = ceammc_hw_sensor_vl53l0x_new(0, 0, on_notify(), //
        { this, [](void* user, std::uint16_t mm) {
             auto obj = static_cast<HwRpiSensorVc53l0x*>(user);
             if (obj)
                 obj->floatTo(0, mm);
         } },
        on_err());
}

bool HwRpiSensorVc53l0x::notify(int code)
{
    return ceammc_hw_sensor_vl53l0x_proc_reply(vc_);
}

void HwRpiSensorVc53l0x::onBang()
{
    ceammc_hw_sensor_vl53l0x_read_mm(vc_);
}

void HwRpiSensorVc53l0x::m_poll(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_sensor_vl53l0x_poll(vc_, lv.boolAt(0, true));
}

void setup_hw_rpi_sensor_vc53l0x()
{
    ObjectFactory<HwRpiSensorVc53l0x> obj("hw.rpi.sensor.vl53l0x");
}

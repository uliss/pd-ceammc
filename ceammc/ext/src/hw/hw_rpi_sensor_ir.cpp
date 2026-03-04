#include "hw_rpi_sensor_ir.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiSensorIR::HwRpiSensorIR(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_infrared>(&ceammc_hw_infrared_free, args)
{
    createOutlet();
    pin_ = addGpioPinProperty("@pin");
    pin_->setArgIndex(0);

    proto_ = new SymbolProperty("@proto", gensym("NEC"));
    addProperty(proto_);
}

bool HwRpiSensorIR::notify(int code)
{
    return ceammc_hw_infrared_free_process_reply(device());
}

void HwRpiSensorIR::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("POLL:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    connect(lv.boolAt(0, false));
}

HwRpiSensorIR::HwRpiDevice::Device HwRpiSensorIR::createDevice()
{
    if (!pin_->checkPin(this)) {
        OBJ_ERR << "GPIO pin is not set";
        return nullDevice();
    }

    Device dev(ceammc_hw_infrared_new(pin_->value(),
                   on_notify(),
                   on_message(), //
                   { this, [](void* user, const char* key, std::int64_t value) {
                        auto obj = static_cast<HwRpiSensorIR*>(user);
                        if (!obj)
                            return;

                        obj->anyTo(0, gensym(key), Atom(value));
                    } }),
        freeDeviceFn());

    if (dev)
        ceammc_hw_infrared_set_protocol(dev.get(), proto_->value()->s_name);

    return dev;
}

void setup_hw_rpi_sensor_ir()
{
    ObjectFactory<HwRpiSensorIR> obj("hw.rpi.sensor.ir");
    obj.addMethod("poll", &HwRpiSensorIR::m_poll);
}

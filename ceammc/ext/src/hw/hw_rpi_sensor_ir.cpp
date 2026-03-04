#include "hw_rpi_sensor_ir.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiSensorIR::HwRpiSensorIR(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
    , ir_(nullptr, &ceammc_hw_infrared_free)
{
    createOutlet();
    pin_ = addGpioPinProperty("@pin");
    pin_->setArgIndex(0);

    proto_ = new SymbolProperty("@proto", gensym("NEC"));
    addProperty(proto_);
}

bool HwRpiSensorIR::notify(int code)
{
    return ceammc_hw_infrared_free_process_reply(ir_.get());
}

void HwRpiSensorIR::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("POLL:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto poll = lv.boolAt(0, false);
    if (poll && !ir_) {
        startSensor();
    } else if (!poll && ir_) {
        ir_.reset();
    }
}

void HwRpiSensorIR::startSensor()
{
    if (!pin_->checkPin(this))
        return;

    ir_.reset(ceammc_hw_infrared_new(pin_->value(),
        on_notify(),
        on_message(), //
        { this, [](void* user, const char* key, std::int64_t value) {
             auto obj = static_cast<HwRpiSensorIR*>(user);
             if (!obj)
                 return;

             obj->anyTo(0, gensym(key), Atom(value));
         } }) //
    );

    if (!ir_) {
        OBJ_ERR << "can't connect to device";
        return;
    }

    ceammc_hw_infrared_set_protocol(ir_.get(), proto_->value()->s_name);
}

void setup_hw_rpi_sensor_ir()
{
    ObjectFactory<HwRpiSensorIR> obj("hw.rpi.sensor.ir");

    obj.addMethod("poll", &HwRpiSensorIR::m_poll);
}

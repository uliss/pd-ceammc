#include "hw_rpi_sensor_ir.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiSensorIR::HwRpiSensorIR(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();
    pin_ = new IntProperty("@pin", 0);
    pin_->setInitOnly();
    pin_->setArgIndex(0);
    addProperty(pin_);
}

HwRpiSensorIR::~HwRpiSensorIR()
{
    stopSensor();
}

bool HwRpiSensorIR::notify(int code)
{
    return ceammc_hw_infrared_free_process_reply(ir_);
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
        stopSensor();
    }
}

void HwRpiSensorIR::m_tolerance(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:i>=0");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_infrared_set_tolerance(ir_, lv.intAt(0, 0));
}

void HwRpiSensorIR::startSensor()
{
    if (ir_) {
        ceammc_hw_infrared_free(ir_);
        ir_ = nullptr;
    }

    ir_ = ceammc_hw_infrared_new(pin_->value(),
        on_notify(),
        on_err(), //
        { this, [](void* user, const char* key, std::int64_t value) {
             auto obj = static_cast<HwRpiSensorIR*>(user);
             if (!obj)
                 return;

             obj->anyTo(0, gensym(key), Atom(value));
         } });
}

void HwRpiSensorIR::stopSensor()
{
    if (ir_) {
        ceammc_hw_infrared_free(ir_);
        ir_ = nullptr;
    }
}

void setup_hw_rpi_sensor_ir()
{
    ObjectFactory<HwRpiSensorIR> obj("hw.rpi.sensor.ir");

    obj.addMethod("tolerance", &HwRpiSensorIR::m_tolerance);
    obj.addMethod("poll", &HwRpiSensorIR::m_poll);
}

#include "hw_rpi_sensor_ir.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_HASH(us)
CEAMMC_DEFINE_HASH(usec)
CEAMMC_DEFINE_HASH(perc)

HwRpiSensorIR::HwRpiSensorIR(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();
    pin_ = new IntProperty("@pin", 0);
    pin_->setInitOnly();
    pin_->setArgIndex(0);
    addProperty(pin_);

    proto_ = new SymbolProperty("@proto", gensym("NEC"));
    addProperty(proto_);

    tolerance_perc_ = new IntProperty("@tolerance_perc", 30);
    tolerance_perc_->checkClosedRange(0, 100);
    addProperty(tolerance_perc_);

    tolerance_usec_ = new IntProperty("@tolerance_usec", 200);
    tolerance_usec_->checkClosedRange(0, 1000);
    addProperty(tolerance_usec_);
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

    ceammc_hw_infrared_set_protocol(ir_, proto_->value()->s_name);
    ceammc_hw_infrared_set_tolerance_usec(ir_, tolerance_usec_->value());
    ceammc_hw_infrared_set_tolerance_perc(ir_, tolerance_perc_->value());
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

    obj.addMethod("poll", &HwRpiSensorIR::m_poll);
}

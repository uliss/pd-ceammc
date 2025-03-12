#include "hw_rpi_sensor_ir.h"
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
    ceammc_hw_infrared_free(ir_);
}

void HwRpiSensorIR::initDone()
{
    ir_ = ceammc_hw_infrared_new(pin_->value(), on_notify(), on_err());
}

bool HwRpiSensorIR::notify(int code)
{
    ceammc_hw_infrared_free_process_reply(ir_);
    return true;
}

void setup_hw_rpi_sensor_ir()
{
    ObjectFactory<HwRpiSensorIR> obj("hw.rpi.sensor.ir");
}

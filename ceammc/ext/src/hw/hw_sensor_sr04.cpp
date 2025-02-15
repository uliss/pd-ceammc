#include "hw_sensor_sr04.h"
#include "ceammc_factory.h"

HwSensorSR04::HwSensorSR04(const PdArgs& args)
    : HwSR04Base(args)
{
    createOutlet();

    trigger_pin_ = new IntProperty("@pin", 5, PropValueAccess::INITONLY);
    trigger_pin_->checkClosedRange(0, 255);
    trigger_pin_->setArgIndex(0);
    addProperty(trigger_pin_);

    echo_pin_ = new IntProperty("@pin", 6, PropValueAccess::INITONLY);
    echo_pin_->checkClosedRange(0, 255);
    echo_pin_->setArgIndex(1);
    addProperty(echo_pin_);
}

HwSensorSR04::~HwSensorSR04()
{
    ceammc_hw_gpio_sr04_free(sr04_);
}

void HwSensorSR04::initDone()
{
    sr04_ = ceammc_hw_gpio_sr04_new(trigger_pin_->value(),
        echo_pin_->value(),
        { size_t(subscriberId()), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } }, //
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwSensorSR04*>(data);
             if (!obj)
                 return;

             Error(obj) << msg;
         } },
        { this, [](void* user, float measure) {
             auto obj = static_cast<HwSensorSR04*>(user);
             if (!obj)
                 return;

             obj->floatTo(0, measure);
         } });
}

bool HwSensorSR04::notify(int code)
{
    return ceammc_hw_gpio_sr04_process(sr04_);
}

void HwSensorSR04::onBang()
{
    ceammc_hw_gpio_sr04_measure(sr04_);
}

void HwSensorSR04::m_poll(t_symbol* s, const AtomListView& lv)
{
}

void setup_hw_sensor_sr04()
{
    ObjectFactory<HwSensorSR04> obj("hw.sensor.sr04");
    //    obj.addMethod("poll", &HwSensorSR04::m_poll);
}

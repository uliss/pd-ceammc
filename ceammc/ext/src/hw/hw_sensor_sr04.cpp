#include "hw_sensor_sr04.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwSensorSR04::HwSensorSR04(const PdArgs& args)
    : HwSR04Base(args)
{
    createOutlet();
    createOutlet();

    trigger_pin_ = new IntProperty("@trig_pin", 5, PropValueAccess::INITONLY);
    trigger_pin_->checkClosedRange(0, 255);
    trigger_pin_->setArgIndex(0);
    addProperty(trigger_pin_);

    echo_pin_ = new IntProperty("@echo_pin", 6, PropValueAccess::INITONLY);
    echo_pin_->checkClosedRange(0, 255);
    echo_pin_->setArgIndex(1);
    addProperty(echo_pin_);

    poll_interval_ = new IntProperty("@poll_interval", ceammc_HW_SR04_DEF_POLL_INTERVAL);
    poll_interval_->checkClosedRange(ceammc_HW_SR04_MIN_POLL_INTERVAL, ceammc_HW_SR04_MAX_POLL_INTERVAL);
    poll_interval_->setUnits(PropValueUnits::MSEC);
    poll_interval_->setSuccessFn([this](Property*) {
        ceammc_hw_gpio_sr04_set_poll_interval(sr04_, poll_interval_->value());
    });
    addProperty(poll_interval_);
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
        { this, [](void* user, float distance_cm, bool is_inf) {
             auto obj = static_cast<HwSensorSR04*>(user);
             if (!obj)
                 return;

             if (!is_inf)
                 obj->floatTo(0, distance_cm);
             else
                 obj->bangTo(1);
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
    static const args::ArgChecker args("STATE:b");
    if (!args.check(lv, this))
        return args.usage(this, s);

    ceammc_hw_gpio_sr04_poll(sr04_, lv.boolAt(0, false));
}

void setup_hw_sensor_sr04()
{
    ObjectFactory<HwSensorSR04> obj("hw.sensor.sr04");
    obj.addMethod("poll", &HwSensorSR04::m_poll);
}

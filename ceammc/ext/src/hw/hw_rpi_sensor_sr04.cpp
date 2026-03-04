#include "hw_rpi_sensor_sr04.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

constexpr int NULL_TRIG_GPIO_PIN = -1;
constexpr int NULL_ECHO_GPIO_PIN = -1;

HwRpiSensorSr04::HwRpiSensorSr04(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
    , sr04_(nullptr, ceammc_hw_gpio_sr04_free)
{
    createOutlet();
    createOutlet();

    trigger_pin_ = new IntProperty("@trig_pin", NULL_TRIG_GPIO_PIN, PropValueAccess::INITONLY);
    trigger_pin_->checkClosedRange(-1, 255);
    trigger_pin_->setArgIndex(0);
    addProperty(trigger_pin_);

    echo_pin_ = new IntProperty("@echo_pin", NULL_ECHO_GPIO_PIN, PropValueAccess::INITONLY);
    echo_pin_->checkClosedRange(-1, 255);
    echo_pin_->setArgIndex(1);
    addProperty(echo_pin_);

    poll_interval_ = new IntProperty("@poll_interval", ceammc_HW_SR04_DEF_POLL_INTERVAL);
    poll_interval_->checkClosedRange(ceammc_HW_SR04_MIN_POLL_INTERVAL, ceammc_HW_SR04_MAX_POLL_INTERVAL);
    poll_interval_->setUnits(PropValueUnits::MSEC);
    poll_interval_->setSuccessFn([this](Property*) {
        if (!check_connected(true))
            return;
        ceammc_hw_gpio_sr04_set_poll_interval(sr04_.get(), poll_interval_->value());
    });
    addProperty(poll_interval_);

    connect_ = new BoolProperty("@connect", false);
    connect_->setSuccessFn([this](Property*) {
        gpio_connect(connect_->value());
    });
    addProperty(connect_);
}

bool HwRpiSensorSr04::notify(int code)
{
    if (!check_connected(true))
        return false;

    return ceammc_hw_gpio_sr04_process(sr04_.get());
}

void HwRpiSensorSr04::onBang()
{
    if (!check_connected(true))
        return;

    ceammc_hw_gpio_sr04_measure(sr04_.get());
}

void HwRpiSensorSr04::m_poll(t_symbol* s, const AtomListView& lv)
{
    if (!check_connected(true))
        return;

    static const args::ArgChecker args("STATE:b");
    if (!args.check(lv, this))
        return args.usage(this, s);

    ceammc_hw_gpio_sr04_poll(sr04_.get(), lv.boolAt(0, false));
}

bool HwRpiSensorSr04::check_connected(bool print_err)
{
    if (!sr04_ && print_err) {
        OBJ_ERR << "device is not connected to the I2C bus";
    }

    return sr04_.get();
}

void HwRpiSensorSr04::gpio_connect(bool state)
{
    if (state) {
        if (trigger_pin_->value() == NULL_TRIG_GPIO_PIN) {
            OBJ_ERR << fmt::format("@trig_pin is not set");
            return;
        }

        if (echo_pin_->value() == NULL_ECHO_GPIO_PIN) {
            OBJ_ERR << fmt::format("@echo_pin is not set");
            return;
        }

        sr04_.reset(ceammc_hw_gpio_sr04_new(trigger_pin_->value(),
            echo_pin_->value(),
            on_notify(), //
            on_message(),
            { this, [](void* user, float distance_cm, bool is_inf) {
                 auto obj = static_cast<HwRpiSensorSr04*>(user);
                 if (!obj)
                     return;

                 if (!is_inf)
                     obj->floatTo(0, distance_cm);
                 else
                     obj->bangTo(1);
             } }));

        if (!sr04_)
            OBJ_ERR << "can't connect to SR04";

    } else {
        sr04_.reset();
    }
}

void setup_hw_rpi_sensor_sr04()
{
    ObjectFactory<HwRpiSensorSr04> obj("hw.rpi.sensor.sr04");
    obj.addMethod("poll", &HwRpiSensorSr04::m_poll);
}

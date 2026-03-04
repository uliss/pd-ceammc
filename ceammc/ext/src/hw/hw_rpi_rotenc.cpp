#include "hw_rpi_rotenc.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#define CHECK_GPIO_PINS(obj)                 \
    {                                        \
        if (!dt_pin_->checkPin(obj)) {       \
            OBJ_ERR << "DT pin is not set";  \
            return;                          \
        }                                    \
        if (!clk_pin_->checkPin(obj)) {      \
            OBJ_ERR << "CLK pin is not set"; \
            return;                          \
        }                                    \
        if (!check_connected(true))          \
            return;                          \
    }

HwRpiRotaryEncoder::HwRpiRotaryEncoder(const PdArgs& args)
    : HwRpiDevice<ceammc_hw_gpio_rotenc>(&ceammc_hw_gpio_rotenc_free, args)
{
    createInlet();
    createOutlet();
    createOutlet();

    dt_pin_ = addGpioPinProperty("@dt");
    clk_pin_ = addGpioPinProperty("@clk");
    btn_pin_ = addGpioPinProperty("@btn");

    init_ = new FloatProperty("@init", 0);
    init_->setInitOnly();
    addProperty(init_);

    step_ = new FloatProperty("@step", 1);
    step_->setSuccessFn([this](Property*) {
        ceammc_hw_gpio_rotenc_set_step(device(), step_->value());
    });
    addProperty(step_);

    min_ = new FloatProperty("@min", -100);
    min_->setSuccessFn([this](Property*) {
        ceammc_hw_gpio_rotenc_set_min(device(), min_->value());
    });
    addProperty(min_);

    max_ = new FloatProperty("@max", 100);
    max_->setSuccessFn([this](Property*) {
        ceammc_hw_gpio_rotenc_set_max(device(), max_->value());
    });
    addProperty(max_);
}

bool HwRpiRotaryEncoder::notify(int code)
{
    return ceammc_hw_gpio_rotenc_process_events(device());
}

void HwRpiRotaryEncoder::onBang()
{
    CHECK_GPIO_PINS(this);

    ceammc_hw_gpio_rotenc_get_value(device());
}

void HwRpiRotaryEncoder::onFloat(t_float f)
{
    CHECK_GPIO_PINS(this);

    if (ceammc_hw_gpio_rotenc_set_value(device(), f))
        floatTo(0, f);
}

void HwRpiRotaryEncoder::onInlet(size_t idx, const AtomListView& lv)
{
    CHECK_GPIO_PINS(this);

    if (idx == 1 && lv.isFloat())
        ceammc_hw_gpio_rotenc_set_value(device(), lv.asFloat());
    else if (idx == 1 && lv.empty())
        ceammc_hw_gpio_rotenc_reset(device());
}

void HwRpiRotaryEncoder::m_get(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void HwRpiRotaryEncoder::m_reset(t_symbol* s, const AtomListView& lv)
{
    CHECK_GPIO_PINS(this);

    ceammc_hw_gpio_rotenc_reset(device());
}

void HwRpiRotaryEncoder::m_set(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_GPIO_PINS(this);

    ceammc_hw_gpio_rotenc_set_value(device(), lv.floatAt(0, 0));
}

HwRpiRotaryEncoder::HwRpiDevice::Device HwRpiRotaryEncoder::createDevice()
{
    if (dt_pin_->isNone()) {
        OBJ_ERR << "GPIO DT pin is not set";
        return nullDevice();
    }

    if (clk_pin_->isNone()) {
        OBJ_ERR << "GPIO CLK pin is not set";
        return nullDevice();
    }

    if (min_->value() >= max_->value())
        OBJ_ERR << fmt::format("@min ({}) >= @max ({})", min_->value(), max_->value());

    return Device(ceammc_hw_gpio_rotenc_new(dt_pin_->value(),
                      clk_pin_->value(),
                      btn_pin_->value(),
                      init_->value(),
                      step_->value(),
                      min_->value(),
                      max_->value(),
                      //
                      on_notify(),
                      { this, [](void* user, double value, std::int8_t dir) {
                           auto obj = static_cast<HwRpiRotaryEncoder*>(user);
                           if (!obj)
                               return;

                           obj->anyTo(1, gensym("dir"), Atom(dir));
                           obj->floatTo(0, value);
                       } },
                      { this, [](void* user, bool state) {
                           auto obj = static_cast<HwRpiRotaryEncoder*>(user);
                           if (!obj)
                               return;

                           obj->anyTo(1, gensym("btn"), Atom(state));
                       } },
                      on_message()),
        freeDeviceFn());
}

void setup_hw_rpi_rotenc()
{
    ObjectFactory<HwRpiRotaryEncoder> obj("hw.rpi.rotenc");

    obj.addMethod("get", &HwRpiRotaryEncoder::m_get);
    obj.addMethod("reset", &HwRpiRotaryEncoder::m_reset);
    obj.addMethod("set", &HwRpiRotaryEncoder::m_set);
}

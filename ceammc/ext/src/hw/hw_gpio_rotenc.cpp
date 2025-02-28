#include "hw_gpio_rotenc.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

HwGpioRotaryEncoder::HwGpioRotaryEncoder(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();
    createOutlet();

    dt_pin_ = new IntProperty("@dt", 0);
    dt_pin_->checkClosedRange(0, 30);
    dt_pin_->setInitOnly();
    addProperty(dt_pin_);

    clk_pin_ = new IntProperty("@clk", 0);
    clk_pin_->checkClosedRange(0, 30);
    clk_pin_->setInitOnly();
    addProperty(clk_pin_);

    btn_pin_ = new IntProperty("@btn", 0);
    btn_pin_->checkClosedRange(0, 30);
    btn_pin_->setInitOnly();
    addProperty(btn_pin_);

    init_ = new FloatProperty("@init", 0);
    init_->setInitOnly();
    addProperty(init_);

    step_ = new FloatProperty("@step", 1);
    step_->setSuccessFn([this](Property*) {
        if (enc_)
            ceammc_hw_gpio_rotenc_set_step(enc_, step_->value());
    });
    addProperty(step_);

    min_ = new FloatProperty("@min", -100);
    min_->setSuccessFn([this](Property*) {
        if (enc_)
            ceammc_hw_gpio_rotenc_set_min(enc_, min_->value());
    });
    addProperty(min_);

    max_ = new FloatProperty("@max", 100);
    max_->setSuccessFn([this](Property*) {
        if (enc_)
            ceammc_hw_gpio_rotenc_set_max(enc_, max_->value());
    });
    addProperty(max_);
}

HwGpioRotaryEncoder::~HwGpioRotaryEncoder()
{
    ceammc_hw_gpio_rotenc_free(enc_);
}

void HwGpioRotaryEncoder::initDone()
{
    if (min_->value() >= max_->value())
        OBJ_ERR << fmt::format("@min ({}) >= @max ({})", min_->value(), max_->value());

    enc_ = ceammc_hw_gpio_rotenc_new(dt_pin_->value(),
        clk_pin_->value(),
        btn_pin_->value(),
        init_->value(),
        step_->value(),
        min_->value(),
        max_->value(),
        //
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, double value, std::int8_t dir) {
             auto obj = static_cast<HwGpioRotaryEncoder*>(user);
             if (!obj)
                 return;

             obj->anyTo(1, gensym("dir"), Atom(dir));
             obj->floatTo(0, value);
         } },
        { this, [](void* user, bool state) {
             auto obj = static_cast<HwGpioRotaryEncoder*>(user);
             if (!obj)
                 return;

             obj->anyTo(1, gensym("btn"), Atom(state));
         } },
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<HwGpioRotaryEncoder*>(user);
             Error(obj) << msg;
         } });
}

bool HwGpioRotaryEncoder::notify(int code)
{
    ceammc_hw_gpio_rotenc_process_events(enc_);
    return true;
}

void HwGpioRotaryEncoder::onBang()
{
    ceammc_hw_gpio_rotenc_get_value(enc_);
}

void HwGpioRotaryEncoder::m_get(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void HwGpioRotaryEncoder::m_reset(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_gpio_rotenc_reset(enc_);
}

void HwGpioRotaryEncoder::m_set(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_gpio_rotenc_set_value(enc_, lv.floatAt(0, 0));
}

void setup_hw_gpio_rotenc()
{
    ObjectFactory<HwGpioRotaryEncoder> obj("hw.gpio.rotenc");
    obj.addMethod("reset", &HwGpioRotaryEncoder::m_reset);
    obj.addMethod("get", &HwGpioRotaryEncoder::m_get);
    obj.addMethod("set", &HwGpioRotaryEncoder::m_set);
}

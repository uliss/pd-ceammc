#include "hw_rpi_rotenc.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#define CHECK_GPIO_PINS()                                              \
    {                                                                  \
        auto has_dt = (dt_pin_->value() != ceammc_HW_GPIO_PIN_NONE);   \
        auto has_clk = (clk_pin_->value() != ceammc_HW_GPIO_PIN_NONE); \
        if (!enc_ || !has_dt || !has_clk) {                            \
            OBJ_ERR << "pins are not set";                             \
        }                                                              \
    }

HwRpiRotaryEncoder::HwRpiRotaryEncoder(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
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

HwRpiRotaryEncoder::~HwRpiRotaryEncoder()
{
    ceammc_hw_gpio_rotenc_free(enc_);
}

void HwRpiRotaryEncoder::initDone()
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
        on_message());
}

bool HwRpiRotaryEncoder::notify(int code)
{
    return ceammc_hw_gpio_rotenc_process_events(enc_);
}

void HwRpiRotaryEncoder::onBang()
{
    CHECK_GPIO_PINS();

    ceammc_hw_gpio_rotenc_get_value(enc_);
}

void HwRpiRotaryEncoder::onFloat(t_float f)
{
    CHECK_GPIO_PINS();

    if (ceammc_hw_gpio_rotenc_set_value(enc_, f))
        floatTo(0, f);
}

void HwRpiRotaryEncoder::onInlet(size_t idx, const AtomListView& lv)
{
    CHECK_GPIO_PINS();

    if (idx == 1 && lv.isFloat())
        ceammc_hw_gpio_rotenc_set_value(enc_, lv.asFloat());
    else if (idx == 1 && lv.empty())
        ceammc_hw_gpio_rotenc_reset(enc_);
}

void HwRpiRotaryEncoder::m_get(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void HwRpiRotaryEncoder::m_reset(t_symbol* s, const AtomListView& lv)
{
    CHECK_GPIO_PINS();

    ceammc_hw_gpio_rotenc_reset(enc_);
}

void HwRpiRotaryEncoder::m_set(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_GPIO_PINS();

    ceammc_hw_gpio_rotenc_set_value(enc_, lv.floatAt(0, 0));
}

void setup_hw_rpi_rotenc()
{
    ObjectFactory<HwRpiRotaryEncoder> obj("hw.rpi.rotenc");

    obj.addMethod("get", &HwRpiRotaryEncoder::m_get);
    obj.addMethod("reset", &HwRpiRotaryEncoder::m_reset);
    obj.addMethod("set", &HwRpiRotaryEncoder::m_set);
}

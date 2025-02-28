#include "hw_gpio_rotenc.h"
#include "ceammc_factory.h"

HwGpioRotaryEncoder::HwGpioRotaryEncoder(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
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
}

HwGpioRotaryEncoder::~HwGpioRotaryEncoder()
{
    ceammc_hw_gpio_rotenc_free(enc_);
}

void HwGpioRotaryEncoder::initDone()
{
    enc_ = ceammc_hw_gpio_rotenc_new(dt_pin_->value(), clk_pin_->value(), btn_pin_->value(), //
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<HwGpioRotaryEncoder*>(user);
             Error(obj) << msg;
         } });
}

bool HwGpioRotaryEncoder::notify(int code)
{
    return true;
}

void HwGpioRotaryEncoder::m_poll(t_symbol* s, const AtomListView& lv)
{
}

void setup_hw_gpio_rotenc()
{
    ObjectFactory<HwGpioRotaryEncoder> obj("hw.gpio.rotenc");
}

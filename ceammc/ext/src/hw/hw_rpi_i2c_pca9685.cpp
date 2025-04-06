#include "hw_rpi_i2c_pca9685.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwI2cPca8695::HwI2cPca8695(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    i2c_bus_ = addI2cBusProperty();
    i2c_bus_->setArgIndex(0);

    i2c_addr_ = addI2cAddrProperty();
}

HwI2cPca8695::~HwI2cPca8695()
{
    ceammc_hw_pca9685_free(pwm_);
}

void HwI2cPca8695::initDone()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus))
        return;

    pwm_ = ceammc_hw_pca9685_new(bus,
        i2c_addr_->value(),
        on_notify(),
        on_message());
}

bool HwI2cPca8695::notify(int code)
{
    return ceammc_hw_pca9685_proc_reply(pwm_);
}

void HwI2cPca8695::m_const(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,16] VALUE:B DELAY:f[0,1)?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, 0);
    auto value = lv.boolAt(1, 0);
    float delay = lv.floatAt(2, 0);

    ceammc_hw_pca9685_set_const(pwm_, chan, value, delay);
}

void HwI2cPca8695::m_duty(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,16] DUTY:f PHASE:f?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, 0);
    auto duty = lv.floatAt(1, 0);
    float phase = lv.floatAt(2, 0);

    ceammc_hw_pca9685_set_duty_cycle(pwm_, chan, duty, lv.size() > 2 ? &phase : nullptr);
}

void HwI2cPca8695::m_set_raw(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,16] ON:i[0,4095] OFF:i[0,4095]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, 0);
    auto on = lv.intAt(1, 0);
    auto off = lv.intAt(2, 0);

    ceammc_hw_pca9685_set_on_off(pwm_, chan, on, off);
}

void HwI2cPca8695::m_width(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,16] WIDTH:f[0.5,40] PHASE:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, 0);
    auto width_ms = lv.floatAt(1, 0);
    auto phase = lv.floatAt(2, 0);

    ceammc_hw_pca9685_set_pulse_width(pwm_, chan, width_ms, phase);
}

void HwI2cPca8695::m_period(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PERIOD:f[0.5,40]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_pca9685_set_period(pwm_, lv.floatAt(0, 0));
}

void HwI2cPca8695::m_polarity(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_pca9685i_set_polarity(pwm_,
        lv.boolAt(0, true)
            ? ceammc_hw_rpi_pwm_polarity::INVERSE
            : ceammc_hw_rpi_pwm_polarity::NORMAL);
}

void HwI2cPca8695::m_enable(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_pca9685_enable(pwm_, lv.boolAt(0, true));
}

void HwI2cPca8695::m_freq(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FREQ:f[24,2048]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto freq = lv.floatAt(0, 0);
    ceammc_hw_pca9685_set_freq(pwm_, freq);
}

void setup_hw_rpi_i2c_pca9685()
{
    ObjectFactory<HwI2cPca8695> obj("hw.rpi.i2c.pca9685");
    obj.addAlias("hw.rpi.pwm.pca9685");

    obj.addMethod("const", &HwI2cPca8695::m_const);
    obj.addMethod("duty", &HwI2cPca8695::m_duty);
    obj.addMethod("enable", &HwI2cPca8695::m_enable);
    obj.addMethod("freq", &HwI2cPca8695::m_freq);
    obj.addMethod("period", &HwI2cPca8695::m_period);
    obj.addMethod("polarity", &HwI2cPca8695::m_polarity);
    obj.addMethod("set_raw", &HwI2cPca8695::m_set_raw);
    obj.addMethod("width", &HwI2cPca8695::m_width);
}

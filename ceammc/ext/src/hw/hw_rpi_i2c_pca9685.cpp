#include "hw_rpi_i2c_pca9685.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwI2cPca8695::HwI2cPca8695(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();
}

HwI2cPca8695::~HwI2cPca8695()
{
    ceammc_hw_rpi_pwm_pca9685_free(pwm_);
}

void HwI2cPca8695::initDone()
{
    pwm_ = ceammc_hw_rpi_pwm_pca9685_new(1, //
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<HwI2cPca8695*>(user);
             Error(obj) << msg;
         } });
}

bool HwI2cPca8695::notify(int code)
{
    return ceammc_hw_rpi_pwm_pca9685_proc_reply(pwm_);
}

void HwI2cPca8695::m_on_off(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,16] ON:i[0,4095] OFF:i[0,4095]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, 0);
    auto on = lv.intAt(1, 0);
    auto off = lv.intAt(2, 0);

    ceammc_hw_rpi_pwm_pca9685_set_on_off(pwm_, chan, on, off);
}

void HwI2cPca8695::m_polarity(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_rpi_pwm_pca9685i_set_polarity(pwm_,
        lv.boolAt(0, true)
            ? ceammc_hw_rpi_pwm_polarity::INVERSE
            : ceammc_hw_rpi_pwm_polarity::NORMAL);
}

void HwI2cPca8695::m_enable(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_rpi_pwm_pca9685_enable(pwm_, lv.boolAt(0, true));
}

void HwI2cPca8695::m_freq(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FREQ:f[24,2048]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto freq = lv.floatAt(0, 0);
    ceammc_hw_rpi_pwm_pca9685_set_freq(pwm_, freq);
}

void setup_hw_rpi_i2c_pca9685()
{
    ObjectFactory<HwI2cPca8695> obj("hw.rpi.pwm.pca9685");
    obj.addAlias("hw.rpi.i2c.pca9685");

    obj.addMethod("set_on_off", &HwI2cPca8695::m_on_off);
    obj.addMethod("enable", &HwI2cPca8695::m_enable);
    obj.addMethod("freq", &HwI2cPca8695::m_freq);
    obj.addMethod("polarity", &HwI2cPca8695::m_polarity);
}

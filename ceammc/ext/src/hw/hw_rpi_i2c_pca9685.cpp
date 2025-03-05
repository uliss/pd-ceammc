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

void setup_hw_rpi_i2c_pca9685()
{
    ObjectFactory<HwI2cPca8695> obj("hw.rpi.pwm.pca9685");
    obj.addAlias("hw.rpi.i2c.pca9685");

    obj.addMethod("set_on_off", &HwI2cPca8695::m_on_off);
}

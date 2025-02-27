#include "hw_rpi_pwm.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwRpiPwm::HwRpiPwm(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    chan_ = new IntProperty("@ch", 0);
    chan_->setInitOnly();
    chan_->checkClosedRange(0, 3);
    chan_->setArgIndex(0);
    addProperty(chan_);
}

HwRpiPwm::~HwRpiPwm()
{
    ceammc_hw_rpi_pwm_free(pwm_);
}

void HwRpiPwm::initDone()
{
    pwm_ = ceammc_hw_rpi_pwm_new(chan_->value(),
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
        { this, [](void* user, const char* msg) {
             Error err(static_cast<HwRpiPwm*>(user));
             err << msg;
         } });
}

bool HwRpiPwm::notify(int code)
{
    return true;
}

void HwRpiPwm::m_enable(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_rpi_pwm_enable(pwm_, lv.boolAt(0, true));
}

void HwRpiPwm::m_freq(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FREQ:f>=0 DUTY:f?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_rpi_pwm_set_freq(pwm_, lv.floatAt(0, 0), lv.floatAt(1, 0.5));
}

void setup_hw_rpi_pwm()
{
    ObjectFactory<HwRpiPwm> obj("hw.rpi.pwm");
    obj.addMethod("freq", &HwRpiPwm::m_freq);
    obj.addMethod("enable", &HwRpiPwm::m_enable);
}

#include "hw_rpi_pwm.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#define CHECK_PWM_CHAN()                                                                                \
    {                                                                                                   \
        if (chan_->value() == ceammc_HW_RPI_PWM_NONE_CHAN) {                                            \
            METHOD_ERR(s) << "PWM channel is not configured, valid channel values are: "                \
                          << (int)ceammc_HW_RPI_PWM_MIN_CHAN << "-" << (int)ceammc_HW_RPI_PWM_MAX_CHAN; \
        }                                                                                               \
    }

HwRpiPwm::HwRpiPwm(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    chan_ = new IntProperty("@ch", ceammc_HW_RPI_PWM_NONE_CHAN);
    chan_->setInitOnly();
    chan_->checkClosedRange(ceammc_HW_RPI_PWM_NONE_CHAN, ceammc_HW_RPI_PWM_MAX_CHAN);
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
    return ceammc_hw_rpi_pwm_proc_reply(pwm_);
}

void HwRpiPwm::m_duty(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("DUTY:f[0,1]");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    ceammc_hw_rpi_pwm_set_duty_cycle(pwm_, lv.floatAt(0, 0.5));
}

void HwRpiPwm::m_enable(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    ceammc_hw_rpi_pwm_enable(pwm_, lv.boolAt(0, true));
}

void HwRpiPwm::m_freq(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FREQ:f>=0 DUTY:f[0,1]?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    ceammc_hw_rpi_pwm_set_freq(pwm_, lv.floatAt(0, 0), lv.floatAt(1, 0.5));
}

void HwRpiPwm::m_period(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PERIOD:f>=0");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    ceammc_hw_rpi_pwm_set_period(pwm_, lv.floatAt(0, 0));
}

void HwRpiPwm::m_polarity(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("POLARITY:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_rpi_pwm_set_polarity(pwm_,
        lv.boolAt(0, false) //
            ? ceammc_hw_rpi_pwm_polarity::INVERSE
            : ceammc_hw_rpi_pwm_polarity::NORMAL);
}

void HwRpiPwm::m_pwm(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("PERIOD:f>=0 WIDTH:f>=0");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    auto period = lv.floatAt(0, 0);
    auto width = lv.floatAt(1, 0);
    if (width > period) {
        METHOD_ERR(s) << fmt::format("expected width<=period, get: {}>{}", width, period);
    }

    ceammc_hw_rpi_pwm_set_pwm(pwm_, period, width);
}

void HwRpiPwm::m_width(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("WIDTH:f>=0");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    CHECK_PWM_CHAN();

    ceammc_hw_rpi_pwm_set_pulse_width(pwm_, lv.floatAt(0, 0));
}

void setup_hw_rpi_pwm()
{
    ObjectFactory<HwRpiPwm> obj("hw.rpi.pwm");

    obj.addMethod("duty", &HwRpiPwm::m_duty);
    obj.addMethod("enable", &HwRpiPwm::m_enable);
    obj.addMethod("freq", &HwRpiPwm::m_freq);
    obj.addMethod("period", &HwRpiPwm::m_period);
    obj.addMethod("polarity", &HwRpiPwm::m_polarity);
    obj.addMethod("pwm", &HwRpiPwm::m_pwm);
    obj.addMethod("width", &HwRpiPwm::m_width);
}

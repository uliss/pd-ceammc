#include "hw_rpi_adc_ads1115.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(single)
CEAMMC_DEFINE_SYM_HASH(diff)

HwRpiAdcAds1115::HwRpiAdcAds1115(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { sym_single(), sym_diff() });
    mode_->setArgIndex(0);
    mode_->setInitOnly();
    addProperty(mode_);
}

HwRpiAdcAds1115::~HwRpiAdcAds1115()
{
    ceammc_hw_ads1115_free(adc_);
}

void HwRpiAdcAds1115::initDone()
{
    ceammc_hw_i2c_ads1115_measure_mode mode {};
    switch (crc32_hash(mode_->value())) {
    case hash_diff:
        mode = ceammc_hw_i2c_ads1115_measure_mode::Diff;
        break;
    case hash_single:
    default:
        mode = ceammc_hw_i2c_ads1115_measure_mode::Single;
        break;
    }

    adc_ = ceammc_hw_ads1115_new(0, 0, mode, on_notify(), on_err(),
        { this,
            [](void* user, std::uint8_t chan, std::int16_t value) {},
            [](void* user, std::int16_t values[4]) {} });
}

bool HwRpiAdcAds1115::notify(int code)
{
    return ceammc_hw_ads1115_process_reply(adc_);
}

void HwRpiAdcAds1115::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_ads1115_poll(adc_, lv.boolAt(0, false));
}

void HwRpiAdcAds1115::m_measure(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAN:i[0,3]?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    auto chan = lv.intAt(0, -1);

    if (chan < 0)
        ceammc_hw_ads1115_measure_all(adc_);
    else
        ceammc_hw_ads1115_measure_chan(adc_, lv.intAt(0, 0));
}

void setup_hw_rpi_adc_ads1115()
{
    ObjectFactory<HwRpiAdcAds1115> obj("hw.rpi.adc.ads1115");

    obj.addMethod("measure", &HwRpiAdcAds1115::m_measure);
    obj.addMethod("poll", &HwRpiAdcAds1115::m_poll);
}

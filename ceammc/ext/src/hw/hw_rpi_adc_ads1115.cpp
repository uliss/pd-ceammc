#include "hw_rpi_adc_ads1115.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
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

    i2c_bus_ = new IntProperty("@bus", ceammc_HW_I2C_DEFAULT_BUS);
    i2c_bus_->setInitOnly();
    i2c_bus_->checkClosedRange(-1, 16);
    addProperty(i2c_bus_);

    range_ = new IntEnumProperty("@range", { 6144, 256, 512, 1024, 2048, 4096 });
    range_->setInitOnly();
    addProperty(range_);

    normalize_ = new FlagProperty("@norm");
    addProperty(normalize_);

    norm_max_ = new FloatProperty("@max", 1);
    addProperty(norm_max_);

    norm_min_ = new FloatProperty("@min", 0);
    addProperty(norm_min_);

    sym_channel_ = gensym("ch");
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

    adc_ = ceammc_hw_ads1115_new(i2c_bus_->value(), ceammc_HW_I2C_DEFAULT_ADDR, mode, on_notify(), on_err(),
        { this,
            [](void* user, std::uint8_t chan, std::int16_t value) {
                auto obj = static_cast<HwRpiAdcAds1115*>(user);
                if (obj)
                    obj->outputValue(chan, value);
            },
            [](void* user, std::int16_t values[4]) {
                auto obj = static_cast<HwRpiAdcAds1115*>(user);
                if (obj) {
                    for (size_t i = 0; i < 4; i++) {
                        obj->outputValue(i, values[i]);
                    }
                }
            } });

    ceammc_hw_i2c_ads1115_range range;

    switch (range_->value()) {
    case 256:
        range = ceammc_hw_i2c_ads1115_range::Within_0_256V;
        break;
    case 512:
        range = ceammc_hw_i2c_ads1115_range::Within_0_512V;
        break;
    case 1024:
        range = ceammc_hw_i2c_ads1115_range::Within_1_024V;
        break;
    case 2048:
        range = ceammc_hw_i2c_ads1115_range::Within_2_048V;
        break;
    case 4096:
        range = ceammc_hw_i2c_ads1115_range::Within_4_096V;
        break;
    default:
        range = ceammc_hw_i2c_ads1115_range::Within_6_144V;
        break;
    }

    ceammc_hw_ads1115_set_input_range(adc_, range);
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

t_float HwRpiAdcAds1115::normalizeValue(std::int16_t value) const
{
    if (normalize_->value()) {
        auto a = norm_min_->value();
        auto b = norm_max_->value();

        return convert::lin2lin<t_float>(value, -0x7fff, 0x8000, a, b);
    } else {
        return value;
    }
}

void HwRpiAdcAds1115::outputValue(uint8_t ch, int16_t value)
{
    AtomArray<2> data;
    data[0] = ch;
    data[1] = normalizeValue(value);
    anyTo(0, sym_channel_, normalizeValue(value));
}

void setup_hw_rpi_adc_ads1115()
{
    ObjectFactory<HwRpiAdcAds1115> obj("hw.rpi.adc.ads1115");

    obj.addMethod("measure", &HwRpiAdcAds1115::m_measure);
    obj.addMethod("poll", &HwRpiAdcAds1115::m_poll);
}

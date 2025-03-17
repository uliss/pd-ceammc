#include "hw_rpi_adc_ads1115.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(single)
CEAMMC_DEFINE_SYM_HASH(diff)

#define MVOLT_VALUE(v)           \
    constexpr t_float mv##v = v; \
    constexpr int int_mv##v = v; \
    CEAMMC_DEFINE_SYM_HASH(v##mv);

MVOLT_VALUE(256)
MVOLT_VALUE(512)
MVOLT_VALUE(1024)
MVOLT_VALUE(2048)
MVOLT_VALUE(4096)
MVOLT_VALUE(6144)

HwRpiAdcAds1115::HwRpiAdcAds1115(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { sym_single(), sym_diff() });
    mode_->setArgIndex(0);
    mode_->setInitOnly();
    addProperty(mode_);

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();

    fsr_ = new SymbolFloatEnumProperty("@fsr", {
                                                   { sym_6144mv(), mv6144 },
                                                   { sym_256mv(), mv256 },
                                                   { sym_512mv(), mv512 },
                                                   { sym_1024mv(), mv1024 },
                                                   { sym_2048mv(), mv2048 },
                                                   { sym_4096mv(), mv4096 },
                                               });
    fsr_->setInitOnly();
    addProperty(fsr_);

    poll_time_ = new FloatProperty("@poll_time", ceammc_HW_ADC_ADS1115_DEF_POLL_TIME_MSEC);
    poll_time_->setInitOnly();
    poll_time_->checkClosedRange(ceammc_HW_ADC_ADS1115_MIN_POLL_TIME_MSEC, ceammc_HW_ADC_ADS1115_MAX_POLL_TIME_MSEC);
    addProperty(poll_time_);

    normalize_ = new BoolProperty("@norm", false);
    addProperty(normalize_);

    out_max_ = new FloatProperty("@out_max", 1);
    addProperty(out_max_);

    out_min_ = new FloatProperty("@out_min", 0);
    addProperty(out_min_);

    in_max_ = new FloatProperty("@in_max", 5000);
    in_max_->checkClosedRange(0, mv6144);
    addProperty(in_max_);

    in_min_ = new FloatProperty("@in_min", 0);
    in_min_->checkClosedRange(0, mv6144);
    addProperty(in_min_);

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

    adc_ = ceammc_hw_ads1115_new(i2c_bus_->value(),
        i2c_addr_->value(),
        mode,
        on_notify(),
        on_message(),
        { this,
            [](void* user, std::uint8_t chan, std::int16_t value) {
                auto obj = static_cast<HwRpiAdcAds1115*>(user);
                if (obj)
                    obj->outputValue(chan, value);
            },
            [](void* user, int16_t a0, int16_t a1, int16_t a2, int16_t a3) {
                auto obj = static_cast<HwRpiAdcAds1115*>(user);
                if (obj) {
                    obj->outputValue(3, a3);
                    obj->outputValue(2, a2);
                    obj->outputValue(1, a1);
                    obj->outputValue(0, a0);
                }
            } });

    ceammc_hw_i2c_ads1115_range range;

    switch (static_cast<int>(fsr_->valuePair())) {
    case int_mv256:
        range = ceammc_hw_i2c_ads1115_range::Within_0_256V;
        break;
    case int_mv512:
        range = ceammc_hw_i2c_ads1115_range::Within_0_512V;
        break;
    case int_mv1024:
        range = ceammc_hw_i2c_ads1115_range::Within_1_024V;
        break;
    case int_mv2048:
        range = ceammc_hw_i2c_ads1115_range::Within_2_048V;
        break;
    case int_mv4096:
        range = ceammc_hw_i2c_ads1115_range::Within_4_096V;
        break;
    case int_mv6144:
    default:
        range = ceammc_hw_i2c_ads1115_range::Within_6_144V;
        break;
    }

    ceammc_hw_ads1115_set_input_range(adc_, range);
    ceammc_hw_ads1115_set_poll_time(adc_, poll_time_->value());
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
    const auto FSR = fsr_->valuePair();

    auto value_in_mv = (mode_->index() == 0) ? convert::lin2lin<double>(value, 0, 0x8000, 0, FSR)
                                             : convert::lin2lin<double>(value, -0x7fff, 0x8000, -FSR, FSR);

    if (normalize_->value()) {
        auto x0 = in_min_->value();
        auto x1 = in_max_->value();
        if (x0 == x1) {
            OBJ_DBG << "zero input range: " << x0 << ' ' << x1;
            return 0;
        }

        auto y0 = out_min_->value();
        auto y1 = out_max_->value();
        if (y0 == y1) {
            OBJ_DBG << "zero output range: " << y0 << ' ' << y1;
            return 0;
        }

        return convert::lin2lin<double>(value_in_mv, x0, x1, y0, y1);
    } else {
        return value_in_mv;
    }
}

void HwRpiAdcAds1115::outputValue(uint8_t ch, int16_t value)
{
    AtomArray<2> data;
    data[0] = ch;
    data[1] = normalizeValue(value);
    anyTo(0, sym_channel_, data.view());
}

void setup_hw_rpi_adc_ads1115()
{
    ObjectFactory<HwRpiAdcAds1115> obj("hw.rpi.adc.ads1115");

    obj.addMethod("measure", &HwRpiAdcAds1115::m_measure);
    obj.addMethod("poll", &HwRpiAdcAds1115::m_poll);
}

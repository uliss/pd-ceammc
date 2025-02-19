#include "hw_spi_max7219.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

CEAMMC_DEFINE_HASH(left)
CEAMMC_DEFINE_HASH(right)
CEAMMC_DEFINE_HASH(center)

HwSpiMax7219::HwSpiMax7219(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    num_ = new IntProperty("@num", 1);
    num_->setInitOnly();
    num_->checkClosedRange(1, 4);
    addProperty(num_);
}

HwSpiMax7219::~HwSpiMax7219()
{
    ceammc_hw_max7219_free(mx_);
}

void HwSpiMax7219::initDone()
{
    mx_ = ceammc_hw_max7219_new(num_->value(),
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<HwSpiMax7219*>(user);
             if (obj)
                 Error(obj) << msg;
         } });
}

bool HwSpiMax7219::notify(int code)
{
    return true;
}

void HwSpiMax7219::m_intensity(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:i[0,15] ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_max7219_intensity(mx_, lv.intAt(0, 0), lv.intAt(1, -1));
}

void HwSpiMax7219::m_power(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_max7219_power(mx_, lv.boolAt(0, false));
}

void HwSpiMax7219::m_write_int(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("INT:i ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_max7219_write_int(mx_, lv.intAt(0, 0), lv.intAt(1, 0));
}

void HwSpiMax7219::m_write_hex(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("HEX:i>=0 ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_max7219_write_hex(mx_, lv.intAt(0, 0), lv.intAt(1, 0));
}

void HwSpiMax7219::m_write_data(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("DIGIT:i[0,7] DATA:b ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto digit = lv.intAt(0, 0);
    const auto data = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_digit_data(mx_, addr, digit, data);
}

void HwSpiMax7219::m_write_float(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f PRECIS:i[0,7] ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto value = lv.floatAt(0, 0);
    const auto precision = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_float(mx_, addr, value, precision);
}

void HwSpiMax7219::m_write_str(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STR:s ALIGN:s=left|right|center ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto str = lv.symbolAt(0, &s_)->s_name;

    ceammc_hw_max7219_string_align align = ceammc_hw_max7219_string_align::Left;

    switch (crc32_hash(lv.symbolAt(1, gensym(str_left)))) {
    case hash_center:
        align = ceammc_hw_max7219_string_align::Center;
        break;
    case hash_right:
        align = ceammc_hw_max7219_string_align::Right;
        break;
    default:
        align = ceammc_hw_max7219_string_align::Left;
        break;
    }

    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_str(mx_, addr, str, align);
}

void HwSpiMax7219::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_max7219_clear(mx_, lv.intAt(0, -1));
}

void setup_hw_spi_max7219()
{
    ObjectFactory<HwSpiMax7219> obj("hw.spi.max7219");
    obj.addMethod("intensity", &HwSpiMax7219::m_intensity);
    obj.addMethod("power", &HwSpiMax7219::m_power);
    obj.addMethod("write_int", &HwSpiMax7219::m_write_int);
    obj.addMethod("write_hex", &HwSpiMax7219::m_write_hex);
    obj.addMethod("write_data", &HwSpiMax7219::m_write_data);
    obj.addMethod("write_float", &HwSpiMax7219::m_write_float);
    obj.addMethod("write_str", &HwSpiMax7219::m_write_str);
    obj.addMethod("clear", &HwSpiMax7219::m_clear);
}

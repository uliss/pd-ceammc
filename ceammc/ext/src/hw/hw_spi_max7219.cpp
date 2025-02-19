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
    displays_ = new IntProperty("@displays", 1);
    displays_->setInitOnly();
    displays_->checkClosedRange(1, 4);
    displays_->setArgIndex(0);
    addProperty(displays_);

    spi_ = new IntProperty("@spi", (int)ceammc_hw_spi_bus::SPI0);
    spi_->setInitOnly();
    spi_->checkClosedRange((int)ceammc_hw_spi_bus::SPI0, (int)ceammc_hw_spi_bus::SPI6);
    addProperty(spi_);

    cs_ = new IntProperty("@cs", (int)ceammc_hw_spi_cs::CS0);
    cs_->setInitOnly();
    cs_->checkClosedRange((int)ceammc_hw_spi_cs::CS0, (int)ceammc_hw_spi_cs::CS3);
    addProperty(cs_);
}

HwSpiMax7219::~HwSpiMax7219()
{
    ceammc_hw_max7219_free(mx_);
}

void HwSpiMax7219::initDone()
{
    mx_ = ceammc_hw_max7219_new(displays_->value(),
        static_cast<ceammc_hw_spi_bus>(spi_->value()),
        static_cast<ceammc_hw_spi_cs>(cs_->value()),
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
    static const args::ArgChecker chk("VALUE:i[0,15] ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto addr = lv.intAt(1, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_intensity(mx_, addr, lv.intAt(0, 0));
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

    const auto addr = lv.intAt(1, 0);
    ceammc_hw_max7219_write_int(mx_, addr, lv.intAt(0, 0));
}

void HwSpiMax7219::m_write_hex(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("HEX:i>=0 ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto addr = lv.intAt(1, 0);
    ceammc_hw_max7219_write_hex(mx_, addr, lv.intAt(0, 0));
}

void HwSpiMax7219::m_write_reg(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("REGISTER:i[0,15] DATA:b ADDR:i>=0?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto reg = lv.intAt(0, 0);
    const auto data = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_reg(mx_, addr, reg, data);
}

void HwSpiMax7219::m_write_float(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f PRECIS:i[0,7] ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto value = lv.floatAt(0, 0);
    const auto precision = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_float(mx_, addr, value, precision);
}

void HwSpiMax7219::m_write_str(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STR:s DOTS:b? ALIGN:s=left|right|center? ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto str = lv.symbolAt(0, &s_)->s_name;
    const auto dots = lv.intAt(1, 0);

    ceammc_hw_max7219_string_align align = ceammc_hw_max7219_string_align::Right;

    switch (crc32_hash(lv.symbolAt(2, gensym(str_right)))) {
    case hash_center:
        align = ceammc_hw_max7219_string_align::Center;
        break;
    case hash_right:
        align = ceammc_hw_max7219_string_align::Right;
        break;
    case hash_left:
        align = ceammc_hw_max7219_string_align::Left;
        break;
    default:
        align = ceammc_hw_max7219_string_align::Right;
        break;
    }

    const auto addr = lv.intAt(3, 0);
    ceammc_hw_max7219_write_str(mx_, addr, str, align, dots);
}

void HwSpiMax7219::m_write_bits(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i BITS:b+");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto addr = lv.intAt(0, 0);
    std::vector<std::uint8_t> bytes;
    bytes.reserve(lv.size() - 1);
    for (auto& a : lv.subView(1))
        bytes.push_back(a.asInt());

    ceammc_hw_max7219_write_bits(mx_, addr, bytes.data(), bytes.size());
}

void HwSpiMax7219::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto addr = lv.intAt(0, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_clear(mx_, addr);
}

void HwSpiMax7219::m_test(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B ADDR:i?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    const auto addr = lv.intAt(0, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_test(mx_, addr, lv.boolAt(0, false));
}

void setup_hw_spi_max7219()
{
    ObjectFactory<HwSpiMax7219> obj("hw.spi.max7219");
    obj.addMethod("intensity", &HwSpiMax7219::m_intensity);
    obj.addMethod("power", &HwSpiMax7219::m_power);
    obj.addMethod("write_int", &HwSpiMax7219::m_write_int);
    obj.addMethod("write_hex", &HwSpiMax7219::m_write_hex);
    obj.addMethod("write_reg", &HwSpiMax7219::m_write_reg);
    obj.addMethod("write_float", &HwSpiMax7219::m_write_float);
    obj.addMethod("write_str", &HwSpiMax7219::m_write_str);
    obj.addMethod("write_bits", &HwSpiMax7219::m_write_bits);
    obj.addMethod("clear", &HwSpiMax7219::m_clear);
    obj.addMethod("test", &HwSpiMax7219::m_test);
}

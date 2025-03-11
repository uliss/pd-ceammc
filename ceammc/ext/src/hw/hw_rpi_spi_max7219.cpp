#include "hw_rpi_spi_max7219.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

CEAMMC_DEFINE_HASH(left)
CEAMMC_DEFINE_HASH(right)
CEAMMC_DEFINE_HASH(center)
CEAMMC_DEFINE_HASH(bits)
CEAMMC_DEFINE_HASH(bytes)
CEAMMC_DEFINE_HASH(float)
CEAMMC_DEFINE_HASH(hex)
CEAMMC_DEFINE_HASH(int)
CEAMMC_DEFINE_HASH(matrix)
CEAMMC_DEFINE_HASH(reg)
CEAMMC_DEFINE_HASH(str)

HwSpiMax7219::HwSpiMax7219(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    displays_ = new IntProperty("@displays", 1);
    displays_->setInitOnly();
    displays_->checkClosedRange(1, 4);
    displays_->setArgIndex(0);
    addProperty(displays_);

    spi_ = new IntProperty("@spi", static_cast<int>(ceammc_hw_spi_bus::SPI0));
    spi_->setInitOnly();
    spi_->checkClosedRange(static_cast<int>(ceammc_hw_spi_bus::SPI0), static_cast<int>(ceammc_hw_spi_bus::SPI6));
    addProperty(spi_);

    cs_ = new IntProperty("@cs", static_cast<int>(ceammc_hw_spi_cs::CS0));
    cs_->setInitOnly();
    cs_->checkClosedRange(static_cast<int>(ceammc_hw_spi_cs::CS0), static_cast<int>(ceammc_hw_spi_cs::CS3));
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
        on_notify(),
        on_err());
}

bool HwSpiMax7219::notify(int code)
{
    return true;
}

void HwSpiMax7219::m_intensity(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:i[0,15] ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(1, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_intensity(mx_, addr, lv.intAt(0, 0));
}

void HwSpiMax7219::m_power(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_hw_max7219_power(mx_, lv.boolAt(0, false));
}

void HwSpiMax7219::writeInt(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("INT:i ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(1, 0);
    ceammc_hw_max7219_write_int(mx_, addr, lv.intAt(0, 0));
}

void HwSpiMax7219::writeHex(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("HEX:i>=0 ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(1, 0);
    ceammc_hw_max7219_write_hex(mx_, addr, lv.intAt(0, 0));
}

void HwSpiMax7219::writeReg(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("REGISTER:i[0,15] DATA:b ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto reg = lv.intAt(0, 0);
    const auto data = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_reg(mx_, addr, reg, data);
}

void HwSpiMax7219::writeFloat(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("VALUE:f PRECIS:i[0,7] ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto value = lv.floatAt(0, 0);
    const auto precision = lv.intAt(1, 0);
    const auto addr = lv.intAt(2, 0);
    ceammc_hw_max7219_write_float(mx_, addr, value, precision);
}

void HwSpiMax7219::writeStr(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STR:s DOTS:b? ALIGN:s=left|right|center? ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

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

void HwSpiMax7219::writeBytes(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i[-1,7] BYTES:b{1,8}");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(0, 0);
    std::vector<std::uint8_t> bytes;
    bytes.reserve(lv.size() - 1);
    for (auto& a : lv.subView(1)) {
        bytes.push_back(a.asInt());
    }

    ceammc_hw_max7219_write_bytes(mx_, addr, bytes.data(), bytes.size());
}

void HwSpiMax7219::writeMatrix(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ROWS:i>=0 COLS:i>=0 MAXTRIX:B{1,512}");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto nrows = lv.intAt(0, 0);
    const auto ncols = lv.intAt(1, 0);
    std::vector<std::uint8_t> bits;
    bits.reserve(lv.size() - 2);
    for (auto& a : lv.subView(2)) {
        bits.push_back(a.asInt());
    }

    ceammc_hw_max7219_write_matrix(mx_, nrows, ncols, bits.data(), bits.size());
}

void HwSpiMax7219::writeBits(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i[-1,7] BITS:B{1,256}");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(0, 0);
    std::vector<std::uint8_t> bits;
    bits.reserve(lv.size() - 1);
    for (auto& a : lv.subView(1)) {
        bits.push_back(a.asInt());
    }

    ceammc_hw_max7219_write_bits(mx_, addr, bits.data(), bits.size());
}

void HwSpiMax7219::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(0, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_clear(mx_, addr);
}

void HwSpiMax7219::m_test(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B ADDR:i[-1,7]?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    const auto addr = lv.intAt(1, ceammc_HW_MAX7219_ADDRESS_ALL);
    ceammc_hw_max7219_test(mx_, addr, lv.boolAt(0, false));
}

void HwSpiMax7219::m_write(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("s=bits|bytes|float|hex|int|matrix|reg|str DATA:a+");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto sel = lv.symbolAt(0, &s_);
    switch (crc32_hash(sel)) {
    case hash_bits:
        return writeBits(sel, lv.subView(1));
    case hash_bytes:
        return writeBytes(sel, lv.subView(1));
    case hash_float:
        return writeFloat(sel, lv.subView(1));
    case hash_hex:
        return writeHex(sel, lv.subView(1));
    case hash_int:
        return writeInt(sel, lv.subView(1));
    case hash_matrix:
        return writeMatrix(sel, lv.subView(1));
    case hash_reg:
        return writeReg(sel, lv.subView(1));
    case hash_str:
        return writeStr(sel, lv.subView(1));
    default:
        chk.usage(this, s);
    }
}

void setup_hw_rpi_spi_max7219()
{
    ObjectFactory<HwSpiMax7219> obj("hw.spi.max7219");
    obj.addMethod("intensity", &HwSpiMax7219::m_intensity);
    obj.addMethod("power", &HwSpiMax7219::m_power);
    obj.addMethod("write", &HwSpiMax7219::m_write);
    obj.addMethod("clear", &HwSpiMax7219::m_clear);
    obj.addMethod("test", &HwSpiMax7219::m_test);
}

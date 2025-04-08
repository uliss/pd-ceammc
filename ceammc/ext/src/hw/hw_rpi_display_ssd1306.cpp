#include "hw_rpi_display_ssd1306.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(bitmap)
CEAMMC_DEFINE_SYM_HASH(bytes)
CEAMMC_DEFINE_SYM_HASH(i2c)
CEAMMC_DEFINE_SYM_HASH(matrix)
CEAMMC_DEFINE_SYM_HASH(spi)

HwRpiDisplaySsd1306::HwRpiDisplaySsd1306(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { sym_i2c(), sym_spi() });
    mode_->setInitOnly();
    mode_->setArgIndex(0);
    addProperty(mode_);

    spi_bus_ = addSpiBusProperty();
    spi_freq_ = addSpiFreqProperty();

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();

    spi_gpio_dc_pin_ = addGpioPinProperty("@spi_dc");
    spi_gpio_rs_pin_ = addGpioPinProperty("@spi_rs");
    spi_gpio_cs_pin_ = addGpioPinProperty("@spi_cs");

    size_ = new ListProperty("@size", { 128, 64 });
    size_->setInitOnly();
    size_->checkRangeElementCount(2, 2);
    size_->setListCheckFn([](const AtomListView& lv) -> bool {
        return lv.size() == 2 && lv.allOf([](const Atom& a) -> bool {
            return a.isInteger() && a.asInt() > 0;
        });
    });
    addProperty(size_);
}

HwRpiDisplaySsd1306::~HwRpiDisplaySsd1306()
{
    ceammc_hw_display_ssd1306_free(display_);
}

void HwRpiDisplaySsd1306::initDone()
{
    auto w = size_->value().intAt(0, 0);
    auto h = size_->value().intAt(1, 0);

    switch (crc32_hash(mode_->value())) {
    case hash_i2c: {
        std::int8_t bus = 0;
        if (i2c_bus_->getBus(bus)) {
            display_ = ceammc_hw_display_ssd1306_new_i2c(
                bus,
                i2c_addr_->value(),
                w, h,
                on_notify(),
                on_message());
        }
    } break;
    case hash_spi: {
        if (spi_gpio_dc_pin_->isNone()) {
            OBJ_ERR << "DC pin should be specified";
            return;
        }

        if (spi_gpio_rs_pin_->isNone()) {
            OBJ_ERR << "RS (reset) should be specified";
            return;
        }

        if (spi_gpio_cs_pin_->isNone()) {
            OBJ_ERR << "CS (chip select) should be specified";
            return;
        }

        display_ = ceammc_hw_display_ssd1306_new_spi(
            spi_bus_->value(),
            spi_gpio_dc_pin_->value(),
            spi_gpio_cs_pin_->value(),
            spi_gpio_rs_pin_->value(),
            spi_freq_->value(),
            w, h,
            on_notify(),
            on_message());
    } break;
    default:
        OBJ_ERR << "not implemented";
        break;
    }
}

bool HwRpiDisplaySsd1306::notify(int code)
{
    return ceammc_hw_display_ssd1306_proc_reply(display_);
}

void HwRpiDisplaySsd1306::m_brightness(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("LEVEL:i[0,4]?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_set_brightness(display_, lv.intAt(0, 0));
}

void HwRpiDisplaySsd1306::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FLUSH:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_clear(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_flush(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_display_ssd1306_flush(display_);
}

void HwRpiDisplaySsd1306::m_font(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FONT:s");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_set_font(display_, lv.symbolAt(0, &s_)->s_name);
}

void HwRpiDisplaySsd1306::m_invert(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("INVERT:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_invert(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_mirror(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("MIRROR:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_mirror(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_pixel(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 VALUE:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_set_pixel(display_, lv.intAt(0, 0), lv.intAt(1, 0), lv.boolAt(2, true));
}

void HwRpiDisplaySsd1306::m_rotation(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ANGLE:i=0|90|180|270");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_rotation rot;
    switch (lv.intAt(0, 0)) {
    case 90:
        rot = ceammc_hw_display_rotation::ROTATE_90;
        break;
    case 180:
        rot = ceammc_hw_display_rotation::ROTATE_180;
        break;
    case 270:
        rot = ceammc_hw_display_rotation::ROTATE_270;
        break;
    default:
        rot = ceammc_hw_display_rotation::ROTATE_0;
        break;
    }

    ceammc_hw_display_ssd1306_set_rotation(display_, rot);
}

void HwRpiDisplaySsd1306::m_switch_on(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ON:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_switch_on(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_text(t_symbol* s, const AtomListView& lv)
{
    auto txt = lv.symbolAt(0, &s_);
    auto x = lv.intAt(1, 0);
    auto y = lv.intAt(2, 0);
    ceammc_hw_display_ssd1306_text(display_, txt->s_name, x, y);
}

void HwRpiDisplaySsd1306::m_write(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("s=bytes|matrix|bitmap DATA:a+");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto sel = lv.symbolAt(0, &s_);
    switch (crc32_hash(sel)) {
    case hash_bytes:
        return writeBytes(sel, lv.subView(1));
    case hash_bitmap:
        return writeBitmap(sel, lv.subView(1));
    case hash_matrix:
        //        return writeMatrix(sel, lv.subView(1));
    default:
        chk.usage(this, s);
    }
}

void HwRpiDisplaySsd1306::writeBytes(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("BYTES:b+");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    std::vector<std::uint8_t> bytes;
    bytes.reserve(lv.size());
    for (auto& a : lv) {
        bytes.push_back(a.asInt());
    }

    ceammc_hw_display_ssd1306_write_bytes(display_, bytes.data(), bytes.size());
}

void HwRpiDisplaySsd1306::writeBitmap(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i Y:i WIDTH:i>0 BYTES:b+");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto w = lv.intAt(2, 0);
    auto data = lv.subView(3);

    std::vector<std::uint8_t> bitmap;
    for (size_t i = 0; i < data.size(); i++) {
        auto bidx = i % 8;

        if (bidx == 0)
            bitmap.push_back(0);

        if (data[i].asT<t_int>())
            bitmap.back() |= (0b10000000 >> bidx);
    }

    ceammc_hw_display_ssd1306_write_bitmap(display_, x, y, w, bitmap.data(), bitmap.size());
}

void setup_hw_rpi_display_ssd1306()
{
    ObjectFactory<HwRpiDisplaySsd1306> obj("hw.rpi.display.ssd1306");

    obj.addMethod("brightness", &HwRpiDisplaySsd1306::m_brightness);
    obj.addMethod("clear", &HwRpiDisplaySsd1306::m_clear);
    obj.addMethod("flush", &HwRpiDisplaySsd1306::m_flush);
    obj.addMethod("font", &HwRpiDisplaySsd1306::m_font);
    obj.addMethod("invert", &HwRpiDisplaySsd1306::m_invert);
    obj.addMethod("mirror", &HwRpiDisplaySsd1306::m_mirror);
    obj.addMethod("pixel", &HwRpiDisplaySsd1306::m_pixel);
    obj.addMethod("rotation", &HwRpiDisplaySsd1306::m_rotation);
    obj.addMethod("switch_on", &HwRpiDisplaySsd1306::m_switch_on);
    obj.addMethod("text", &HwRpiDisplaySsd1306::m_text);
    obj.addMethod("write", &HwRpiDisplaySsd1306::m_write);
}

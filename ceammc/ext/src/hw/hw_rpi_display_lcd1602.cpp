#include "hw_rpi_display_lcd1602.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/core.h"

HwRpiDisplayLcd1602::HwRpiDisplayLcd1602(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    // for i2c PCF8574 using NXP chip: most likely 0x3F
    // for i2c PCF8574 using TI (Texas Instruments): most likely: 0x27

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();

    rows_ = new IntEnumProperty("@rows", { 1, 2, 4 });
    rows_->setInitOnly();
    addProperty(rows_);
}

HwRpiDisplayLcd1602::~HwRpiDisplayLcd1602()
{
    ceammc_hw_lcd1602_free(lcd_);
}

void HwRpiDisplayLcd1602::initDone()
{
    std::int8_t bus = 0;
    if (!i2c_bus_->getBus(bus))
        return;

    lcd_ = ceammc_hw_lcd1602_new(
        bus,
        i2c_addr_->value(),
        rows_->value(),
        on_notify(),
        on_message());
}

bool HwRpiDisplayLcd1602::notify(int code)
{
    return ceammc_hw_lcd1602_process(lcd_);
}

void HwRpiDisplayLcd1602::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_clear(lcd_);
}

void HwRpiDisplayLcd1602::m_backlight(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_backlight(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayLcd1602::m_write(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_write_text(lcd_, to_string(lv).c_str());
}

void HwRpiDisplayLcd1602::m_cursor_on(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_cursor_on(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayLcd1602::m_cursor_blink(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_cursor_blink(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayLcd1602::m_cursor_pos(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("LINE:i[0,3] COL:i[0,20]");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    auto line = lv.intAt(0, 0);
    auto col = lv.intAt(1, 0);

    if (line >= rows_->value()) {
        METHOD_ERR(s) << fmt::format("line number expected to be <{}, got: {}", rows_->value(), line);
        return;
    }

    ceammc_hw_lcd1602_cursor_pos(lcd_, line, col);
}

void HwRpiDisplayLcd1602::m_font(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FONT:i=8|10");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    switch (lv.intAt(0, 0)) {
    case 10:
        ceammc_hw_lcd1602_set_font(lcd_, ceammc_hw_hd44780_font::FONT_5x10);
        break;
    default:
        ceammc_hw_lcd1602_set_font(lcd_, ceammc_hw_hd44780_font::FONT_5x8);
    }
}

void HwRpiDisplayLcd1602::m_cursor_move(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SHIFT:i?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_move_cursor(lcd_, lv.intAt(0, 1));
}

void HwRpiDisplayLcd1602::m_display_move(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SHIFT:i?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_scroll_text(lcd_, lv.intAt(0, 1));
}

void setup_hw_rpi_display_lcd1602()
{
    ObjectFactory<HwRpiDisplayLcd1602> obj("hw.i2c.lcd1602");

    obj.addMethod("clear", &HwRpiDisplayLcd1602::m_clear);
    obj.addMethod("backlight", &HwRpiDisplayLcd1602::m_backlight);
    obj.addMethod("write", &HwRpiDisplayLcd1602::m_write);

    obj.addMethod("cursor_on", &HwRpiDisplayLcd1602::m_cursor_on);
    obj.addMethod("cursor_blink", &HwRpiDisplayLcd1602::m_cursor_blink);
    obj.addMethod("cursor_pos", &HwRpiDisplayLcd1602::m_cursor_pos);

    obj.addMethod("move_cursor", &HwRpiDisplayLcd1602::m_cursor_move);
    obj.addMethod("move_display", &HwRpiDisplayLcd1602::m_display_move);

    obj.addMethod("font", &HwRpiDisplayLcd1602::m_font);
}

#include "hw_rpi_display_hd44780.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/core.h"

CEAMMC_DEFINE_SYMBOL2(lcd1602_alias, "hw.rpi.display.lcd1602");
CEAMMC_DEFINE_SYMBOL2(lcd2004_alias, "hw.rpi.display.lcd2004");

HwRpiDisplayHd44780::HwRpiDisplayHd44780(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
{
    createOutlet();

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();

    rows_ = new IntEnumProperty("@rows", { 1, 2, 4 });
    rows_->setInitOnly();
    addProperty(rows_);
}

HwRpiDisplayHd44780::~HwRpiDisplayHd44780()
{
    ceammc_hw_lcd1602_free(lcd_);
}

void HwRpiDisplayHd44780::initDone()
{
    if (pdArgs().creationName == sym_lcd1602_alias())
        rows_->setValue(2);
    else if (pdArgs().creationName == sym_lcd2004_alias())
        rows_->setValue(4);

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

bool HwRpiDisplayHd44780::notify(int code)
{
    return ceammc_hw_lcd1602_process(lcd_);
}

void HwRpiDisplayHd44780::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_clear(lcd_);
}

void HwRpiDisplayHd44780::m_clear_line(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("LINE:i[0,3]");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    auto line = lv.intAt(0, 0);

    if (line >= rows_->value()) {
        METHOD_ERR(s) << fmt::format("line number expected to be <{}, got: {}", rows_->value(), line);
        return;
    }

    constexpr const char* SPACES_20 = "                    ";

    ceammc_hw_lcd1602_cursor_pos(lcd_, line, 0);
    ceammc_hw_lcd1602_write_text(lcd_, SPACES_20);
    ceammc_hw_lcd1602_cursor_pos(lcd_, line, 0);
}

void HwRpiDisplayHd44780::m_backlight(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_backlight(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayHd44780::m_write(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("TXT:a+");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_write_text(lcd_, to_string(lv).c_str());
}

void HwRpiDisplayHd44780::m_char(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("CHAR:i[0,255]");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_write_char(lcd_, lv.intAt(0, 0));
}

void HwRpiDisplayHd44780::m_cursor_on(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_cursor_on(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayHd44780::m_cursor_blink(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_cursor_blink(lcd_, lv.boolAt(0, false));
}

void HwRpiDisplayHd44780::m_cursor_pos(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("LINE:i[0,3] COL:i[0,20)");
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

void HwRpiDisplayHd44780::m_font(t_symbol* s, const AtomListView& lv)
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

void HwRpiDisplayHd44780::m_cursor_move(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SHIFT:i?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_move_cursor(lcd_, lv.intAt(0, 1));
}

void HwRpiDisplayHd44780::m_display_move(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("SHIFT:i?");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    ceammc_hw_lcd1602_scroll_text(lcd_, lv.intAt(0, 1));
}

void setup_hw_rpi_display_hd44780()
{
    ObjectFactory<HwRpiDisplayHd44780> obj("hw.rpi.display.hd44780");
    obj.addAlias(sym_lcd1602_alias()->s_name);
    obj.addAlias(sym_lcd2004_alias()->s_name);

    obj.addMethod("clear", &HwRpiDisplayHd44780::m_clear);
    obj.addMethod("clear_line", &HwRpiDisplayHd44780::m_clear_line);
    obj.addMethod("backlight", &HwRpiDisplayHd44780::m_backlight);
    obj.addMethod("write", &HwRpiDisplayHd44780::m_write);
    obj.addMethod("char", &HwRpiDisplayHd44780::m_char);

    obj.addMethod("cursor_on", &HwRpiDisplayHd44780::m_cursor_on);
    obj.addMethod("cursor_blink", &HwRpiDisplayHd44780::m_cursor_blink);
    obj.addMethod("cursor_pos", &HwRpiDisplayHd44780::m_cursor_pos);

    obj.addMethod("move_cursor", &HwRpiDisplayHd44780::m_cursor_move);
    obj.addMethod("move_display", &HwRpiDisplayHd44780::m_display_move);

    obj.addMethod("font", &HwRpiDisplayHd44780::m_font);
}

#include "hw_display_lcd1602.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

HwI2cLcd1602::HwI2cLcd1602(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    lcd_ = ceammc_hw_lcd1602_new(
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, const char* msg) {
             LIB_ERR << msg;
         } });
}

HwI2cLcd1602::~HwI2cLcd1602()
{
    ceammc_hw_lcd1602_free(lcd_);
}

bool HwI2cLcd1602::notify(int code)
{
    return true;
}

void HwI2cLcd1602::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_clear(lcd_);
}

void HwI2cLcd1602::m_backlight(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_backlight(lcd_, lv.boolAt(0, false));
}

void HwI2cLcd1602::m_write(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_write_text(lcd_, to_string(lv).c_str());
}

void HwI2cLcd1602::m_cursor_on(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_cursor_on(lcd_, lv.boolAt(0, false));
}

void HwI2cLcd1602::m_cursor_blink(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_cursor_blink(lcd_, lv.boolAt(0, false));
}

void HwI2cLcd1602::m_cursor_pos(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_lcd1602_cursor_pos(lcd_, lv.intAt(0, 0), lv.intAt(1, 0));
}

void setup_hw_i2c_lcd1602()
{
    ObjectFactory<HwI2cLcd1602> obj("hw.i2c.lcd1602");
    obj.addMethod("clear", &HwI2cLcd1602::m_clear);
    obj.addMethod("backlight", &HwI2cLcd1602::m_backlight);
    obj.addMethod("write", &HwI2cLcd1602::m_write);

    obj.addMethod("cursor_on", &HwI2cLcd1602::m_cursor_on);
    obj.addMethod("cursor_blink", &HwI2cLcd1602::m_cursor_blink);
    obj.addMethod("cursor_pos", &HwI2cLcd1602::m_cursor_pos);
}

#ifndef HW_DISPLAY_LCD1602_H
#define HW_DISPLAY_LCD1602_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwI2cLcd1602 : public DispatchedObject<BaseObject> {
    ceammc_hw_lcd1602* lcd_ { nullptr };
    IntProperty* addr_ { nullptr };

public:
    HwI2cLcd1602(const PdArgs& args);
    ~HwI2cLcd1602();

    void initDone() final;
    bool notify(int code) final;

    //    void onBang() final;
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_backlight(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

    void m_cursor_on(t_symbol* s, const AtomListView& lv);
    void m_cursor_blink(t_symbol* s, const AtomListView& lv);
    void m_cursor_pos(t_symbol* s, const AtomListView& lv);
};

void setup_hw_i2c_lcd1602();

#endif // HW_DISPLAY_LCD1602_H

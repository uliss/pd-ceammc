#ifndef HW_RPI_DISPLAY_LCD1602_H
#define HW_RPI_DISPLAY_LCD1602_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiDisplayLcd1602 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_lcd1602* lcd_ { nullptr };
    I2cBusProperty* i2c_bus_ { nullptr };
    IntProperty* i2c_addr_ { nullptr };
    IntEnumProperty* rows_ { nullptr };

public:
    explicit HwRpiDisplayLcd1602(const PdArgs& args);
    ~HwRpiDisplayLcd1602();

    void initDone() final;
    bool notify(int code) final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_backlight(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

    void m_cursor_on(t_symbol* s, const AtomListView& lv);
    void m_cursor_blink(t_symbol* s, const AtomListView& lv);
    void m_cursor_pos(t_symbol* s, const AtomListView& lv);

    void m_font(t_symbol* s, const AtomListView& lv);

    void m_cursor_move(t_symbol* s, const AtomListView& lv);
    void m_display_move(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_display_lcd1602();

#endif // HW_RPI_DISPLAY_LCD1602_H

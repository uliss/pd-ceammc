#ifndef HW_RPI_DISPLAY_HD44780_H
#define HW_RPI_DISPLAY_HD44780_H

#ifdef WITH_RPI

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiDisplayHd44780 : public HwRpiDevice<ceammc_hw_hd44780> {
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };
    IntEnumProperty* rows_ { nullptr };

public:
    explicit HwRpiDisplayHd44780(const PdArgs& args);

    void initDone() final;
    bool notify(int code) final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_clear_line(t_symbol* s, const AtomListView& lv);
    void m_backlight(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);
    void m_char(t_symbol* s, const AtomListView& lv);

    void m_cursor_on(t_symbol* s, const AtomListView& lv);
    void m_cursor_blink(t_symbol* s, const AtomListView& lv);
    void m_cursor_pos(t_symbol* s, const AtomListView& lv);

    void m_font(t_symbol* s, const AtomListView& lv);

    void m_cursor_move(t_symbol* s, const AtomListView& lv);
    void m_display_move(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_display_hd44780();

#else

#include "ceammc_stub.h"

CONTROL_OBJECT_STUB(HwRpiDisplayHd44780, 1, 1, "compiled without RPi support");
OBJECT_STUB_SETUP(HwRpiDisplayHd44780, hw_rpi_display_hd44780, "hw.rpi.display.hd44780");

#endif

#endif // HW_RPI_DISPLAY_HD44780_H

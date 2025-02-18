#ifndef HW_SPI_MAX7219_H
#define HW_SPI_MAX7219_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwSpiMax7219 : public DispatchedObject<BaseObject> {
    ceammc_hw_max7219* mx_ { nullptr };
//    IntProperty* addr_ { nullptr };
//    IntEnumProperty* rows_ { nullptr };

public:
    HwSpiMax7219(const PdArgs& args);
    ~HwSpiMax7219();

//    void initDone() final;
    bool notify(int code) final;

    void m_intensity(t_symbol* s, const AtomListView& lv);
    void m_power(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

//    void m_clear(t_symbol* s, const AtomListView& lv);
//    void m_backlight(t_symbol* s, const AtomListView& lv);
//    void m_write(t_symbol* s, const AtomListView& lv);

//    void m_cursor_on(t_symbol* s, const AtomListView& lv);
//    void m_cursor_blink(t_symbol* s, const AtomListView& lv);
//    void m_cursor_pos(t_symbol* s, const AtomListView& lv);

//    void m_cursor_move(t_symbol* s, const AtomListView& lv);
//    void m_display_move(t_symbol* s, const AtomListView& lv);
};

void setup_hw_spi_max7219();

#endif // HW_SPI_MAX7219_H

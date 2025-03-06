#ifndef HW_RPI_DISPLAY_SSD1306_H
#define HW_RPI_DISPLAY_SSD1306_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiDisplaySsd1306 : public DispatchedObject<BaseObject> {
    ceammc_hw_display_ssd1306* display_ { nullptr };
    SymbolEnumProperty* mode_ = { nullptr };
    ListProperty* i2c_ = { nullptr };
    ListProperty* spi_ = { nullptr };

public:
    HwRpiDisplaySsd1306(const PdArgs& args);
    ~HwRpiDisplaySsd1306();

    void initDone() final;
    bool notify(int code) final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_invert(t_symbol* s, const AtomListView& lv);
    void m_mirror(t_symbol* s, const AtomListView& lv);
    void m_text(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_display_ssd1306();
#endif // HW_RPI_DISPLAY_SSD1306_H

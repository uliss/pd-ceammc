#ifndef HW_RPI_DISPLAY_SSD1306_H
#define HW_RPI_DISPLAY_SSD1306_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiDisplaySsd1306 : public DispatchedObject<BaseObject> {
    ceammc_hw_display_ssd1306* display_ { nullptr };
    //    IntProperty* chan_ { nullptr };

public:
    HwRpiDisplaySsd1306(const PdArgs& args);
    ~HwRpiDisplaySsd1306();

    void initDone() final;
    bool notify(int code) final;

    void m_text(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_display_ssd1306();
#endif // HW_RPI_DISPLAY_SSD1306_H

#ifndef HW_SPI_WS2812_H
#define HW_SPI_WS2812_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwSpiWs2812 : public DispatchedObject<BaseObject> {
    ceammc_hw_spi_ws2812* ws_ { nullptr };
    IntProperty* spi_ { nullptr };
    IntProperty* cs_ { nullptr };
    IntProperty* size_ { nullptr };
    BoolProperty* clear_on_exit_ { nullptr };

public:
    HwSpiWs2812(const PdArgs& args);
    ~HwSpiWs2812();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_brightness(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_fx(t_symbol* s, const AtomListView& lv);
    void m_rotate(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
    void m_set_range(t_symbol* s, const AtomListView& lv);
    void m_set_slice(t_symbol* s, const AtomListView& lv);
};

void setup_hw_spi_ws2812();

#endif // HW_SPI_WS2812_H

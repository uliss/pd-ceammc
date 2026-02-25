#ifndef HW_RPI_SPI_WS2812_H
#define HW_RPI_SPI_WS2812_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwSpiWs2812 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_spi_ws2812* ws_ { nullptr };
    SpiBusProperty* spi_ { nullptr };
    SpiCsPinProperty* cs_ { nullptr };
    IntProperty* size_ { nullptr };
    BoolProperty* clear_on_exit_ { nullptr };

public:
    explicit HwSpiWs2812(const PdArgs& args);
    ~HwSpiWs2812();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_brightness(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_fx(t_symbol* s, const AtomListView& lv);
    void m_fx_next(t_symbol* s, const AtomListView& lv);
    void m_rotate(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
    void m_set_range(t_symbol* s, const AtomListView& lv);
    void m_set_slice(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_spi_ws2812();

#endif // HW_RPI_SPI_WS2812_H

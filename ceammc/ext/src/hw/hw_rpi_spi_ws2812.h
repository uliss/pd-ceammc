#ifndef HW_RPI_SPI_WS2812_H
#define HW_RPI_SPI_WS2812_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwSpiWs2812 : public HwRpiDevice<ceammc_hw_spi_ws2812> {
    SpiBusProperty* spi_ { nullptr };
    SpiCsPinProperty* cs_ { nullptr };
    IntProperty* size_ { nullptr };
    BoolProperty* clear_on_exit_ { nullptr };

public:
    explicit HwSpiWs2812(const PdArgs& args);

    bool notify(int code) final;

    void onBang() final;

    void m_brightness(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_fill_bits(t_symbol* s, const AtomListView& lv);
    void m_fill_pixels(t_symbol* s, const AtomListView& lv);
    void m_flush(t_symbol* s, const AtomListView& lv);
    void m_fx(t_symbol* s, const AtomListView& lv);
    void m_rotate(t_symbol* s, const AtomListView& lv);
    void m_set_pixel(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_spi_ws2812();

#endif // HW_RPI_SPI_WS2812_H

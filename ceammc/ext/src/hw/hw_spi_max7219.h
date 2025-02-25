#ifndef HW_SPI_MAX7219_H
#define HW_SPI_MAX7219_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwSpiMax7219 : public DispatchedObject<BaseObject> {
    ceammc_hw_max7219* mx_ { nullptr };
    IntProperty* displays_ { nullptr };
    IntProperty* spi_ { nullptr };
    IntProperty* cs_ { nullptr };

public:
    HwSpiMax7219(const PdArgs& args);
    ~HwSpiMax7219();

    void initDone() final;
    bool notify(int code) final;

    void m_intensity(t_symbol* s, const AtomListView& lv);

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_power(t_symbol* s, const AtomListView& lv);
    void m_test(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

private:
    void writeBits(t_symbol* s, const AtomListView& lv);
    void writeBytes(t_symbol* s, const AtomListView& lv);
    void writeFloat(t_symbol* s, const AtomListView& lv);
    void writeHex(t_symbol* s, const AtomListView& lv);
    void writeInt(t_symbol* s, const AtomListView& lv);
    void writeMatrix(t_symbol* s, const AtomListView& lv);
    void writeReg(t_symbol* s, const AtomListView& lv);
    void writeStr(t_symbol* s, const AtomListView& lv);
};

void setup_hw_spi_max7219();

#endif // HW_SPI_MAX7219_H

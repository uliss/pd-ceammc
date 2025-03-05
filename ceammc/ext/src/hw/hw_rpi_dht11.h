#ifndef HW_RPI_DHT11_H
#define HW_RPI_DHT11_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiDht11 : public DispatchedObject<BaseObject> {
    ceammc_hw_gpio_dht11* dht_ { nullptr };
    IntProperty* pin_ { nullptr };

public:
    explicit HwRpiDht11(const PdArgs& args);
    ~HwRpiDht11();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_dht11();

#endif // HW_RPI_DHT11_H

#ifndef HW_RPI_ADC_ADS1115_H
#define HW_RPI_ADC_ADS1115_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiAdcAds1115 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_i2c_ads1115* adc_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };
    IntProperty* i2c_bus_ { nullptr };

public:
    explicit HwRpiAdcAds1115(const PdArgs& args);
    ~HwRpiAdcAds1115();

    void initDone() final;
    bool notify(int code) final;

    void m_poll(t_symbol* s, const AtomListView& lv);
    void m_measure(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_adc_ads1115();

#endif // HW_RPI_ADC_ADS1115_H

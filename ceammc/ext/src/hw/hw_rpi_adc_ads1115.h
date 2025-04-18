#ifndef HW_RPI_ADC_ADS1115_H
#define HW_RPI_ADC_ADS1115_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiAdcAds1115 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_i2c_ads1115* adc_ { nullptr };

    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };

    SymbolEnumProperty* mode_ { nullptr };
    SymbolFloatEnumProperty* fsr_ { nullptr };
    FloatProperty* poll_time_ { nullptr };
    // normalization
    BoolProperty* normalize_ { nullptr };
    FloatProperty* out_min_ { nullptr };
    FloatProperty* out_max_ { nullptr };
    FloatProperty* in_min_ { nullptr };
    FloatProperty* in_max_ { nullptr };
    //
    t_symbol* sym_channel_ { nullptr };

public:
    explicit HwRpiAdcAds1115(const PdArgs& args);
    ~HwRpiAdcAds1115();

    void initDone() final;
    bool notify(int code) final;

    void m_poll(t_symbol* s, const AtomListView& lv);
    void m_measure(t_symbol* s, const AtomListView& lv);

private:
    t_float normalizeValue(int16_t value) const;
    void outputValue(std::uint8_t ch, std::int16_t value);
};

void setup_hw_rpi_adc_ads1115();

#endif // HW_RPI_ADC_ADS1115_H

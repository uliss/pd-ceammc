#ifndef HW_SENSOR_SR04_H
#define HW_SENSOR_SR04_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

using HwSR04Base = DispatchedObject<BaseObject>;

class HwSensorSR04 : public HwSR04Base {
    ceammc_hw_gpio_sr04* sr04_ { nullptr };
    IntProperty* trigger_pin_ { nullptr };
    IntProperty* echo_pin_ { nullptr };
    IntProperty* poll_interval_ { nullptr };

public:
    HwSensorSR04(const PdArgs& args);
    ~HwSensorSR04();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;
    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_sensor_sr04();

#endif // HW_SENSOR_SR04_H

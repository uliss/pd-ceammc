#ifndef HW_ARDUINO_H
#define HW_ARDUINO_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <memory>

namespace ceammc {
namespace hw {
    class Arduino;
}
}

using namespace ceammc;

class ArduinoExternal : public BaseObject {
    std::shared_ptr<ceammc::hw::Arduino> arduino_;
    SymbolProperty* port_;
    SymbolProperty* on_connect_;
    SymbolProperty* serial_;
    IntProperty* vid_;
    IntProperty* pid_;
    IntEnumProperty* baud_rate_;
    FlagProperty* reconnect_;
    t_clock* read_clock_;
    bool ready_;

public:
    ArduinoExternal(const PdArgs& args);
    ~ArduinoExternal();
    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;
    void initDone() override;

    void tick();

public:
    void m_connect(t_symbol*, const AtomListView& args);
    void m_disconnect(t_symbol*, const AtomListView&);

private:
    void onConnect();
    void processMessages();
    void initProperties();
};

void hw_setup_arduino();

#endif // HW_ARDUINO_H

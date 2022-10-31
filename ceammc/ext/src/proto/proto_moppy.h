#ifndef PROTO_MOPPY_H
#define PROTO_MOPPY_H

#include <cstdint>

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class ProtoMoppy : public BaseObject {
    IntEnumProperty* vers_;
    IntProperty* device_addr_;
    IntProperty* floppy_addr_;

public:
    ProtoMoppy(const PdArgs& args);

    void m_note_on(t_symbol* s, const AtomListView& lv);
    void m_note_off(t_symbol* s, const AtomListView& lv);
    void m_ping(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_bendf(t_symbol* s, const AtomListView& lv);

private:
    static bool checkArgBytesN(uint8_t n, const AtomListView& lv);
};

void setup_proto_moppy();

#endif // PROTO_MOPPY_H

#ifndef PROTO_FEELWORLD_H
#define PROTO_FEELWORLD_H

#include "ceammc_object.h"
using namespace ceammc;

class ProtoFeelworld : public BaseObject {
    int msg_id_;

public:
    ProtoFeelworld(const PdArgs& args);

    void m_input(t_symbol* s, const AtomListView& lv);
};

void setup_proto_feelworld();

#endif // PROTO_FEELWORLD_H

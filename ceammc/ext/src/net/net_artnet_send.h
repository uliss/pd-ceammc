#ifndef CEAMMC_NET_NET_ARTNET_SEND_H
#define CEAMMC_NET_NET_ARTNET_SEND_H

#include <memory>

#include "ceammc_object.h"
using namespace ceammc;

namespace ceammc {
namespace net {

    class NetArtnetSend : public BaseObject {
        IntProperty* universe_;
        IntProperty* offset_;

    public:
        NetArtnetSend(const PdArgs& args);

        void m_dmx(t_symbol* s, const AtomListView& lv);
        void m_dmx_set(t_symbol* s, const AtomListView& lv);
        void m_dmx_fill(t_symbol* s, const AtomListView& lv);
        void m_poll(t_symbol* s, const AtomListView& lv);
    };

} // namespace net
} // namespace ceammc

void setup_net_artnet_send();

#endif // CEAMMC_NET_NET_ARTNET_SEND_H

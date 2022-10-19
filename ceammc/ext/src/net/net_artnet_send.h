#ifndef CEAMMC_NET_NET_ARTNET_SEND_H
#define CEAMMC_NET_NET_ARTNET_SEND_H

#include <memory>

#include "ceammc_object.h"
using namespace ceammc;

namespace ceammc {
namespace net {

    class NetArtnetSend : public BaseObject {
        IntProperty* universe_;

    public:
        NetArtnetSend(const PdArgs& args);
        void initDone() override;

        void m_dmx(t_symbol* s, const AtomListView& lv);
    };

} // namespace net
} // namespace ceammc

void setup_net_artnet_send();

#endif // CEAMMC_NET_NET_ARTNET_SEND_H

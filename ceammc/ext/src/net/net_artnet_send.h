#ifndef CEAMMC_NET_NET_ARTNET_SEND_H
#define CEAMMC_NET_NET_ARTNET_SEND_H

#include <cstdint>
#include <memory>

#include "ceammc_clock.h"
#include "ceammc_object.h"
using namespace ceammc;

namespace ceammc {
namespace net {

    class ArtnetSendWorker;
    using SharedArtnetSendWorker = std::shared_ptr<ArtnetSendWorker>;

    constexpr int MAX_DMX_CHANNELS = 512;

    enum ArtNetCmdType : std::uint8_t {
        ARTNET_CMD_NONE = 0,
        ARTNET_CMD_SEND_DMX,
        ARTNET_CMD_POLL,
    };

    struct ArtNetCommand {
        std::array<uint8_t, MAX_DMX_CHANNELS> data;
        ArtNetCmdType cmd;
        std::uint8_t port;

        ArtNetCommand();
        ArtNetCommand(ArtNetCmdType acmd, std::uint8_t aport);
        void set(ArtNetCmdType acmd, std::uint8_t aport);
        void setIp(const char* ip);
    };

    class NetArtnetSend : public BaseObject {
        IntProperty* universe_ { nullptr };
        IntProperty* subnet_ { nullptr };
        IntProperty* offset_ { nullptr };
        BoolProperty* sync_ { nullptr };
        SymbolProperty* ip_ { nullptr };
        FloatProperty* send_rate_ { nullptr };
        ArtNetCommand packet_;
        SharedArtnetSendWorker worker_;
        ClockLambdaFunction send_cb_;
        bool needs_update_ { false };

    public:
        NetArtnetSend(const PdArgs& args);
        void initDone() final;

        void onBang() override;
        void dump() const final;

        void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) final;

        void m_blackout(t_symbol* s, const AtomListView& lv);
        void m_dmx(t_symbol* s, const AtomListView& lv);
        void m_dmx_set(t_symbol* s, const AtomListView& lv);
        void m_dmx_rgb(t_symbol* s, const AtomListView& lv);
        void m_dmx_fill(t_symbol* s, const AtomListView& lv);
        void m_poll(t_symbol* s, const AtomListView& lv);

    private:
        void initPacket(ArtNetCmdType t);
    };

} // namespace net
} // namespace ceammc

void setup_net_artnet_send();

#endif // CEAMMC_NET_NET_ARTNET_SEND_H

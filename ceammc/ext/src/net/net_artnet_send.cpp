#include "net_artnet_send.h"
#include "ceammc_factory.h"
#include "ceammc_thread.h"

#include "artnet/artnet.h"
#include "fmt/format.h"
#include "readerwriterqueue.h"

#include <future>

namespace {

constexpr int MAX_DMX_CHANNELS = 512;

#define ARTNET_PREFIX "[artnet] "

enum ArtNetCmd : std::uint8_t {
    ARTNET_CMD_NONE = 0,
    ARTNET_CMD_SEND_DMX,
    ARTNET_CMD_POLL,
};

struct ArtNetCommand {
    ArtNetCmd cmd;
    std::uint8_t port;

    ArtNetCommand()
        : cmd(ARTNET_CMD_NONE)
        , port(0)
    {
    }

    ArtNetCommand(ArtNetCmd acmd, std::uint8_t aport)
        : cmd(acmd)
        , port(aport)
    {
    }
};

using ArtnetPtr = std::unique_ptr<void, int (*)(void*)>;

ArtnetPtr make_artnet(const char* ip)
{
    return ArtnetPtr(artnet_new(ip, 1), &artnet_destroy);
}

struct ReplyContext {
    int nodes_found;
};

void print_node_config(artnet_node_entry ne)
{
    post("--------- %d.%d.%d.%d -------------\n", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]);
    post("Short Name:   %s\n", ne->shortname);
    post("Long Name:    %s\n", ne->longname);
    post("Node Report:  %s\n", ne->nodereport);
    post("Subnet:       0x%02x\n", ne->sub);
    post("Numb Ports:   %d\n", ne->numbports);
    post("Input Addrs:  0x%02x, 0x%02x, 0x%02x, 0x%02x\n", ne->swin[0], ne->swin[1], ne->swin[2], ne->swin[3]);
    post("Output Addrs: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n", ne->swout[0], ne->swout[1], ne->swout[2], ne->swout[3]);
    post("----------------------------------\n");
}

int reply_handler(artnet_node n, void* pp, void* d)
{
    auto nl = artnet_get_nl(n);
    auto ctx = static_cast<ReplyContext*>(d);

    if (ctx->nodes_found == artnet_nl_get_length(nl)) {
        // this is not a new node, just a previously discovered one sending
        // another reply
        return 0;
    } else if (ctx->nodes_found == 0) {
        // first node found
        ctx->nodes_found++;
        print_node_config(artnet_nl_first(nl));
    } else {
        // new node
        ctx->nodes_found++;
        print_node_config(artnet_nl_next(nl));
    }

    return 0;
}

class ArtnetSendWorker {
    using Pipe = moodycamel::ReaderWriterQueue<ArtNetCommand, 64>;

    std::array<uint8_t, MAX_DMX_CHANNELS> dmx_;

    ArtnetSendWorker()
        : pipe_(64)
        , quit_(false)
        , node_(nullptr, nullptr)
    {
        dmx_.fill(0);

        LIB_LOG << ARTNET_PREFIX "launch ArtNet sender worker process";

        node_ = make_artnet(nullptr);
        if (!node_) {
            LIB_ERR << "can't create artnet node";
            return;
        }

        artnet_set_short_name(node_.get(), "pd-ArtNet");
        artnet_set_long_name(node_.get(), "PureData ceammc ArtNet node");
        artnet_set_node_type(node_.get(), ARTNET_SRV);

        artnet_set_port_type(node_.get(), 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
        artnet_set_port_addr(node_.get(), 0, ARTNET_INPUT_PORT, 0x00);
        artnet_set_port_type(node_.get(), 1, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
        artnet_set_port_addr(node_.get(), 1, ARTNET_INPUT_PORT, 0x01);
        artnet_set_port_type(node_.get(), 2, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
        artnet_set_port_addr(node_.get(), 2, ARTNET_INPUT_PORT, 0x02);
        artnet_set_port_type(node_.get(), 3, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
        artnet_set_port_addr(node_.get(), 3, ARTNET_INPUT_PORT, 0x03);

        artnet_set_bcast_limit(node_.get(), 0);

        future_ = std::async(
            std::launch::async,
            [this]() {
                ReplyContext ctx { 0 };
                // set poll reply handler
                artnet_set_handler(node_.get(), ARTNET_REPLY_HANDLER, reply_handler, &ctx);

                auto rc = artnet_start(node_.get());
                if (rc != 0) {
                    logger_.error(fmt::format("[artnet] can't start artnet: '{}'", artnet_strerror()));
                    return false;
                }

                fd_set rset;
                auto sd = artnet_get_sd(node_.get());

                while (!quit_) {
                    try {
                        ArtNetCommand event;
                        while (pipe_.try_dequeue(event)) {
                            if (quit_) {
                                artnet_stop(node_.get());
                                return true;
                            }

                            switch (event.cmd) {
                            case ARTNET_CMD_SEND_DMX: {
                                logger_.debug(fmt::format(ARTNET_PREFIX "send DMX to universe [{}]", event.port));
                                auto rc = artnet_send_dmx(node_.get(), event.port, dmx_.size(), dmx_.data());
                                if (rc != 0) {
                                    logger_.error(fmt::format(ARTNET_PREFIX "send error: '{}'", artnet_strerror()));
                                }
                            } break;
                            case ARTNET_CMD_POLL: {
                                if (artnet_send_poll(node_.get(), nullptr, ARTNET_TTM_DEFAULT) != ARTNET_EOK) {
                                    logger_.error(ARTNET_PREFIX "send poll failed");
                                }
                            } break;
                            default:
                                break;
                            }
                        }

                        FD_ZERO(&rset);
                        FD_SET(sd, &rset);

                        struct timeval tv;
                        tv.tv_usec = 0;
                        tv.tv_sec = 0;

                        switch (select(sd + 1, &rset, NULL, NULL, &tv)) {
                        case 0:
                            // timeout
                            break;
                        case -1:
                            logger_.error(ARTNET_PREFIX "select error\n");
                            break;
                        default:
                            artnet_read(node_.get(), 0);
                            break;
                        }

                        notify_.waitFor(200);

                    } catch (std::exception& e) {
                        logger_.error(fmt::format(ARTNET_PREFIX "exception: {}", e.what()));
                    }
                }

                return true;
            });
    }

    ~ArtnetSendWorker()
    {
        quit_ = true;
        future_.get();
    }

    Pipe pipe_;
    std::future<bool> future_;
    std::atomic_bool quit_;
    ThreadNotify notify_;
    ThreadPdLogger logger_;
    ArtnetPtr node_;

public:
    static ArtnetSendWorker& instance()
    {
        static ArtnetSendWorker w;
        return w;
    }

    void fillDmx(uint8_t v)
    {
        dmx_.fill(v);
    }

    void setDmx(size_t c, uint8_t v)
    {
        if (c < dmx_.size())
            dmx_[c] = v;
    }

    bool add(ArtNetCommand cmd)
    {
        auto ok = pipe_.enqueue(cmd);
        if (ok)
            notify_.notifyOne();

        return ok;
    }
};
}

namespace ceammc {
namespace net {

    NetArtnetSend::NetArtnetSend(const PdArgs& args)
        : BaseObject(args)
        , universe_(nullptr)
    {
        universe_ = new IntProperty("@universe", 0);
        universe_->checkClosedRange(0, 4);
        addProperty(universe_);
    }

    void NetArtnetSend::initDone()
    {
    }

    void NetArtnetSend::m_dmx(t_symbol* s, const AtomListView& lv)
    {
        auto& dmx = ArtnetSendWorker::instance();

        for (size_t i = 0; i < lv.size(); i++) {
            dmx.setDmx(i, lv[i].asInt());
        }

        dmx.add(ArtNetCommand(ARTNET_CMD_SEND_DMX, universe_->value()));
    }

} // namespace net
} // namespace ceammc

void setup_net_artnet_send()
{
#ifdef WITH_ARTNET
    using namespace ceammc;
    ObjectFactory<net::NetArtnetSend> obj("net.artnet.send");
    obj.addAlias("artnet.send");
    obj.addAlias("artnet.s");

    obj.addMethod("dmx", &net::NetArtnetSend::m_dmx);

#endif
}

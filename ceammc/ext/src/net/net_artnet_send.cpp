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

class ArtnetSendWorker {
    using Pipe = moodycamel::ReaderWriterQueue<ArtNetCommand, 64>;

    std::array<uint8_t, MAX_DMX_CHANNELS> dmx_;

    ArtnetSendWorker()
        : pipe_(64)
        , quit_(false)
        , node_(nullptr, nullptr)
        , nodes_found_(0)
    {
        dmx_.fill(0);

        LIB_LOG << ARTNET_PREFIX "launch ArtNet sender worker process";

        node_ = make_artnet(nullptr);
        if (!node_) {
            LIB_ERR << ARTNET_PREFIX "can't create artnet node";
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
                if (artnet_set_handler(node_.get(), ARTNET_REPLY_HANDLER, pollReplyHandler, this) != 0) {
                    logger_.error(fmt::format(ARTNET_PREFIX "can't set reply handler: '{}'", artnet_strerror()));
                    return false;
                }

                if (artnet_start(node_.get()) != 0) {
                    logger_.error(fmt::format(ARTNET_PREFIX "can't start artnet: '{}'", artnet_strerror()));
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
                                nodes_found_ = 0;
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
    std::atomic_int nodes_found_;

public:
    static ArtnetSendWorker& instance()
    {
        static ArtnetSendWorker w;
        return w;
    }

    void dumpNodeConfig(artnet_node_entry ne)
    {
        logger_.post(fmt::format("--------- {}.{}.{}.{} -------------\n", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]));
        logger_.post(fmt::format("Short Name:   {}", ne->shortname));
        logger_.post(fmt::format("Long Name:    {}", ne->longname));
        logger_.post(fmt::format("Node Report:  {}", ne->nodereport));
        logger_.post(fmt::format("Subnet:       {:02x}", ne->sub));
        logger_.post(fmt::format("Numb Ports:   {}", ne->numbports));
        logger_.post(fmt::format("Input Addrs:  {:02x}, {:02x}, {:02x}, {:02x}", ne->swin[0], ne->swin[1], ne->swin[2], ne->swin[3]));
        logger_.post(fmt::format("Output Addrs: {:02x}, {:02x}, {:02x}, {:02x}", ne->swout[0], ne->swout[1], ne->swout[2], ne->swout[3]));
    }

    static int pollReplyHandler(artnet_node n, void* pp, void* x)
    {
        auto nl = artnet_get_nl(n);
        auto w = static_cast<ArtnetSendWorker*>(x);

        if (w->nodes_found_ == artnet_nl_get_length(nl)) {
            // this is not a new node, just a previously discovered one sending
            // another reply
            return 0;
        } else if (w->nodes_found_ == 0) {
            // first node found
            w->nodes_found_++;
            w->dumpNodeConfig(artnet_nl_first(nl));
        } else {
            // new node
            w->nodes_found_++;
            w->dumpNodeConfig(artnet_nl_next(nl));
        }

        return 0;
    }

public:
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
        , offset_(nullptr)
    {
        universe_ = new IntProperty("@universe", 0);
        universe_->checkClosedRange(0, 4);
        universe_->setArgIndex(0);
        addProperty(universe_);

        offset_ = new IntProperty("@offset", 0);
        offset_->checkClosedRange(0, MAX_DMX_CHANNELS - 1);
        addProperty(offset_);
    }

    void NetArtnetSend::m_dmx(t_symbol* s, const AtomListView& lv)
    {
        auto& dmx = ArtnetSendWorker::instance();
        auto N = std::min<size_t>(MAX_DMX_CHANNELS - offset_->value(), std::min<size_t>(lv.size(), MAX_DMX_CHANNELS));

        for (size_t i = 0; i < N; i++)
            dmx.setDmx(i + offset_->value(), lv[i].asInt());

        dmx.add(ArtNetCommand(ARTNET_CMD_SEND_DMX, universe_->value()));
    }

    void NetArtnetSend::m_dmx_set(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_INT, ARG_BYTE))
            return;

        auto& dmx = ArtnetSendWorker::instance();
        dmx.setDmx(lv[0].asInt() + offset_->value(), lv[1].asInt());
        dmx.add(ArtNetCommand(ARTNET_CMD_SEND_DMX, universe_->value()));
    }

    void NetArtnetSend::m_dmx_fill(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_BYTE))
            return;

        auto& dmx = ArtnetSendWorker::instance();
        dmx.fillDmx(lv[0].asInt());
        dmx.add(ArtNetCommand(ARTNET_CMD_SEND_DMX, universe_->value()));
    }

    void NetArtnetSend::m_poll(t_symbol* s, const AtomListView& lv)
    {
        ArtnetSendWorker::instance().add(ArtNetCommand(ARTNET_CMD_POLL, 0));
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
    obj.addMethod("dmx_set", &net::NetArtnetSend::m_dmx_set);
    obj.addMethod("dmx_fill", &net::NetArtnetSend::m_dmx_fill);
    obj.addMethod("poll", &net::NetArtnetSend::m_poll);

#endif
}

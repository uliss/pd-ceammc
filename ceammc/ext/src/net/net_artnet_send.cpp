#include "net_artnet_send.h"
#include "artnet/artnet.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "net_artnet_impl.h"
#include "readerwriterqueue.h"

#include <future>
#include <ifaddrs.h>

using namespace ceammc::net;

ArtNetCommand::ArtNetCommand()
    : cmd(ARTNET_CMD_NONE)
    , port(0)
{
}

ArtNetCommand::ArtNetCommand(ArtNetCmdType acmd, std::uint8_t aport)
    : cmd(acmd)
    , port(aport)
{
}

void ArtNetCommand::set(ArtNetCmdType acmd, std::uint8_t aport)
{
    cmd = acmd;
    port = aport;
}

void ArtNetCommand::setIp(const char* ip)
{
    if (!ip) {
        data[0] = 0;
    } else {
        strlcpy((char*)data.data(), ip, sizeof(data));
    }
}

namespace {

#define ARTNET_PREFIX "[artnet] "

using ArtnetPtr = std::unique_ptr<void, int (*)(void*)>;

ArtnetPtr make_artnet(const char* ip)
{
    return ArtnetPtr(artnet_new(ip, 1), &artnet_destroy);
}

}

namespace ceammc {
namespace net {

    class ArtnetSendWorker {
        using Pipe = moodycamel::ReaderWriterQueue<ArtNetCommand, 64>;

        ArtnetSendWorker(const char* ip)
            : pipe_(64)
            , quit_(false)
            , node_(nullptr, nullptr)
            , nodes_found_(0)
            , ip_hash_(crc32_hash(ip))
        {
            // treat empty string as nullptr
            node_ = make_artnet(ip[0] ? ip : nullptr);
            if (!node_) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't create artnet node: {}", ip);
                LIB_POST << fmt::format("available ifaces: {}", fmt::join(platform::net_ifaces_ip(), ", "));
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

            artnet_set_subnet_addr(node_.get(), 0);

            if (artnet_set_handler(node_.get(), ARTNET_REPLY_HANDLER, pollReplyHandler, this) != 0) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set reply handler: '{}'", artnet_strerror());
                return;
            }

            if (artnet_start(node_.get()) != 0) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't start artnet: '{}'", artnet_strerror());
                return;
            }

            LIB_DBG << fmt::format(ARTNET_PREFIX "create new Artnet sender: {}", artnet_node_addr(node_.get()));

            future_ = std::async(
                std::launch::async,
                [this]() {
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
                                    // logger_.debug(fmt::format(ARTNET_PREFIX "send DMX to universe [{}]", event.port));

                                    auto rc = artnet_send_dmx(node_.get(), event.port, event.data.size(), event.data.data());
                                    if (rc != 0)
                                        logger_.error(fmt::format(ARTNET_PREFIX "send error: '{}'", artnet_strerror()));

                                } break;
                                case ARTNET_CMD_POLL: {
                                    nodes_found_ = 0;
                                    const char* ip = event.data[0] ? reinterpret_cast<const char*>(&event.data.front()) : nullptr;

                                    if (artnet_send_poll(node_.get(), ip, ARTNET_TTM_DEFAULT) != ARTNET_EOK)
                                        logger_.error(ARTNET_PREFIX "send poll failed");

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

    public:
        ~ArtnetSendWorker()
        {
            quit_ = true;
            if (future_.valid())
                future_.get();
        }

    private:
        Pipe pipe_;
        std::future<bool> future_;
        std::atomic_bool quit_;
        ThreadNotify notify_;
        ThreadPdLogger logger_;
        ArtnetPtr node_;
        std::atomic_int nodes_found_;
        size_t ip_hash_;

        using WeakArtnetSendWorker = std::weak_ptr<ArtnetSendWorker>;
        static std::vector<WeakArtnetSendWorker> workers_;

    public:
        static SharedArtnetSendWorker create(t_symbol* ip)
        {
            auto ip_hash = crc32_hash(ip->s_name);
            for (auto& w : workers_) {
                if (!w.expired()) {
                    auto ptr = w.lock();
                    if (ptr && ptr->ip_hash_ == ip_hash)
                        return w.lock();
                }
            }

            SharedArtnetSendWorker new_worker(new ArtnetSendWorker(ip->s_name));
            workers_.emplace_back(new_worker);
            return new_worker;
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
        bool add(const ArtNetCommand& cmd)
        {
            auto ok = pipe_.enqueue(cmd);
            if (ok)
                notify_.notifyOne();

            return ok;
        }
    };

    std::vector<ArtnetSendWorker::WeakArtnetSendWorker> ArtnetSendWorker::workers_;

}
}

namespace ceammc {
namespace net {

    NetArtnetSend::NetArtnetSend(const PdArgs& args)
        : BaseObject(args)
        , send_cb_([this]() {
            onBang();
            send_cb_.delay(1000 / send_rate_->value());
        })
    {
        createOutlet();

        universe_ = new IntProperty("@universe", 0);
        universe_->checkClosedRange(0, 3);
        universe_->setArgIndex(0);
        addProperty(universe_);

        offset_ = new IntProperty("@offset", 0);
        offset_->checkClosedRange(0, MAX_DMX_CHANNELS - 1);
        addProperty(offset_);

        sync_ = new BoolProperty("@sync", true);
        addProperty(sync_);

        ip_ = new SymbolProperty("@ip", &s_, PropValueAccess::INITONLY);
        addProperty(ip_);

        send_rate_ = new FloatProperty("@rate", 44);
        send_rate_->setUnitsHz();
        send_rate_->checkClosedRange(1, 100);
        addProperty(send_rate_);
    }

    void NetArtnetSend::initDone()
    {
        worker_ = ArtnetSendWorker::create(ip_->value());
        send_cb_.exec();
    }

    void NetArtnetSend::onBang()
    {
        if (worker_ && needs_update_) {
            worker_->add(packet_);
            needs_update_ = false;
        }
    }

    void NetArtnetSend::m_dmx(t_symbol* s, const AtomListView& lv)
    {
        packet_.set(ARTNET_CMD_SEND_DMX, universe_->value());
        auto OFFSET = offset_->value();
        for (size_t i = 0; i < packet_.data.size(); i++) {
            if (i >= OFFSET && i < (lv.size() + OFFSET))
                packet_.data[i] = lv[i - OFFSET].asInt();
            else
                packet_.data[i] = 0;
        }

        needs_update_ = true;
    }

    void NetArtnetSend::m_dmx_set(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_INT, ARG_BYTE))
            return;

        auto idx = lv[0].asInt() + offset_->value();
        if (idx < 0 || idx >= packet_.data.size()) {
            METHOD_ERR(s) << fmt::format("invalid DMX channel: {}", idx);
            return;
        }

        packet_.set(ARTNET_CMD_SEND_DMX, universe_->value());
        packet_.data[idx] = lv[1].asInt();

        needs_update_ = true;
    }

    void NetArtnetSend::m_dmx_rgb(t_symbol* s, const AtomListView& lv)
    {
    }

    void NetArtnetSend::m_dmx_fill(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_BYTE))
            return;

        packet_.set(ARTNET_CMD_SEND_DMX, universe_->value());
        packet_.data.fill(lv[0].asInt());

        needs_update_ = true;
    }

    void NetArtnetSend::m_poll(t_symbol* s, const AtomListView& lv)
    {
        ArtNetCommand cmd(ARTNET_CMD_POLL, 0);
        if (lv.isSymbol())
            cmd.setIp(lv.asSymbol()->s_name);
        else
            cmd.setIp(nullptr);

        if (worker_)
            worker_->add(cmd);
    }

} // namespace net
} // namespace ceammc

void setup_net_artnet_send()
{
#ifdef WITH_ARTNET
    using namespace ceammc;
    ObjectFactory<net::NetArtnetSend> obj("net.artnet.send");
    obj.addAlias("artnet.send");

    obj.addMethod("dmx", &net::NetArtnetSend::m_dmx);
    obj.addMethod("dmx_set", &net::NetArtnetSend::m_dmx_set);
    obj.addMethod("dmx_fill", &net::NetArtnetSend::m_dmx_fill);
    obj.addMethod("poll", &net::NetArtnetSend::m_poll);

#endif
}

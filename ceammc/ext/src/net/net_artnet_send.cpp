#include "net_artnet_send.h"

#ifdef WITH_ARTNET
#include "artnet/artnet.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "net_artnet_impl.h"
#include "net_artnet_send_args.hpp"
#include "readerwriterqueue.h"

#include <future>

using namespace ceammc::net;

constexpr const char* DEFAULT_ARTNET_SHORT_NAME = "pd-ArtNet";
constexpr const char* DEFAULT_ARTNET_FULL_NAME = "PureData ceammc ArtNet node";
constexpr int DEFAULT_BCAST_LIMIT = 0;

ArtNetCommand::ArtNetCommand()
    : cmd(ARTNET_CMD_NONE)
    , port(0)
{
    data.fill(0);
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
        strncpy((char*)data.data(), ip, sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0';
    }
}

namespace {

#define ARTNET_PREFIX "[artnet] "

using ArtnetPtr = std::unique_ptr<void, int (*)(void*)>;

ArtnetPtr make_artnet(const char* ip)
{
    return ArtnetPtr(artnet_new(ip, 0), &artnet_destroy);
}

} // namespace

namespace ceammc {
namespace net {

    constexpr t_int MIN_DMX_CHAN = 1;
    constexpr t_int MAX_DMX_CHAN = 512;
    constexpr t_int MIN_DMX_VALUE = 0;
    constexpr t_int MAX_DMX_VALUE = 511;

    class ArtnetSendWorker {
        using Pipe = moodycamel::ReaderWriterQueue<ArtNetCommand, 64>;

    private:
        // runs in Pd thread
        /**
         * @brief ArtnetSendWorker
         */
        explicit ArtnetSendWorker(t_symbol* ip)
            : pipe_(64)
            , quit_(false)
            , node_(nullptr, nullptr)
            , nodes_found_(0)
            , ip_(ip)
        {
            // treat empty string as nullptr
            node_ = make_artnet(ip_->s_name[0] ? ip_->s_name : nullptr);
            if (!node_) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't create artnet node '{}': {}", ip_->s_name, artnet_strerror());
                LIB_POST << fmt::format("available ifaces: {}", fmt::join(platform::net_ifaces_ip(platform::ADDR_IPV4, true), ", "));
                return;
            }

            if (ARTNET_EOK != artnet_set_short_name(node_.get(), DEFAULT_ARTNET_SHORT_NAME)) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set short name '{}': {}", DEFAULT_ARTNET_SHORT_NAME, artnet_strerror());
                return;
            }

            if (ARTNET_EOK != artnet_set_long_name(node_.get(), DEFAULT_ARTNET_FULL_NAME)) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set long name '{}': {}", DEFAULT_ARTNET_FULL_NAME, artnet_strerror());
                return;
            }

            if (ARTNET_EOK != artnet_set_node_type(node_.get(), ARTNET_RAW)) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set node type: {}", artnet_strerror());
                return;
            }

            artnet_set_port_type(node_.get(), 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
            //            artnet_set_port_addr(node_.get(), 0, ARTNET_INPUT_PORT, port);
            //            artnet_set_port_type(node_.get(), 1, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
            //            artnet_set_port_addr(node_.get(), 1, ARTNET_INPUT_PORT, in_port_start + 1);
            //            artnet_set_port_type(node_.get(), 2, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
            //            artnet_set_port_addr(node_.get(), 2, ARTNET_INPUT_PORT, 0x02);
            //            artnet_set_port_type(node_.get(), 3, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
            //            artnet_set_port_addr(node_.get(), 3, ARTNET_INPUT_PORT, 0x03);

            if (ARTNET_EOK != artnet_set_bcast_limit(node_.get(), DEFAULT_BCAST_LIMIT)) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set broadcast limit: {}", DEFAULT_BCAST_LIMIT);
                return;
            }

            if (ARTNET_EOK != artnet_set_handler(node_.get(), ARTNET_REPLY_HANDLER, pollReplyHandler, this)) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't set reply handler: '{}'", artnet_strerror());
                return;
            }

            if (ARTNET_EOK != artnet_start(node_.get())) {
                LIB_ERR << fmt::format(ARTNET_PREFIX "can't start artnet: '{}'", artnet_strerror());
                return;
            }

            LIB_DBG << fmt::format(ARTNET_PREFIX "create new sender: {}", artnet_node_addr(node_.get()));

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
                                    if (ARTNET_EOK != artnet_raw_send_dmx(node_.get(), event.port, event.data.size(), event.data.data()))
                                        logger_.error(fmt::format(ARTNET_PREFIX "send error: '{}'", artnet_strerror()));

                                } break;
                                case ARTNET_CMD_POLL: {
                                    nodes_found_ = 0;
                                    const char* ip = event.data[0] ? reinterpret_cast<const char*>(&event.data.front()) : nullptr;

                                    if (ARTNET_EOK != artnet_send_poll(node_.get(), ip, ARTNET_TTM_DEFAULT))
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
        t_symbol* ip_;

        using WeakArtnetSendWorker = std::weak_ptr<ArtnetSendWorker>;
        using WorkerList = std::vector<WeakArtnetSendWorker>;
        static WorkerList workers_;

    public:
        static SharedArtnetSendWorker create(t_symbol* ip)
        {
            auto it = std::find_if(workers_.begin(), workers_.end(), [ip](const WeakArtnetSendWorker& w) {
                if (!w.expired())
                    return w.lock()->ip_ == ip;
                else
                    return false;
            });
            if (it != workers_.end())
                return it->lock();

            SharedArtnetSendWorker new_worker(new ArtnetSendWorker(ip));
            workers_.push_back(new_worker);
            return new_worker;
        }

        void dumpNodeConfig(artnet_node_entry ne)
        {
            logger_.post(fmt::format("IP:           {}.{}.{}.{}", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]));
            logger_.post(fmt::format("Short Name:   {}", (const char*)ne->shortname));
            logger_.post(fmt::format("Long Name:    {}", (const char*)ne->longname));
            logger_.post(fmt::format("Node Report:  {}", (const char*)ne->nodereport));
            logger_.post(fmt::format("Subnet:       {:02x}", ne->sub));
            logger_.post(fmt::format("Num Ports:    {}", ne->numbports));
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

    ArtnetSendWorker::WorkerList ArtnetSendWorker::workers_;
} // namespace net
} // namespace ceammc

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
        universe_->checkClosedRange(0, 15);
        universe_->setArgIndex(0);
        addProperty(universe_);

        subnet_ = new IntProperty("@subnet", 0);
        subnet_->checkClosedRange(0, 15);
        subnet_->setArgIndex(1);
        addProperty(subnet_);

        address_ = new IntProperty("@addr", 1);
        address_->checkClosedRange(1, MAX_DMX_CHANNELS);
        addProperty(address_);

        sync_ = new BoolProperty("@sync", false);
        sync_->setSuccessFn([this](Property* p) {
            if (sync_->value())
                send_cb_.unset();
            else
                send_cb_.delay(0);
        });
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

    void NetArtnetSend::dump() const
    {
        BaseObject::dump();
        for (size_t i = 0; i < packet_.data.size(); i += 4) {
            OBJ_POST << fmt::format("[{:03d}]: 0x{:02X}", i, packet_.data[i])
                     << ' ' << fmt::format("[{:03d}]: 0x{:02X}", i + 1, packet_.data[i + 1])
                     << ' ' << fmt::format("[{:03d}]: 0x{:02X}", i + 2, packet_.data[i + 2])
                     << ' ' << fmt::format("[{:03d}]: 0x{:02X}", i + 3, packet_.data[i + 3]);
        }
    }

    void NetArtnetSend::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
    {
        if (alt)
            m_blackout(&s_, {});
    }

    void NetArtnetSend::m_blackout(t_symbol* s, const AtomListView& lv)
    {
        initPacket(ARTNET_CMD_SEND_DMX);
        packet_.data.fill(0);
        needs_update_ = true;
    }

    void NetArtnetSend::m_dmx(t_symbol* s, const AtomListView& lv)
    {
        initPacket(ARTNET_CMD_SEND_DMX);

        for (size_t i = 0; i < lv.size(); i++) {
            auto chan = address_->value() + i;
            auto value = lv[i].asInt();
            if (!setDmxChannel(chan, value)) {
                METHOD_ERR(s) << fmt::format("invalid DMX[{}] = {}", chan, value);
                return;
            }
        }

        needs_update_ = true;
    }

    void NetArtnetSend::m_dmx_set(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_INT, ARG_BYTE))
            return;

        auto chan = lv[0].asInt() + address_->value();
        if (!checkDmxChannel(chan))
            return;

        initPacket(ARTNET_CMD_SEND_DMX);

        auto value = lv[1].asInt();
        if (!setDmxChannel(chan, value)) {
            METHOD_ERR(s) << fmt::format("invalid DMX[{}] = {}", chan, value);
            return;
        }

        needs_update_ = true;
    }

    /// @function "set DMX channels to RGB integer list color" {
    ///     #red   byte "RED"   {}
    ///     #green byte "GREEN" {}
    ///     #blue  byte "BLUE"  {}
    ///     @addr  ? "DMX address" {
    ///         #channel int "value" { default: 1, check: [1..510] }
    ///     }
    /// }
    void NetArtnetSend::m_rgb8(t_symbol* s, const AtomListView& lv)
    {
        m_rgb8_args args;
        if (!args.parse_args(lv, this))
            return;

        initPacket(ARTNET_CMD_SEND_DMX);

        auto chan = args.prop_addr.channel + address_->value();
        if (!checkDmxChannel(chan, 3))
            return;

        if (setDmxChannel(chan, args.red)
            && setDmxChannel(chan + 1, args.green)
            && setDmxChannel(chan + 2, args.blue)) {
            needs_update_ = true;
        }
    }

    void NetArtnetSend::m_dmx_fill(t_symbol* s, const AtomListView& lv)
    {
        if (!checkArgs(lv, ARG_BYTE))
            return;

        initPacket(ARTNET_CMD_SEND_DMX);
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

    void NetArtnetSend::initPacket(ArtNetCmdType t)
    {
        switch (t) {
        case ARTNET_CMD_SEND_DMX:
            packet_.set(ARTNET_CMD_SEND_DMX, universe_->value() | (subnet_->value() << 4));
            break;
        default:
            break;
        }
    }

    bool NetArtnetSend::checkDmxChannel(t_int chan, size_t len) const
    {
        if (chan < MIN_DMX_CHAN || (chan + len > MAX_DMX_CHAN + 1)) {
            OBJ_ERR << fmt::format("invalid DMX channel: {}, expected in {}..{} range", chan, MIN_DMX_CHAN, MAX_DMX_CHAN);
            return false;
        } else {
            return true;
        }
    }

    bool NetArtnetSend::setDmxChannel(t_int chan, t_int value)
    {
        if (chan < MIN_DMX_CHAN
            || chan > MAX_DMX_CHAN
            || value < MIN_DMX_VALUE
            || value > MAX_DMX_VALUE) {
            return false;
        } else {
            const size_t idx = value - 1;
            if (idx < packet_.data.size()) {
                packet_.data[idx] = value;
                return true;
            } else {
                return false;
            }
        }
    }

} // namespace net
} // namespace ceammc

void setup_net_artnet_send()
{
    using namespace ceammc;
    using namespace net;
    ObjectFactory<net::NetArtnetSend> obj("net.artnet.send");
    obj.addAlias("artnet.send");

    obj.useClick();

    obj.addMethod("blackout", &NetArtnetSend::m_blackout);
    obj.addMethod("dmx", &NetArtnetSend::m_dmx);
    obj.addMethod("dmx_fill", &NetArtnetSend::m_dmx_fill);
    obj.addMethod("dmx_set", &NetArtnetSend::m_dmx_set);
    obj.addMethod("rgb8", &NetArtnetSend::m_rgb8);
    obj.addMethod("poll", &NetArtnetSend::m_poll);
}

#endif

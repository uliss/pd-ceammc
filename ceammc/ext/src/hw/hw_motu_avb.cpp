#include "hw_motu_avb.h"
#include "ceammc_factory.h"

#include "fmt/core.h"
#include "httplib.h"

constexpr const char* KEY_REQUEST_TYPE = "reqtype";
constexpr const char* KEY_HOST = "host";
constexpr const char* KEY_HTTP_PORT = "http-port";
constexpr const char* KEY_DEVICE = "dev-id";
constexpr const char* KEY_PHANTOM_CHAN = "phantom-ch";
constexpr const char* KEY_PHANTOM_VAL = "phantom-val";

namespace {

enum RequestType {
    REQ_SYNC = 1,
    REQ_SET,
};

bool getRequestKey(const DataTypeDict& dict, const char* key, int& val)
{
    auto it = dict.find(key);
    if (it == dict.end() || it->second.size() != 1 || !it->second[0].isInteger())
        return false;

    val = static_cast<int>(it->second[0].asInt());
    return true;
}

bool getRequestKey(const DataTypeDict& dict, const char* key, std::string& val)
{
    auto it = dict.find(key);
    if (it == dict.end() || it->second.size() != 1 || !it->second[0].isSymbol())
        return false;

    val = it->second[0].asSymbol()->s_name;
    return true;
}

bool getRequestHostPort(const DataTypeDict& dict,
    RequestType& type,
    std::string& host,
    int& port,
    std::string& dev)
{
    int itype = 0;
    if (!getRequestKey(dict, KEY_REQUEST_TYPE, itype))
        return false;

    type = static_cast<RequestType>(itype);

    if (!getRequestKey(dict, KEY_HOST, host))
        return false;

    if (!getRequestKey(dict, KEY_HTTP_PORT, port))
        return false;

    if (!getRequestKey(dict, KEY_DEVICE, dev))
        return false;

    return true;
}

httplib::Client make_http_cli(const std::string& host, int http_port)
{
    httplib::Client cli(host, http_port);
    cli.set_connection_timeout(1);
    cli.set_tcp_nodelay(true);
    return cli;
}

}

HwMotuAvb::HwMotuAvb(const PdArgs& args)
    : MotuAvbBase(args)
    , device_(nullptr)
    , host_(nullptr)
    , port_(nullptr)
    , logger_()
{
    createOutlet();

    device_ = new SymbolProperty("@dev", &s_);
    device_->setArgIndex(0);
    addProperty(device_);

    host_ = new SymbolProperty("@host", gensym("localhost"));
    addProperty(host_);

    port_ = new IntProperty("@port", 1280);
    addProperty(port_);

    Dispatcher::instance().subscribe(this, subscriberId());
}

HwMotuAvb::Future HwMotuAvb::createTask()
{
    return std::async(std::launch::async, [this]() {
        DataTypeDict dict;
        RequestType req_type;
        std::string host;
        std::string dev_id;
        int http_port;

        try {

            while (inPipe().try_dequeue(dict)) {
                if (!getRequestHostPort(dict, req_type, host, http_port, dev_id)) {
                    logger_.error(fmt::format("invalid request params: {}", dict.toString()));
                    continue;
                }

                switch (req_type) {
                case REQ_SYNC: {
                    auto cli = make_http_cli(host, http_port);
                    auto url = fmt::format("/{}/datastore", dev_id);
                    auto res = cli.Get(url.c_str());
                    if (res) {
                        if (res->status == 200) {
                            DataTypeDict resp;
                            if (resp.fromJSON(res->body)) {
                                outPipe().enqueue(resp);
                                logger_.debug("HTTP ok");
                            } else {
                                logger_.error("JSON parse error");
                            }
                        } else {
                            logger_.error(fmt::format("http error status: '{}'", res->status));
                            logger_.debug(fmt::format("{}", res->body));
                            for (auto& kv : res->headers) {
                                logger_.debug(fmt::format("{}: {}", kv.first, kv.second));
                            }
                        }
                    } else {
                        logger_.error(fmt::format("http request error: '{}'", to_string(res.error())));
                    }
                } break;
                case REQ_SET: {
                    if (dict.contains(KEY_PHANTOM_CHAN) && dict.contains(KEY_PHANTOM_VAL)) {
                        auto cli = make_http_cli(host, http_port);
                        int ch = dict.at(KEY_PHANTOM_CHAN).intAt(0, 0);
                        int val = dict.at(KEY_PHANTOM_VAL).boolAt(0, false);
                        auto url = fmt::format("/{}/datastore/ext/ibank/0/ch/{}/48V", dev_id, ch);
                        auto res = cli.Patch(url.c_str(),
                            fmt::format("json={{\"value\":{}}}", val).c_str(),
                            "application/x-www-form-urlencoded");

                        logger_.debug(fmt::format("url: {}", url));

                        if (res) {
                            if (res->status != 200 && res->status != 204) {
                                logger_.error(fmt::format("http error status: '{}'", res->status));
                                logger_.debug(fmt::format("{}", res->body));

                                for (auto& kv : res->headers) {
                                    logger_.debug(fmt::format("{}: {}", kv.first, kv.second));
                                }
                            }
                        } else {
                            logger_.error(fmt::format("http request error: '{}'", to_string(res.error())));
                        }
                    }
                } break;
                default:
                    logger_.error(fmt::format("unknown request type: {}", req_type));
                    break;
                }

                Dispatcher::instance().send({ subscriberId(), NOTIFY_UPDATE });
            }
        } catch (std::exception& e) {
            logger_.error(fmt::format("run thread exception: {}", e.what()));
        }
    });
}

void HwMotuAvb::processMessage(const DataTypeDict& msg)
{
}

void HwMotuAvb::m_sync(t_symbol* s, const AtomListView& lv)
{
    DataTypeDict dict;
    if (!fillRequestDict(s, dict))
        return;

    if (!inPipe().try_enqueue(std::move(dict))) {
        METHOD_ERR(s) << "can't make request";
        return;
    }

    if (!runTask()) {
        METHOD_ERR(s) << "can't run task";
        return;
    }
}

void HwMotuAvb::m_phantom(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_BOOL))
        return;

    DataTypeDict dict;
    if (!fillRequestDict(s, dict))
        return;

    dict.insert(KEY_PHANTOM_CHAN, lv.intAt(0, 0));
    dict.insert(KEY_PHANTOM_VAL, lv.boolAt(1, false));

    if (!inPipe().try_enqueue(std::move(dict))) {
        METHOD_ERR(s) << "can't make request";
        return;
    }

    if (!runTask()) {
        METHOD_ERR(s) << "can't run task";
        return;
    }
}

bool HwMotuAvb::fillRequestDict(t_symbol* s, DataTypeDict& dict) const
{
    if (device_->value() == &s_) {
        METHOD_ERR(s) << "empty device id";
        return false;
    }

    dict.insert(KEY_REQUEST_TYPE, Atom(REQ_SET));
    dict.insert(KEY_HOST, host_->value());
    dict.insert(KEY_HTTP_PORT, port_->value());
    dict.insert(KEY_DEVICE, device_->value());
    return true;
}

void setup_hw_motu_avb()
{
    ObjectFactory<HwMotuAvb> obj("hw.motu.avb");
    obj.addMethod("sync", &HwMotuAvb::m_sync);

    obj.addMethod("phantom", &HwMotuAvb::m_phantom);
}

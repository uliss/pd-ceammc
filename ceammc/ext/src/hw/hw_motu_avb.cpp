#include "hw_motu_avb.h"
#include "ceammc_factory.h"

#include "fmt/core.h"
#include "httplib.h"

constexpr const char* KEY_REQUEST_TYPE = "reqtype";
constexpr const char* KEY_HOST = "host";
constexpr const char* KEY_HTTP_PORT = "http-port";
constexpr const char* KEY_DEVICE = "dev-id";
constexpr const char* KEY_PHANTOM = "phantom-ch";
constexpr const char* KEY_MIC_GAIN = "mic-gain";
constexpr const char* KEY_GUITAR_GAIN = "guitar-gain";
constexpr const char* KEY_INPUT_GAIN = "input-gain";
constexpr const char* KEY_MAIN_GAIN = "main-gain";
constexpr const char* KEY_PHONES_GAIN = "phones-gain";
constexpr const char* KEY_OUTPUT_GAIN = "output-gain";

constexpr const char* KEY_VALUE = "val";

namespace {

enum RequestType {
    REQ_SYNC = 1,
    REQ_SET,
};

const std::unordered_map<const char*, std::string> UrlMap = {
    { KEY_PHANTOM, "/{}/datastore/ext/ibank/0/ch/{}/48V" },
    { KEY_MIC_GAIN, "/{}/datastore/ext/ibank/0/ch/{}/trim" },
    { KEY_GUITAR_GAIN, "/{}/datastore/ext/ibank/1/ch/{}/trim" },
    { KEY_INPUT_GAIN, "/{}/datastore/ext/ibank/2/ch/{}/trim" },
    { KEY_MAIN_GAIN, "/{}/datastore/ext/obank/1/ch/0/stereoTrim" },
    { KEY_PHONES_GAIN, "/{}/datastore/ext/obank/0/ch/0/stereoTrim" },
    { KEY_OUTPUT_GAIN, "/{}/datastore/ext/obank/2/ch/{}/trim" },
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

bool setSingleValue(httplib::Client& cli,
    const std::string& device,
    const char* key,
    const DataTypeDict& dict,
    ThreadPdLogger& log)
{
    if (!dict.contains(key) || !dict.contains(KEY_VALUE))
        return false;

    auto it = UrlMap.find(key);
    if (it == UrlMap.end()) {
        log.error(fmt::format("key not found: '{}'", key));
        return false;
    }

    int ch = dict.at(key).intAt(0, 0);
    int val = dict.at(KEY_VALUE).intAt(0, 0);

    auto path = fmt::format(it->second, device, ch);
    auto json = fmt::format("json={{\"value\":{}}}", val);
    auto res = cli.Patch(path.c_str(), json.c_str(), "application/x-www-form-urlencoded");

    log.debug(fmt::format("url: {}, json: {}", path, json));

    if (res) {
        if (res->status != 200 && res->status != 204) {
            log.error(fmt::format("http error status: '{}'", res->status));
            return false;
        } else
            return true;
    } else {
        log.error(fmt::format("http request error: '{}'", to_string(res.error())));
        return false;
    }
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
                    auto cli = make_http_cli(host, http_port);

                    setSingleValue(cli, dev_id, KEY_PHANTOM, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_MIC_GAIN, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_GUITAR_GAIN, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_INPUT_GAIN, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_MAIN_GAIN, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_PHONES_GAIN, dict, logger_);
                    setSingleValue(cli, dev_id, KEY_OUTPUT_GAIN, dict, logger_);

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

bool HwMotuAvb::scheduleTask(t_symbol* s, DataTypeDict&& dict)
{
    if (!inPipe().try_enqueue(std::move(dict))) {
        METHOD_ERR(s) << "can't make request";
        return false;
    }

    if (!runTask()) {
        METHOD_ERR(s) << "can't run task";
        return false;
    }

    return true;
}

void HwMotuAvb::m_set_single(t_symbol* s, const char* key, int ch, int val, const AtomListView& lv)
{
    DataTypeDict dict;
    if (!fillRequestDict(s, dict, REQ_SET))
        return;

    dict.insert(key, ch);
    dict.insert(KEY_VALUE, val);

    scheduleTask(s, std::move(dict));
}

void HwMotuAvb::m_sync(t_symbol* s, const AtomListView& lv)
{
    DataTypeDict dict;
    if (!fillRequestDict(s, dict, REQ_SYNC))
        return;

    scheduleTask(s, std::move(dict));
}

void HwMotuAvb::m_phantom(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_BOOL))
        return;

    m_set_single(s, KEY_PHANTOM, lv.intAt(0, 0), lv.boolAt(1, false), lv);
}

void HwMotuAvb::m_mic_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_MIC_GAIN, lv.intAt(0, 0), lv.intAt(1, 0), lv);
}

void HwMotuAvb::m_guitar_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_GUITAR_GAIN, lv.intAt(0, 0), lv.intAt(1, 0), lv);
}

void HwMotuAvb::m_input_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_INPUT_GAIN, lv.intAt(0, 0), lv.intAt(1, 0), lv);
}

void HwMotuAvb::m_main_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT))
        return;

    m_set_single(s, KEY_MAIN_GAIN, 0, lv.intAt(0, 0), lv);
}

void HwMotuAvb::m_phones_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT))
        return;

    m_set_single(s, KEY_PHONES_GAIN, 0, lv.intAt(0, 0), lv);
}

void HwMotuAvb::m_output_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_OUTPUT_GAIN, lv.intAt(0, 0), lv.intAt(1, 0), lv);
}

bool HwMotuAvb::fillRequestDict(t_symbol* s, DataTypeDict& dict, int type) const
{
    if (device_->value() == &s_) {
        METHOD_ERR(s) << "empty device id";
        return false;
    }

    dict.insert(KEY_REQUEST_TYPE, Atom(type));
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
    obj.addMethod("mic_gain", &HwMotuAvb::m_mic_gain);
    obj.addMethod("guitar_gain", &HwMotuAvb::m_guitar_gain);
    obj.addMethod("input_gain", &HwMotuAvb::m_input_gain);
    obj.addMethod("main_gain", &HwMotuAvb::m_main_gain);
    obj.addMethod("phones_gain", &HwMotuAvb::m_phones_gain);
    obj.addMethod("output_gain", &HwMotuAvb::m_output_gain);
}

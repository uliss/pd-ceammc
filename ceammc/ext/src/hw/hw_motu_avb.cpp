#include "hw_motu_avb.h"
#include "ceammc_factory.h"

#include "fmt/core.h"
#include "httplib.h"

constexpr size_t KEY_MAX_LENGTH = 32;
constexpr const char* KEY_PHANTOM = "k:phantom-ch";
constexpr const char* KEY_MIC_GAIN = "k:mic-gain";
constexpr const char* KEY_MIC_PAD = "k:mic-pad";
constexpr const char* KEY_GUITAR_GAIN = "k:guitar-gain";
constexpr const char* KEY_INPUT_GAIN = "k:input-gain";
constexpr const char* KEY_MAIN_GAIN = "k:main-gain";
constexpr const char* KEY_PHONES_GAIN = "k:phones-gain";
constexpr const char* KEY_OUTPUT_GAIN = "k:output-gain";

constexpr int MOTU_DEFAULT_HTTP_PORT = 1280;

namespace {

enum RequestType {
    REQ_SYNC = 1,
    REQ_SET,
};

const std::unordered_map<const char*, std::string> UrlMap = {
    { KEY_PHANTOM, "/{}/datastore/ext/ibank/0/ch/{}/48V" },
    { KEY_MIC_GAIN, "/{}/datastore/ext/ibank/0/ch/{}/trim" },
    { KEY_MIC_PAD, "/{}/datastore/ext/ibank/0/ch/{}/pad" },
    { KEY_GUITAR_GAIN, "/{}/datastore/ext/ibank/1/ch/{}/trim" },
    { KEY_INPUT_GAIN, "/{}/datastore/ext/ibank/2/ch/{}/trim" },
    { KEY_MAIN_GAIN, "/{}/datastore/ext/obank/1/ch/0/stereoTrim" },
    { KEY_PHONES_GAIN, "/{}/datastore/ext/obank/0/ch/0/stereoTrim" },
    { KEY_OUTPUT_GAIN, "/{}/datastore/ext/obank/2/ch/{}/trim" },
};

void makeKeyChan(const char* key, char* dest, size_t max_size)
{
    strncpy(dest, key, max_size);
    dest[max_size - 1] = '\0';
    dest[0] = 'c';
}

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
    const MotuAvbRequest& req,
    ThreadPdLogger& log)
{
    if (!req.data.contains(key))
        return false;

    char key_chan[KEY_MAX_LENGTH];
    makeKeyChan(key, key_chan, KEY_MAX_LENGTH - 1);

    if (!req.data.contains(key_chan))
        return false;

    auto it = UrlMap.find(key);
    if (it == UrlMap.end()) {
        log.error(fmt::format("key not found: '{}'", key));
        return false;
    }

    auto chan = req.data.at(key_chan).intAt(0, 0);
    auto path_pattern = it->second;
    auto path = fmt::format(path_pattern, device, chan);

    int val = req.data.at(key).intAt(0, 0);
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
    , req_timer_([this]() {
        scheduleTask(req_set_);
    })
{
    createOutlet();

    device_ = new SymbolProperty("@dev", &s_);
    device_->setArgIndex(0);
    addProperty(device_);

    host_ = new SymbolProperty("@host", gensym("localhost"));
    addProperty(host_);

    port_ = new IntProperty("@port", MOTU_DEFAULT_HTTP_PORT);
    addProperty(port_);

    Dispatcher::instance().subscribe(this, subscriberId());
}

HwMotuAvb::Future HwMotuAvb::createTask()
{
    return std::async(std::launch::async, [this]() {
        MotuAvbRequest req;

        try {

            while (inPipe().try_dequeue(req)) {
                switch (req.type) {
                case REQ_SYNC: {
                    auto cli = make_http_cli(req.host, req.port);
                    auto url = fmt::format("/{}/datastore", req.device);
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
                    auto cli = make_http_cli(req.host, req.port);

                    setSingleValue(cli, req.device, KEY_PHANTOM, req, logger_);
                    setSingleValue(cli, req.device, KEY_MIC_GAIN, req, logger_);
                    setSingleValue(cli, req.device, KEY_MIC_PAD, req, logger_);
                    setSingleValue(cli, req.device, KEY_GUITAR_GAIN, req, logger_);
                    setSingleValue(cli, req.device, KEY_INPUT_GAIN, req, logger_);
                    setSingleValue(cli, req.device, KEY_MAIN_GAIN, req, logger_);
                    setSingleValue(cli, req.device, KEY_PHONES_GAIN, req, logger_);
                    setSingleValue(cli, req.device, KEY_OUTPUT_GAIN, req, logger_);

                } break;
                default:
                    logger_.error(fmt::format("unknown request type: {}", req.type));
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
    req_set_.data.clear();
}

void HwMotuAvb::scheduleTask(const MotuAvbRequest& req)
{
    if (!inPipe().enqueue(req)) {
        OBJ_ERR << "can't make request: command queue is full";
        return;
    }

    if (taskState() != TASK_RUNNING)
        runTask();
}

void HwMotuAvb::m_set_single(t_symbol* s, const char* key, int ch, int val, const AtomListView& lv)
{
    if (!updateRequest(s, req_set_, REQ_SET))
        return;

    char key_chan[KEY_MAX_LENGTH];
    makeKeyChan(key, key_chan, KEY_MAX_LENGTH - 1);

    req_set_.data.insert(key, val);
    req_set_.data.insert(key_chan, ch);
    req_timer_.delay(100);
}

void HwMotuAvb::m_sync(t_symbol* s, const AtomListView& lv)
{
    MotuAvbRequest req;
    if (!updateRequest(s, req, REQ_SYNC))
        return;

    scheduleTask(req);
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

void HwMotuAvb::m_mic_pad(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_BOOL))
        return;

    m_set_single(s, KEY_MIC_PAD, lv.intAt(0, 0), lv.boolAt(1, false), lv);
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

bool HwMotuAvb::updateRequest(t_symbol* s, MotuAvbRequest& req, int type) const
{
    if (device_->value() == &s_) {
        METHOD_ERR(s) << "empty device id";
        return false;
    }

    req.host = host_->value()->s_name;
    req.device = device_->value()->s_name;
    req.type = type;
    req.port = port_->value();
    return true;
}

void setup_hw_motu_avb()
{
    ObjectFactory<HwMotuAvb> obj("hw.motu.avb");
    obj.addMethod("sync", &HwMotuAvb::m_sync);

    obj.addMethod("phantom", &HwMotuAvb::m_phantom);
    obj.addMethod("mic_gain", &HwMotuAvb::m_mic_gain);
    obj.addMethod("mic_pad", &HwMotuAvb::m_mic_pad);
    obj.addMethod("guitar_gain", &HwMotuAvb::m_guitar_gain);
    obj.addMethod("main_gain", &HwMotuAvb::m_main_gain);
    obj.addMethod("phones_gain", &HwMotuAvb::m_phones_gain);

    obj.addMethod("input_gain", &HwMotuAvb::m_input_gain);
    obj.addMethod("output_gain", &HwMotuAvb::m_output_gain);
}

#include "hw_motu_avb.h"
#include "args/argcheck2.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include "fmt/core.h"
#include "httplib.h"

#ifdef NDEBUG
#define TDBG(...)
#define TDBG_FMT(...)
#else
#define TDBG(msg)           \
    do {                    \
        logger_.debug(msg); \
    } while (0)

#define TDBG_FMT(msg, ...)                            \
    do {                                              \
        logger_.debug(fmt::format(msg, __VA_ARGS__)); \
    } while (0)
#endif

#define TERR(msg)           \
    do {                    \
        logger_.error(msg); \
    } while (0)

#define TERR_FMT(msg, ...)                            \
    do {                                              \
        logger_.error(fmt::format(msg, __VA_ARGS__)); \
    } while (0)

constexpr size_t KEY_MAX_LENGTH = 32;
constexpr const char* KEY_PHANTOM = "k:phantom-ch";
constexpr const char* KEY_MIC_GAIN = "k:mic-gain";
constexpr const char* KEY_MIC_PAD = "k:mic-pad";
constexpr const char* KEY_MIC_PHASE = "k:mic-phase";
constexpr const char* KEY_MIC_NAME = "k:mic-name";
constexpr const char* KEY_MIC_TO_OUTPUT = "k:mic->out";
constexpr const char* KEY_MIC_TO_COMPUTER = "k:mic->comp";
constexpr const char* KEY_MIC_TO_MIXER = "k:mic->mix";
constexpr const char* KEY_GUITAR_TO_OUTPUT = "k:guitar->out";
constexpr const char* KEY_GUITAR_TO_COMPUTER = "k:guitar->comp";
constexpr const char* KEY_GUITAR_TO_MIXER = "k:guitar->mix";
constexpr const char* KEY_GUITAR_GAIN = "k:guitar-gain";
constexpr const char* KEY_GUITAR_PHASE = "k:guitar-phase";
constexpr const char* KEY_GUITAR_NAME = "k:guitar-name";
constexpr const char* KEY_INPUT_GAIN = "k:input-gain";
constexpr const char* KEY_INPUT_NAME = "k:input-name";
constexpr const char* KEY_INPUT_COMP_NAME = "k:input-cname";
constexpr const char* KEY_MAIN_GAIN = "k:main-gain";
constexpr const char* KEY_PHONES_GAIN = "k:phones-gain";
constexpr const char* KEY_OUTPUT_GAIN = "k:output-gain";
constexpr const char* KEY_INPUT_TO_OUTPUT = "k:in->out";
constexpr const char* KEY_INPUT_TO_COMPUTER = "k:in->comp";
constexpr const char* KEY_INPUT_TO_MIXER = "k:in->mix";

constexpr int MOTU_DEFAULT_HTTP_PORT = 1280;

constexpr int GROUP_INPUT_MIC = 0;
constexpr int GROUP_INPUT_GUITAR = 1;
constexpr int GROUP_INPUT_ANALOG = 2;
constexpr int GROUP_INPUT_TOSLINK = 3;
constexpr int GROUP_INPUT_COMPUTER = 4;
constexpr int GROUP_INPUT_MIX_IN = 9;
constexpr int GROUP_INPUT_MIX_REVERB = 10;
constexpr int GROUP_INPUT_MIX_GROUP = 11;
constexpr int GROUP_INPUT_MIX_MAIN = 12;
constexpr int GROUP_INPUT_MIX_AUX = 13;
constexpr int GROUP_INPUT_MIX_MONITOR = 14;

constexpr int GROUP_OUTPUT_PHONES = 0;
constexpr int GROUP_OUTPUT_MAIN = 1;
constexpr int GROUP_OUTPUT_ANALOG = 2;
constexpr int GROUP_OUTPUT_TOSLINK = 3;
constexpr int GROUP_OUTPUT_COMPUTER = 4;
constexpr int GROUP_OUTPUT_MIXER = 9;

namespace {

using HttpClientPtr = std::shared_ptr<httplib::Client>;

enum RequestType {
    REQ_SYNC = 1,
    REQ_SET,
};

constexpr const char* STR_INPUT_PATTERN = "/{{}}/datastore/ext/ibank/{}/ch/{{}}";
const std::string STR_INPUT_MIC = fmt::format(STR_INPUT_PATTERN, GROUP_INPUT_MIC);
const std::string STR_INPUT_GUITAR = fmt::format(STR_INPUT_PATTERN, GROUP_INPUT_GUITAR);
const std::string STR_INPUT_ANALOG = fmt::format(STR_INPUT_PATTERN, GROUP_INPUT_ANALOG);
const std::string STR_INPUT_TOSLINK = fmt::format(STR_INPUT_PATTERN, GROUP_INPUT_TOSLINK);
const std::string STR_INPUT_COMPUTER = fmt::format(STR_INPUT_PATTERN, GROUP_INPUT_COMPUTER);

constexpr const char* STR_OUT_PATTERN = "/{{}}/datastore/ext/obank/{}/ch/{{}}";
const std::string STR_OUTPUT_PHONES = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_PHONES);
const std::string STR_OUTPUT_MAIN = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_MAIN);
const std::string STR_OUTPUT_ANALOG = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_ANALOG);
const std::string STR_OUTPUT_TOSLINK = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_TOSLINK);
const std::string STR_OUTPUT_COMPUTER = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_COMPUTER);
const std::string STR_OUTPUT_MIXER = fmt::format(STR_OUT_PATTERN, GROUP_OUTPUT_MIXER);

const std::unordered_map<const char*, std::string> UrlMap = {
    // mic
    { KEY_PHANTOM, STR_INPUT_MIC + "/48V" },
    { KEY_MIC_GAIN, STR_INPUT_MIC + "/trim" },
    { KEY_MIC_PAD, STR_INPUT_MIC + "/pad" },
    { KEY_MIC_PHASE, STR_INPUT_MIC + "/phase" },
    { KEY_MIC_NAME, STR_INPUT_MIC + "/name" },
    // guitar
    { KEY_GUITAR_GAIN, STR_INPUT_GUITAR + "/trim" },
    { KEY_GUITAR_PHASE, STR_INPUT_GUITAR + "/phase" },
    { KEY_GUITAR_NAME, STR_INPUT_GUITAR + "/name" },
    // analog in
    { KEY_INPUT_GAIN, STR_INPUT_ANALOG + "/trim" },
    { KEY_INPUT_NAME, STR_INPUT_ANALOG + "/name" },
    // comp im
    { KEY_INPUT_COMP_NAME, STR_INPUT_COMPUTER + "/name" },
    // routing
    { KEY_MIC_TO_OUTPUT, STR_OUTPUT_ANALOG + "/src" },
    { KEY_MIC_TO_COMPUTER, STR_OUTPUT_COMPUTER + "/src" },
    { KEY_MIC_TO_MIXER, STR_OUTPUT_MIXER + "/src" },
    { KEY_GUITAR_TO_OUTPUT, STR_OUTPUT_ANALOG + "/src" },
    { KEY_GUITAR_TO_COMPUTER, STR_OUTPUT_COMPUTER + "/src" },
    { KEY_GUITAR_TO_MIXER, STR_OUTPUT_MIXER + "/src" },
    { KEY_INPUT_TO_OUTPUT, STR_OUTPUT_ANALOG + "/src" },
    { KEY_INPUT_TO_COMPUTER, STR_OUTPUT_COMPUTER + "/src" },
    { KEY_INPUT_TO_MIXER, STR_OUTPUT_MIXER + "/src" },
    // output
    { KEY_MAIN_GAIN, STR_OUTPUT_MAIN + "/stereoTrim" },
    { KEY_PHONES_GAIN, STR_OUTPUT_PHONES + "/stereoTrim" },
    { KEY_OUTPUT_GAIN, STR_OUTPUT_ANALOG + "/trim" },
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

HttpClientPtr make_http_cli(const std::string& host, int http_port)
{
    HttpClientPtr cli(new httplib::Client(host, http_port));
    if (cli) {
        cli->set_connection_timeout(1);
        cli->set_tcp_nodelay(true);
    }
    return cli;
}

enum SetValueStatus {
    SET_VALUE_OK,
    SET_VALUE_NOT_FOUND,
    SET_VALUE_ERROR,
};

SetValueStatus setSingleValue(HttpClientPtr& cli,
    const std::string& device,
    const char* key,
    const MotuAvbRequest& req,
    ThreadPdLogger& log)
{
    if (!cli)
        return SET_VALUE_ERROR;

    if(!req.data.contains(key))
        return SET_VALUE_NOT_FOUND;

    char key_chan[KEY_MAX_LENGTH];
    makeKeyChan(key, key_chan, KEY_MAX_LENGTH - 1);

    if (!req.data.contains(key_chan))
        return SET_VALUE_NOT_FOUND;

    auto it = UrlMap.find(key);
    if (it == UrlMap.end()) {
        log.error(fmt::format("key not found: '{}'", key));
        return SET_VALUE_NOT_FOUND;
    }

    auto chan = req.data.at(key_chan).intAt(0, 0);
    auto path_pattern = it->second;
    auto path = fmt::format(path_pattern, device, chan);

    auto val = to_string(req.data.at(key));
    auto json = fmt::format("json={{\"value\":\"{}\"}}", val);
    auto res = cli->Patch(path.c_str(), json.c_str(), "application/x-www-form-urlencoded");

    log.debug(fmt::format("url: {}, json: {}", path, json));

    if (res) {
        if (res->status != 200 && res->status != 204) {
            log.error(fmt::format("http error status: '{}'", res->status));
            return SET_VALUE_ERROR;
        } else
            return SET_VALUE_OK;
    } else {
        log.error(fmt::format("http request error: '{}'", to_string(res.error())));
        return SET_VALUE_ERROR;
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
        req_set_.data.clear();
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
}

HwMotuAvb::Future HwMotuAvb::createTask()
{
    return std::async(std::launch::async, [this]() {
        MotuAvbRequest req;

        try {
            HttpClientPtr http_cli;

            while (inPipe().try_dequeue(req)) {
                if (!http_cli)
                    http_cli = make_http_cli(req.host, req.port);

                switch (req.type) {
                case REQ_SYNC: {

                    auto url = fmt::format("/{}/datastore", req.device);
                    auto res = http_cli->Get(url.c_str());
                    if (res) {
                        if (res->status == 200) {
                            DataTypeDict resp;
                            if (resp.fromJSON(res->body)) {
                                outPipe().enqueue(resp);
                                TDBG("HTTP ok");
                            } else {
                                TERR("JSON parse error");
                            }
                        } else {
                            TERR_FMT("http error status: '{}'", res->status);
                            TDBG_FMT("{}", res->body);
                            for (auto& kv : res->headers) {
                                TDBG_FMT("{}: {}", kv.first, kv.second);
                            }
                        }
                    } else {
                        TERR_FMT("http request error: '{}'", to_string(res.error()));
                    }
                } break;
                case REQ_SET: {
                    for (auto& kv : UrlMap) {
                        if (SET_VALUE_ERROR == setSingleValue(http_cli, req.device, kv.first, req, logger_)) {
                            while (inPipe().try_dequeue(req)) // clean queue
                                ;

                            break;
                        }
                    }
                } break;
                default:
                    TERR_FMT("unknown request type: {}", req.type);
                    break;
                }
            }

            Dispatcher::instance().send({ subscriberId(), 0 });
        } catch (std::exception& e) {
            TERR_FMT("run thread exception: {}", e.what());
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

void HwMotuAvb::m_set_single(t_symbol* s, const char* key, int ch, const Atom& val)
{
    if (!updateRequest(s, req_set_, REQ_SET))
        return;

    char key_chan[KEY_MAX_LENGTH];
    makeKeyChan(key, key_chan, KEY_MAX_LENGTH - 1);

    req_set_.data.insert(key, val);
    req_set_.data.insert(key_chan, ch);
    req_timer_.delay(100);
}

void HwMotuAvb::routeEnable(t_symbol* s, int in_group, int in_channel, const char* out_key, int out_channel, bool val)
{
    if (val) {
        auto msg = fmt::format("{}:{}", in_group, in_channel);
        m_set_single(s, out_key, out_channel, gensym(msg.c_str()));
    } else
        m_set_single(s, out_key, out_channel, &s_);
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

    m_set_single(s, KEY_PHANTOM, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_mic_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_MIC_GAIN, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_guitar_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_GUITAR_GAIN, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_guitar_phase(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_GUITAR_PHASE, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_guitar_name(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i>=0 s?");

    if (!chk.check(lv, this))
        return chk.usage(this, s);

    m_set_single(s, KEY_GUITAR_NAME, lv.intAt(0, 0), lv.symbolAt(1, &s_));
}

void HwMotuAvb::m_input_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_INPUT_GAIN, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_input_name(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i>=0 s?");

    if (!chk.check(lv, this))
        return chk.usage(this, s);

    m_set_single(s, KEY_INPUT_NAME, lv.intAt(0, 0), lv.symbolAt(1, &s_));
}

void HwMotuAvb::m_mic_pad(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_BOOL))
        return;

    m_set_single(s, KEY_MIC_PAD, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_mic_phase(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_BOOL))
        return;

    m_set_single(s, KEY_MIC_PHASE, lv.intAt(0, 0), lv.at(1));
}

void HwMotuAvb::m_mic_name(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i>=0 s?");

    if (!chk.check(lv, this))
        return chk.usage(this, s);

    m_set_single(s, KEY_MIC_NAME, lv.intAt(0, 0), lv.symbolAt(1, &s_));
}

void HwMotuAvb::m_mic_to_output(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto mic_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_MIC, mic_idx, KEY_MIC_TO_OUTPUT, out_idx, state);
}

void HwMotuAvb::m_mic_to_computer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto mic_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_MIC, mic_idx, KEY_MIC_TO_COMPUTER, out_idx, state);
}

void HwMotuAvb::m_mic_to_mixer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto mic_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_MIC, mic_idx, KEY_MIC_TO_MIXER, out_idx, state);
}

void HwMotuAvb::m_guitar_to_output(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_GUITAR, guitar_idx, KEY_GUITAR_TO_OUTPUT, out_idx, state);
}

void HwMotuAvb::m_guitar_to_computer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_GUITAR, guitar_idx, KEY_GUITAR_TO_COMPUTER, out_idx, state);
}

void HwMotuAvb::m_guitar_to_mixer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_GUITAR, guitar_idx, KEY_GUITAR_TO_MIXER, out_idx, state);
}

void HwMotuAvb::m_input_to_output(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_ANALOG, guitar_idx, KEY_INPUT_TO_OUTPUT, out_idx, state);
}

void HwMotuAvb::m_input_to_computer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_ANALOG, guitar_idx, KEY_INPUT_TO_COMPUTER, out_idx, state);
}

void HwMotuAvb::m_input_to_mixer(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_INT, ARG_BOOL))
        return;

    auto guitar_idx = lv.intAt(0, 0);
    auto out_idx = lv.intAt(1, 0);
    auto state = lv.boolAt(2, false);

    routeEnable(s, GROUP_INPUT_ANALOG, guitar_idx, KEY_INPUT_TO_MIXER, out_idx, state);
}

void HwMotuAvb::m_comp_input_name(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("i>=0 s?");

    if (!chk.check(lv, this))
        return chk.usage(this, s);

    m_set_single(s, KEY_INPUT_COMP_NAME, lv.intAt(0, 0), lv.symbolAt(1, &s_));
}

void HwMotuAvb::m_main_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT))
        return;

    m_set_single(s, KEY_MAIN_GAIN, 0, lv.at(0));
}

void HwMotuAvb::m_phones_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT))
        return;

    m_set_single(s, KEY_PHONES_GAIN, 0, lv.at(0));
}

void HwMotuAvb::m_output_gain(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_INT, ARG_FLOAT))
        return;

    m_set_single(s, KEY_OUTPUT_GAIN, lv.intAt(0, 0), lv.at(1));
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
    obj.addMethod("mic_phase", &HwMotuAvb::m_mic_phase);
    obj.addMethod("mic_name", &HwMotuAvb::m_mic_name);
    obj.addMethod("mic->output", &HwMotuAvb::m_mic_to_output);
    obj.addMethod("mic->comp", &HwMotuAvb::m_mic_to_computer);
    obj.addMethod("mic->mix", &HwMotuAvb::m_mic_to_mixer);

    obj.addMethod("guitar_gain", &HwMotuAvb::m_guitar_gain);
    obj.addMethod("guitar_phase", &HwMotuAvb::m_guitar_phase);
    obj.addMethod("guitar_name", &HwMotuAvb::m_guitar_name);
    obj.addMethod("guitar->output", &HwMotuAvb::m_guitar_to_output);
    obj.addMethod("guitar->comp", &HwMotuAvb::m_guitar_to_computer);
    obj.addMethod("guitar->mix", &HwMotuAvb::m_guitar_to_mixer);

    obj.addMethod("main_gain", &HwMotuAvb::m_main_gain);
    obj.addMethod("phones_gain", &HwMotuAvb::m_phones_gain);

    obj.addMethod("input_gain", &HwMotuAvb::m_input_gain);
    obj.addMethod("input_name", &HwMotuAvb::m_input_name);
    obj.addMethod("input->output", &HwMotuAvb::m_input_to_output);
    obj.addMethod("input->comp", &HwMotuAvb::m_input_to_computer);
    obj.addMethod("input->mix", &HwMotuAvb::m_input_to_mixer);

    obj.addMethod("comp_input_name", &HwMotuAvb::m_comp_input_name);

    obj.addMethod("output_gain", &HwMotuAvb::m_output_gain);
}

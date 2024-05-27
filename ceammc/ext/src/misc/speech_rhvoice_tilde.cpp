#include "speech_rhvoice_tilde.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "ceammc_string_types.h"
#include "fmt/core.h"

#include "RHVoice.h"
#include "soxr.h"

#include <chrono>
#include <fstream>
#include <thread>

#define RHVOICE_DEBUG 1

enum RhvoiceEventType {
    RHVOICE_DONE,
    RHVOICE_WORD_START,
    RHVOICE_WORD_END,
    RHVOICE_SENTENCE_START,
    RHVOICE_SENTENCE_END,
    RHVOICE_FILE_READ
};

static inline SpeechRhvoiceTilde* toThis(void* x) { return static_cast<SpeechRhvoiceTilde*>(x); }

static inline FloatProperty* propRange(FloatProperty* p, t_float a, t_float b)
{
    p->checkClosedRange(a, b);
    return p;
}

static inline bool looks_like_ssml(const std::string& str)
{
    return string::starts_with(str, "<?xml") || string::starts_with(str, "<speak");
}

static const std::array<const char*, 4> RHVOICE_CONFIG_PATHS {
    "~/.local/etc/RHVoice.conf",
    "/usr/local/etc/RHVoice/RHVoice.conf",
    "/opt/local/etc/RHVoice/RHVoice.conf",
    "/etc/RHVoice/RHVoice.conf",
};

static const std::array<const char*, 4> RHVOICE_DATA_PATHS {
    "~/.local/share/RHVoice",
    "/usr/local/share/RHVoice",
    "/opt/local/share/RHVoice",
    "/usr/share/RHVoice",
};

class SynthFloatProperty : public FloatProperty {
    using ValType = typeof(RHVoice_synth_params::absolute_pitch);
    ValType* v_;

public:
    SynthFloatProperty(const char* name, ValType* v)
        : FloatProperty(name, *v, PropValueAccess::READWRITE)
        , v_(v)
    {
        setSuccessFn([this](Property*) { *v_ = value(); });
        setFloatCheckFn([](t_float v) -> bool { return v >= -1 && v <= 1; });
    }
};

Resampler::Resampler()
    : soxr_(nullptr, soxr_delete)
    , in_rate_(0)
    , out_rate_(0)
{
}

bool Resampler::setRates(float inRate, float outRate)
{
    if (!soxr_ || in_rate_ != inRate || out_rate_ != outRate) {
        in_rate_ = inRate;
        out_rate_ = outRate;

        auto io = soxr_io_spec(SOXR_INT16, SOXR_FLOAT32);
        auto q = soxr_quality_spec(SOXR_QQ, 0);
        soxr_error_t no_err = nullptr;

        {
            Lock lock(mtx_);
            soxr_.reset(soxr_create(in_rate_, out_rate_, 1, &no_err, &io, &q, nullptr));
        }

        if (no_err != nullptr || !soxr_) {
            std::cerr << fmt::format("{} error: {}\n", __FUNCTION__, soxr_strerror(no_err));
            return false;
        } else
            return true;
    }

    return true;
}

bool Resampler::setInRate(float inRate)
{
    return setRates(inRate, out_rate_);
}

bool Resampler::setOutRate(float outRate)
{
    return setRates(in_rate_, outRate);
}

soxr_error_t Resampler::process(const short* in, size_t ilen, size_t* idone, float* out, size_t olen, size_t* odone)
{
    if (!soxr_)
        return "soxr init error";

    Lock lock(mtx_);
    return soxr_process(soxr_.get(), in, ilen, idone, out, olen, odone);
}

SpeechRhvoiceTilde::SpeechRhvoiceTilde(const PdArgs& args)
    : DispatchedObject<SoundExternal>(args)
    , tts_(nullptr, &RHVoice_delete_tts_engine)
    , dsp_queue_(TtsQueueSize)
    , txt_queue_(16)
    , quit_(false)
    , stop_(false)
    , punct_(nullptr)
{
    createSignalOutlet();
    createOutlet();

    punct_ = new SymbolProperty("@punct", &s_);
    punct_->setSuccessFn([this](Property*) {
        auto sym = punct_->value();
        if (sym == &s_) {
            synth_params_.punctuation_mode = RHVoice_punctuation_none;
            synth_params_.punctuation_list = nullptr;
        } else if (sym == gensym("all")) {
            synth_params_.punctuation_mode = RHVoice_punctuation_all;
            synth_params_.punctuation_list = nullptr;
        } else {
            synth_params_.punctuation_mode = RHVoice_punctuation_some;
            synth_params_.punctuation_list = sym->s_name;
        }
    });
    addProperty(punct_);

    initEngineParams();
    tts_.reset(RHVoice_new_tts_engine(&engine_params_));

    initSynthParams();
    initProperties();

    initWorker();
}

SpeechRhvoiceTilde::~SpeechRhvoiceTilde()
{
    quit_ = true;
    proc_.get();
}

void SpeechRhvoiceTilde::onFloat(t_float f)
{
    stop_ = false;
    txt_queue_.emplace(fmt::format("{}", f), synth_params_);
    notify_.notifyOne();
}

void SpeechRhvoiceTilde::onSymbol(t_symbol* s)
{
    if (s == &s_) {
        OBJ_ERR << "empty symbol";
        return;
    }

    stop_ = false;
    txt_queue_.emplace(s->s_name, synth_params_);
    notify_.notifyOne();
}

void SpeechRhvoiceTilde::onList(const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "empty list";
        return;
    }

    string::StaticString str;

    try {
        for (auto& a : lv) {
            if (a.isInteger()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_int>());
            } else if (a.isFloat()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_float>());
            } else if (a.isSymbol()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_symbol*>()->s_name);
            }
        }
    } catch (std::length_error& e) {
        OBJ_ERR << fmt::format("result string is too long: {}, extra characters will be trimmed", str.max_size());
    }

    // remove last space
    if (str.size() > 0 || str.back() == ' ')
        str.pop_back();

    stop_ = false;
    txt_queue_.enqueue({ { str.c_str(), str.size() }, synth_params_ });
    notify_.notifyOne();
}

void SpeechRhvoiceTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        t_sample s = 0;
        if (dsp_queue_.try_dequeue(s)) {
            out[0][i] = s;
        } else
            out[0][i] = 0;
    }
}

void SpeechRhvoiceTilde::samplerateChanged(size_t sr)
{
    resampler_.setOutRate(sr);
}

bool SpeechRhvoiceTilde::notify(int code)
{
    switch (code) {
    case RHVOICE_DONE:
        bangTo(1);
        break;
    case RHVOICE_WORD_START:
        anyTo(1, gensym("word"), 1);
        break;
    case RHVOICE_WORD_END:
        anyTo(1, gensym("word"), 0.);
        break;
    case RHVOICE_SENTENCE_START:
        anyTo(1, gensym("sentence"), 1);
        break;
    case RHVOICE_SENTENCE_END:
        anyTo(1, gensym("sentence"), 0.);
        break;
    default:
        return true;
    }

    return true;
}

void SpeechRhvoiceTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    stop_ = true;

    float samp;
    while (dsp_queue_.try_dequeue(samp))
        ;

    Msg msg;
    while (txt_queue_.try_dequeue(msg))
        ;
}

void SpeechRhvoiceTilde::m_clear(t_symbol* s, const AtomListView& lv)
{
    Msg msg;
    while (txt_queue_.try_dequeue(msg))
        ;
}

void SpeechRhvoiceTilde::m_read(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, s))
        return;

    auto cstr_path = lv.symbolAt(0, &s_)->s_name;
    auto path = findInStdPaths(cstr_path);
    if (path.empty()) {
        METHOD_ERR(s) << fmt::format("file not found: '{}'", cstr_path);
        return;
    }

    Msg msg(path, synth_params_);
    msg.cmd = RHVOICE_CMD_READ_FILE;
    txt_queue_.emplace(msg);

    notify_.notifyOne();
}

void SpeechRhvoiceTilde::m_ssml(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "empty list";
        return;
    }

    stop_ = false;
    Msg msg(to_string(lv), synth_params_);
    msg.cmd = RHVOICE_CMD_SAY_SSML;
    txt_queue_.emplace(msg);
    notify_.notifyOne();
}

void SpeechRhvoiceTilde::onDone()
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), RHVOICE_DONE });
}

void SpeechRhvoiceTilde::onWordStart(int pos, int len)
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), RHVOICE_WORD_START });
}

void SpeechRhvoiceTilde::onWordEnd(int pos, int len)
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), RHVOICE_WORD_END });
}

void SpeechRhvoiceTilde::onSentenceStart(int pos, int len)
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), RHVOICE_SENTENCE_START });
}

void SpeechRhvoiceTilde::onSentenceEnd(int pos, int len)
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), RHVOICE_SENTENCE_END });
}

void SpeechRhvoiceTilde::onTtsSampleRate(int sr)
{
    resampler_.setRates(sr, samplerate());

#if RHVOICE_DEBUG
    std::cerr << fmt::format("[{}] SR={}\n", __FUNCTION__, sr) << std::flush;
#endif
}

int SpeechRhvoiceTilde::onDsp(const short* data, unsigned int n)
{
    static_assert(std::is_same<short, int16_t>::value, "");

    constexpr int RHVOICE_STOP = 0;
    constexpr int RHVOICE_CONTINUE = 1;
    if (quit_)
        return RHVOICE_STOP;

    constexpr int BUF_SIZE = 2048;
    float out_buf[BUF_SIZE];

    int in_done_total = 0;
    int in_left = n;

    while (true) {
        if (quit_)
            return RHVOICE_STOP;

        if (stop_)
            break;

        const short* in_buf = data + in_done_total;
        size_t in_done = 0;
        size_t out_done = 0;

        auto no_err = resampler_.process(in_buf, in_left, &in_done, out_buf, BUF_SIZE, &out_done);

        if (no_err != nullptr) {
            std::cerr << fmt::format("[{}] error: {}\n", __FUNCTION__, soxr_strerror(no_err));
            break;
        }

        in_left -= in_done;
        in_done_total += in_done;

        if (out_done != 0) {
            const auto wait_ms = (1000 * TtsQueueSize) / samplerate();
            for (size_t i = 0; i < out_done; i++) {
                while (!dsp_queue_.try_enqueue(out_buf[i])) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));

                    if (stop_)
                        break;

                    if (quit_)
                        return RHVOICE_STOP;
                }
            }
        } else
            break;
    }

    return quit_ ? RHVOICE_STOP : RHVOICE_CONTINUE;
}

void SpeechRhvoiceTilde::initEngineParams()
{
    data_dir_ = platform::pd_user_directory();
    data_dir_ += "/rhvoice";
    if (!platform::path_exists(data_dir_.c_str())) {
        bool found = false;
        for (auto& p : RHVOICE_DATA_PATHS) {
            if (platform::path_exists(platform::expand_tilde_path(p).c_str())) {
                OBJ_DBG << "RHVoice data dir: " << p;
                found = true;
                data_dir_ = p;
                break;
            }
        }

        if (!found)
            OBJ_DBG << fmt::format("RHVoice data directory not exists: '{}'", data_dir_);
    }

    conf_path_ = platform::pd_user_directory();
    conf_path_ += "/rhvoice/RHVoice.conf";
    if (!platform::path_exists(conf_path_.c_str())) {
        bool found = false;
        for (auto& p : RHVOICE_CONFIG_PATHS) {
            auto exp_p = platform::expand_tilde_path(p);
            if (platform::path_exists(exp_p.c_str())) {
                conf_path_ = exp_p;
                found = true;
                break;
            }
        }

        if (!found)
            conf_path_ = findInStdPaths("RHVoice.conf");
    }

    if (conf_path_.empty()) {
        OBJ_DBG << "RHVoice.conf not found";
    } else {
        conf_path_ = platform::dirname(conf_path_.c_str());
        OBJ_DBG << "RHVoice.conf dir: " << conf_path_;
    }

    memset(&engine_params_, 0, sizeof(engine_params_));
    engine_params_.data_path = data_dir_.c_str();
    engine_params_.config_path = conf_path_.c_str();
    engine_params_.resource_paths = nullptr;
    engine_params_.options = RHVoice_preload_voices;
    engine_params_.callbacks.done =
        [](void* obj) { toThis(obj)->onDone(); };
    engine_params_.callbacks.word_starts =
        [](unsigned int pos, unsigned int len, void* obj) -> int { toThis(obj)->onWordStart(pos, len); return 1; };
    engine_params_.callbacks.word_ends =
        [](unsigned int pos, unsigned int len, void* obj) -> int { toThis(obj)->onWordEnd(pos, len); return 1; };
    engine_params_.callbacks.sentence_starts =
        [](unsigned int pos, unsigned int len, void* obj) -> int { toThis(obj)->onSentenceStart(pos, len); return 1; };
    engine_params_.callbacks.sentence_ends =
        [](unsigned int pos, unsigned int len, void* obj) -> int { toThis(obj)->onSentenceEnd(pos, len); return 1; };
    engine_params_.callbacks.play_audio = nullptr;
    engine_params_.callbacks.process_mark = nullptr;
    engine_params_.callbacks.set_sample_rate = [](int sr, void* obj) -> int {
        toThis(obj)->onTtsSampleRate(sr);
        return 1;
    };
    engine_params_.callbacks.play_speech = [](const short* data, unsigned int n, void* obj) -> int {
        return toThis(obj)->onDsp(data, n);
    };
}

void SpeechRhvoiceTilde::initSynthParams()
{
    synth_params_.voice_profile = "Anna";
    synth_params_.absolute_pitch = 0;
    synth_params_.absolute_rate = 0;
    synth_params_.absolute_volume = 1;
    synth_params_.relative_rate = 1;
    synth_params_.relative_volume = 1;
    synth_params_.relative_pitch = 1;
    synth_params_.punctuation_list = nullptr;
    synth_params_.capitals_mode = RHVoice_capitals_default;
    synth_params_.punctuation_mode = RHVoice_punctuation_default;
    synth_params_.flags = 0;
}

void SpeechRhvoiceTilde::initProperties()
{
    addProperty(propRange(new SynthFloatProperty("@rate", &synth_params_.absolute_rate), -1, 1));
    addProperty(propRange(new SynthFloatProperty("@pitch", &synth_params_.absolute_pitch), -1, 1));
    addProperty(propRange(new SynthFloatProperty("@volume", &synth_params_.absolute_volume), 0, 1));

    auto nprofiles = RHVoice_get_number_of_voice_profiles(tts_.get());
    auto profiles = RHVoice_get_voice_profiles(tts_.get());
    OBJ_DBG << "number of profiles: " << nprofiles;
    for (unsigned int i = 0; i < nprofiles; i++) {
        OBJ_DBG << "\t-" << profiles[i];
    }

    auto nvoices = RHVoice_get_number_of_voices(tts_.get());
    auto voices = RHVoice_get_voices(tts_.get());
    for (unsigned int i = 0; i < nvoices; i++)
        voices_.append(gensym(voices[i].name));

    OBJ_DBG << "voices: " << voices_;

    auto voice_prop = addProperty(new SymbolProperty("@voice", gensym("Anna")));
    voice_prop->setSymbolCheckFn(
        [this](t_symbol* v) { return voices_.contains(v); },
        "invalid voice name");
    voice_prop->setSuccessFn([this](Property* p) {
        synth_params_.voice_profile = static_cast<SymbolProperty*>(p)->value()->s_name;
    });
}

static inline void speakText(const std::string& txt, RHVoice_message_type type, RHVoice_tts_engine_struct* tts, const RHVoice_synth_params& params, SpeechRhvoiceTilde* obj)
{
    auto msg = RHVoice_new_message(
        tts,
        txt.c_str(),
        txt.size(),
        type,
        &params,
        obj);

    auto rc = RHVoice_speak(msg);
#if RHVOICE_DEBUG
    std::cerr << fmt::format("speak '{}':  {}\n", txt, rc);
#endif
    RHVoice_delete_message(msg);
}

void SpeechRhvoiceTilde::initWorker()
{
    proc_ = std::async(
        std::launch::async,
        [this]() {
            while (!quit_) {
                Msg txt;
                if (txt_queue_.try_dequeue(txt)) {
                    switch (txt.cmd) {
                    case RHVOICE_CMD_READ_FILE: {
                        std::ifstream ifs(txt.txt.c_str());
                        if (!ifs) {
                            logger_.error(fmt::format("can't open file: '{}'", txt.txt));
                            return;
                        }

                        std::vector<std::string> pars;
                        std::string line;
                        int nl = 0;
                        while (std::getline(ifs, line)) {
                            if (line.size() > 0) {
                                if (nl != 0 || pars.empty())
                                    pars.push_back({});

                                nl = 0;
                                pars.back().append(line);
                                pars.back().push_back(' ');
                            } else
                                nl++;
                        }

                        stop_ = false;
                        const bool ssml = pars.size() > 0 && looks_like_ssml(pars.front());

                        if (ssml) {
                            std::string all;
                            for (auto& l : pars) {
                                all.append(l);
                                all.push_back(' ');
                            }

                            speakText(all, RHVoice_message_ssml, tts_.get(), txt.params, this);
                        } else {
                            for (auto& p : pars) {
                                if (stop_)
                                    break;

                                speakText(p, RHVoice_message_text, tts_.get(), txt.params, this);
                            }
                        }
                    } break;
                    case RHVOICE_CMD_SAY_SSML:
                        speakText(txt.txt, RHVoice_message_ssml, tts_.get(), txt.params, this);
                        break;
                    case RHVOICE_CMD_SAY_TEXT:
                    default:
                        speakText(txt.txt, RHVoice_message_text, tts_.get(), txt.params, this);
                        break;
                    }
                }

                if (stop_) {
                    while (txt_queue_.try_dequeue(txt))
                        ;
                }

                notify_.waitFor(250);
            }
    });
}

void setup_speech_rhvoice_tilde()
{
    SoundExternalFactory<SpeechRhvoiceTilde> obj("speech.rhvoice~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("rhvoice~");
    obj.addMethod("stop", &SpeechRhvoiceTilde::m_stop);
    obj.addMethod("clear", &SpeechRhvoiceTilde::m_clear);
    obj.addMethod("read", &SpeechRhvoiceTilde::m_read);
    obj.addMethod("ssml", &SpeechRhvoiceTilde::m_ssml);

    obj.setXletsInfo({ "float: speak number\n"
                       "symbol: speak symbol\n"
                       "list: speak list words" },
        { "signal: tts output", "'done' when done" });

    LIB_DBG << fmt::format("RHVoice version: {}", RHVoice_get_version());
}

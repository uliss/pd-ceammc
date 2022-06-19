#include "speech_rhvoice_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include "RHVoice.h"
#include "soxr.h"

#include <boost/static_string.hpp>
#include <chrono>

#define RHVOICE_DEBUG 1

static inline SpeechRhvoiceTilde* toThis(void* x) { return static_cast<SpeechRhvoiceTilde*>(x); }

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

SpeechRhvoiceTilde::SpeechRhvoiceTilde(const PdArgs& args)
    : SoundExternal(args)
    , tts_(nullptr, &RHVoice_delete_tts_engine)
    , quit_(false)
    , stop_(false)
    , voice_sr_(0)
    , soxr_(nullptr, soxr_delete)
    , dsp_queue_(TtsQueueSize)
    , txt_queue_(16)
{
    createSignalOutlet();
    createOutlet();

    initEngineParams();
    tts_.reset(RHVoice_new_tts_engine(&engine_params_));

    initSynthParams();
    initProperties();

    initWorker();

    Dispatcher::instance().subscribe(this, reinterpret_cast<SubscriberId>(this));
}

SpeechRhvoiceTilde::~SpeechRhvoiceTilde()
{
    quit_ = true;
    proc_.get();

    Dispatcher::instance().unsubscribe(this);
}

void SpeechRhvoiceTilde::onSymbol(t_symbol* s)
{
    if (s == &s_) {
        OBJ_ERR << "empty symbol";
        return;
    }

    stop_ = false;
    txt_queue_.emplace(s->s_name);
    notify_.notifyOne();
}

void SpeechRhvoiceTilde::onList(const AtomList& lst)
{
    if (lst.empty()) {
        OBJ_ERR << "empty list";
        return;
    }

    constexpr int MAX_STRING_LEN = 1024;
    using StaticString = boost::static_string<MAX_STRING_LEN>;
    StaticString str;

    try {
        for (auto& a : lst) {
            if (a.isInteger()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<int>());
            } else if (a.isFloat()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_float>());
            } else if (a.isSymbol()) {
                fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_symbol*>()->s_name);
            }
        }
    } catch (std::length_error& e) {
        OBJ_ERR << fmt::format("result string is too long: {}, extra characters will be trimmed", MAX_STRING_LEN);
    }

    // remove last space
    if (str.size() > 0 || str.back() == ' ')
        str.pop_back();

    stop_ = false;
    txt_queue_.emplace(str.c_str(), str.size());
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
    soxrInit();
}

bool SpeechRhvoiceTilde::notify(NotifyEventType code)
{
    switch (code) {
    case NOTIFY_DONE:
        symbolTo(1, gensym("done"));
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

    std::string str;
    while (txt_queue_.try_dequeue(str))
        ;
}

void SpeechRhvoiceTilde::onDone()
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), NOTIFY_DONE });
}

void SpeechRhvoiceTilde::onWordStart(int pos, int len)
{
    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), NOTIFY_UPDATE });
    std::cerr << fmt::format("word start: {} {}\n", pos, len);
}

void SpeechRhvoiceTilde::onSampleRate(int sr)
{
    if (voice_sr_ != sr) {
        voice_sr_ = sr;
        soxrInit();

#if RHVOICE_DEBUG
        std::cerr << fmt::format("[{}] SR={}\n", __FUNCTION__, sr) << std::flush;
#endif
    }
}

int SpeechRhvoiceTilde::onDsp(const short* data, unsigned int n)
{
    static_assert(std::is_same<short, int16_t>::value, "");

    constexpr int RHVOICE_STOP = 0;
    constexpr int RHVOICE_CONTINUE = 1;
    if (quit_)
        return RHVOICE_STOP;

    if (!soxr_) {
        // try to init samplerate converter
        if (!soxrInit())
            return RHVOICE_STOP;
    }

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
        soxr_error_t no_err = 0;

        {
            std::unique_lock<std::mutex> lock(soxr_mtx_);
            no_err = soxr_process(soxr_.get(), in_buf, in_left, &in_done, out_buf, BUF_SIZE, &out_done);
        }

        if (no_err != 0) {
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
    memset(&engine_params_, 0, sizeof(engine_params_));
    engine_params_.data_path = "/Users/serge/.local/share/RHVoice";
    engine_params_.config_path = "/Users/serge/.local/etc/RHVoice";
    engine_params_.resource_paths = nullptr;
    engine_params_.options = RHVoice_preload_voices;
    engine_params_.callbacks.done =
        [](void* obj) { toThis(obj)->onDone(); };
    engine_params_.callbacks.word_starts =
        [](unsigned int pos, unsigned int len, void* obj) -> int { toThis(obj)->onWordStart(pos, len); return 1; };
    engine_params_.callbacks.play_audio = nullptr;
    engine_params_.callbacks.process_mark = nullptr;
    engine_params_.callbacks.set_sample_rate = [](int sr, void* obj) -> int {
        toThis(obj)->onSampleRate(sr);
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
    addProperty(new SynthFloatProperty("@rate", &synth_params_.absolute_rate));
    addProperty(new SynthFloatProperty("@pitch", &synth_params_.absolute_pitch));
    addProperty(new SynthFloatProperty("@volume", &synth_params_.absolute_volume));

    auto nprofiles = RHVoice_get_number_of_voice_profiles(tts_.get());
    auto profiles = RHVoice_get_voice_profiles(tts_.get());
    OBJ_DBG << "number of profiles: " << nprofiles;
    for (int i = 0; i < nprofiles; i++) {
        OBJ_DBG << "\t-" << profiles[i];
    }

    auto nvoices = RHVoice_get_number_of_voices(tts_.get());
    auto voices = RHVoice_get_voices(tts_.get());
    for (int i = 0; i < nvoices; i++)
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

void SpeechRhvoiceTilde::initWorker()
{
    proc_ = std::async(
        std::launch::async,
        [this]() {
            while (!quit_) {
                std::string txt;
                if (txt_queue_.try_dequeue(txt)) {
                    auto msg = RHVoice_new_message(
                        tts_.get(),
                        txt.c_str(),
                        txt.size(),
                        RHVoice_message_text,
                        &synth_params_,
                        this);

                    auto rc = RHVoice_speak(msg);
#if RHVOICE_DEBUG
                    std::cerr << fmt::format("speak '{}':  {}\n", txt, rc);
#endif
                    RHVoice_delete_message(msg);
                }

                if (stop_) {
                    while (txt_queue_.try_dequeue(txt))
                        ;
                }

                notify_.waitFor(250);
            }
        });
}

bool SpeechRhvoiceTilde::soxrInit()
{
    auto io = soxr_io_spec(SOXR_INT16, SOXR_FLOAT32);
    auto q = soxr_quality_spec(SOXR_QQ, 0);
    soxr_error_t no_err = 0;

    {
        std::unique_lock<std::mutex> lock(soxr_mtx_);
        soxr_.reset(soxr_create(voice_sr_, samplerate(), 1, &no_err, &io, &q, nullptr));
    }

    if (no_err != 0 || !soxr_) {
        std::cerr << fmt::format("{} error: {}\n", __FUNCTION__, soxr_strerror(no_err));
        return false;
    } else
        return true;
}

void setup_speech_rhvoice_tilde()
{
    SoundExternalFactory<SpeechRhvoiceTilde> obj("speech.rhvoice~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("rhvoice~");
    obj.addMethod("stop", &SpeechRhvoiceTilde::m_stop);

    obj.setXletsInfo({ "symbol: speak symbol" }, { "signal: tts output", "'done' when done" });

    LIB_POST << fmt::format("RHVoice version: {}", RHVoice_get_version());
}

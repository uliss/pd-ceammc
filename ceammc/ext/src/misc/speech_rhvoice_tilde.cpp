#include "speech_rhvoice_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include "RHVoice.h"
#include "soxr.h"

#include <boost/static_string.hpp>

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
    , done_(false)
    , voice_sr_(0)
    , soxr_(nullptr, soxr_delete)
    , txt_queue_(512)
{
    params_.data_path = "/Users/serge/.local/share/RHVoice";
    params_.config_path = "/Users/serge/.local/etc/RHVoice";
    params_.resource_paths = nullptr;
    params_.options = RHVoice_preload_voices;
    params_.callbacks.done =
        [](void* obj) { toThis(obj)->onDone(); };
    params_.callbacks.word_starts =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onWordStart(pos, length);
        return 1;
    };
    params_.callbacks.word_ends =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onWordEnd(pos, length);
        return 1;
    };
    params_.callbacks.sentence_starts =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onSentenceStart(pos, length);
        return 1;
    };
    params_.callbacks.sentence_ends =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onSentenceEnd(pos, length);
        return 1;
    };
    params_.callbacks.play_audio = nullptr;
    params_.callbacks.process_mark = nullptr;
    params_.callbacks.set_sample_rate = [](int sr, void* obj) -> int {
        toThis(obj)->onSampleRate(sr);
        return 1;
    };
    params_.callbacks.play_speech = [](const short* data, unsigned int n, void* obj) -> int {
        return toThis(obj)->onDsp(data, n);
    };

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

    addProperty(new SynthFloatProperty("@rate", &synth_params_.absolute_rate));
    addProperty(new SynthFloatProperty("@pitch", &synth_params_.absolute_pitch));
    addProperty(new SynthFloatProperty("@volume", &synth_params_.absolute_volume));

    tts_.reset(RHVoice_new_tts_engine(&params_));

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

    proc_ = std::async(
        std::launch::async,
        [this]() {
            while (!done_) {
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
            }
        });

    createSignalOutlet();
}

SpeechRhvoiceTilde::~SpeechRhvoiceTilde()
{
    done_ = true;
    proc_.get();
}

void SpeechRhvoiceTilde::onSymbol(t_symbol* s)
{
    if (s == &s_) {
        OBJ_ERR << "empty symbol";
        return;
    }

    txt_queue_.emplace(s->s_name);
}

void SpeechRhvoiceTilde::onList(const AtomList& lst)
{
    if (lst.empty()) {
        OBJ_ERR << "empty list";
        return;
    }

    using StaticString = boost::static_string<1024>;
    StaticString str;

    for (auto& a : lst) {
        if (a.isInteger()) {
            fmt::format_to(std::back_inserter(str), "{} ", a.asT<int>());
        } else if (a.isFloat()) {
            fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_float>());
        } else if (a.isSymbol()) {
            fmt::format_to(std::back_inserter(str), "{} ", a.asT<t_symbol*>()->s_name);
        }
    }

    if (str.size() > 0)
        str.pop_back();

    txt_queue_.emplace(str.c_str(), str.size());
}

void SpeechRhvoiceTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        t_sample s = 0;
        if (queue_.try_dequeue(s)) {
            out[0][i] = s;
        } else
            out[0][i] = 0;
    }
}

void SpeechRhvoiceTilde::m_stop(t_symbol* s, const AtomListView& lv)
{
    done_ = true;
}

void SpeechRhvoiceTilde::onDone()
{
}

void SpeechRhvoiceTilde::onWordStart(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onWordEnd(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onSentenceStart(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onSentenceEnd(unsigned int pos, unsigned int length)
{
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
    if (done_)
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
            for (size_t i = 0; i < out_done; i++)
                queue_.enqueue(out_buf[i]);
        } else
            break;
    }

    return done_ ? RHVOICE_STOP : RHVOICE_CONTINUE;
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

    LIB_POST << fmt::format("RHVoice version: {}", RHVoice_get_version());
}

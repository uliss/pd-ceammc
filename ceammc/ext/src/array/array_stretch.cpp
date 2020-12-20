#include "array_stretch.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "SoundTouch.h"

static t_symbol* PROP_SEQUENCE;
static t_symbol* PROP_SEEK_WINDOW;
static t_symbol* PROP_OVERLAP;
static t_symbol* PROP_ANTIALIAS;
static t_symbol* PROP_ANTIALIAS_LENGTH;
static t_symbol* PROP_SPEECH;

class PdSoundTouch : public soundtouch::SoundTouch {
public:
    template <size_t BS>
    void putArrayData(Array::const_iterator begin, Array::const_iterator end)
    {
        float buf[BS];

        std::ptrdiff_t N = end - begin;
        for (size_t i = 0, hop = BS; i < N; i += BS) {
            if ((i + BS) > N)
                hop = N - i;

            std::copy(begin + i, begin + i + hop, buf);
            soundtouch::SoundTouch::putSamples(buf, hop);
        }
    }

    void getArrayData(std::vector<float>& dest)
    {
        static const size_t BUFSIZE = 4096;
        float buf[BUFSIZE];

        uint n = 0;
        do {
            // put in buffer
            n = receiveSamples(buf, BUFSIZE);
            dest.insert(dest.end(), buf, buf + n);
        } while (n != 0);
    }

    /**
     * Return number of output samples
     */
    size_t process(const Array& in, std::vector<float>& out)
    {
        static const size_t IN_BUFSIZE = 1024;

        const size_t N = in.size();
        // reserve space
        out.reserve(N);

        Array::const_iterator it = in.begin();

        for (size_t i = 0, hop = IN_BUFSIZE; i < N; i += IN_BUFSIZE) {
            // check hop size at last loop iteration
            if ((i + IN_BUFSIZE) > N)
                hop = N - i;

            // copy floats
            putArrayData<IN_BUFSIZE>(it, it + hop);
            it += hop;

            // append processed data
            getArrayData(out);
        }

        // flush last samples (if any)
        flush();
        getArrayData(out);

        return out.size();
    }
};

ArrayStretch::ArrayStretch(const PdArgs& a)
    : BaseObject(a)
    , src_array_name_(&s_)
    , dest_array_name_(&s_)
    , speech_(false)
    , pitch_(nullptr)
    , tempo_(nullptr)
    , rate_(nullptr)
    , soundtouch_(new PdSoundTouch())
{
    createOutlet();

    pitch_ = new FloatProperty("@pitch", 0);
    pitch_->setUnits(PropValueUnits::SEMITONE);
    tempo_ = new FloatProperty("@tempo", 0);
    tempo_->setUnits(PropValueUnits::PERCENT);
    rate_ = new FloatProperty("@rate", 1);

    addProperty(pitch_);
    addProperty(tempo_);
    addProperty(rate_);

    createCbSymbolProperty(
        "@src",
        [this]() -> t_symbol* { return src_array_name_; },
        [this](t_symbol* s) -> bool { return setSrcArray(s); })
        ->setArgIndex(0);

    createCbSymbolProperty(
        "@dest",
        [this]() -> t_symbol* { return dest_array_name_; },
        [this](t_symbol* s) -> bool { return setDestArray(s); })
        ->setArgIndex(1);

    // time-stretch params
    // @sequence
    Property* pseq = createCbIntProperty(
        PROP_SEQUENCE->s_name,
        [this]() -> int { return soundtouch_->getSetting(SETTING_SEQUENCE_MS); },
        [this](int v) -> bool { return soundtouch_->setSetting(SETTING_SEQUENCE_MS, v); });
    pseq->setIntCheck(PropValueConstraints::CLOSED_RANGE, 0, 100);
    pseq->setUnitsMs();

    // @seekwindow
    Property* seekwindow = createCbIntProperty(
        PROP_SEEK_WINDOW->s_name,
        [this]() -> int { return soundtouch_->getSetting(SETTING_SEEKWINDOW_MS); },
        [this](int v) -> bool { return soundtouch_->setSetting(SETTING_SEEKWINDOW_MS, v); });
    seekwindow->setIntCheck(PropValueConstraints::CLOSED_RANGE, 0, 100);
    seekwindow->setUnitsMs();

    // @overlap
    Property* overlap = createCbIntProperty(
        PROP_OVERLAP->s_name,
        [this]() -> int { return soundtouch_->getSetting(SETTING_OVERLAP_MS); },
        [this](int v) -> bool { return soundtouch_->setSetting(SETTING_OVERLAP_MS, v); });
    overlap->setIntCheck(PropValueConstraints::CLOSED_RANGE, 0, 100);
    overlap->setUnitsMs();

    // pitch-shift params
    // antialias
    createCbBoolProperty(
        PROP_ANTIALIAS->s_name,
        [this]() -> bool { return soundtouch_->getSetting(SETTING_USE_AA_FILTER); },
        [this](bool b) -> bool { return soundtouch_->setSetting(SETTING_USE_AA_FILTER, b); });

    // antialias length
    auto alen = createCbIntProperty(
        PROP_ANTIALIAS_LENGTH->s_name,
        [this]() -> int { return soundtouch_->getSetting(SETTING_AA_FILTER_LENGTH); },
        [this](int v) -> bool { return soundtouch_->setSetting(SETTING_AA_FILTER_LENGTH, v); });
    alen->setIntCheck(PropValueConstraints::CLOSED_RANGE, 8, 128);

    // optimise for speech
    createCbBoolProperty(
        PROP_SPEECH->s_name,
        [this]() -> bool { return speech_; },
        [this](bool b) -> bool {
            speech_ = b;

            if (speech_) {
                soundtouch_->setSetting(SETTING_SEQUENCE_MS, 40);
                soundtouch_->setSetting(SETTING_SEEKWINDOW_MS, 15);
                soundtouch_->setSetting(SETTING_OVERLAP_MS, 8);
            }

            return true;
        });
}

void ArrayStretch::onBang()
{
    soundtouch_->clear();

    if (src_array_.name() == dest_array_.name()) {
        OBJ_ERR << "source and destination arrays should be different";
        return;
    }

    if (!src_array_.update()) {
        OBJ_ERR << "can't open source array: " << src_array_.name();
        return;
    }

    if (src_array_.size() == 0) {
        OBJ_ERR << "empty input array";
        return;
    }

    if (!dest_array_.update()) {
        OBJ_ERR << "can't open destination array: " << dest_array_.name();
        return;
    }

    // setup SoundTouch paramters
    setupSoundTouch();

    // array for output data
    std::vector<float> out_array;

    // do timestretch and/or pitchshift
    const size_t OUT_N = soundtouch_->process(src_array_, out_array);

    if (OUT_N == 0) {
        OBJ_ERR << "no output";
        return;
    }

    // copy to destination array
    dest_array_.resize(OUT_N);
    std::copy(out_array.begin(), out_array.end(), dest_array_.begin());

    // redraw
    dest_array_.redraw();

    // output size
    floatTo(0, out_array.size());
}

bool ArrayStretch::setSrcArray(t_symbol* s)
{
    src_array_name_ = s;
    if (!src_array_.open(src_array_name_)) {
        OBJ_ERR << "source array is not found: " << s->s_name;
        return false;
    }

    return true;
}

bool ArrayStretch::setDestArray(t_symbol* s)
{
    dest_array_name_ = s;
    if (!dest_array_.open(dest_array_name_)) {
        OBJ_ERR << "destination array not found: " << s->s_name;
        return false;
    }

    return true;
}

void ArrayStretch::propSetOverlap(const AtomList& ms)
{
    if (!checkArgs(ms, ARG_FLOAT, PROP_OVERLAP))
        return;

    int overlap = clip<int>(ms[0].asInt(), 1, 100);
    ;
}

void ArrayStretch::setupSoundTouch()
{
    const uint SR = sys_getsr() ? sys_getsr() : 44100;

    soundtouch_->setSampleRate(SR);
    soundtouch_->setChannels(1);

    soundtouch_->setTempoChange(tempo_->value());
    soundtouch_->setPitchSemiTones(pitch_->value());
    soundtouch_->setRate(rate_->value());
}

void setup_array_stretch()
{
    PROP_SEQUENCE = gensym("@sequence");
    PROP_SEEK_WINDOW = gensym("@seekwindow");
    PROP_OVERLAP = gensym("@overlap");
    PROP_ANTIALIAS = gensym("@antialias");
    PROP_ANTIALIAS_LENGTH = gensym("@aalength");
    PROP_SPEECH = gensym("@speech");

    ObjectFactory<ArrayStretch> obj("array.stretch");
    soundtouch::SoundTouch st;
    LIB_DBG << "SoundTouch " << st.getVersionString();
}

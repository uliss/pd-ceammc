#include "array_stretch.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "SoundTouch.h"

static t_symbol* PROP_SEQUENCE = gensym("@sequence");
static t_symbol* PROP_SEEK_WINDOW = gensym("@seekwindow");
static t_symbol* PROP_OVERLAP = gensym("@overlap");
static t_symbol* PROP_ANTIALIAS = gensym("@antialias");
static t_symbol* PROP_ANTIALIAS_LENGTH = gensym("@aalength");

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
    , src_array_name_(positionalSymbolArgument(0))
    , dest_array_name_(positionalSymbolArgument(1))
    , speech_(false)
    , pitch_(0)
    , tempo_(0)
    , rate_(0)
    , soundtouch_(new PdSoundTouch())
{
    createOutlet();

    pitch_ = new FloatProperty("@pitch", 0);
    tempo_ = new FloatProperty("@tempo", 0);
    rate_ = new FloatProperty("@rate", 1);

    createProperty(pitch_);
    createProperty(tempo_);
    createProperty(rate_);

    if (src_array_name_)
        src_array_.open(src_array_name_);

    if (dest_array_name_)
        dest_array_.open(dest_array_name_);

    createCbProperty("@src", &ArrayStretch::propSrcArray, &ArrayStretch::propSetSrcArray);
    createCbProperty("@dest", &ArrayStretch::propDestArray, &ArrayStretch::propSetDestArray);

    // time-stretch params
    // @sequence
    createCbProperty(PROP_SEQUENCE->s_name,
        &ArrayStretch::propSequence,
        &ArrayStretch::propSetSequence);

    // @seekwindow
    createCbProperty(PROP_SEEK_WINDOW->s_name,
        &ArrayStretch::propSeekWindow,
        &ArrayStretch::propSetSeekWindow);

    // @overlap
    createCbProperty(PROP_OVERLAP->s_name,
        &ArrayStretch::propOverlap,
        &ArrayStretch::propSetOverlap);

    // pitch-shift params
    // antialias
    createCbProperty(PROP_ANTIALIAS->s_name,
        &ArrayStretch::propAnitAlias,
        &ArrayStretch::propSetAntiAlias);

    // antialias length
    createCbProperty(PROP_ANTIALIAS_LENGTH->s_name,
        &ArrayStretch::propAnitAliasLength,
        &ArrayStretch::propSetAntiAliasLength);

    // optimise for speech
    createCbProperty("@speech",
        &ArrayStretch::propSpeech,
        &ArrayStretch::propSetSpeech);
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

AtomList ArrayStretch::propSrcArray() const
{
    return AtomList(src_array_name_ ? src_array_name_ : Atom());
}

AtomList ArrayStretch::propDestArray() const
{
    return AtomList(dest_array_name_ ? dest_array_name_ : Atom());
}

void ArrayStretch::propSetSrcArray(const AtomList& l)
{
    if (l.empty() || !l[0].isSymbol()) {
        OBJ_ERR << "source array name required";
        return;
    }

    setSrcArray(l[0].asSymbol());
}

void ArrayStretch::propSetDestArray(const AtomList& l)
{
    if (l.empty() || !l[0].isSymbol()) {
        OBJ_ERR << "destination array name required";
        return;
    }

    setDestArray(l[0].asSymbol());
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

AtomList ArrayStretch::propAnitAlias() const
{
    return Atom(soundtouch_->getSetting(SETTING_USE_AA_FILTER));
}

void ArrayStretch::propSetAntiAlias(const AtomList& l)
{
    if (!checkArgs(l, ARG_BOOL, gensym("@antialias")))
        return;

    soundtouch_->setSetting(SETTING_USE_AA_FILTER, l[0].asInt());
}

AtomList ArrayStretch::propAnitAliasLength() const
{
    return Atom(soundtouch_->getSetting(SETTING_AA_FILTER_LENGTH));
}

void ArrayStretch::propSetAntiAliasLength(const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT, PROP_ANTIALIAS_LENGTH))
        return;

    int len = clip<int>(l[0].asInt(), 8, 128);
    soundtouch_->setSetting(SETTING_AA_FILTER_LENGTH, len);
}

AtomList ArrayStretch::propSequence() const
{
    return Atom(soundtouch_->getSetting(SETTING_SEQUENCE_MS));
}

void ArrayStretch::propSetSequence(const AtomList& ms)
{
    if (!checkArgs(ms, ARG_FLOAT, PROP_SEQUENCE))
        return;

    int sequence = clip<int>(ms[0].asInt(), 0, 100);
    soundtouch_->setSetting(SETTING_SEQUENCE_MS, sequence);
}

AtomList ArrayStretch::propSeekWindow() const
{
    return Atom(soundtouch_->getSetting(SETTING_SEEKWINDOW_MS));
}

void ArrayStretch::propSetSeekWindow(const AtomList& ms)
{
    if (!checkArgs(ms, ARG_FLOAT, PROP_SEEK_WINDOW))
        return;

    int sw = clip<int>(ms[0].asInt(), 0, 100);
    soundtouch_->setSetting(SETTING_SEEKWINDOW_MS, sw);
}

AtomList ArrayStretch::propOverlap() const
{
    return Atom(soundtouch_->getSetting(SETTING_OVERLAP_MS));
}

void ArrayStretch::propSetOverlap(const AtomList& ms)
{
    if (!checkArgs(ms, ARG_FLOAT, PROP_OVERLAP))
        return;

    int overlap = clip<int>(ms[0].asInt(), 1, 100);
    soundtouch_->setSetting(SETTING_OVERLAP_MS, overlap);
}

AtomList ArrayStretch::propSpeech() const
{
    return Atom(speech_ ? 1 : 0);
}

void ArrayStretch::propSetSpeech(const AtomList& l)
{
    if (l.empty())
        speech_ = true;
    else
        speech_ = l[0].asInt(0) != 0;

    if (speech_) {
        soundtouch_->setSetting(SETTING_SEQUENCE_MS, 40);
        soundtouch_->setSetting(SETTING_SEEKWINDOW_MS, 15);
        soundtouch_->setSetting(SETTING_OVERLAP_MS, 8);
    }
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

extern "C" void setup_array0x2estretch()
{
    ObjectFactory<ArrayStretch> obj("array.stretch");
    soundtouch::SoundTouch st;
    LIB_DBG << " SoundTouch " << st.getVersionString();
}

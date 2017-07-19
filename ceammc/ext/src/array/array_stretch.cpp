#include "array_stretch.h"
#include "ceammc_factory.h"

#include "SoundTouch.h"

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
    , pitch_(0)
    , tempo_(0)
    , rate_(0)
    , speech_(0)
    , anti_alias_(0)
    , soundtouch_(new PdSoundTouch())
{
    createOutlet();

    pitch_ = new FloatProperty("@pitch", 0);
    tempo_ = new FloatProperty("@tempo", 0);
    rate_ = new FloatProperty("@rate", 1);
    speech_ = new FlagProperty("@speech");
    anti_alias_ = new BoolProperty("@anti_alias", 1);

    createProperty(pitch_);
    createProperty(tempo_);
    createProperty(rate_);
    createProperty(speech_);

    if (src_array_name_)
        src_array_.open(src_array_name_);

    if (dest_array_name_)
        dest_array_.open(dest_array_name_);

    createCbProperty("@src", &ArrayStretch::propSrcArray, &ArrayStretch::propSetSrcArray);
    createCbProperty("@dest", &ArrayStretch::propDestArray, &ArrayStretch::propSetDestArray);
}

void ArrayStretch::onBang()
{
    soundtouch_->clear();

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

void ArrayStretch::setupSoundTouch()
{
    const uint SR = sys_getsr() ? sys_getsr() : 44100;

    soundtouch_->setSampleRate(SR);
    soundtouch_->setChannels(1);

    soundtouch_->setTempoChange(tempo_->value());
    soundtouch_->setPitchSemiTones(pitch_->value());
    soundtouch_->setRate(rate_->value());

    soundtouch_->setSetting(SETTING_USE_AA_FILTER, anti_alias_->value() ? 1 : 0);

    if (speech_->value()) {
        // use settings for speech processing
        soundtouch_->setSetting(SETTING_SEQUENCE_MS, 40);
        soundtouch_->setSetting(SETTING_SEEKWINDOW_MS, 15);
        soundtouch_->setSetting(SETTING_OVERLAP_MS, 8);
    }
}

extern "C" void setup_array0x2estretch()
{
    ObjectFactory<ArrayStretch> obj("array.stretch");
    soundtouch::SoundTouch st;
    LIB_DBG << " SoundTouch " << st.getVersionString();
}

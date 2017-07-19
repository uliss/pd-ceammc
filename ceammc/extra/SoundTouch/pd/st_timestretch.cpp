#include "st_timestretch.h"
#include "ceammc_factory.h"

#include <vector>

TimeStretch::TimeStretch(const PdArgs& a)
    : BaseObject(a)
    , pitch_(0)
    , tempo_(0)
    , rate_(0)
    , speech_(0)
{
    createOutlet();

    const int argn = positionalArguments().size();
    if (argn > 0)
        array_src_.open(positionalSymbolArgument(0));

    if (argn > 1)
        array_dest_.open(positionalSymbolArgument(1));

    pitch_ = new FloatProperty("@pitch", 1.f);
    tempo_ = new FloatProperty("@tempo", 1.f);
    rate_ = new FloatProperty("@rate", 1.f);
    speech_ = new FlagProperty("@speech");

    createProperty(pitch_);
    createProperty(tempo_);
    createProperty(rate_);
    createProperty(speech_);
}

void TimeStretch::onBang()
{
    tstretch_.clear();

    if (!array_src_.update())
        return;
    if (!array_dest_.update())
        return;

    tstretch_.setSampleRate(44100);
    tstretch_.setChannels(1);

    tstretch_.setTempoChange(tempo_->value());
    tstretch_.setPitchSemiTones(pitch_->value());
    tstretch_.setRateChange(rate_->value());

    tstretch_.setSetting(SETTING_USE_AA_FILTER, 0);

    if (speech_->value()) {
        // use settings for speech processing
        tstretch_.setSetting(SETTING_SEQUENCE_MS, 40);
        tstretch_.setSetting(SETTING_SEEKWINDOW_MS, 15);
        tstretch_.setSetting(SETTING_OVERLAP_MS, 8);
    }

    const size_t IN_N = array_src_.size();
    std::vector<t_sample> output;
    output.reserve(IN_N);
    float buf[4096];

    OBJ_DBG << "input samples: " << IN_N;

    for (size_t i = 0; i < (IN_N - 256); i += 256) {
        float in_buf[256];
        for (size_t j = 0; j < 256; j++)
            in_buf[j] = array_src_[i + j];

        tstretch_.putSamples(in_buf, 256);

        uint n = 0;
        do {
            n = tstretch_.receiveSamples(buf, 4096);
            output.insert(output.end(), buf, buf + n);
            //            OBJ_DBG << "   ... append " << n << "samples";
        } while (n != 0);

        //        if ((i + 256) > IN_N)
        //            l = IN_N - i;
    }

    tstretch_.flush();
    uint n = 0;
    do {
        n = tstretch_.receiveSamples(buf, 4096);
        output.insert(output.end(), buf, buf + n);
    } while (n != 0);

    const size_t OUT_N = output.size();
    if (OUT_N < 1) {
        OBJ_ERR << "no output";
        return;
    }

    array_dest_.resize(OUT_N);
    for (size_t i = 0; i < OUT_N; i++) {
        array_dest_[i] = output[i];
    }

    array_dest_.redraw();
    floatTo(0, output.size());
}

extern "C" void timestretch_setup()
{
    ObjectFactory<TimeStretch> obj("timestretch");
}

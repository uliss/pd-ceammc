#include "an_pitchtrack.h"
#include "ceammc_factory.h"

PitchTrack::PitchTrack(const PdArgs& args)
    : SoundExternal(args)
    , freq_(0)
    , fidelity_(0)
    , framesize_(nullptr)
    , overlap_(nullptr)
    , bias_(nullptr)
    , maxfreq_(nullptr)
    , fidelity_threshold_(nullptr)
    , helmholtz_(new Helmholtz)
    , clock_(this, &PitchTrack::tick)
{
    createSignalOutlet();
    createOutlet();
    createOutlet();

    initProperties();
}

void PitchTrack::initProperties()
{
    constexpr int DEFAULT_FRAME_SIZE = 1024;
    framesize_ = new IntEnumProperty("@framesize", { DEFAULT_FRAME_SIZE, 128, 256, 512, 2048 });
    framesize_->setUnitsSamp();
    framesize_->setArgIndex(0);
    framesize_->setSuccessFn([this](Property*) { helmholtz_->setframesize(framesize_->value()); });
    addProperty(framesize_);

    constexpr int DEFAULT_OVERLAP = 1;
    overlap_ = new IntEnumProperty("@overlap", { DEFAULT_OVERLAP, 2, 4, 8 });
    overlap_->setSuccessFn([this](Property*) { helmholtz_->setoverlap(overlap_->value()); });
    addProperty(overlap_);

    bias_ = new FloatProperty("@bias", 0.2);
    bias_->checkClosedRange(0, 1);
    bias_->setSuccessFn([this](Property*) { helmholtz_->setbias(bias_->value()); });
    addProperty(bias_);

    // default maximum frequency for reporting pitch result
    constexpr t_float DEF_MAXFREQ = 1500;
    maxfreq_ = new FloatProperty("@maxfreq", DEF_MAXFREQ);
    constexpr t_float MIN_MAXFREQ = 10;
    constexpr t_float MAX_MAXFREQ = 10000;
    maxfreq_->checkClosedRange(MIN_MAXFREQ, MAX_MAXFREQ);
    maxfreq_->setUnitsHz();
    addProperty(maxfreq_);

    constexpr t_float DEFFIDELITY = 0.95; // default fidelity threshold for reporting pitch result
    fidelity_threshold_ = new FloatProperty("@fidthr", DEFFIDELITY);
    fidelity_threshold_->checkClosedRange(0, 1);
    addProperty(fidelity_threshold_);
}

void PitchTrack::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();
    const auto SR = samplerate();

    // send and receive one block of samples
    helmholtz_->iosamples((t_float*)in[0], out[0], BS);

    // get freq and fidelity values
    t_float freq = SR / helmholtz_->getperiod();
    t_float fidelity = helmholtz_->getfidelity();

    // if there is new pitch info, send messages to outlets
    if ((freq_ != freq)
        && (fidelity > fidelity_threshold_->value())
        && (freq < maxfreq_->value())) {
        freq_ = freq;
        fidelity_ = fidelity;
        clock_.delay(0);
    }
}

void PitchTrack::tick()
{
    floatTo(2, fidelity_);
    floatTo(1, freq_);
}

void setup_an_pitchtrack()
{
    SoundExternalFactory<PitchTrack> obj("an.pitchtrack~");
    obj.addAlias("helmholtz~");

    obj.setXletsInfo({ "input signal" }, { "output signal", "float: freq in hz", "float: fidelity" });
}

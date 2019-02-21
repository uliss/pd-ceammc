#include "an_pitchtrack.h"
#include "ceammc_factory.h"

#define DEFFIDELITY 0.95 // default fidelity threshold for reporting pitch result
#define DEFMAXFREQ 1500. // default maximum frequency for reporting pitch result

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
    auto p_framesize = new IntEnumProperty("@framesize", 1024);
    p_framesize->appendEnum(128);
    p_framesize->appendEnum(256);
    p_framesize->appendEnum(512);
    p_framesize->appendEnum(2048);
    p_framesize->set(Atom(positionalFloatArgument(0, 1024)));

    framesize_ = new PitchTrackEnumProperty(p_framesize, this, [](PitchTrack* this_, IntEnumProperty* p) {
        LIB_DBG << "setting frame size to: " << p->value();
        this_->helmholtz_->setframesize(p->value());
    });
    framesize_->info().setUnits(PropertyInfoUnits::SAMP);
    createProperty(framesize_);

    auto p_overlap = new IntEnumProperty("@overlap", 1);
    p_overlap->appendEnum(2);
    p_overlap->appendEnum(4);
    p_overlap->appendEnum(8);
    p_overlap->set(Atom(positionalFloatArgument(1, 1)));

    overlap_ = new PitchTrackEnumProperty(p_overlap, this, [](PitchTrack* this_, IntEnumProperty* p) {
        LIB_DBG << "setting overlap to: " << p->value();
        this_->helmholtz_->setoverlap(p->value());
    });
    createProperty(overlap_);

    bias_ = new PitchTrackFloatProperty(new FloatPropertyClosedRange("@bias", 0.2, 0, 1), this, [](PitchTrack* this_, FloatPropertyClosedRange* p) {
        LIB_DBG << "setting bias to: " << p->value();
        this_->helmholtz_->setbias(p->value());
    });
    createProperty(bias_);

    maxfreq_ = new FloatPropertyClosedRange("@maxfreq", DEFMAXFREQ, 10, 10000);
    maxfreq_->info().setUnits(PropertyInfoUnits::HZ);
    createProperty(maxfreq_);

    fidelity_threshold_ = new FloatPropertyClosedRange("@fidthr", DEFFIDELITY, 0, 1);
    createProperty(fidelity_threshold_);
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
}

#include "st_bpm.h"
#include "ceammc_factory.h"

SoundTouchBPMDetect::SoundTouchBPMDetect(const PdArgs& a)
    : SoundExternal(a)
    , detector_(0)
    , on_(false)
{
    createOutlet();
    detector_ = new soundtouch::BPMDetect(1, sys_getsr());
}

SoundTouchBPMDetect::~SoundTouchBPMDetect()
{
    delete detector_;
}

void SoundTouchBPMDetect::onBang()
{
    floatTo(0, detector_->getBpm());
}

void SoundTouchBPMDetect::processBlock(const t_sample** in, t_sample** out)
{
    if (!on_)
        return;

    detector_->inputSamples(in[0], blockSize());
}

void SoundTouchBPMDetect::m_on(t_symbol*, const AtomList& v)
{
    if (v.empty()) {
        on_ = true;
    } else {
        on_ = (v[0].asInt(0) != 0);
    }
}

void SoundTouchBPMDetect::m_off(t_symbol*, const AtomList&)
{
    on_ = false;
}

extern "C" void setup_soundtouch0x2ebpm_tilde()
{
    SoundExternalFactory<SoundTouchBPMDetect> obj("soundtouch.bpm~");
    obj.addMethod("on", &SoundTouchBPMDetect::m_on);
    obj.addMethod("off", &SoundTouchBPMDetect::m_off);
}

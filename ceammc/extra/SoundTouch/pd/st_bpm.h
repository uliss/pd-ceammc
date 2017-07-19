#ifndef ST_BPM_H
#define ST_BPM_H

#include "BPMDetect.h"
#include "ceammc_object.h"
using namespace ceammc;

class SoundTouchBPMDetect : public SoundExternal {
    soundtouch::BPMDetect* detector_;
    bool on_;

public:
    SoundTouchBPMDetect(const PdArgs& a);
    ~SoundTouchBPMDetect();
    void onBang();

    void processBlock(const t_sample** in, t_sample** out);
    void m_on(t_symbol*, const AtomList& v);
    void m_off(t_symbol*, const AtomList&);
};

#endif // ST_BPM_H

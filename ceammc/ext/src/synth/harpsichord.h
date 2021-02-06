//Parameters for harpsichord.dsp
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: FAUST-STK

#ifndef FAUST_SYNTH_HARPSICHORD_H_
#define FAUST_SYNTH_HARPSICHORD_H_

namespace faust {
namespace synth {
    namespace harpsichord {

        float getValueDryTapAmpT60(float index);
        float getValueReleaseLoopGain(float index);
        float getValueLoopFilterb0(float index);
        float getValueLoopFilterb1(float index);
        float getValueLoopFilterb2(float index);
        float getValueLoopFiltera1(float index);
        float getValueLoopFiltera2(float index);

    }
}
}

using namespace faust::synth::harpsichord;

#endif

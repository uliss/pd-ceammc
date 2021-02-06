//Parameters for piano.dsp
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: FAUST-STK

#ifndef FAUST_SYNTH_PIANO_H_
#define FAUST_SYNTH_PIANO_H_

#include "instrument.h"

namespace faust {
namespace synth {
    namespace piano {

        float getValueDryTapAmpT60(float index);
        float getValueSustainPedalLevel(float index);
        float getValueLoudPole(float index);
        float getValuePoleValue(float index);
        float getValueLoudGain(float index);
        float getValueSoftGain(float index);
        float getValueDCBa1(float index);
        float getValuer1_1db(float index);
        float getValuer1_2db(float index);
        float getValuer2db(float index);
        float getValuer3db(float index);
        float getValueSecondStageAmpRatio(float index);
        float getValueSecondPartialFactor(float index);
        float getValueThirdPartialFactor(float index);
        float getValueBq4_gEarBalled(float index);
        float getValueStrikePosition(float index);
        float getValueEQBandWidthFactor(float index);
        float getValueEQGain(float index);
        float getValueDetuningHz(float index);
        float getValueSingleStringDecayRate(float index);
        float getValueSingleStringZero(float index);
        float getValueSingleStringPole(float index);
        float getValueStiffnessCoefficient(float index);
        float getValueReleaseLoopGain(float index);
        float getValueLoopFilterb0(float index);
        float getValueLoopFilterb1(float index);
        float getValueLoopFilterb2(float index);
        float getValueLoopFiltera1(float index);
        float getValueLoopFiltera2(float index);
        float getValueBassLoopFilterb0(float index);
        float getValueBassLoopFilterb1(float index);
        float getValueBassLoopFiltera1(float index);
    }
}
}

using namespace faust::synth::piano;

#endif

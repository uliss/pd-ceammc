//Parameters for piano.dsp
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: FAUST-STK

#ifndef FAUST_SYNTH_PIANO_H_
#define FAUST_SYNTH_PIANO_H_

#include "instrument.h"

#define _LOOKUP_TABLE_H_

//**********************************************************************

namespace faust {
namespace synth {

    extern LookupTable noteOffDelayTime;

    /* pianoDriverC.sb */
    /* Coupling Filter */

    extern const LookupTable singleStringDecayRate;
    extern const LookupTable singleStringZero;
    extern const LookupTable singleStringPole;
    extern const LookupTable releaseLoopGain;
    extern const LookupTable detuningHz;
    extern const LookupTable stiffnessCoefficient;
    extern const LookupTable strikePosition;
    extern const LookupTable EQGain;
    extern const LookupTable EQBandwidthFactor;

    /* PianoDriverA */
    /* HammerFilter */
    extern const LookupTable loudPole;
    extern const LookupTable softPole;
    extern const LookupTable normalizedVelocity;
    extern const LookupTable loudGain;
    extern const LookupTable softGain;

    /* Soundboard */
    extern const LookupTable sustainPedalLevel;
    extern const LookupTable DryTapAmpT60;
    extern const LookupTable DCBa1;
    constexpr double DryTapAmpCurrent = 0.15;

    /* pianoDriverB */
    /* High Notes */
    extern const LookupTable secondStageAmpRatio;
    extern const LookupTable r1_1db;
    extern const LookupTable r1_2db;
    extern const LookupTable r2db;
    extern const LookupTable r3db;
    extern const LookupTable secondPartialFactor;
    extern const LookupTable thirdPartialFactor;
    extern const LookupTable bq4_gEarBalled;

    // harpsichord
    extern const LookupTable loopFilterb0;
    extern const LookupTable loopFilterb1;
    extern const LookupTable loopFilterb2;
    extern const LookupTable loopFiltera1;
    extern const LookupTable loopFiltera2;

    // bass
    extern const LookupTable bassLoopFilterb1;
    extern const LookupTable bassLoopFilterb0;
    extern const LookupTable bassLoopFiltera1;
}
}

namespace {

float getValueDryTapAmpT60(float index)
{
    using namespace faust::synth;
    return DryTapAmpT60.getValue(index);
}

float getValueSustainPedalLevel(float index)
{
    using namespace faust::synth;
    return sustainPedalLevel.getValue(index);
}

float getValueLoudPole(float index)
{
    using namespace faust::synth;
    return loudPole.getValue(index);
}

float getValuePoleValue(float index)
{
    using namespace faust::synth;
    return softPole.getValue(index);
}

float getValueLoudGain(float index)
{
    using namespace faust::synth;
    return loudGain.getValue(index);
}

float getValueSoftGain(float index)
{
    using namespace faust::synth;
    return softGain.getValue(index);
}

float getValueDCBa1(float index)
{
    using namespace faust::synth;
    return DCBa1.getValue(index);
}

float getValuer1_1db(float index)
{
    using namespace faust::synth;
    return r1_1db.getValue(index);
}

float getValuer1_2db(float index)
{
    using namespace faust::synth;
    return r1_2db.getValue(index);
}

float getValuer2db(float index)
{
    using namespace faust::synth;
    return r2db.getValue(index);
}

float getValuer3db(float index)
{
    using namespace faust::synth;
    return r3db.getValue(index);
}

float getValueSecondStageAmpRatio(float index)
{
    using namespace faust::synth;
    return secondStageAmpRatio.getValue(index);
}

float getValueSecondPartialFactor(float index)
{
    using namespace faust::synth;
    return secondPartialFactor.getValue(index);
}

float getValueThirdPartialFactor(float index)
{
    using namespace faust::synth;
    return thirdPartialFactor.getValue(index);
}

float getValueBq4_gEarBalled(float index)
{
    using namespace faust::synth;
    return bq4_gEarBalled.getValue(index);
}

float getValueStrikePosition(float index)
{
    using namespace faust::synth;
    return strikePosition.getValue(index);
}

float getValueEQBandWidthFactor(float index)
{
    using namespace faust::synth;
    return EQBandwidthFactor.getValue(index);
}

float getValueEQGain(float index)
{
    using namespace faust::synth;
    return EQGain.getValue(index);
}

float getValueDetuningHz(float index)
{
    using namespace faust::synth;
    return detuningHz.getValue(index);
}

float getValueSingleStringDecayRate(float index)
{
    using namespace faust::synth;
    return singleStringDecayRate.getValue(index);
}

float getValueSingleStringZero(float index)
{
    using namespace faust::synth;
    return singleStringZero.getValue(index);
}

float getValueSingleStringPole(float index)
{
    using namespace faust::synth;
    return singleStringPole.getValue(index);
}

float getValueStiffnessCoefficient(float index)
{
    using namespace faust::synth;
    return stiffnessCoefficient.getValue(index);
}

float getValueReleaseLoopGain(float index)
{
    using namespace faust::synth;
    return releaseLoopGain.getValue(index);
}

float getValueLoopFilterb0(float index)
{
    using namespace faust::synth;
    return loopFilterb0.getValue(index);
}

float getValueLoopFilterb1(float index)
{
    using namespace faust::synth;
    return loopFilterb1.getValue(index);
}

float getValueLoopFilterb2(float index)
{
    using namespace faust::synth;
    return loopFilterb2.getValue(index);
}

float getValueLoopFiltera1(float index)
{
    using namespace faust::synth;
    return loopFiltera1.getValue(index);
}

float getValueLoopFiltera2(float index)
{
    using namespace faust::synth;
    return loopFiltera2.getValue(index);
}

float getValueBassLoopFilterb0(float index)
{
    using namespace faust::synth;
    return bassLoopFilterb0.getValue(index);
}

float getValueBassLoopFilterb1(float index)
{
    using namespace faust::synth;
    return bassLoopFilterb1.getValue(index);
}

float getValueBassLoopFiltera1(float index)
{
    using namespace faust::synth;
    return bassLoopFiltera1.getValue(index);
}
}

#endif

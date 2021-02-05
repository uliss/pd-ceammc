//Parameters for harpsichord.dsp
//©Romain Michon (rmichon@ccrma.stanford.edu), 2011
//licence: FAUST-STK

#ifndef FAUST_SYNTH_HARPSICHORD_H_
#define FAUST_SYNTH_HARPSICHORD_H_

float harpsichordGetValueDryTapAmpT60(float index);
float harpsichordGetValueReleaseLoopGain(float index);
float harpsichordGetValueLoopFilterb0(float index);
float harpsichordGetValueLoopFilterb1(float index);
float harpsichordGetValueLoopFilterb2(float index);
float harpsichordGetValueLoopFiltera1(float index);
float harpsichordGetValueLoopFiltera2(float index);

#endif

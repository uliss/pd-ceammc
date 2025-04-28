declare name "fx.granulator";
declare version "2.8.1";
declare author "Lukas Hartmann & Luca Hilbrich";

// declare options "[osc:on]";
// declare options "[midi:on]";
import("stdfaust.lib");

// USER INTERFACE //

// volumeSlider = vslider("h:/h:[0]Granulator/ [0]Input Gain [midi:ctrl 0]", 0.5, 0, 1, 0.01); // volume slider
// windowSlider = vslider("h:/h:[0]Granulator/ [1]Window [style:radio{'Sine Window':window1;'Hamming Window':window2;'Flat Top Window':window3}] [midi:ctrl 1]",1,1,3,1);
// pitchSliderA = vslider("h:/h:[0]Granulator/ [2] Pitch Alpha [midi:ctrl 2]", 1, -2, 2, 0.01); // pitch control (downsampling)
// pitchSliderB = vslider("h:/h:[0]Granulator/ [2] Pitch Beta [midi:ctrl 3]", 1, -2, 2, 0.01); // pitch control (downsampling)

// grainAmountSlider =  vslider("h:/h:[1]Grain/ [1] Density [midi:ctrl 4]", 1, 1, MAXN, 1); // number of grains
// grainbufferSizeSlider = vslider("h:/h:[1]Grain/ [2] Size [midi:ctrl 5]", 0.1, 0.01, 0.5, 0.01); // grain size in samples
// delayLengthSlider = vslider("h:/h:[1]Grain/ [3] Delay [midi:ctrl 6]", 2, 0.5, 2, 0.1); // delay length in samples
// positionspreadSlider = vslider("h:/h:[1]Grain/ [4] Spread [midi:ctrl 7]", 1, 0.1, 2, 0.1); // position spread of grains

// dry_wetA = vslider("h:/v:[2]Alpha/ [0]Reverb Dry/Wet[style:knob] [midi:ctrl 16]", 0.0, 0, 1, 0.001);
// filter_cuttoffA = vslider("h:/v:[2]Alpha/ [1]HPF[unit:Hz][style:knob] [midi:ctrl 17]", 100, 100, 10000, 1);
// volumeSliderAL = vslider("h:/v:[2]Alpha/ [2]GainL[style:knob] [midi:ctrl 18]", 1.0, 0, 1, 0.01);
// volumeSliderAR = vslider("h:/v:[2]Alpha/ [3]GainR[style:knob] [midi:ctrl 19]", 1.0, 0, 1, 0.01);

// dry_wetB = vslider("h:/v:[3]Beta/ [0]Reverb Dry/Wet[style:knob] [midi:ctrl 20]", 0.0, 0, 1, 0.001);
// filter_cuttoffB = vslider("h:/v:[3]Beta/ [1]HPF[unit:Hz][style:knob] [midi:ctrl 21]", 100, 100, 10000, 1);
// volumeSliderBL = vslider("h:/v:[3]Beta/ [2]GainL[style:knob] [midi:ctrl 22]", 1.0, 0, 1, 0.01);
// volumeSliderBR = vslider("h:/v:[3]Beta/ [3]GainR[style:knob] [midi:ctrl 23]", 1.0, 0, 1, 0.01);

// UI CEAMMC //

// volumeSlider = vslider("input_gain", 0.5, 0, 1, 0.01); // volume slider
// windowSlider = vslider("window [style:radio{'Sine Window':window1;'Hamming Window':window2;'Flat Top Window':window3}]",1,1,3,1);
pitchSliderA = vslider("pitchA", 1, -2, 2, 0.01); // pitch control (downsampling)
pitchSliderB = vslider("pitchB", 1, -2, 2, 0.01);  // pitch control (downsampling)

grainAmountSlider     = vslider("density", 1, 1, MAXN, 1);     // number of grains
grainbufferSizeSlider = vslider("size", 0.1, 0.01, 0.5, 0.01); // grain size in samples
delayLengthSlider     = vslider("delay", 2, 0.5, 2, 0.1);      // delay length in samples
positionspreadSlider  = vslider("spread", 1, 0.1, 2, 0.1);      // position spread of grains

dry_wetA        = vslider("drywetA [style:knob]", 0.0, 0, 1, 0.001);
filter_cuttoffA = vslider("hpfA    [unit:hz][style:knob]", 100, 50, 10000, 1);
volumeSliderAL  = vslider("gainAL  [style:knob]", 0.8, 0, 1, 0.01);
volumeSliderAR  = vslider("gainAR  [style:knob]", 0.2, 0, 1, 0.01);

dry_wetB        = vslider("drywetB [style:knob]", 0.0, 0, 1, 0.001);
filter_cuttoffB = vslider("hpfB    [unit:hz][style:knob]", 100, 50, 10000, 1);
volumeSliderBL  = vslider("gainBL  [style:knob]", 0.2, 0, 1, 0.01);
volumeSliderBR  = vslider("gainBR  [style:knob]", 0.8, 0, 1, 0.01);


// CODE //

SR = 44100; // samplerate in samples per second
MAXN = 16; // maximum numbers of grains
Volume = volumeSlider; // volume slider

Gain1 = 0.8; // gain of channel1 Alpha
Gain2 = 0.85; // gain of channel1 Beta
Gain3 = 0.9; // gain of channel2 Alpha
Gain4 = 0.95; // gain of channel2 Beta

bufferSize = 3 * SR; // size of input buffer in samples
bufferCounter = + (1) % delayLength ~ _; // counter to cycle through the input buffer from 0 to bufferSize
delayLength = SR * delayLengthSlider; // set delay length with delay length slider

grainbufferSize = SR * grainbufferSizeSlider; // size of grainbuffer in samples

grainbufferCounterA = + (pitchSliderA) % grainbufferSize ~ _; // counter to cycle through the grains from 0 to grainSize
grainbufferCounterB = + (pitchSliderB) % grainbufferSize ~ _; // counter to cycle through the grains from 0 to grainSize

SH(trigger, signal) = ( * (1 - trigger) + signal * trigger) ~ _; // sample and hold function definiton for grain offset
grainOffset(i) = int(SH(1 - 1', int(delayLength * noise(i)))); // delay length between grainbuffer refresh

grainCounterA(i) = (grainbufferCounterA + grainOffset(i)) % grainbufferSize; // grain-specific grain counter (Alpha)
grainCounterB(i) = (grainbufferCounterB + grainOffset(i)) % grainbufferSize; // grain-specific grain counter (Beta)

grainRandomStartPosA(i) = int(SH(int(grainCounterA(i) / (grainbufferSize - 1)), int(delayLength * noise(i))));
grainRandomStartPosB(i) = int(SH(int(grainCounterB(i) / (grainbufferSize - 1)), int(delayLength * noise(i))));

grainPositionA(i) = grainCounterA(i) + grainRandomStartPosA(i);
grainPositionB(i) = grainCounterB(i) + grainRandomStartPosB(i);

buffer(writeIndex, readIndex, signal) = rwtable(bufferSize, 0.0, int(writeIndex % delayLength), signal, int(readIndex % delayLength)); // function definition of cycling buffer

window1A(i) = sin(2 * 3.14159 * grainCounterA(i) / (grainbufferSize - 1)); // window function
window2A(i) = 0.54 - 0.64 * cos(2 * 3.14159 * grainCounterA(i) / grainbufferSize - 1); // hamming window
window3A(i) = 1 - 1.93 * cos(2 * 3.14159 * grainCounterA(i) / grainbufferSize - 1) + 1.29 * cos(4 * 3.14159 * grainCounterA(i) / grainbufferSize - 1) - 0.388 * cos(6 * 3.14159 * grainCounterA(i) / grainbufferSize - 1) + 0.028 * cos(8 * 3.14159 * grainCounterA(i) / grainbufferSize - 1); // flat top window

window1B(i) = sin(2 * 3.14159 * grainCounterB(i) / (grainbufferSize - 1)); // window function
window2B(i) = 0.54 - 0.64 * cos(2 * 3.14159 * grainCounterB(i) / grainbufferSize - 1); // hamming window
window3B(i) = 1 - 1.93 * cos(2 * 3.14159 * grainCounterB(i) / grainbufferSize - 1) + 1.29 * cos(4 * 3.14159 * grainCounterB(i) / grainbufferSize - 1) - 0.388 * cos(6 * 3.14159 * grainCounterB(i) / grainbufferSize - 1) + 0.028 * cos(8 * 3.14159 * grainCounterB(i) / grainbufferSize - 1); // flat top window


// NOISE GENERATOR //

RANDMAX = (2147483647.0) * 2 ; // max number of random number generatorn

polyRandom(n) = scramble(n) ~ _
    with {
        scramble (1) = * (1103515245) : + (12345); // pseudo random number generator
        scramble (n) = scramble(1) <: scramble(n - 1), _;
};

polyNoise(n) = polyRandom(n) : par(i, n, /(RANDMAX));

noiseChan(n, 0) = polyNoise(MAXN + 1) :> _, par(j, n - 1, !);
noiseChan(n, i) = polyNoise(MAXN + 1) :> par(j, i, !), _, par(j, n - i - 1, !);

noise(i) = (noiseChan(MAXN + 1, i) + 1) / 2 * positionspreadSlider; //to get nth channel of multi-channel noiser (not quite sure how)

// EFFECTS //

reverbA = ( * (dry_wetA) * fixedgain:re.mono_freeverb(fb1, fb2, damp, spread)), * (1 - dry_wetA)
    with {
        fb1 = alpha_fb1; // coefficient of the lowpass comb filters (0-1)
        fb2 = 0.5; // coefficient of the allpass comb filters (0-1)
        damp = 0.5; // damping of the lowpass comb filter (0-1)
        spread = 1; //spatial spread in number of samples (for stereo)
        fixedgain = 0.15;
    }; // mono with dry/wet implementation

hpf_filterA =  (fi.highpass(filter_order, filter_cuttoffA))
    with {
        filter_order = 1;
    };

reverbB = ( * (dry_wetB) * fixedgain:re.mono_freeverb(fb1, fb2, damp, spread)), * (1 - dry_wetB)
    with {
        fb1 = beta_fb1; // coefficient of the lowpass comb filters (0-1)
        fb2 = 0.5; // coefficient of the allpass comb filters (0-1)
        damp = 0.5; // damping of the lowpass comb filter (0-1)
        spread = 1; //spatial spread in number of samples (for stereo)
        fixedgain = 0.15;
    }; // mono with dry/wet implementation

hpf_filterB =  (fi.highpass(filter_order, filter_cuttoffB))
    with {
        filter_order = 1;
    };

// GRANULATORS //

granAlpha = par(i, MAXN, buffer(int(bufferCounter), int(grainPositionA(i)), _) * window1A(i) *
(i < grainAmountSlider) / grainAmountSlider);

granBeta = par(i, MAXN, buffer(int(bufferCounter), int(grainPositionB(i)), _) * window1B(i) *
(i < grainAmountSlider) / grainAmountSlider);

// PROCESS //

process =

_ <:

    ((granAlpha:>_*Gain1 : hpf_filterA * volumeSliderAL),(granBeta:>_*Gain2 : hpf_filterB * volumeSliderBL):>_),

    ((granAlpha:>_*Gain3 : hpf_filterA * volumeSliderAR),(granBeta:>_*Gain4 : hpf_filterB * volumeSliderBR):>_)

;

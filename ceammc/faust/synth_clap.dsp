declare name "synth.clap";

import("stdfaust.lib");
cm = library("ceammc.lib");

process = sy.clap(tone, attack, decay, gate) with {
    // bandpass filter cutoff frequency
    tone = vslider("tone [unit:Hz]", 3500, 400, 3500, 1) : si.smoo;
    // attack time in ms
    attack = vslider("attack [unit:ms]", 0, 0, 400, 1) * 0.001 : si.smoo;
    // decay time in ms
    decay = vslider("release [unit:ms]", 0, 0, 4000, 1) * 0.001 : si.smoo;
    gate  = checkbox("gate [type:float]");
};

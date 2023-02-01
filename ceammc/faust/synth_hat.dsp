declare name "synth.hat";

import("stdfaust.lib");
cm = library("ceammc.lib");

process = sy.hat(freq, tone, attack, decay, gate) with {
    // base frequency
    freq  = vslider("freq [unit:Hz]", 317, 300, 4000, 0.1) : si.smoo;
    // bandpass filter cutoff frequency
    tone = vslider("tone [unit:Hz]", 6400, 800, 18000, 1) : si.smoo;
    // attack time in ms
    attack = vslider("attack [unit:ms]", 20, 5, 400, 1) * 0.001 : si.smoo;
    // decay time in ms
    decay = vslider("release [unit:ms]", 100, 5, 4000, 1) * 0.001 : si.smoo;
    gate  = checkbox("gate [type:float]");
};

declare name "synth.kick2";

import("stdfaust.lib");
cm = library("ceammc.lib");

process = sy.kick(freq, click, attack, decay, drive, gate) with {
    freq  = vslider("freq [unit:hz]", 40, 10, 5000, 0.1) : si.smoo;
    // the speed of the pitch envelope
    click = vslider("click [unit:ms]", 20, 5, 1000, 1) * 0.001 : si.smoo;
    // attack time in ms
    attack = vslider("attack [unit:ms]", 10, 5, 400, 1) * 0.001 : si.smoo;
    // decay time in ms
    decay = vslider("release [unit:ms]", 500, 5, 4000, 1) * 0.001 : si.smoo;
    // a gain multiplier going into the saturator
    drive = vslider("drive", 1, 1, 10, 0.1);
    gate  = checkbox("gate [type:float]");
};

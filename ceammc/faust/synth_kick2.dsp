declare name "synth.kick2";

import("stdfaust.lib");
cm = library("ceammc.lib");

process = sy.kick(freq, click, attack, decay, drive, gate) with {
    freq  = vslider("freq [unit:Hz]", 44, 10, 5000, 0.1) : si.smoo;
    // the speed of the pitch envelope
    click = vslider("click [unit:ms]", 20, 5, 1000, 1) * 0.001 : si.smoo;
    // attack time in ms
    attack = vslider("attack [unit:ms]", 20, 5, 400, 1) * 0.001 : si.smoo;
    // decay time in ms
    decay = vslider("release [unit:ms]", 100, 5, 4000, 1) * 0.001 : si.smoo;
    // a gain multiplier going into the saturator
    drive = vslider("drive", 2.5, 1, 10, 0.1) : si.smoo;
    gate  = checkbox("gate [type:float]");
};

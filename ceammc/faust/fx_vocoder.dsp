declare name "fx.vocoder";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

effect(source, excitation) = ve.vocoder(nBands, att, rel, bwratio, source, excitation)
with {
    nBands = 32;
    att = hslider("attack [unit:ms]", 5, 0.1, 100, 0.01) : cm.time_pd2faust : si.smoo;
    rel = hslider("release [unit:ms]", 5, 0.1, 100, 0.01) : cm.time_pd2faust : si.smoo;
    bwratio = hslider("bwratio", 0.5, 0.1, 2, 0.01);
};

process = _ , _ : effect;

declare name "fx.pitchshift";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

sec2samp = *(ma.SR);

win = vslider("window [unit:ms]", 200, 10, 2000, 0.1) : cm.time_pd2faust : sec2samp;
fade = vslider("fade [unit:ms]", 100, 0, 1000, 0.1) : cm.time_pd2faust : sec2samp;
shift = vslider("pitch", 0, -38, 60, 0.001) : si.smoo;

fx = _ : ef.transpose(win, fade, shift) : _;
process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), fx) : _;

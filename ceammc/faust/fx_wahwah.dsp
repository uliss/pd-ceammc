declare name "fx.wahwah";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");

effect = _ : ve.crybaby(wah)
with {
    wah = hslider("angle", 0.6, 0, 1, 0.01) : ba.automat(bps, 15, 0.0);
    bps = hslider("speed", 540, 360, 780, 0.1) : si.smoo : int;
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), effect) : _;

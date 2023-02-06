declare name "fx.overdrive";
declare author "Serge Poltavski";
declare version "1.0";

import("stdfaust.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), overdrive) with {
    th = 1/3;
    fx_pos(x) = ba.if(x < th, 2*x, fx_pos2(x));
    fx_pos2(x) = ba.if(x < 2*th, (3-(2-(3*x))^2)/3, fx_pos3);
    fx_pos3 = 1;
    overdrive(in) = fx_pos(abs(in * drive)) * ma.signum(in);
    drive = vslider("drive", 1, 1, 5, 0.001) : si.smoo;
};

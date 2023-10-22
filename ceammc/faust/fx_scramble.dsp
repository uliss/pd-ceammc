declare name 	"fx.scramble";
declare author 	"Serge Poltavski";
declare version "0.1";

import("stdfaust.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

process = _ <: cm.fx_wrap2(ui.bypass, ui.drywet(0.5), fx)
with {
    MAX_LEN = 6 : ba.sec2samp;

    fx = scrambler(dt), scrambler(dt2);
    scrambler(t) = _ : (+ : de.fdelay3(MAX_LEN, t))~(*(fb) : comp) : _;

    f1 = hslider("freq1 [unit:hz]", 7, 0.25, 50, 0.01);
    f2 = hslider("freq2 [unit:hz]", 5, 0.25, 50, 0.01);
    dev = hslider("dev", 0.25, 0, 1, 0.001) * 0.99;

    dust1 = no.lfnoise0(f1 + (no.lfnoise0(f2) * f1 * dev));
    dust2 = no.lfnoise0(f2 + (no.lfnoise0(f1) * f2 * dev));

    dt =  dust1 : +(1) : *(0.5) : *((MAX_LEN-1)*range);
    dt2 = dust2 : +(1) : *(0.5) : *((MAX_LEN-1)*range);
    range = hslider("range", 0.5, 0, 1, 0.001);
    smoo = 1 - (hslider("smooth", 1, 0, 10, 0.001) * 0.0001);

    fb = hslider("feedback", 0.25, 0, 0.99, 0.001);
    comp = co.FFcompressor_N_chan(0.5, -10, 0.001, 0.100, 2, 1, 0, _, 1);
};

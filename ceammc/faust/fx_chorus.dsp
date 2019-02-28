declare name "fx.chorus";
declare author "Albert Graef";
declare version "1.0";

import("stdfaust.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

freq	= hslider("freq [unit:Hz]", 3, 0, 10, 0.01) : si.smoo;
dtime	= hslider("delay [unit:ms]", 25, 0, 200, 0.001) : cm.time_pd2faust : si.smoo;
depth	= hslider("depth [style:knob]", 0.02, 0, 1, 0.001) : si.smoo;

tblosc(n, f, freq, mod) = (1-d)*rdtable(n, wave, i&(n-1)) + d*rdtable(n, wave, (i+1)&(n-1))
with {
    wave	 	= ba.time * (2.0 * ma.PI)/n : f;
    phase		= freq / ma.SR : (+ : ma.decimal) ~ _;
    modphase	= ma.decimal(phase + mod/(2 * ma.PI))*n;
    i		= int(floor(modphase));
    d		= ma.decimal(modphase);
};

chorus(dtime, freq, depth, phase, x) = de.fdelay(1<<16, t, x)
with {
    t = ma.SR * dtime/2* (1 + depth * tblosc(1<<16, sin, freq, phase));
};

effect = chorus(dtime, freq, depth, 0);
process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), effect) : _;

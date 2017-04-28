pf = library("phaflangers.lib");
os = library("oscillators.lib");
ma = library("maths.lib");
ba = library("basics.lib");
si = library("signals.lib");
ui = library("ceammc_ui.lib");
cm = library("ceammc.lib");

effect = _ : pf.flanger_mono(dmax, curdel, depth, fb, invert) : _ with {
    dmax = 2048;
    lfol = os.oscrs;
    dflange = 0.001 * ma.SR * hslider("delay [unit:ms] [style:knob]", 10, 0, 20, 0.001);
    odflange = 0.001 * ma.SR * hslider("offset [unit:ms] [style:knob]", 1, 0, 20, 0.001);
    freq = hslider("speed [unit:Hz] [style:knob]", 0.5, 0, 10, 0.01);
    depth = hslider("drywet [style:knob]", 1, 0, 1, 0.001) : si.smoo;
    fb = hslider("feedback [style:knob]", 0, -0.999, 0.999, 0.001) : si.smoo;
    invert = checkbox("invert");
    curdel = odflange + dflange * (1 + lfol(freq)) / 2 : si.smoo;
};

process = _ : ba.bypass1(ui.bypass, effect) : _;

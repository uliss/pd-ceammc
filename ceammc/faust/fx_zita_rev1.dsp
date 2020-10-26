declare name "fx.zita_rev1";

re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
si = library("signals.lib");

fx = _,_ : re.zita_rev1_stereo(rdel, f1, f2, t60dc, t60m, fsmax)
with {
    fsmax = 96000;  // highest sampling rate that will be used
    rdel = vslider("delay [unit:ms] [style:knob]", 60, 20, 100, 1) : si.smoo;
    f1 = vslider("freq_low [unit:Hz] [style:knob] [scale:log]", 200, 50, 1000, 1);
    f2 = vslider("dump_hf [unit:Hz] [style:knob] [scale:log]", 6000, 1500, 0.49 * fsmax, 1);
    t60dc = vslider("decay_low [unit:sec] [style:knob] [scale:log]", 3, 1, 25, 0.1);
    t60m = vslider("decay_mid [unit:sec] [style:knob] [scale:log]", 2, 1, 25, 0.1);
};

process = _,_ : cm.fx_wrap2(ui.bypass, ui.drywet(1), fx) : _,_;

declare name "fx.zita_rev1";

ba = library("basics.lib");
re = library("reverbs.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
si = library("signals.lib");

fx = in_level : re.zita_rev1_stereo(rdel, f1, f2, t60dc, t60m, fsmax)
with {
    fsmax = 96000;  // highest sampling rate that will be used

    ui_freeze = checkbox("freeze [type:bool]") : int;
    ui_freeze_time = hslider("fr_time [unit:ms][style:knob]", 100, 0.0, 1000, 0.001);
    ui_freeze_t60dc = hslider("fr_ldecay [unit:sec] [style:knob]", 40, 0, 60, 0.001);
    ui_freeze_t60m = hslider("fr_mdecay [unit:sec] [style:knob]", 40, 0, 60, 0.001);

    ui_t60dc = vslider("decay_low [unit:sec] [style:knob] [scale:log]", 3, 1, 60, 0.1);
    ui_t60m = vslider("decay_mid [unit:sec] [style:knob] [scale:log]", 2, 1, 60, 0.1);

    smoo = _ : si.smooth(ba.tau2pole(ui_freeze_time / 1000.0)) : _;
    gate = (1 - ui_freeze) : smoo;
    in_level = *(gate),*(gate);

    rdel = vslider("delay [unit:ms] [style:knob]", 60, 20, 100, 1) : si.smoo;
    f1 = vslider("freq_low [unit:Hz] [style:knob] [scale:log]", 200, 50, 1000, 1);
    f2 = vslider("damp_hf [unit:Hz] [style:knob] [scale:log]", 6000, 1500, 0.49 * fsmax, 1);

    t60dc = ba.if(ui_freeze, ui_freeze_t60dc, ui_t60dc);
    t60m = ba.if(ui_freeze, ui_freeze_t60m, ui_t60m);
};

process = _,_ : cm.fx_wrap2(ui.bypass, ui.drywet(1), fx) : _,_;

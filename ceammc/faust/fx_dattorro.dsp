declare name "fx.dattorro";

import("stdfaust.lib");
cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");


my_dattorro_rev(pre_delay, bw, i_diff1, i_diff2, decay, d_diff1, d_diff2, damping) =
    si.bus(2) : + : *(0.5) : predelay : bw_filter : diffusion_network <: ((si.bus(4) :> _,_) ~ (reverb_network : ro.cross(2)))
with {
    // allpass using delay with fixed size
    allpass_f(t, a) = (+ <: @(t),*(a)) ~ *(-a) : mem,_ : +;

    // input pre-delay and diffusion
    predelay = de.fdelay(ba.sec2samp(0.1), ba.sec2samp(pre_delay));
    bw_filter = *(bw) : +~(mem : *(1-bw));
    diffusion_network = allpass_f(142, i_diff1) : allpass_f(107, i_diff1) : allpass_f(379, i_diff2) : allpass_f(277, i_diff2);

    // reverb loop
    reverb_network = par(i, 2, block(i)) with {
        d = (672, 908, 4453, 4217, 1800, 2656, 3720, 3163);
        block(i) = allpass_f(ba.take(i+1, d),-d_diff1) : @(ba.take(i+3, d)) : damp :
            allpass_f(ba.take(i+5, d), d_diff2) : @(ba.take(i+5, d)) : *(decay)
        with {
            damp = *(1-damping) : +~*(damping) : *(decay);
        };
    };
};

process = cm.fx_wrap2(ui.bypass, ui.drywet(0.33), fx) with {
    fx = _,_ : my_dattorro_rev(pre_delay, bw, i_diff1, i_diff2, decay, d_diff1, d_diff2, damping) with {
        pre_delay = vslider("predelay [unit:ms] [style:knob]", 40, 0, 100, 1) * 0.001 : si.smoo;
        damping = vslider("damp", 0.0005, 0, 1, 0.0001) : si.smoo;
        d_diff1 = vslider("ddiff1", 0.5, 0, 1, 0.001) : si.smoo;
        d_diff2 = vslider("ddiff2", 0.7, 0, 1, 0.001) : si.smoo;
        decay = vslider("decay", 0.5, 0, 1, 0.001) : si.smoo;
        i_diff1 = vslider("idiff1", 0.625, 0, 1, 0.001) : si.smoo;
        i_diff2 = vslider("idiff2", 0.75, 0, 1, 0.001) : si.smoo;
        bw = vslider("bw", 0.9995, 0, 1, 0.001) : si.smoo;
    };
};




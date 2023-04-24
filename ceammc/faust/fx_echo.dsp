declare name "fx.echo";
declare version "0.3";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

echo = echo_noclick(MAX_TIME_SEC, time, feedback) with {
    MAX_TIME_SEC = 10;

    time = hslider("delay [unit:ms]", 500, 10, MAX_TIME_SEC * 1000, 1) * 0.001 : ba.sec2samp : si.smoo;
    feedback = hslider("feedback", 0.3, 0, 0.99, 0.001) : si.smoo;

    echo_noclick(maxDuration, dur_samples, feedback) = +~(fx)  with {
        fx = de.sdelay(maxDel, interp, dur_samples) * feedback : comp : filter;
        interp   = hslider("smooth [unit:ms]", 50, 0, 500, 0.1) * 0.001 : ba.sec2samp : int;
        maxDel = ma.SR * maxDuration;

        comp = _ : co.FFcompressor_N_chan(strength, thresh, att, rel, 3, 1, 0, _, 1) : _;
        strength = hslider("compress", 0.5, 0, 1, 0.01);
        thresh   = hslider("cthreshold [unit:db]", 0, -60, 0, 0.1);
        att      = hslider("catttack [unit:ms]", 10, 0.1, 100, 0.1) * 0.001;
        rel      = hslider("crelease [unit:ms]", 50, 1, 500, 0.1) * 0.001;

        filter = _ <: ba.if(checkbox("filter"), fn, _) with {
            fn = fi.lowpass(4, lpf) : fi.highpass(4, hpf);
            hpf = hslider("hpf [unit:hz]", 300, 20, 20000, 0.01);
            lpf = hslider("lpf [unit:hz]", 9000, 20, 20000, 0.01);
        };
};
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), echo) : _;

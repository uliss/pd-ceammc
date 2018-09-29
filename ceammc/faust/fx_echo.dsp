declare name "fx.echo";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

echo = ef.echo(10.0, time, feedback) with {
    time = hslider("time [unit:ms]", 500, 10, 10000, 1) : cm.clip(10, 10000) : cm.time_pd2faust;
    feedback = hslider("feedback", 0.3, 0, 0.8, 0.001) : si.smoo;
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), echo) : _;

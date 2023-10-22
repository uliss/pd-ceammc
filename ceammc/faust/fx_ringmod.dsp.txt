declare name "fx.echo";
declare version "0.2";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
import("stdfaust.lib");

echo = ef.echo(max_sec, time, feedback) with {
    max_sec = 10;
    time = hslider("delay [unit:ms]", 500, 10, max_sec * 1000, 1) : cm.time_pd2faust;
    feedback = hslider("feedback", 0.3, 0, 0.99, 0.001) : si.smoo;
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), echo) : _;

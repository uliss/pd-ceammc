import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

d = hslider("duration", 100, 0, 100000, 1) : time_pd2faust;
t = ui.gate;

process = _ * en.smoothEnvelope(d, t);

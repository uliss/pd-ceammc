import("stdfaust.lib");
import("ceammc.lib");

d = hslider("duration", 100, 0, 100000, 1) : time_pd2faust;
t = hslider("trigger", 0, 0, 1, 0.001);
process = _ * en.smoothEnvelope(d, t);

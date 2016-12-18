import("stdfaust.lib");
import("ceammc.lib");

a = hslider("attack", 10, 0, 100000, 1) : time_pd2faust;
r = hslider("release", 10, 0, 100000, 1) : time_pd2faust;
t = hslider("trigger", 0, 0, 1, 0.001);
process = _ * en.ar(a, r, t);

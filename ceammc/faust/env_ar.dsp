import("stdfaust.lib");
import("ceammc.lib");

a = hslider("attack", 10, 0, 100000, 1) : time_pd2faust;
r = hslider("release", 100, 0, 100000, 1) : time_pd2faust;
t = button("gate");
process = _ * en.ar(a, r, t);

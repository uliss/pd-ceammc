import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

a = ui.attack(10);
d = ui.decay(10);
s = ui.sustain(50);
r = ui.release(10);
t = hslider("trigger", 0, 0, 1, 0.001);
process = _ * en.adsr(a, d, s, r, t);

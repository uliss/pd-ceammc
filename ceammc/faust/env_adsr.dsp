import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

a = ui.attack(10);
d = ui.decay(10);
s = ui.sustain(50);
r = ui.release(300);
t = ui.fgate;

process = _ * en.adsr(a, d, s, r, t);

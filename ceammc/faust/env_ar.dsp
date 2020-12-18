import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

a = ui.attack(10);
r = ui.release(300);
t = ui.igate;

process = _ * en.ar(a, r, t);

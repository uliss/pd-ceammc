declare name "env.asr";

import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

a = ui.attack(10);
s = ui.sustain(100);
r = ui.release(300);
t = ui.fgate;

process = _ * en.asr(a, s, r, t);

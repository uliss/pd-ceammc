declare name "env.asr2";

import("stdfaust.lib");
import("ceammc.lib");
ui = library("ceammc_ui.lib");

process = env with {
    env = par(i, 2, _ * en.asr(a, s, r, t));
    a = ui.attack(10);
    s = ui.sustain(100);
    r = ui.release(300);
    t = ui.fgate;
};

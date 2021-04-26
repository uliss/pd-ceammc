declare name "fx.recho";

cm = library("ceammc.lib");
ui = library("ceammc_ui.lib");
ma = library("maths.lib");
import("stdfaust.lib");


fx_recho =  ef.reverseEchoN(N, delay) : si.bus(N) with {
    N = 1;
    // should be power of 2!
    delay = 512 * 128;
};

process = _ : cm.fx_wrap_mono(ui.bypass, ui.drywet(1), fx_recho) : _;
